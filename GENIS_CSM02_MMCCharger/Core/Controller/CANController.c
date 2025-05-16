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
 *  Filename: CANController.c
 *  Created: May 9, 2025
 *  Author: yeo
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "CANController.h"
#include "../Genis/Charger/User_Impl.h"
#include "../Genis/Charger/Charger.h"
#include "../Driver/CAN.h"
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
void CAN_Controller_MainFunction(void)
{
  if (CanRxFlag)
  {
    CanRxFlag = 0;
    if (CanRxHeader.IdType == FDCAN_STANDARD_ID)
    {
      printf("H743_Mode : CAN_TEST_MODE, RX, std_id : %03lX   %02X %02X %02X %02X %02X %02X %02X %02X \r\n",
             (uint16_t) CanRxHeader.Identifier, CanRxData[0], CanRxData[1], CanRxData[2], CanRxData[3], CanRxData[4],
             CanRxData[5], CanRxData[6], CanRxData[7]);
    }
    else
    {
      printf("H743_Mode : CAN_TEST_MODE, RX, ext_id : %08lX  %02X %02X %02X %02X %02X %02X %02X %02X \r\n",
             (uint16_t) CanRxHeader.Identifier, CanRxData[0], CanRxData[1], CanRxData[2], CanRxData[3], CanRxData[4],
             CanRxData[5], CanRxData[6], CanRxData[7]);
    }
  }
}

void App_CanRxMainFunction(GenisCsm_ChargerType *Charger)
{
  while (TRUE)
  {
    Can_DataType CanData;
    // uint8 CanData[8];
    if (xQueueReceive(App_CanRxQueue, &CanData, 1000) == pdPASS)
    {
      // printf("CAN_RxData : %02X %02X %02X %02X %02X %02X %02X %02X \r\n",
      //     CanData[0],CanData[1],CanData[2],CanData[3],
      //     CanData[4],CanData[5],CanData[6],CanData[7]);

      uint64_t u64;
      memcpy(&u64, CanData.Data, 8);
      candb_csm_secc_unpack_message(&Charger->SeccBus, CanData.Id | 0x80000000, u64, 8, 0);
      //SeccInformation
      switch (CanData.Id)
      {
        case 0x30001:
          SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;

          switch (seccStatus)
          {
            case SECC_STATUS_NONE:
              Charger->UserHandler.Logging(
                  "[PWM Voltage: %dV, PWM DutyCycle : %d%%] Waiting for start charging\r\n",
                  Charger->SeccBus.SeccInformation.PwmVoltage, Charger->SeccBus.SeccInformation.PwmDutyCycle);
              break;
            case SECC_STATUS_READY:
              // SECC 정보 출력
              Charger->UserHandler.Logging(
                  "[PWM Voltage: %dV, PWM DutyCycle : %d%%]\r\n",
                  Charger->SeccBus.SeccInformation.PwmVoltage, Charger->SeccBus.SeccInformation.PwmDutyCycle);
              break;
            case SECC_STATUS_WAIT_HANDSHAKE:
              Charger->UserHandler.Logging(
                  "[PWM Voltage: %dV, PWM DutyCycle : %d%%]\r\n",
                  Charger->SeccBus.SeccInformation.PwmVoltage, Charger->SeccBus.SeccInformation.PwmDutyCycle);
              break;
            case SECC_STATUS_SESSION_READY:
              Charger->UserHandler.Logging(
                  "[PWM Voltage: %dV, PWM DutyCycle : %d%%]\r\n",
                  Charger->SeccBus.SeccInformation.PwmVoltage, Charger->SeccBus.SeccInformation.PwmDutyCycle);
              break;
          }
          break;
          // EvEvccId
        case 0x30002:
          Charger->UserHandler.Logging("SECC EvccId : %d\r\n", SeccStatus_ToString(Charger->SeccBus.EvEvccId.EvccId));
          break;
          // EvDcChargingStatus
        case 0x30004:
          Charger->UserHandler.Logging("EvReady : %d\r\n", Charger->SeccBus.EvDcChargingStatus.EvReady);
          break;
        case 0x30005:
          break;
          // SeccUdsServer
        case 0x300FF:
          break;
      }
    }
  }
  Delay(100);
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

      //  id += (Charger->CanIndex * 0x100);
      CAN_Tx(id - 0x80000000, &data);
      Delay(delayMs);
    }
    for (uint32 msg = csm_charger_msg_ChargerDcChargeParameter1_80040006h;
        msg < csm_charger_msg_ChargerAcChargeParam_80040009h; ++msg)
    {
      uint64 data = 0;

      uint32 id = candb_csm_charger_get_can_id(msg);
      candb_csm_charger_pack_message(&Charger->ChargerBus, id, &data);

//      id += (Charger->CanIndex * 0x100);
      CAN_Tx(id - 0x80000000, &data);
      Delay(delayMs);
    }
    osDelayUntil(now + 100);
  }
}

