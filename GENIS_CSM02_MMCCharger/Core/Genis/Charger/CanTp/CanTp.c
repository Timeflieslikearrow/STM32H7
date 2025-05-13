#include "CanTp.h"

int CanTp_Parse(CanTp_Type* CanTp, uint64_t Data)
{
  uint8_t frameType = (Data >> 4) & 0x0f;
  if (frameType == 1)
  {
    CanTp->SN = 0;
    CanTp->Index = 0;
    CanTp->ExpectedSN = 0;
    CanTp->Length = ((Data & 0x0f) << 8) | ((Data >> 8) & 0xff);

    uint8_t* u8Ptr = &Data;
    memcpy(&CanTp->Data[CanTp->Index], &u8Ptr[2], 6);
    CanTp->Index += 6;

    return 1;
  }
  else if (frameType == 2)
  {
    CanTp->SN = Data & 0xf;
    ++CanTp->ExpectedSN;
    CanTp->ExpectedSN &= 0xf;

    if (CanTp->SN == CanTp->ExpectedSN)
    {
      uint8_t* u8Ptr = &Data;
      memcpy(&CanTp->Data[CanTp->Index], &u8Ptr[1], 7);
      CanTp->Index += 7;

      return 1;
    }
  }

  return 0;
}
