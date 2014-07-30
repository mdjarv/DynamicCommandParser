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
  for(int i = 0; i < MAX_COMMANDS; i++)
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

void SerialDataParser::addParser(String command, void (*commandFunction)(String*, int))
{
  for(int i = 0; i < MAX_COMMANDS; i++)
  {
    if(commandParsers[i] == NULL)
    {
      #ifdef DEBUG
      Serial.print("Empty parser spot at ");
      Serial.println(i);
      #endif
      
      commandParsers[i] = commandFunction;
      commandParsersLookup[i] = command;
      
      break;
    }
  }
}
