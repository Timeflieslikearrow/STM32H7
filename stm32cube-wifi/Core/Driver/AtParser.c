#include "AtParser.h"



void AtParser_Insert(AtParserType *atParser, uint8_t data)
{
  switch (atParser->atParserState)
  {
    case AtParser_State_Idle:
      if (atParser->atSize == 0)
      {
        if (data == 'A' || data == 'a')
        {
          atParser->atBuffer[atParser->atSize] = data;
          atParser->atSize++;
          atParser->atParserState = AtParser_State_AtParsing;
        }
      }
      break;
    case AtParser_State_AtParsing:
      if (atParser->atSize == 1)
      {
        // parser already has word 'a' or 'A'
        if (data == 'T' || data == 't')
        {
          atParser->atBuffer[atParser->atSize] = data;
          atParser->atSize++;
          atParser->atParserState = AtParser_State_AtData;
        }
        else
        {
          atParser->atParserState = AtParser_State_Idle;
          atParser->atSize = 0;
          memset(atParser->atBuffer, 0, sizeof(atParser->atBuffer));
          atParser->atParserState = AtParser_State_Idle;
        }
      }
      break;
    case AtParser_State_AtData:
      if (data == '\n')
      {
        // End of AT Command
        atParser->atBuffer[atParser->atSize] = data;
        atParser->atSize++;
        // 종료 시 null-terminate
        atParser->atBuffer[atParser->atSize] = '\0';
        atParser->atParserState = AtParser_State_AtParseFinished;
      }
      else
      {
        atParser->atBuffer[atParser->atSize] = data;
        atParser->atSize++;
        atParser->atParserState = AtParser_State_AtData;
      }
      break;
    case AtParser_State_AtParseFinished:
      // read buffer

      break;
  }
}

uint8_t* Get_At_Buffer(AtParserType *atParser)
{
  return (atParser->atSize > 0) ? atParser->atBuffer : NULL;
}

uint8_t AtParser_IsAssembled(AtParserType *atParser)
{
  if (atParser->atParserState == AtParser_State_AtParseFinished)
  {
    return 1;
  }
  return 0;
}

void AtParser_Reset(AtParserType *atParser)
{
  atParser->atParserState = AtParser_State_Idle;
  atParser->atSize = 0;
  memset(atParser->atBuffer, 0, sizeof(atParser->atBuffer));
}
