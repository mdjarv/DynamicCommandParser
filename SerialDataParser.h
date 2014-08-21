#ifndef SERIALDATAPARSER_H
#define SERIALDATAPARSER_H

#include "Arduino.h"

#define BUFFER_SIZE 64

typedef void (* ParserFunction)(char **values, int valueCount);

typedef struct
{
  char *command;
  ParserFunction function;
} ParserFunctionLookup;

class SerialDataParser
{
public:
  SerialDataParser(char start, char end, char delim)
  {
    mInCommand = false;
    mStart = start;
    mEnd = end;
    mDelimiter = delim;

    mParserLookup = NULL;
    mParserLookupSize = 0;
    buffer[0] = '\0';
  }

  ~SerialDataParser()
  {
    free(mParserLookup);
  }

  void addParser(char *cmd, ParserFunction function);
  void append(char *str);
  void appendChar(char c);

private:
  bool mInCommand;
  char buffer[BUFFER_SIZE];
  char mStart;
  char mEnd;
  char mDelimiter;

  size_t mParserLookupSize;
  ParserFunctionLookup *mParserLookup;

  void parseBuffer();
  int getBufferPartCount();
};

#endif