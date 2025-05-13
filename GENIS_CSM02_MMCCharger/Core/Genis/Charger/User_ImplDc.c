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
 *  Filename: User_ImplDc.c
 *  Created: Jun 21, 2024
 *  Author: sbkang
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "User_ImplDc.h"

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
#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_DC)

void SetTargetVoltage(GenisCsm_ChargerType *Charger, uint16 Voltage)
{
  // TODO: Voltage setting for PowerBank.
}
void SetTargetCurrent(GenisCsm_ChargerType *Charger, uint16 Current)
{
  // TODO: Current setting for PowerBank.
}
sint16 GetEVSEPresentCurrent(GenisCsm_ChargerType *Charger)
{
  // TODO: return the present current.
  return 0;
}
uint16 GetEVSEPresentVoltage(GenisCsm_ChargerType *Charger)
{
  // TODO: return the present voltage.
  return 0;
}

void GenisCsm_IsDCableChecked(GenisCsm_ChargerType* Charger)
{
  /* TODO: 케이블 검사 로직 수행 후 TRUE 로 설정할 것. */
  Charger->ChargerBus.ChargerStatus.DcCableIsolationChecked = TRUE;
}

void GenisCsm_DcInitialize(GenisCsm_ChargerType* Charger)
{
  can_csm_charger_t* ChargerBus = &Charger->ChargerBus;

  ChargerBus->ChargerStatus.ChargingEnergyTransferType = DC_EXTENDED;

  ChargerBus->ChargerDcChargeParameter1.EVSEMaximumCurrentLimit = (GENIS_CSM_CHARGER_MAX_CURRENT_A * 10); // 1000 = 100A
  ChargerBus->ChargerDcChargeParameter1.EVSEMaximumVoltageLimit = (GENIS_CSM_CHARGER_MAX_VOLTAGE_V * 10); // 10000 = 1000V
  ChargerBus->ChargerDcChargeParameter2.EVSEMaximumPowerLimit = (GENIS_CSM_CHARGER_MAX_CHARGE_POWER_KW * 10);    // 1000 = 100kW

  ChargerBus->ChargerDcDischargeParam.MaximumDischargePower = (GENIS_CSM_CHARGER_MAX_DISCHARGE_POWER_KW * 10);    // -1000 = -100kW
  ChargerBus->ChargerDcDischargeParam.MaximumDischargeCurrent = (GENIS_CSM_CHARGER_MAX_DISCHARGE_CURRENT_A * 10); // -1000 = -100A
}

void GenisCsm_ModbusReadDc(GenisCsm_ChargerType* Charger)
{
  uint16_t buffer[120] = { 0, };
  uint8_t size = 0;
  if (Charger->UserHandler.ReadInputRegs)
  {
    Charger->UserHandler.ReadInputRegs(1, Csm_ModbusAddr_ReadDc, 70, &buffer[0], &size);
  }
}
void GenisCsm_ModbusWriteDc(GenisCsm_ChargerType* Charger)
{
  uint16_t write_map[120] = { 0, };
  uint8 count = can_csm_charger_to_map(&Charger->ChargerBus, write_map);
  if (Charger->UserHandler.WriteHoldingRegs)
  {
    Charger->UserHandler.WriteHoldingRegs(1, Csm_ModbusAddr_WriteDc, write_map, count);
  }
}
#endif  // #if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_DC)