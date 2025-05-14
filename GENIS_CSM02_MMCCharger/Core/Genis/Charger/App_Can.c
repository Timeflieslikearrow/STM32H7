/**********************************************************************************************************************
 *  GENIS KOREA SOFTWARE
 *  -------------------------------------------------------------------------------------------------------------------
 *  Copyright (c) 2019-2024 GENIS KOREA Inc.                                                 All rights reserved.
 *
 *                This software is copyright protected and proprietary to GENIS KOREA.
 *                Restricted Distribution:
 *                  Not to be distributed This source code to anyone without the approval from GENIS KOREA.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Filename: App_Can.c
 *  Created: Jun 24, 2024
 *  Author: sbkang
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "App_Can.h"

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue.h"

#include "User_Impl.h"
/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
//#pragma pack(push,1)
//#pragma pack(pop)

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

static FDCAN_HandleTypeDef *App_CanHandle;
static QueueHandle_t App_CanRxQueue;
static FDCAN_FilterTypeDef sFilterConfig;

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

void HAL_FDCAN_ErrorStatusCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t ErrorStatusITs)
{
  if (FDCAN_IR_BO & ErrorStatusITs)
  {
    HAL_FDCAN_Stop(hfdcan);
    HAL_FDCAN_Start(hfdcan);
  }
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  Can_DataType data;

  FDCAN_RxHeaderTypeDef RxHeader;
  if (hfdcan == App_CanHandle)
  {
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
      /* Retrieve Rx messages from RX FIFO0 */
      HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, data.Data);
      data.Id = RxHeader.Identifier;
      data.Dlc = (RxHeader.DataLength >> 16);

      if (App_CanRxQueue != NULL_PTR)
      {
        xQueueSendFromISR(App_CanRxQueue, &data, 0);
      }
    }
  }
}

void App_CanInit(FDCAN_HandleTypeDef *Handle, uint8 Index)
{
  App_CanHandle = Handle;

  HAL_FDCAN_MspDeInit(App_CanHandle);
  HAL_FDCAN_MspInit(App_CanHandle);

  sFilterConfig.IdType = FDCAN_EXTENDED_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x30001 + (Index * 0x100);
  sFilterConfig.FilterID2 = 0x300FF + (Index * 0x100);

  if (HAL_FDCAN_ConfigFilter(App_CanHandle, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure global filter on both FDCAN instances:
   Filter all remote frames with STD and EXT ID
   Reject non matching frames with STD ID and EXT ID */
  if (HAL_FDCAN_ConfigGlobalFilter(App_CanHandle, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE)
      != HAL_OK)
  {
    Error_Handler();
  }

  /* Activate Rx FIFO 0 new message notification on both FDCAN instances */
  if (HAL_FDCAN_ActivateNotification(App_CanHandle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_FDCAN_ActivateNotification(App_CanHandle, FDCAN_IT_BUS_OFF, 0) != HAL_OK)
  {
    Error_Handler();
  }

  App_CanRxQueue = xQueueCreate(10, sizeof(Can_DataType));

  HAL_FDCAN_Start(App_CanHandle);
}

void App_CanSend(uint32 Id, const uint8 *Data, uint8 Size)
{
  FDCAN_TxHeaderTypeDef txHeader = { 0 };
  txHeader.Identifier = Id;
  txHeader.IdType = FDCAN_EXTENDED_ID;
  txHeader.TxFrameType = FDCAN_DATA_FRAME;
  txHeader.DataLength = (0x10000 * Size);  //FDCAN_DLC_BYTES_8;
  txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  txHeader.BitRateSwitch = FDCAN_BRS_OFF;
  txHeader.FDFormat = FDCAN_CLASSIC_CAN;
  txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  txHeader.MessageMarker = 0;

  if (App_CanHandle != NULL_PTR)
  {
    HAL_FDCAN_AddMessageToTxFifoQ(App_CanHandle, &txHeader, Data);
  }
}

void App_CanTxMainFunction(GenisCsm_ChargerType *Charger)
{
  uint32 delayMs = 100 / csm_charger_msg_max_count;
  while (TRUE)
  {
    uint32 now = osKernelGetTickCount();

    for (uint32 msg = csm_charger_msg_ChargerStatus_80040001h;
         msg < csm_charger_msg_ChargerDcChargeParameter1_80040006h; ++msg)
     {
       uint64 data = 0;

       uint32 id = candb_csm_charger_get_can_id(msg);
       candb_csm_charger_pack_message(&Charger->ChargerBus, id, &data);

       id += (Charger->CanIndex * 0x100);
       App_CanSend(id - 0x80000000, &data, 8);
       Delay(delayMs);
     }

    if (Charger->ChargingType == CHARGER_CHARGING_TYPE_AC)
    {
      for (uint32 msg = csm_charger_msg_ChargerAcChargeParam_80040009h;
          msg < csm_charger_msg_ChargerPncResponseHeader1_80040051h; ++msg)
      {
        uint64 data = 0;

        uint32 id = candb_csm_charger_get_can_id(msg);
        candb_csm_charger_pack_message(&Charger->ChargerBus, id, &data);

        id += (Charger->CanIndex * 0x100);
        App_CanSend(id - 0x80000000, &data, 8);
        Delay(delayMs);
      }
    }
    else
    {
      for (uint32 msg = csm_charger_msg_ChargerDcChargeParameter1_80040006h;
          msg < csm_charger_msg_ChargerAcChargeParam_80040009h; ++msg)
      {
        uint64 data = 0;

        uint32 id = candb_csm_charger_get_can_id(msg);
        candb_csm_charger_pack_message(&Charger->ChargerBus, id, &data);

        id += (Charger->CanIndex * 0x100);
        App_CanSend(id - 0x80000000, &data, 8);
        Delay(delayMs);
      }
    }
    osDelayUntil(now + 100);
  }
}

void App_CanRxMainFunction(GenisCsm_ChargerType *Charger)
{
  while (TRUE)
  {
    Can_DataType data;
    if (xQueueReceive(App_CanRxQueue, &data, 1000))
    {

      uint64_t u64 = 0;
      memcpy(&u64, data.Data, 8);
      candb_csm_secc_unpack_message(&Charger->SeccBus, data.Id | 0x80000000, u64, 8, 0);
      if (data.Id == 0x30001)
      {
      }
      if (data.Id == 0x300FF)
      {
      }
      else if (data.Id == 0x300A1 || data.Id == 0x300A2)
      {
        GenisCsm_CanParseVasData(Charger, u64);
      }
    }
  }
}

