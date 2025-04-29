/*
 * AtController.h
 *
 *  Created on: Apr 29, 2025
 *      Author: yeo
 */

#ifndef CONTROLLER_ATCONTROLLER_H_
#define CONTROLLER_ATCONTROLLER_H_
#include "main.h"
#include "../Driver/AtParser.h"

// MainState: Idle, ReceivedFromUser, ReceivedFromESP,
typedef enum
{
	MainState_Idle,
	MainState_RecevFromUser,
	MainState_RecvFromESP
} eMainStateType;

void At_Controller_Init(void);
extern uint8_t uart2RxFlag;
extern uint8_t uart2RxBuff[50];



#endif /* CONTROLLER_ATCONTROLLER_H_ */
