Serial Data Parser for Arduino
==============================
_Created by Mathias Djärv, June 31, 2014._  
_Released under Creative Commons Attribution 4.0 International (CC BY 4.0)_  
_http://creativecommons.org/licenses/by/4.0/_

An Arduino library for simple M2M communicaton


Description
-----------

This library makes it easy to parse serial commands with (or without) variables.

Start, stop and delimiter characters are configurable

Simple usage example
--------------------

```Arduino
#include <SerialDataParser.h>

SerialDataParser sdp('^', '$', ',');

void myParser(String *values, int valueCount)
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
    sdp.readSerialData();
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

`^CMD,v1,v2$ ^CMD,v3,v4$ ^CMD$ ^foo$`

The above example will also trigger an error message as the `foo` command does not have a parser. The error message will look like this:

`^ERROR,No valid parser found for foo$`

The *values memory will be deleted by SerialDataParser after the function has exited

Command Timeout
---------------
The library also features a command timeout.

If a start-of-command has been read (but not the end-of-command) and no serial activity has ocurred within a second it will discard the buffer and ignore any following end-of-command on the next serial event.
