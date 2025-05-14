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
 *  Filename: CAN.c
 *  Created: May 9, 2025
 *  Author: yeo
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "CAN.h"
#include "UART.h"


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
FDCAN_RxHeaderTypeDef CanRxHeader;
uint8_t CanRxData[8];
uint8_t CanTxData[8];
volatile uint8_t CanRxFlag = 0;

QueueHandle_t App_CanRxQueue;
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
void HAL_FDCAN_TxBufferCompleteCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t BufferIndex)
{

  char msg[50];
  sprintf(msg, "Transmit IT\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
    char msg[50];

    /* Retrieve Rx messages from RX FIFO0 */
    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &CanRxHeader, CanRxData) != HAL_OK)
    {
      Error_Handler();
    }
    CanRxFlag = 1;
    if(App_CanRxQueue != NULL_PTR)
    {
      xQueueSendFromISR(App_CanRxQueue, &CanRxData, 0);
    }  
//    sprintf(msg,"Message Received : %02X\r\n",CanRxData);
//    HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
  }
}

void FDCAN_Filter_Config_mask32_Init(void)
{
  /* Configure Rx filter */
  sFilterConfig.IdType = FDCAN_EXTENDED_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x12345678;
  sFilterConfig.FilterID2 = 0;  // all message receive

  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure global filter:
   Filter all remote frames with STD and EXT ID
   Reject non matching frames with STD ID and EXT ID */
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE)
      != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_FDCAN_ActivateNotification(&hfdcan1,
  FDCAN_IT_TX_COMPLETE |
  FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_BUS_OFF,
                                     0) != HAL_OK)
  {
    Error_Handler();
  }

  App_CanRxQueue = xQueueCreate(10, sizeof(Can_DataType));

  /* Start the FDCAN module */
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }

}

void CAN_Tx(uint8 *Data)
{
  /* Prepare Tx Header */
  FDCAN_TxHeaderTypeDef CanTxHeader = {0};
  CanTxHeader.Identifier = 0x106;   // 0x321;
  CanTxHeader.IdType = FDCAN_EXTENDED_ID;
  CanTxHeader.TxFrameType = FDCAN_DATA_FRAME;
  CanTxHeader.DataLength = FDCAN_DLC_BYTES_8;
  CanTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  CanTxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  CanTxHeader.FDFormat = FDCAN_CLASSIC_CAN;
  CanTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  CanTxHeader.MessageMarker = 0;

  char msg[50];
  for (int i = 0; i < 8; i++)
    Data[i] = (uint8_t) i;
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &CanTxHeader, Data) != HAL_OK)
  {
    Error_Handler();
  }

//  sprintf(msg, "Message Transmitted\r\n");
//  HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
}

