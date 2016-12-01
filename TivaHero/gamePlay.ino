#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/eeprom.h"

#define E2PROM_TEST_ADRES 0x0000 
const int HITBAR_X = 120;
const int LEFT_Y = 22 ;
const int CENTER_Y = 12;
const int RIGHT_Y = 2;
const int updateFrequency = 50; //Decrease the value to make the notes move faster
int difficultyLevel = 1;
struct note {
  int x; // 
  int column; //Value between 0 an 2
  int fallVelocity; //How many y values to move it per iteration]
  bool hit;
};






struct note notes[20]; //Maximum 20 notes on the screen at once4
bool arrayOccupied[20] = {false};
int activeNotes = 0;
int score = 0;
int hitStreak = 0;
int life = 100;
char sprite[8] = { //8 x 8 character
	 0xFF, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
char hitbar[16] = { 
   0xFF, 0x01, 0x01, 0xFF,
   0xFF, 0x00, 0x00, 0xFF, 
   0xFF, 0x00, 0x00, 0xFF,
   0xFF, 0x40, 0x40, 0xFF
};
char shortNote[4]= { //4 x 8 note
  0x7F, 0x7F, 0x7F, 0x7F
};

char blankScreen[] = {
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40};

static void playGameInit(int difficulty) {
  difficultyLevel = difficulty;
  activeNotes = 0;
  score = 0;
  hitStreak = 0;
  life = 100;
  for (int i=0 ; i< 20; i++) {
    arrayOccupied[i] = false;
  }
  
}

static void clearScreen(){
	OrbitOledSetCursor(0, 0);
	OrbitOledPutBmp(128, 32, blankScreen);
}

static void createNote(int type, int noteLength,int velocity) { //Type is an int that defines which type of note it is
															//The 
  for (int i = 0; i < 20; i++){
  	if (! arrayOccupied[i]){
  		int columnNumber = random(0,3); //
		  if (type == 0){ //One single note
		  	struct note newNote = {.x = 0, .column = columnNumber, .fallVelocity = velocity * difficultyLevel, .hit = false};
        notes[i] = newNote;
		  }
		  else if (type == 1){ //Two notes at once

		  }
		  else { //All 3 notes

		  }
		  activeNotes++;
		  arrayOccupied[i] = true;
      break;
	  	}
  }
  
}

static void checkHit(){
  //Serial.print(gameInputState.buttons[3].isRising);
  //Serial.print(gameInputState.buttons[0].isRising);
  //Serial.print(gameInputState.buttons[1].isRising);
  //Serial.print("\n");
  int healthChange = 0;
	if (gameInputState.buttons[3].isRising){ //Left
    bool miss = true;
    for (int i = 0; i < 20; i++) {
      if (arrayOccupied[i] && notes[i].column == 0){
        if (notes[i].x > 118 && notes[i].x < 122){ //Highest accuracy
          score += 300;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 2;
        }
        else if (notes[i].x > 116 && notes[i].x < 124){//Somewhat accurate
          score += 200;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 1;
        }
        else if (notes[i].x > 114 && notes[i].x < 126) {//Barely hit the note
          score += 50;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
        }
      }
    }
    if (miss) {
      Serial.print("Miss\n");
      hitStreak = 0;
      healthChange -= 5;
    }
	}
 
  if (gameInputState.buttons[0].isRising){ //Center
    bool miss = true;
    for (int i = 0; i < 20; i++) {
      if (arrayOccupied[i] && notes[i].column == 1){
        if (notes[i].x > 118 && notes[i].x < 122){
          score += 300;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 2;
        }
        else if (notes[i].x > 116 && notes[i].x < 124){
          score += 200;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 1;
        }
        else if (notes[i].x > 114 && notes[i].x < 126) {
          score += 50;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
        }
      }
    }
    if (miss) {
      Serial.print("Miss\n");
      hitStreak = 0;
      healthChange -= 5;
    }
  }
  
  if (gameInputState.buttons[1].isRising){ //Right
    bool miss = true;
    for (int i = 0; i < 20; i++) {
      if (arrayOccupied[i] && notes[i].column == 2){
        if (notes[i].x > 118 && notes[i].x < 122){
          score += 300;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 2;
        }
        else if (notes[i].x > 116 && notes[i].x < 124){
          score += 200;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 1;
        }
        else if (notes[i].x > 114 && notes[i].x < 126) {
          score += 50;
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
        }
      }
    }
    if (miss) {
      hitStreak = 0;
      Serial.print("Miss\n");
      healthChange -= 5;
    }
  }
  if (life + healthChange <= 100){
    if (life + healthChange > 0){
      life += healthChange;
    }
    else {
      life = 0;
      
      struct E2PROM e2prom_write_value = {0}; /* Write struct */
      struct E2PROM e2prom_read_value =  {0}; /* Read struct */
      uint32_t e2size,e2block,returnCode;
      SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // EEPROM activate
    
    do
    {
        returnCode=EEPROMInit(); // EEPROM start
    }
    while(returnCode!=EEPROM_INIT_OK);
    
    
    /*******************************/

 
 
    e2size = EEPROMSizeGet(); // Get EEPROM Size 
    
 
    e2block = EEPROMBlockCountGet(); // Get EEPROM Block Count
  
  EEPROMRead((uint32_t *)&e2prom_read_value, E2PROM_TEST_ADRES, sizeof(e2prom_read_value)); //Read from struct at EEPROM start from 0x0000
   if(score > e2prom_read_value.value1) {
    e2prom_write_value.value1 = score;
      EEPROMProgram((uint32_t *)&e2prom_write_value, E2PROM_TEST_ADRES, sizeof(e2prom_write_value)); //Write struct to EEPROM start from 0x0000

      }

  EEPROMRead((uint32_t *)&e2prom_read_value, E2PROM_TEST_ADRES, sizeof(e2prom_read_value)); //Read from struct at EEPROM start from 0x0000

            OrbitOledClear();
          char str[1];
          OrbitOledMoveTo(30,0);
          OrbitOledDrawString("Game Over");
          OrbitOledMoveTo(30,20);

          sprintf(str, "Score:%d", score);
         
          OrbitOledDrawString(str);
            currentState = Welcome;
            menuDisplay = 0;
    }
  }
  else {
    life = 100;
  }
}

static void updateNoteLocation() { //This function iterates through the array of notes that we have and updates their location
							                     //Note that this is not drawing the notes, it simply changes their x and y coordinates
  for (int i = 0; i < 20; i++) {
  	if (arrayOccupied[i]){
  		notes[i].x = notes[i].x + notes[i].fallVelocity;
  		if (notes[i].x > 128){ //If statement which checks if the note is off the screen, then we can delete it.
  			arrayOccupied[i] = false;
  			activeNotes--;
        if (!notes[i].hit) {
          hitStreak = 0;
          Serial.print("Lost note\n");
          if (life - 5 > 0){
            life -= 5;
          }
          else {
            Serial.print("You have died\n");
            life = 0;
          }
        }
  		}
  	}
  }
}

static void drawNotes() {
  for (int i = 0; i < 20; i++) {
    if (arrayOccupied[i]){
      if (notes[i].column == 0){
        OrbitOledMoveTo(notes[i].x, LEFT_Y);
        OrbitOledPutBmp(4,8,shortNote);
  
      }
      else if (notes[i].column == 1){
        OrbitOledMoveTo(notes[i].x, CENTER_Y);
        OrbitOledPutBmp(4,8,shortNote);
  
      }
      else {
        OrbitOledMoveTo(notes[i].x, RIGHT_Y);
        OrbitOledPutBmp(4,8,shortNote);
  
      }
    }
    
    
    //If statement which checks if the note is off the screen, then we can delete it.
  }
}

static void render() { //Main function that does the clearing and drawing of the OLED
    clearScreen();
    OrbitOledMoveTo(HITBAR_X,0);
    OrbitOledPutBmp(4,32, hitbar);
    
    if (millis() % 300 == 0){
      if (activeNotes < 20){
        createNote(0, 1, 3); 
      }
      //Serial.print(notes[0].x);
      //Serial.print("\n.");
    
    }
    updateNoteLocation();
    drawNotes();
    

    
    /*
    OrbitOledMoveTo(2,LEFT_Y);
    OrbitOledPutBmp(4,8,shortNote);

    OrbitOledMoveTo(2,CENTER_Y);
    OrbitOledPutBmp(4,8,shortNote);

    OrbitOledMoveTo(2,RIGHT_Y);
    OrbitOledPutBmp(4,8,shortNote);
    */
}

static void updateScoreStreak() {
  /*
  Serial.print(score); Serial.print(" ");
  Serial.print(hitStreak);Serial.print(" ");
  Serial.print(life);Serial.print(" ");
  Serial.print("\n");
  */
  //Using the Orbit's LEDs to display life stats
  if (life > 80) { //All are lit
    GPIOPinWrite(LED4Port, LED4, LED4);
    GPIOPinWrite(LED3Port, LED3, LED3);
    GPIOPinWrite(LED2Port, LED2, LED2);
    GPIOPinWrite(LED1Port, LED1, LED1);
  }
  else if (life > 60){ //3 out of 4 are lit
    GPIOPinWrite(LED4Port, LED4, LOW);
    GPIOPinWrite(LED3Port, LED3, LED3);
    GPIOPinWrite(LED2Port, LED2, LED2);
    GPIOPinWrite(LED1Port, LED1, LED1);
  }
  else if (life > 40){//2 out of 4 are lit
    GPIOPinWrite(LED4Port, LED4, LOW);
    GPIOPinWrite(LED3Port, LED3, LOW);
    GPIOPinWrite(LED2Port, LED2, LED2);
    GPIOPinWrite(LED1Port, LED1, LED1);
  }
  else if (life > 10){//1 out of 4 are lit
    GPIOPinWrite(LED4Port, LED4, LOW);
    GPIOPinWrite(LED3Port, LED3, LOW);
    GPIOPinWrite(LED2Port, LED2, LOW);
    GPIOPinWrite(LED1Port, LED1, LED1);
  }
  else {//All lights are off
    GPIOPinWrite(LED4Port, LED4, LOW);
    GPIOPinWrite(LED3Port, LED3, LOW);
    GPIOPinWrite(LED2Port, LED2, LOW);
    GPIOPinWrite(LED1Port, LED1, LOW);
  }

  if (hitStreak > 2){
      digitalWrite(LEDGREEN, HIGH);
      
/*
    else {
      digitalWrite(LEDBLUE, LOW);
    }*/
  }
  else if (hitStreak > 4){
      digitalWrite(LEDGREEN, LOW);
      digitalWrite(LEDBLUE, HIGH);
    /*
    else {
      digitalWrite(LEDBLUE, LOW);
      digitalWrite(LEDGREEN, LOW);
    }*/
  }
  else if (hitStreak > 6){
      digitalWrite(LEDBLUE, LOW);
      digitalWrite(LEDRED, HIGH);
    /*
    else {
      digitalWrite(LEDBLUE, LOW);
      digitalWrite(LEDGREEN, LOW);
      digitalWrite(LEDRED, LOW);
    }*/
  }
  else {
    digitalWrite(LEDBLUE, LOW);
    digitalWrite(LEDGREEN, LOW);
    digitalWrite(LEDRED, LOW);
  }
  
}

