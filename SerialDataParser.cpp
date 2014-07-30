#include "SerialDataParser.h"
#include "Arduino.h"

void SerialDataParser::readSerialData()
{
  if(Serial.available())
  {
    if(millis() - latestSerialEvent >= latestSerialEventTimeout)
    {
      // If latest serial event has timed out, clear the serial buffer
      serialBuffer = "";
      inCommand = false;
    }
    
    while(Serial.available() > 0)
    {
      char c = Serial.read();

      // Check if character is start or end of command
      if(c == startOfData)
      {
        inCommand = true;
        serialBuffer = "";
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
          serialBuffer += c;
        }
      }
    }
    latestSerialEvent = millis();
  }
}

void SerialDataParser::parseCommand()
{
  #ifdef DEBUG
  Serial.print("Parsing command: ");
  Serial.println(serialBuffer);
  #endif
  
  // Split into array
  int parserIndex;
  int valueCount = countValuesInBuffer();
  
  #ifdef DEBUG
  Serial.print("valueCount = ");
  Serial.println(valueCount);
  #endif
  
  String *valueArray = new String[valueCount];
  
  if(valueCount == 1)
  {
    parserIndex = lookupParserIndex(serialBuffer);
    valueArray[0] = serialBuffer;

    #ifdef DEBUG
    Serial.print("parserIndex = ");
    Serial.println(parserIndex);
    #endif

    if(parserIndex < 0)
    {
      error("No valid parser found for " + valueArray[0]);
    }
    else
    {
      commandParsers[parserIndex](valueArray, valueCount);
    }
  }
  else
  {
    // Split values into array
    
    int separator = serialBuffer.indexOf(delimiter);
    int lastSeparator = 0;
    int valueIndex = 0;

    while(separator > 0)
    {
      valueArray[valueIndex++] = serialBuffer.substring(lastSeparator, separator);
      lastSeparator = separator+1;
      separator = serialBuffer.indexOf(delimiter, separator+1);
    }

    valueArray[valueIndex] = serialBuffer.substring(lastSeparator);
    
    
    parserIndex = lookupParserIndex(valueArray[0]);
    
    #ifdef DEBUG
    Serial.print("parserIndex = ");
    Serial.println(parserIndex);
    #endif
    
    if(parserIndex < 0)
    {
      error("No valid parser found for " + valueArray[0]);
    }
    else
    {
      commandParsers[parserIndex](valueArray, valueCount);
    }
  }

  delete[] valueArray;
}

void SerialDataParser::error(String message)
{
  Serial.print(startOfData);
  Serial.print("ERROR");
  Serial.print(delimiter);
  Serial.print(message);
  Serial.println(endOfData);
}

int SerialDataParser::countValuesInBuffer()
{
  int valueCount = 1;
  int index = 0;
  while((index = serialBuffer.indexOf(delimiter, index+1)) > 0)
  {
    valueCount++;
  }
  return valueCount;
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
    if(commandParsersLookup[i] == cmd)
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
