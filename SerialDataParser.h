/*
  FANCY HEADER GOES HERE
*/

#ifndef SERIALDATAPARSER_H
#define SERIALDATAPARSER_H

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
      
      latestSerialEvent = 0;
      latestSerialEventTimeout = 1000;
      
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

    /**
     * Shorthand function to print error message with the correct start, end and delimiter.
     * Example output:
     *   ^ERROR,TEST was not a valid command$
     */
    void error(String message);
    
  private:
    char startOfData;
    char endOfData;
    char delimiter;
    
    unsigned long latestSerialEvent;
    unsigned long latestSerialEventTimeout;
    
    bool inCommand;
    
    String serialBuffer;
        
    // Command parsers
    unsigned int commandParsersCount;
    String *commandParsersLookup;
    SerialDataParserFunction *commandParsers;
    
    int lookupParserIndex(String cmd);
    
    void parseCommand();
    int countValuesInBuffer();
};

#endif
