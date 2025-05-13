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
 *  Filename: Charger_Cfg.h
 *  Created: Jul 23, 2024
 *  Author: sbkang
 *
 *********************************************************************************************************************/

#ifndef CHARGER_CFG_H_
#define CHARGER_CFG_H_
 
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

#define GENIS_CSM_CHARGER_SW_VERSION  "0.4"

#define GENIS_CSM_STOP_NEED_PLUG_OUT         (TRUE)

#define GENIS_CSM_CHARGING_TYPE_AC 1
#define GENIS_CSM_CHARGING_TYPE_DC 2
#define GENIS_CSM_CHARGING_TYPE GENIS_CSM_CHARGING_TYPE_DC

#if (GENIS_CSM_CHARGING_TYPE == GENIS_CSM_CHARGING_TYPE_AC)
  #define GENIS_CSM_CHARGER_MAXIMUM_POWER_KW (7u)
  #define GENIS_CSM_CHARGER_MAXIMUM_CURRENT (32u)
  #define GENIS_CSM_CHARGER_NOMINAL_VOLTAGE (220u)
  #define GENIS_CSM_CHARGER_NOMINAL_FREQ    (60u)
  #define GENIS_CSM_CHARGER_TARGET_FREQ    (60u)
#else
  #define GENIS_CSM_CHARGER_MAX_VOLTAGE_V  (1000u)
  #define GENIS_CSM_CHARGER_MAX_CURRENT_A  (100u)
  #define GENIS_CSM_CHARGER_MAX_CHARGE_POWER_KW    (100u)
  #define GENIS_CSM_CHARGER_MAX_DISCHARGE_CURRENT_A (-100)
  #define GENIS_CSM_CHARGER_MAX_DISCHARGE_POWER_KW (-100)
#endif

#define GENIS_CSM_COMMUNICATION_TYPE_CAN 1
#define GENIS_CSM_COMMUNICATION_TYPE_MODBUS 2
#define GENIS_CSM_COMMUNICATION_TYPE GENIS_CSM_COMMUNICATION_TYPE_CAN

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

#ifdef __cplusplus
}
#endif

#endif /* CHARGER_CFG_H_ */
