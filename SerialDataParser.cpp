#include "SerialDataParser.h"
#include "Arduino.h"

void SerialDataParser::appendChar(char c)
{
  // Check if character is start or end of command
  if(c == startOfData)
  {
    inCommand = true;
    serialBuffer[0] = '\0';
  }
  else if (c == endOfData)
  {
    if(inCommand)
    {
      parseCommand();
    }
    
    inCommand = false;
  }
  else
  {
    if(inCommand)
    {
      int len = strlen(serialBuffer);
      serialBuffer[len] = c;
      serialBuffer[len+1] = '\0';
    }
  }
}

void SerialDataParser::readSerialData()
{
  if(Serial.available())
  {
    while(Serial.available() > 0)
    {
      char c = Serial.read();

      appendChar(c);
    }
  }
}

void SerialDataParser::parseCommand()
{
  #ifdef DEBUG
  Serial.print("Parsing command: ");
  Serial.println(serialBuffer);
  #endif

  int parserIndex;

  if(indexOf(serialBuffer, delimiter) <= 0)
  {
    parserIndex = lookupParserIndex(serialBuffer);

    if(parserIndex >= 0)
    {
      String *valueArray = new String[1];
      valueArray[0] = serialBuffer;
      commandParsers[parserIndex](valueArray, 1);
      delete[] valueArray;
    }
  }
  else
  {
    // Split values into array
    String *valueArray = new String[MAX_VALUE_COUNT];
    
    int valueIndex = 0;

    char * pch;
    pch = strtok (serialBuffer,",");
    while (pch != NULL)
    {
      valueArray[valueIndex++] = pch;
      pch = strtok (NULL, ",");
    }

    parserIndex = lookupParserIndex(valueArray[0]);
    
    #ifdef DEBUG
    Serial.print("parserIndex = ");
    Serial.println(parserIndex);
    #endif
    
    if(parserIndex >= 0)
    {
      commandParsers[parserIndex](valueArray, valueIndex);
    }
    delete[] valueArray;
  }
}

int SerialDataParser::indexOf(char *str, char c)
{
  char *ptr = strchr(str, (int)c);
  if(ptr)
    return ptr - str;

  return -1;
}

int SerialDataParser::lookupParserIndex(String cmd)
{
  int index = -1;
  for(int i = 0; i < commandParsersCount; i++)
  {
    if(commandParsersLookup[i] == NULL)
    {
      break;
    }
    if(cmd == commandParsersLookup[i])
    {
      return i;
    }
  }
  
  return index;
}

void SerialDataParser::addParser(String command, SerialDataParserFunction commandFunction)
{
  SerialDataParserFunction *newParserList = new SerialDataParserFunction[commandParsersCount+1];
  String *newParserListLookup = new String[commandParsersCount+1];

  // Copy old parser list to new
  for(int i = 0; i < commandParsersCount; i++)
  {
    newParserList[i] = commandParsers[i];
    newParserListLookup[i] = commandParsersLookup[i];
  }

  // Append new parser
  newParserList[commandParsersCount] = commandFunction;
  newParserListLookup[commandParsersCount] = command;

  // Delete old parser list
  delete [] commandParsers;

  // Replace with new list
  commandParsersCount++;
  commandParsers = newParserList;
  commandParsersLookup = newParserListLookup;

  #ifdef DEBUG
  Serial.print("Now handling ");
  Serial.print(commandParsersCount);
  Serial.println(" parsers");
  #endif
}
