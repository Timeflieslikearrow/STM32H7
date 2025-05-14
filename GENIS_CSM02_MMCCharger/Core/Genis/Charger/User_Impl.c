/**********************************************************************************************************************
 *  GENIS KOREA SOFTWARE
 *  -------------------------------------------------------------------------------------------------------------------
 *  Copyright (c) 2019-2023 GENIS KOREA Inc.                                                 All rights reserved.
 *
 *                This software is copyright protected and proprietary to GENIS KOREA.
 *                Restricted Distribution:
 *                  Not to be distributed This source code to anyone without the approval from GENIS KOREA.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Filename: User_Impl.c
 *  Created: 2024-03-20
 *  Author: sbkang
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "User_Impl.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#else
#include "FreeRTOS.h"
#include "task.h"
#endif

#if (GENIS_CSM_PNC_SUPPORTED == TRUE)
#include "PnC/User_ImplPnC.h"
#endif

#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_AC)
  #include "User_ImplAc.h"
#else
  #include "User_ImplDc.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

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

void Delay(uint32 DelayMsec)
{
  /* TODO: Platform 에 맞게 sleep 함수 수정(FreeRTOS: vTaskDelay(), Windows:
   * Sleep(), Linux: usleep())*/
#ifdef _WIN32
  Sleep(DelayMsec);
#elif __linux__
  usleep(DelayMsec * 1000);
#else
  vTaskDelay(DelayMsec);
#endif
}

static boolean GenisCsm_IsCableDisconnected(can_csm_secc_t *Secc)
{
  if (Secc->SeccInformation.PwmVoltage > 114 && Secc->SeccInformation.PwmVoltage < 126)
  {
    return TRUE;
  }

  return FALSE;
}

void GenisCsm_Initialize(GenisCsm_ChargerType *Charger)
{
  can_csm_charger_t *ChargerBus = &Charger->ChargerBus;
  ChargerBus->ChargerStatus.IdentificationOptionType = PAYMENT_OPTION_ALL;
  ChargerBus->ChargerStatus.ChargingPriority = CHARGING_PRIORITY_IEC61851;
  Charger->UserHandler.Logging =  UART_Logging;

#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_AC)
  GenisCsm_AcInitialize(Charger);
#elif (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_DC)
  GenisCsm_DcInitialize(Charger);
#endif

#if (GENIS_CSM_PNC_SUPPORTED == TRUE)
  GenisCsm_PncInitialize(Charger);
#endif
}

void GenisCsm_IsStartCharging(GenisCsm_ChargerType *Charger)
{
  /* TODO: 플러그 인식되어 시작 상태. 사용자 인증(예: 카드태깅) 후 ChargingStatus에 TRUE 삽입.
   * TRUE로 줄 경우, 충전 시작 진행 됨. */

  if (!GenisCsm_IsCableDisconnected(&Charger->SeccBus))
  {
    Charger->ChargerBus.ChargerStatus.ChargingStatus = TRUE;
  }
}

void GenisCsm_IsAuthChecked(GenisCsm_ChargerType* Charger)
{
  /* TODO: 사용자 인증(예: 카드) 확인 후, 올바르면 AuthFinished 플래그를 TRUE로 설정 할 것.
   * FALSE로 줄 경우, 차량 및 CSM간 Auhtorization 메시지 계속 주고 받다가 타임아웃(60초) 걸려 종료 됨. */
  if (Charger->SeccBus.EvChargingService.SelectedPaymentOption == USING_PNC)
  {
#if (GENIS_CSM_PNC_SUPPORTED == TRUE)
    boolean isPnCChecked = GenisCsm_PnCAuthCheck(Charger);
    Charger->ChargerBus.ChargerStatus.AuthFinished = isPnCChecked;
#endif
  }
  else
  {
    Charger->ChargerBus.ChargerStatus.AuthFinished = TRUE;
  }
}

void GenisCsm_IsChargeParamChecked(GenisCsm_ChargerType *Charger)
{
  /* TODO: SeccBus에 있는 ChargeParam(차량 충전 파라미터) 확인 후, ChargeParamFinished 에 TRUE 설정 할 것.
   * FALSE로 줄 경우, 차량 및 CSM간 ChargeParamCheck 메시지 주고 받다가 타임아웃(60초) 걸려 종료 됨. */

  Charger->ChargerBus.ChargerStatus.ChargeParameterFinished = TRUE;
}

boolean GenisCsm_IsChargingFinished(GenisCsm_ChargerType *Charger)
{
  /* TODO: 충전 종료 후 충전기에서 해야할 작업 추가한 후 TRUE로 반환할 것.
   * FALSE 일 경우, 중지 상태로 유지되며 본 함수 계속 호출 됨. */
#if (GENIS_CSM_STOP_NEED_PLUG_OUT == TRUE)
  if (GenisCsm_IsCableDisconnected(&Charger->SeccBus))
  {
    return TRUE;
  }
#endif

  return FALSE;
}

StopControlType GenisCsm_IsStopCharging(GenisCsm_ChargerType *Charger)
{
  /* TODO: 충전 종료를 위한 코드. 종료를 원할 경우(예: HMI 충전 종료) ChargerStopControl에 플래그 할당 */
  // Charger->ChargerBus.ChargerStatus.ChargingStopControl = StopControl_NormalStop;
  if (GenisCsm_IsCableDisconnected(&Charger->SeccBus))
  {
    return StopControl_NormalStop;
  }

  return StopControl_None;
}

void GenisCsm_SetRelay(GenisCsm_ChargerType *Charger, boolean IsClose)
{
  if (Charger->UserHandler.SetRelay)
  {
    Charger->UserHandler.SetRelay(IsClose);
  }
  else
  {
    /* TODO: Insert the realy control code here. */
  }
}

void GenisCsm_ModbusReadInputRegs(GenisCsm_ChargerType* Charger)
{
#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_AC)
  GenisCsm_ModbusReadAc(Charger);
#else
  GenisCsm_ModbusReadDc(Charger);
#endif

#if (GENIS_CSM_PNC_SUPPORTED == TRUE)
  if (Charger->SeccBus.EvChargingService.SelectedPaymentOption == USING_PNC)
  {
    GenisCsm_ModbusReadPnc(Charger);
  }
#endif
}

void GenisCsm_ModbusWriteMultipleRegs(GenisCsm_ChargerType *Charger)
{
#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_AC)
    GenisCsm_ModbusWriteAc(Charger);
#else
    GenisCsm_ModbusWriteDc(Charger);
#endif

#if (GENIS_CSM_PNC_SUPPORTED == TRUE)
    GenisCsm_ModbusWritePnc(Charger);
#endif

}
