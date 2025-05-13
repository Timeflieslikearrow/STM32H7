#pragma once
#include <stdint.h>

typedef struct
{
  uint8_t SN;
  uint8_t ExpectedSN;
  uint8_t Data[512];
  uint16_t Index;
  uint16_t Length;
} CanTp_Type;
