/*
 * AtParser.h
 *
 *  Created on: Apr 29, 2025
 *      Author: yeo
 */

#ifndef DRIVER_ATPARSER_H_
#define DRIVER_ATPARSER_H_
#include "main.h"

typedef enum
{
	AtParser_State_Idle,
	AtParser_State_AtParsing, // "AT"데이터 입력 받았을 경우
	AtParser_State_AtData, // \r\n만나면 AT command 종료
	AtParser_State_AtParseFinished,
} eAtParser_StateType;

typedef struct
{
	eAtParser_StateType atParserState;
	uint8_t atBuffer[255];
	uint32_t atSize;
} AtParserType;

extern AtParserType userParser;
extern AtParserType esp32Parser;

void AtParser_Insert(AtParserType* atParser,uint8_t data);
uint8_t AtParser_IsAssembled(AtParserType* atParser);
uint8_t* Get_At_Buffer(AtParserType* atParser);
void AtParser_Reset(AtParserType* atParser);

#endif /* DRIVER_ATPARSER_H_ */
