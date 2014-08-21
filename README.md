Serial Data Parser for Arduino
==============================
_Created by Mathias Djärv, June 31, 2014._  
_Released under Creative Commons Attribution 4.0 International (CC BY 4.0)_  
_http://creativecommons.org/licenses/by/4.0/_

An Arduino library for simple M2M communicaton

Description
-----------

This library makes it easy to parse serial commands with (or without) variables.

Start, stop and delimiter characters are configurable, and the command buffer size is currently set to 64 characters.

Simple usage example
--------------------

```c++
#include <SerialDataParser.h>

SerialDataParser sdp('^', '$', ',');

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
    
    sdp.addParser("CMD", myParser);
}

void loop()
{
  while(Serial.available())
  {
    sdp.appendChar(Serial.read());
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

However it is worth noting that not using linebreaks at the end of sending may cause your serial connection to not automatically flush its buffers which will cause a delay before the Arduino catches the string. This can be avoided by explicitly flushing the buffers after sending.