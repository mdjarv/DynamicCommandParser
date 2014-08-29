#ifndef SERIALDATAPARSER_H
#define SERIALDATAPARSER_H

#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64

typedef void (* ParserFunction)(char **values, int valueCount);

typedef struct
{
  char *command;
  ParserFunction function;
} ParserFunctionLookup;

class DynamicCommandParser
{
public:
  DynamicCommandParser(char start, char end, char delim)
  {
    mInCommand = false;
    mStart = start;
    mEnd = end;
    mDelimiter = delim;

    mParserLookup = NULL;
    mParserLookupSize = 0;
    buffer[0] = '\0';
  }

  ~DynamicCommandParser()
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
