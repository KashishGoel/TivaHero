#include "FillPat.h"
#include "Launchpad.h"
#include "OrbitBoosterPackDefs.h"
#include "OrbitOled.h"
#include "OrbitOledChar.h"
#include "OrbitOledGrph.h"


#include <stdlib.h>
#include <stdbool.h>

#define LEDRED RED_LED
#define LEDGREEN GREEN_LED
#define LEDBLUE BLUE_LED

const int MAX_ACTIVE_NOTES = 35;

void setup(){
  Serial.begin(9600);
  
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDBLUE, OUTPUT);

  delay(100);
  
  GameUIInit();
}

void loop(){
  GameUITick();
}

