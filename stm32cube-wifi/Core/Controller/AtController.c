#include "AtController.h"

eMainStateType mainState;
uint8_t *atCmd;

void At_Controller_Init(void)
{
  mainState = MainState_Idle;
}

void At_Controller(void)
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
