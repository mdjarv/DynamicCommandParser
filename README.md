DynamicCommandParser
====================
_Created by Mathias Djärv, June 31, 2014._  
_Released under Creative Commons Attribution 4.0 International (CC BY 4.0)_  
_http://creativecommons.org/licenses/by/4.0/_

A library for simple M2M command and parameter communicaton

Description
-----------

This library makes it easy to parse command strings with (or without) variables.

Start, stop and delimiter characters are configurable, and the command buffer size is currently set to 64 characters.

Simple usage example
--------------------

```c++
#include <DynamicCommandParser.h>

DynamicCommandParser dcp('^', '$', ',');

void myParser(char **values, int valueCount)
{
    
    Serial.println("myParser:");
    for(int i = 1; i < valueCount; i++)
    {
        Serial.print("  ");
        Serial.println(values[i]);
    }
}

void setup()
{
    Serial.begin(9600);
    
    dcp.addParser("CMD", myParser);
}

void loop()
{
  while(Serial.available())
  {
    dcp.appendChar(Serial.read());
  }
}

```

Open a serial terminal and send in the string `^CMD,val 1,val 2,val 3$` and you should see the following output:

```
myParser:
  val 1
  val 2
  val 3
```

Since the library does not care if you use linebreaks or not you can string multiple commands on one row:

`^CMD,v1,v2$ ^CMD,v3,v4$ ^CMD$`
