#include "AT.h"

AtCmdBuf atCmdBuf1 = {{0,},0};
AtCmdBuf atCmdBuf2 = {{0,},0};


// 수신 버퍼에 쌓인 데이터가 "\r\n" 으로 끝났는지 검사
uint8_t isAssembled(const AtCmdBuf *at){
	if(at -> len < 2) return 0;
	return (at->ATbuf[at->len-2] == '\r' && at->ATbuf[at->len-1] == '\n');
}

// AT 명령 문자열(cmdStr)에 CRLF를 붙여 전송 버퍼에 조립
void assembleAtCommand(const char *cmdStr, AtCmdBuf *at){

}

uint8_t receiveAtCommand(AtCmdBuf* at,uint8_t data){
	if(at->len >= AT_CMD_MAX_LEN){
		at->len = 0;
	}
	// 계속 메시지 받는다
	at->ATbuf[at->len++] = data;
	at->ATbuf[at->len] = '\0';

	if(isAssembled(at)){
		return 1;
	}

	return 0;
}
