#include "DynamicCommandParser.h"

void DynamicCommandParser::addParser(char *cmd, ParserFunction function)
{
  mParserLookupSize++;
  mParserLookup = (ParserFunctionLookup*)realloc(mParserLookup, (mParserLookupSize) * sizeof(ParserFunctionLookup));
  mParserLookup[mParserLookupSize-1].command = cmd;
  mParserLookup[mParserLookupSize-1].function = function;
}

void DynamicCommandParser::append(char *str)
{
  for(size_t i = 0; i < strlen(str); i++)
  {
    appendChar(str[i]);
  }
}

void DynamicCommandParser::appendChar(char c)
{
  size_t bufferLength = strlen(buffer);

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

void DynamicCommandParser::parseBuffer()
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

  for(size_t i = 0; i < mParserLookupSize; i++)
  {
    if(strcmp(mParserLookup[i].command, parts[0]) == 0)
    {
      mParserLookup[i].function(parts, partCount);
      break;
    }
  }

  free(parts);
}

int DynamicCommandParser::getBufferPartCount()
{
  int count = 1;
  for(size_t i = 0; i < strlen(buffer); i++)
  {
    if(buffer[i] == mDelimiter)
      count++;
  }
  return count;
}
