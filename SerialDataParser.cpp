#include "SerialDataParser.h"

void SerialDataParser::addParser(char *cmd, ParserFunction function)
{
  mParserLookupSize++;
  mParserLookup = (ParserFunctionLookup*)realloc(mParserLookup, (mParserLookupSize) * sizeof(ParserFunctionLookup));
  mParserLookup[mParserLookupSize-1].command = cmd;
  mParserLookup[mParserLookupSize-1].function = function;
}

void SerialDataParser::append(char *str)
{
  for(int i = 0; i < strlen(str); i++)
  {
    appendChar(str[i]);
  }
}

void SerialDataParser::appendChar(char c)
{
  int bufferLength = strlen(buffer);

  if(c == mStart)
  {
    mInCommand = true;
    buffer[0] = 0;
    return;
  }
  else if(c == mEnd)
  {
    parseBuffer();
    buffer[0] = '\0';
    mInCommand = false;
  }
  else if(mInCommand)
  {
    buffer[bufferLength] = c;
    buffer[bufferLength+1] = '\0';
  }
}

void SerialDataParser::parseBuffer()
{
  // Split buffer
  int partCount = getBufferPartCount();
  char **parts = (char**)malloc(partCount * sizeof(char*));

  parts[0] = buffer;
  int currentPart = 0;

  for(int i = 0; buffer[i] != 0; i++)
  {
    if(buffer[i] == mDelimiter)
    {
      buffer[i] = 0;
      currentPart++;
      parts[currentPart] = &buffer[i+1];
    }
  }

  for(int i = 0; i < mParserLookupSize; i++)
  {
    if(strcmp(mParserLookup[i].command, parts[0]) == 0)
    {
      mParserLookup[i].function(parts, partCount);
      break;
    }
  }

  free(parts);
}

int SerialDataParser::getBufferPartCount()
{
  int count = 1;
  for(int i = 0; i < strlen(buffer); i++)
  {
    if(buffer[i] == mDelimiter)
      count++;
  }
  return count;
}
