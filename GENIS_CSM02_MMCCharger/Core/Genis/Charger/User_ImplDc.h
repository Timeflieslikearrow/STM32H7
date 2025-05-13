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
 *  Filename: User_ImplDc.h
 *  Created: Jun 21, 2024
 *  Author: sbkang
 *
 *********************************************************************************************************************/

#ifndef USER_IMPLDC_H_
#define USER_IMPLDC_H_
 
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Platform_Types.h"
#include "Charger.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
//#pragma pack(push,1)
//#pragma pack(pop)

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
 
#ifdef __cplusplus
extern "C" {
#endif

#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_DC)
void GenisCsm_DcInitialize(GenisCsm_ChargerType* Charger);

void SetTargetVoltage(GenisCsm_ChargerType *Charger, uint16 Voltage);
void SetTargetCurrent(GenisCsm_ChargerType *Charger, uint16 Current);

sint16 GetEVSEPresentCurrent(GenisCsm_ChargerType *Charger);
uint16 GetEVSEPresentVoltage(GenisCsm_ChargerType *Charger);

void GenisCsm_IsDCableChecked(GenisCsm_ChargerType* Charger);

void GenisCsm_ModbusReadDc(GenisCsm_ChargerType* Charger);
void GenisCsm_ModbusWriteDc(GenisCsm_ChargerType* Charger);

#endif // #if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_DC)

#ifdef __cplusplus
}
#endif

#endif /* USER_IMPLDC_H_ */
