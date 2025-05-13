#include "AtController.h"
#include "../Driver/AtParser.h"
#include "main.h"

// MainState: Idle, ReceivedFromUser, ReceivedFromESP,
typedef enum
{
  MainState_Idle,
  MainState_RecevFromUser,
  MainState_RecvFromESP
} eMainStateType;

static eMainStateType mainState;
static uint8_t *atCmd;

static AtParserType userParser = { AtParser_State_Idle, { 0, }, 0 };
static AtParserType esp32Parser = { AtParser_State_Idle, { 0, }, 0 };
static uint8_t uart1RxData;
static uint8_t uart1RxFlag;
static uint8_t uart1RxTail = 0;
static uint8_t uart1RxBuff[50];
static uint8_t uart2RxData;
static uint8_t uart2RxFlag;
static uint8_t uart2RxTail = 0;
static uint8_t uart2RxBuff[50];

typedef struct {
  eMainStateType mainState;
  uint8_t *atCmd;

  AtParserType userParser;
  AtParserType esp32Parser;

  uint8_t uart1RxData;
  uint8_t uart1RxFlag;
  uint8_t uart1RxTail = 0;
  uint8_t uart1RxBuff[50];
  uint8_t uart2RxData;
  uint8_t uart2RxFlag;
  uint8_t uart2RxTail = 0;
  uint8_t uart2RxBuff[50];
} At_ControllerType;

static At_ControllerType Controller;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    AtParser_Insert(&userParser, uart1RxData);
    HAL_UART_Receive_IT(&huart1, &uart1RxData, 1);
  }
  else if (huart->Instance == USART2)
  {
    // Queue the message until a newline character('\n') appears.
    // AT responses end with \r\n. ('\r':0x0d, '\n':0x0a)
    if (uart2RxData == 0x0a)
    {
      uart2RxBuff[uart2RxTail] = uart2RxData;  // put 0x0a('\n')
      uart2RxTail++;
      uart2RxBuff[uart2RxTail] = '\0';
      uart2RxTail = 0;
      uart2RxFlag = 1;
    }
    else
    {
      uart2RxBuff[uart2RxTail] = uart2RxData;
      uart2RxTail++;
    }
    HAL_UART_Receive_IT(&huart2, &uart2RxData, 1);
  }
}

void At_Controller_Init(void)
{
  mainState = MainState_Idle;
}

void At_Controller_MainFunction(void)
{
  switch (mainState)
  {
    case MainState_Idle:
      if (AtParser_IsAssembled(&userParser))
      {
        mainState = MainState_RecevFromUser;
      }
      else if (uart2RxFlag)
      {
        mainState = MainState_RecvFromESP;
      }
      break;
    case MainState_RecevFromUser:  // get AT Command From User
      atCmd = Get_At_Buffer(&userParser);
      if (atCmd != NULL)
      {
        HAL_UART_Transmit(&huart2, atCmd, strlen(atCmd), 1000);
        AtParser_Reset(&userParser);
        mainState = MainState_Idle;
      }
      break;
    case MainState_RecvFromESP:  // send AT response from ESP
      uart2RxFlag = 0;
      HAL_UART_Transmit(&huart1, uart2RxBuff, strlen(uart2RxBuff), 1000);
      mainState = MainState_Idle;
      break;
  }
}
