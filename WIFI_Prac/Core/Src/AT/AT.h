/*
 * AT.h
 *
 *  Created on: Apr 28, 2025
 *      Author: yeo
 */

#ifndef SRC_AT_AT_H_
#define SRC_AT_AT_H_
#include "main.h"

#define AT_CMD_MAX_LEN 50    // 최대 AT 명령 길이
#define AT_PARAM_MAX    10    // 최대 파라미터 개수

typedef struct {
    uint8_t ATbuf[AT_CMD_MAX_LEN];
    uint16_t len;
} AtCmdBuf;

extern AtCmdBuf atCmdBuf1;
extern AtCmdBuf atCmdBuf2;

// 수신 버퍼에 쌓인 데이터가 "\r\n" 으로 끝났는지 검사
uint8_t isAssembled(const AtCmdBuf *b);


#endif /* SRC_AT_AT_H_ */
