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
 *  Filename: CAN.h
 *  Created: May 9, 2025
 *  Author: yeo
 *
 *********************************************************************************************************************/

#ifndef DRIVER_CAN_H_
#define DRIVER_CAN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "main.h"
#include "fdcan.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "../Genis/Charger/Platform_Types.h"
#include "../Genis/Charger/Charger.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#pragma pack(push,1)

typedef struct
{
  uint32 Id;
  uint8 Dlc;
  uint8 Data[8];
} Can_DataType;

#pragma pack(pop)
/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern FDCAN_RxHeaderTypeDef CanRxHeader;
extern uint8_t CanRxData[8];
extern FDCAN_TxHeaderTypeDef CanTxHeader;
extern uint8_t CanTxData[8];
extern volatile uint8_t CanRxFlag;
extern QueueHandle_t App_CanRxQueue;
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void FDCAN_Filter_Config_mask32_Init(void);
void CAN_Tx(uint32 Id, uint8 *Data);
void App_CanRxMainFunction(GenisCsm_ChargerType *Charger);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* DRIVER_CAN_H_ */
