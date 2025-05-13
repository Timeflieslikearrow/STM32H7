/**********************************************************************************************************************
 *  GENIS KOREA SOFTWARE
 *  -------------------------------------------------------------------------------------------------------------------
 *  Copyright (c) 2019-2021 GENIS KOREA Inc.                                                 All rights reserved.
 *
 *                This software is copyright protected and proprietary to GENIS KOREA.
 *                Restricted Distribution:
 *                  Not to be distributed This source code to anyone without the approval from GENIS KOREA.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Filename: Charger_Types.h
 *  Created: Apr 3, 2024
 *  Author: sbkang
 *
 *********************************************************************************************************************/

#ifndef CHARGER_CHARGER_TYPES_H_
#define CHARGER_CHARGER_TYPES_H_
 
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef enum SeccStatusCodeType {
  SECC_STATUS_NONE = 0,
  SECC_STATUS_READY = 1,
  SECC_STATUS_WAIT_HANDSHAKE = 2,
  SECC_STATUS_SESSION_READY = 3,
  SECC_STATUS_AUTH_CHECK = 4,
  SECC_STATUS_CHARGE_PARAM_CHECK = 5,
  SECC_STATUS_CABLE_CHECK = 6,
  SECC_STATUS_PRE_CHARGE = 7,
  SECC_STATUS_CHARGING_LOOP = 8,
  SECC_STATUS_STOP_CHARGING = 9,
  SECC_STATUS_FAULT_STOPPED = 10,
  SECC_STATUS_NORMAL_STOPPED = 11,

  SECC_STATUS_PAUSE_SESSION = 21,
} SeccStatusCodeType;

typedef enum PaymentOption {
  PAYMENT_OPTION_EIM_ONLY = 0,
  PAYMENT_OPTION_PNC_ONLY = 1,
  PAYMENT_OPTION_ALL = 2
} PaymentOption;

typedef enum SelectedPaymentOption {
  USING_EIM = 0,
  USING_PNC = 1,
};

typedef enum ChargingType {
  CHARGING_TYPE_AC = 0,
  CHARGING_TYPE_DC = 1,
  CHARGING_TYPE_AC_BPT = 2,
  CHARGING_TYPE_DC_BPT = 3,
} ChargingType;

typedef enum {
  CHARGING_PRIORITY_ISO15118 = 0,
  CHARGING_PRIORITY_IEC61851 = 1,
  CHARGING_PRIORITY_ISO15118_AC_FALLBACK = 2,
  CHARGING_PRIORITY_ISO15118_NOMINAL = 3,
} ChargingPriority;

typedef enum {
  SECC_CP_STATE_None = 0,
  SECC_CP_STATE_A = 1,
  SECC_CP_STATE_B = 2,
  SECC_CP_STATE_C = 3,
  SECC_CP_STATE_E = 4,
  SECC_CP_STATE_F = 5,
} CpStateType;

typedef enum {
  AC_SINGLE_PHASE_CORE = 0,
  AC_THREE_PHASE_CORE = 1,
  DC_CORE = 2,
  DC_EXTENDED = 3,
  DC_COMBO_CORE = 4,
  DC_DUAL = 5
} ChargingEnergyTransfertType;

typedef enum {
  StopControl_None = 0,
  StopControl_NormalStop = 0b001,         // 1
  StopControl_EvseMalfunction = 0b010,    // 2
  StopControl_EmergencyShutdown = 0b100,  // 4
} StopControlType;

typedef enum {
  Iso2_Control_Mode_Schedule = 1,
  Iso2_Control_Mode_Dynamic = 2,
} Iso2_Control_ModeType;

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

#endif /* CHARGER_CHARGER_TYPES_H_ */
