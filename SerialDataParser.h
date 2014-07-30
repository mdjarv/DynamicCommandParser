#ifndef SERIALDATAPARSER_H
#define SERIALDATAPARSER_H

// #define DEBUG

#define MAX_COMMANDS 10

#include "Arduino.h"

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
      
      inCommand = false;
    };
    
    /**
     * Read the serial input data
     */
    void readSerialData();
    
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
    String commandParsersLookup[MAX_COMMANDS];
    void (*commandParsers[MAX_COMMANDS])(String*, int);
    int lookupParserIndex(String cmd);
    
    void parseCommand();
    int countValuesInBuffer();
};

#endif
