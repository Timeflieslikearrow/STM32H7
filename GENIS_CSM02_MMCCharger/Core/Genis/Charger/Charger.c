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
 *  Filename: Charger.c
 *  Created: 2024-03-19
 *  Author: sbkang
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <stdio.h>

#include "User_Impl.h"
#include "Charger.h"

#include "User_ImplDc.h"

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
#define ABS(x)   (((x) < 0) ? -(x) : (x) )

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

static void GenisCsm_ClearControlFlags(can_csm_charger_t * ChargerBus)
{
  ChargerStatus_0x80040001_t* status = &ChargerBus->ChargerStatus;

  status->ChargingStatus = FALSE;
  status->ChargingStopControl = FALSE;
  status->EVSEMalfunction = FALSE;
  status->EmergencyShutdown = FALSE;
  status->AuthFinished = FALSE;
  status->ChargeParameterFinished = FALSE;
  status->DcCableIsolationChecked = FALSE;
  status->DcEVSECurrentLimitAchieved = FALSE;
  status->DcEVSEVoltageLimitAchieved = FALSE;
  status->DcEVSEPowerLimitAchieved = FALSE;
  status->DcCableIsolationFaulted = FALSE;
}

const char* ChargerStateType_ToString(ChargerStateType State)
{
  const static char* StringTable[] =
    { 
    "CHARGER_STATE_INIT", 
    "CHARGER_STATE_WAIT_START", 
    "CHARGER_STATE_WAIT_HANDSHAKE",
    "CHARGER_STATE_WAIT_AUTH",
    "CHARGER_STATE_WAIT_CHARGE_PARAM",
    "CHARGER_STATE_DC_CABLE_CHECK",
    "CHARGER_STATE_DC_PRECHARGE",
    "CHARGER_STATE_AC_CHARGING",
    "CHARGER_STATE_DC_CHARGING",
    "CHARGER_STATE_STOP_CHARGING",
  };

  return StringTable[State];
}

const char *SeccStatus_ToString(SeccStatusCodeType Status) 
{
  const static char *StringTable[] = {
      "SECC_STATUS_NONE",           "SECC_STATUS_READY",
      "SECC_STATUS_WAIT_HANDSHAKE", "SECC_STATUS_SESSION_READY",
      "SECC_STATUS_AUTH_CHECK",     "SECC_STATUS_CHARGE_PARAM_CHECK",
      "SECC_STATUS_CABLE_CHECK",    "SECC_STATUS_PRE_CHARGE",
      "SECC_STATUS_CHARGING_LOOP",  "SECC_STATUS_STOP_CHARGING",
      "SECC_STATUS_FAULT_STOPPED",  "SECC_STATUS_NORMAL_STOPPED",
      "", "", "", "", "", "", "", "",
      "SECC_STATUS_RENEGOTIATION", "SECC_STATUS_PAUSE_SESSION"
  };
  
  return StringTable[Status];
}

void StateTransition(GenisCsm_ChargerType* Charger, ChargerStateType State)
{
  if (State == CHARGER_STATE_STOP_CHARGING)
  {
    Charger->UserHandler.Logging("State Changed from %s to %s\r\n",
      ChargerStateType_ToString(Charger->State),
      ChargerStateType_ToString(State)
    );
  }

  Charger->State = State;
}
static boolean DoStopCharging(GenisCsm_ChargerType *Charger) 
{ 
  if (!Charger->ChargerBus.ChargerStatus.ChargingStopControl)
  {
    Charger->ChargerBus.ChargerStatus.ChargingStopControl = StopControl_NormalStop;
  }

  GenisCsm_SetRelay(Charger, FALSE);
  return TRUE;
}

void GenisCsm_StateInit(GenisCsm_ChargerType* Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;
  GenisCsm_ClearControlFlags(&Charger->ChargerBus);
  GenisCsm_Initialize(Charger);

  if (seccStatus == SECC_STATUS_READY)
  {
    StateTransition(Charger, CHARGER_STATE_WAIT_START);
  }
  else if (seccStatus >= SECC_STATUS_WAIT_HANDSHAKE) 
  {
    StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
  }
}

void GenisCsm_StateWaitStart(GenisCsm_ChargerType* Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;
  if (seccStatus == SECC_STATUS_READY)
  {
    GenisCsm_IsStartCharging(Charger);
  }
  else if (seccStatus == SECC_STATUS_WAIT_HANDSHAKE || seccStatus == SECC_STATUS_SESSION_READY)
  {
    StateTransition(Charger, CHARGER_STATE_WAIT_HANDSHAKE);
  }
  else
  {
    StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
  }
}

void GenisCsm_StateWaitHandshake(GenisCsm_ChargerType* Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;
  if (seccStatus == SECC_STATUS_WAIT_HANDSHAKE || seccStatus == SECC_STATUS_SESSION_READY)
  {
  }
  else if (seccStatus == SECC_STATUS_AUTH_CHECK)
  {
    StateTransition(Charger, CHARGER_STATE_WAIT_AUTH);
  }
  else
  {
    StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
  }
}

void GenisCsm_StateWaitAuth(GenisCsm_ChargerType* Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;
  if (seccStatus == SECC_STATUS_AUTH_CHECK)
  {
    GenisCsm_IsAuthChecked(Charger);
  }
  else if (seccStatus == SECC_STATUS_CHARGE_PARAM_CHECK)
  {
    StateTransition(Charger, CHARGER_STATE_WAIT_CHARGE_PARAM);
  }
  else
  {
    StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
  }
}

void GenisCsm_StateWaitChargeParam(GenisCsm_ChargerType *Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;

  if (seccStatus == SECC_STATUS_CHARGE_PARAM_CHECK)
  {
    GenisCsm_IsChargeParamChecked(Charger);
  }
#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_DC)
  else if (seccStatus == SECC_STATUS_CABLE_CHECK)
  {
    StateTransition(Charger, CHARGER_STATE_DC_CABLE_CHECK);
  }
#else
  else if (seccStatus == SECC_STATUS_CHARGING_LOOP)
  {
    GenisCsm_SetRelay(Charger, TRUE);
    StateTransition(Charger, CHARGER_STATE_AC_CHARGING);
  }
#endif
  else
  {
    StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
  }
}
#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_DC)
void GenisCsm_StateDcCableCheck(GenisCsm_ChargerType *Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;

  if (seccStatus == SECC_STATUS_CABLE_CHECK)
  {
    GenisCsm_IsDCableChecked(Charger);
  }
  else if (seccStatus == SECC_STATUS_PRE_CHARGE)
  {
    StateTransition(Charger, CHARGER_STATE_DC_PRECHARGE);
  }
  else if (seccStatus == SECC_STATUS_CHARGING_LOOP)
  {
    StateTransition(Charger, CHARGER_STATE_DC_CHARGING);
  }
  else
  {
    StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
  }
}

void GenisCsm_StateDcPrecharge(GenisCsm_ChargerType *Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;
  if (seccStatus == SECC_STATUS_PRE_CHARGE)
  {
    SetTargetVoltage(Charger, Charger->SeccBus.EvDcChargingVariable.EVTargetVoltage);
    Charger->ChargerBus.ChargerChargingVariable.EVSEPresentVoltage = GetEVSEPresentVoltage(Charger);
  }
  else if (seccStatus == SECC_STATUS_CHARGING_LOOP)
  {
    GenisCsm_SetRelay(Charger, TRUE);
    StateTransition(Charger, CHARGER_STATE_DC_CHARGING);
  }
  else
  {
    StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
  }
}

void GenisCsm_StateDcCharging(GenisCsm_ChargerType* Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;

  if (seccStatus == SECC_STATUS_CHARGING_LOOP)
  {
    if (Charger->SeccBus.EvChargingService.Iso20ControlMode == Iso2_Control_Mode_Dynamic)
    {
      // TODO: 하기 파라미터들을 이용하여 EVTargetEnergyRequest <= 0 을 도달하도록 설정
      //Charger->SeccBus.EvTargetEnergy.EVTargetEnergyRequest
      //
      //Charger->SeccBus.EvDcChargingVariable.EVMaximumVoltageLimit;
      //Charger->SeccBus.EvDcMinimumChargeParam.EVMinimumVoltageLimit;
      // 
      //Charger->SeccBus.EvDcChargingVariable.EVMaximumCurrentLimit;
      //Charger->SeccBus.EvDcChargeParameter.EVMaximumPowerLimit;
      //
      //Charger->SeccBus.EvDcDischargeParam.EVMaximumDischargePower;
      //Charger->SeccBus.EvDcDischargeParam.EVMaximumDischargeCurrent;
      
      SetTargetCurrent(Charger, 0);
    }
    else 
    {
      SetTargetVoltage(Charger, Charger->SeccBus.EvDcChargingVariable.EVTargetVoltage);
      SetTargetCurrent(Charger, Charger->SeccBus.EvDcChargingVariable.EVTargetCurrent);
    }
    
    Charger->ChargerBus.ChargerChargingVariable.EVSEPresentVoltage = GetEVSEPresentVoltage(Charger);
    Charger->ChargerBus.ChargerChargingVariable.EVSEPresentCurrent = GetEVSEPresentCurrent(Charger);
  }
  else
  {
    SetTargetCurrent(Charger, 0);
    if (ABS(GetEVSEPresentCurrent(Charger)) < 5)
    {
      GenisCsm_SetRelay(Charger, FALSE);
      StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
    }
  }
}
#else
void GenisCsm_StateAcCharging(GenisCsm_ChargerType* Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;

  /* TODO: 미터기 값을 읽어서 현재 미터값, PresentActivePower, TargetActivePower 할당 */
  Charger->ChargerBus.ChargerChargingVariable.MeteringValue = 1000;  // 단위: 0.01kW (1000 = 10kWh)
  Charger->ChargerBus.ChargerAcPresentActivePower.EVSEPresentActivePower = 0;   // 단위: 0.1kW (70 = 7kW)
  Charger->ChargerBus.ChargerAcTargetActivePower.EVSETargetActivePower = 0;     // 단위: 0.1kW (70 = 7kW)
  Charger->ChargerBus.ChargerAcTargetReactivePower.EVSETargetReactivePower = 0;  // 단위: 0.1kW (70 = 7kW)

  if (seccStatus == SECC_STATUS_CHARGING_LOOP)
  {
    GenisCsm_SetRelay(Charger, TRUE);
  }
  else if (seccStatus == SECC_STATUS_PAUSE_SESSION) 
  {
    GenisCsm_SetRelay(Charger, FALSE);
  }
  else
  {
    GenisCsm_SetRelay(Charger, FALSE);
    StateTransition(Charger, CHARGER_STATE_STOP_CHARGING);
  }
}
#endif

void GenisCsm_StateStopCharging(GenisCsm_ChargerType* Charger)
{
  SeccStatusCodeType seccStatus = Charger->SeccBus.SeccInformation.SeccStatusCode;

  if (DoStopCharging(Charger)) 
  {
    if (GenisCsm_IsChargingFinished(Charger))
    {
      if (seccStatus == SECC_STATUS_NONE || seccStatus == SECC_STATUS_READY) 
      {
        StateTransition(Charger, CHARGER_STATE_INIT);
      }
    }
  }
}
static void GenisCsm_SetStopControl(GenisCsm_ChargerType* Charger, StopControlType Stop)
{
  switch (Stop)
  {
  case StopControl_None:
    Charger->ChargerBus.ChargerStatus.ChargingStopControl = 0;
    Charger->ChargerBus.ChargerStatus.EVSEMalfunction = 0;
    Charger->ChargerBus.ChargerStatus.EmergencyShutdown = 0;
    break;
  case StopControl_NormalStop:
    Charger->ChargerBus.ChargerStatus.ChargingStopControl = 1;
    Charger->ChargerBus.ChargerStatus.EVSEMalfunction = 0;
    Charger->ChargerBus.ChargerStatus.EmergencyShutdown = 0;
    break;
  case StopControl_EvseMalfunction:
    Charger->ChargerBus.ChargerStatus.ChargingStopControl = 0;
    Charger->ChargerBus.ChargerStatus.EVSEMalfunction = 1;
    Charger->ChargerBus.ChargerStatus.EmergencyShutdown = 0;
    break;
  case StopControl_EmergencyShutdown:
    Charger->ChargerBus.ChargerStatus.ChargingStopControl = 0;
    Charger->ChargerBus.ChargerStatus.EVSEMalfunction = 0;
    Charger->ChargerBus.ChargerStatus.EmergencyShutdown = 1;
    break;
  }
}
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


void GenisCsm_Charger_Init(GenisCsm_ChargerType* Charger)
{
  GenisCsm_Initialize(Charger);
}

void GenisCsm_Charger_MainFunction(GenisCsm_ChargerType* Charger)
{
  /* TODO: CSM을 이용한 기본 StateMachine. 수정 지양할 것(User_Impl.h / c 부분 위주로 수정 할 것). */
  while (1)
  {
#if (GENIS_CSM_COMMUNICATION_TYPE == GENIS_CSM_COMMUNICATION_TYPE_MODBUS)
    GenisCsm_ModbusReadInputRegs(Charger);
#endif

    Charger->UserHandler.Logging(
      "[Version]: %d, [Charger]: %s, [CSM]: %s\r\n",
      Charger->SeccBus.SeccInformation.SeccSWVersion,
      ChargerStateType_ToString(Charger->State),
      SeccStatus_ToString(Charger->SeccBus.SeccInformation.SeccStatusCode));

    switch (Charger->State)
    {
    case CHARGER_STATE_INIT:
      GenisCsm_StateInit(Charger);
      break;
    case CHARGER_STATE_WAIT_START:
      GenisCsm_StateWaitStart(Charger);
      break;
    case CHARGER_STATE_WAIT_HANDSHAKE:
      GenisCsm_StateWaitHandshake(Charger);
      break;
    case CHARGER_STATE_WAIT_AUTH:
      GenisCsm_StateWaitAuth(Charger);
      break;
    case CHARGER_STATE_WAIT_CHARGE_PARAM:
      GenisCsm_StateWaitChargeParam(Charger);
      break;
#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_DC)
    case CHARGER_STATE_DC_CABLE_CHECK:
      GenisCsm_StateDcCableCheck(Charger);
      break;
    case CHARGER_STATE_DC_PRECHARGE:
      GenisCsm_StateDcPrecharge(Charger);
      break;
    case CHARGER_STATE_DC_CHARGING:
      GenisCsm_StateDcCharging(Charger);
      break;
#else
    case CHARGER_STATE_AC_CHARGING:
      GenisCsm_StateAcCharging(Charger);
      break;
#endif
    case CHARGER_STATE_STOP_CHARGING:
      GenisCsm_StateStopCharging(Charger);
      break;
    }
    StopControlType stop = GenisCsm_IsStopCharging(Charger);
    GenisCsm_SetStopControl(Charger, stop);

#if (GENIS_CSM_COMMUNICATION_TYPE == GENIS_CSM_COMMUNICATION_TYPE_MODBUS)
    GenisCsm_ModbusWriteMultipleRegs(Charger);
#endif

    Delay(50);
  }
}
