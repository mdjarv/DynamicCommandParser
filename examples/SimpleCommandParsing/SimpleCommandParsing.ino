/*
  Usage example for SerialDataParser

  In this example we initialize the SerialDataParser with the following control characters:
    Start of command:   ^
    End of command:     $
    Variable delimiter: ,

  We also set up two parsers: COP and MVP
    commandOnlyParser (COP) - Handle a single word command
    multipleVariableParser (MVP) - Handles a command with multiple variables

  To execute the commandOnlyParser function we send the following over the serial port:
    ^COP$

  To execute multipleVariableParser with a few random variables:
    ^MVP,var 1,var 2,var 3,and,a,few,more,variables$
*/ 
#include <SerialDataParser.h>

// Initialize the data parser using the start, end and delimiting character
SerialDataParser sdp('^', '$', ',');

// Parser for the COP command
void commandOnlyParser(String *values, int valueCount)
{
  if(valueCount > 1)
    sdp.error("commandOnlyParser does not like variables");
  else
    Serial.println("commandOnlyParser executed successfully");
}

// Parser for the MVP command
void multipleVariableParser(String *values, int valueCount)
{
  Serial.println("multipleVariableParser:");
  for(int i = 0; i < valueCount; i++)
  {
    Serial.print("  values[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(values[i]);
  }
}

void setup() {
  Serial.begin(57600);

  // Add the two parser commands to the SerialDataParser
  sdp.addParser("COP", commandOnlyParser);
  sdp.addParser("MVP", multipleVariableParser);

  Serial.println("^READY$");
}

void loop() {
  unsigned long start;
  unsigned long finish;
  
  start = millis();
  sdp.readSerialData();
  finish = millis()-start;

  if(finish > 1)
  {
    Serial.print("readSerialData() took ");
    Serial.print(finish);
    Serial.println(" ms");
  }
  delay(100);
}
