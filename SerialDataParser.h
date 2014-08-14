/*
  FANCY HEADER GOES HERE
*/

#ifndef SERIALDATAPARSER_H
#define SERIALDATAPARSER_H

#define MAX_VALUE_COUNT 10

// #define DEBUG

#include "Arduino.h"

typedef void (* SerialDataParserFunction) (String*, int);

class SerialDataParser
{
  public:
    SerialDataParser(char sod, char eod, char delim)
    {
      startOfData = sod;
      endOfData = eod;
      delimiter = delim;
      
      commandParsersCount = 0;

      inCommand = false;
    };
    
    /**
     * Read the serial input data
     */
    void readSerialData();

    /**
     * Lower level function to read a single char at a time
     */
    void appendChar(char c);
    
    /**
     * Add a parser function for a command, the parser will take the array of values as a parameter
     */
    void addParser(String command, void (*commandFunction)(String*, int));

  private:
    char startOfData;
    char endOfData;
    char delimiter;
    
    bool inCommand;
    
    char serialBuffer[30];
        
    // Command parsers
    unsigned int commandParsersCount;
    String *commandParsersLookup;
    SerialDataParserFunction *commandParsers;
    
    int lookupParserIndex(String cmd);
    void parseCommand();
    int indexOf(char *str, char c);
};

#endif
