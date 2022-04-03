/*
  main.cpp

  @author:    Henrique Vilela
  @version:   1.0
  @date:      2022-04-02

  @description:
    Example on minimum usage of ESP8266 Web Update Library.

*/

#include "WebUpdate.h"

void setup() {
  WebUpdateSetup(192,168,15,199);
}

void loop() {
  WebUpdateLoop();
}