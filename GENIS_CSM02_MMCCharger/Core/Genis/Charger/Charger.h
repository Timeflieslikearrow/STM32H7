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
 *  Filename: Charger.h
 *  Created: 2024-03-19
 *  Author: sbkang
 *
 *********************************************************************************************************************/

#ifndef CHARGER_SOURCE_CHARGER_H_
#define CHARGER_SOURCE_CHARGER_H_

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include <stdint.h>
#include "Charger_Types.h"
#include "../Genis/Dbc/Csm_Dbcs.h"
#include "Charger_Cfg.h"

#if __has_include("PnC/Charger_PnC.h")
# include "PnC/Charger_PnC.h"
#endif

/**********************************************************************************************************************
*  GLOBAL CONSTANT MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL FUNCTION MACROS
*********************************************************************************************************************/

/**********************************************************************************************************************
*  GLOBAL DATA TYPES AND STRUCTURES
*********************************************************************************************************************/

typedef enum
{
  CHARGER_STATE_INIT,
  CHARGER_STATE_WAIT_START,
  CHARGER_STATE_WAIT_HANDSHAKE,
  CHARGER_STATE_WAIT_AUTH,
  CHARGER_STATE_WAIT_CHARGE_PARAM,
  CHARGER_STATE_DC_CABLE_CHECK,
  CHARGER_STATE_DC_PRECHARGE,
  CHARGER_STATE_AC_CHARGING,
  CHARGER_STATE_DC_CHARGING,
  CHARGER_STATE_STOP_CHARGING,
} ChargerStateType;

typedef enum {
  Csm_ModbusAddr_ReadAc = 30001,
  Csm_ModbusAddr_ReadDc = 31001,
  Csm_ModbusAddr_ReadKVas = 32001,
  Csm_ModbusAddr_ReadPncHeader = 35001,
  Csm_ModbusAddr_ReadPncField1 = 35011,
  Csm_ModbusAddr_ReadPncField2 = 36001,
  Csm_ModbusAddr_ReadPncField3 = 37001,
  Csm_ModbusAddr_ReadPncField4 = 38001,
  Csm_ModbusAddr_ReadConf = 39001,

  Csm_MdobusAddr_WriteAc = 40001,
  Csm_ModbusAddr_WriteDc = 41001,
  Csm_ModbusAddr_WritePncHeader = 45001,
  Csm_ModbusAddr_WritePncBody = 45011,
  Csm_ModbusAddr_WriteConf = 49001,
} Csm_ModbusAddrType;

typedef struct
{
  void (*ReadInputRegs)(uint16 Id, uint16 ReadAddr, uint16 NumOfRegs, uint16* OutMap, uint8* OutByteCount);
  void (*WriteHoldingRegs)(uint16 Id, uint16 WriteAddr, uint16 *Map, uint8 MapCount);
  void (*SetRelay)(boolean IsClose);
  void (*Logging)(const char* __restrict __format, ...);
} GenisCsm_HandlerType;

typedef struct Csm_Modbus_VasHeaderType
{
  uint16 VasMapCount;       // 32001
  uint16 VasEnabled;        // 30002
  uint16 VasState;          // 32003
  uint16 VasErrorCode;      // 32004
  uint16 VasReceivedCount;  // 32005
  uint16 Reserved[3];       // 30006, 30007, 30008
  uint16 VasDataChecksum;   // 30009
  uint16 VasLength;         //32010
} Csm_Modbus_VasHeaderType;

typedef struct
{
  ChargerStateType State;

  can_csm_charger_t ChargerBus;
  can_csm_secc_t SeccBus;

  Csm_Modbus_VasHeaderType VasHeader;

#if (GENIS_CSM_PNC_SUPPORTED == TRUE)
  GenisCsm_PncRequestType PncRequest;
  GenisCsm_PncResponseType PncResponse;
#endif

  GenisCsm_HandlerType UserHandler; /* TODO: 사용자 콜백 삽입 */
} GenisCsm_ChargerType;


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void GenisCsm_Charger_Init(GenisCsm_ChargerType* Charger);
void GenisCsm_Charger_MainFunction(GenisCsm_ChargerType* Charger);

#ifdef __cplusplus
}
#endif

#endif //CHARGER_SOURCE_CHARGER_H_
