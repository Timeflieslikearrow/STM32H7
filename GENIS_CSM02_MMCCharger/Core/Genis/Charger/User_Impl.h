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
 *  Filename: User_Impl.h
 *  Created: 2024-03-19
 *  Author: sbkang
 *
 *********************************************************************************************************************/

#ifndef CHARGER_SOURCE_USER_IMPL_H_
#define CHARGER_SOURCE_USER_IMPL_H_

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include "Platform_Types.h"
#include "Charger.h"
#include "../Driver/UART.h"

/**********************************************************************************************************************
*  GLOBAL CONSTANT MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION PROTOTYPES
*********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/* Utility Functions */

void Delay(uint32 DelayMsec);

/* Charging Control Functions */
void GenisCsm_Initialize(GenisCsm_ChargerType* Charger);
void GenisCsm_IsStartCharging(GenisCsm_ChargerType* Charger);
void GenisCsm_IsAuthChecked(GenisCsm_ChargerType* Charger);
void GenisCsm_IsChargeParamChecked(GenisCsm_ChargerType* Charger);
boolean GenisCsm_IsChargingFinished(GenisCsm_ChargerType* Charger);
StopControlType GenisCsm_IsStopCharging(GenisCsm_ChargerType* Charger);
void GenisCsm_SetRelay(GenisCsm_ChargerType* Charger, boolean IsClose);

/* Modbus Functions */
void GenisCsm_ModbusReadInputRegs(GenisCsm_ChargerType* Charger);
void GenisCsm_ModbusWriteMultipleRegs(GenisCsm_ChargerType* Charger);

/* CAN Functions */
void GenisCsm_CanParseVasData(GenisCsm_ChargerType *Charger, uint64_t Data);

#ifdef __cplusplus
}

#endif

#endif //CHARGER_SOURCE_USER_IMPL_H_
