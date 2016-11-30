#include <stdbool.h>
#include <stdlib.h>

const int HITBAR_X = 120;
const int LEFT_Y = 22 ;
const int CENTER_Y = 12;
const int RIGHT_Y = 2;
const int updateFrequency = 50; //Decrease the value to make the notes move faster

extern char beatMap[1165];

int startTime;
int gameTime;
int sec;
int secTick;
int previousIndex = -1;
int noteIndex = 0;

struct note {
  int x; // 
  int column; //Value between 0 an 2
  int fallVelocity; //How many y values to move it per iteration]
  bool hit;
};

struct note notes[MAX_ACTIVE_NOTES]; //Maximum 20 notes on the screen at once4
bool arrayOccupied[MAX_ACTIVE_NOTES] = {false};
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





static void playGameInit() {
  startTime = millis();
  activeNotes = 0;
  score = 0;
  hitStreak = 0;
  life = 100;
  for (int i=0 ; i< MAX_ACTIVE_NOTES; i++) {
    arrayOccupied[i] = false;
  }
  
}

static void clearScreen(){
	OrbitOledSetCursor(0, 0);
	OrbitOledPutBmp(128, 32, blankScreen);
}

static void createNoteRandom(int type, int noteLength,int velocity) { //Type is an int that defines which type of note it is
															//The 
  for (int i = 0; i < MAX_ACTIVE_NOTES; i++){
  	if (! arrayOccupied[i]){
  		int columnNumber = random(0,3); //
		  if (type == 0){ //One single note
		  	struct note newNote = {.x = 0, .column = columnNumber, .fallVelocity = velocity, .hit = false};
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

static void createNoteTimed(int velocity, float bpm){

  gameTime = millis() - startTime;
  //int noteInterval = 211.7;//(60 / bpm) * 1000;
  sec = gameTime / 1000;
  secTick = (gameTime - (sec*1000)) / 100;
  noteIndex = sec * 10 + secTick;
  if (noteIndex != previousIndex){
    previousIndex = noteIndex;
  //if (noteInterval * noteIndex < gameTime){ 
    //noteIndex += 1;
    //Serial.print(noteIndex);
    
    if (beatMap[noteIndex] == 0x01){
      for (int i = 0; i < MAX_ACTIVE_NOTES; i++){
        if (! arrayOccupied[i]){
          struct note newNote = {.x = 0, .column = 0, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          break;
        }
      }
    }
    else if (beatMap[noteIndex] == 0x02){
      for (int i = 0; i < MAX_ACTIVE_NOTES; i++){
        if (! arrayOccupied[i]){
          struct note newNote = {.x = 0, .column = 1, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          break;
        }
      }
    }
    else if (beatMap[noteIndex] == 0x03){
      for (int i = 0; i < MAX_ACTIVE_NOTES; i++){
        if (! arrayOccupied[i]){
          struct note newNote = {.x = 0, .column = 2, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          break;
        }
      }
    }
    else if (beatMap[noteIndex] == 0x04){
      int created = 0;
      for (int i = 0; i < MAX_ACTIVE_NOTES; i++){
        if (! arrayOccupied[i] && created == 0){
          struct note newNote = {.x = 0, .column = 0, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          created = 1;
        }
        else if (! arrayOccupied[i] && created == 1){
          struct note newNote = {.x = 0, .column = 1, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          break;
        }
      }
      Serial.print(4);
    }
    else if (beatMap[noteIndex] == 0x05){
      int created = 0;
      for (int i = 0; i < MAX_ACTIVE_NOTES; i++){
        if (! arrayOccupied[i] && created == 0){
          struct note newNote = {.x = 0, .column = 0, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          created = 1;
          //Serial.print(51);
        }
        else if (! arrayOccupied[i] && created == 1){
          struct note newNote = {.x = 0, .column = 2, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          //Serial.print(52);
          break;
        }
      }
    }
    else if (beatMap[noteIndex] == 0x06){
      Serial.print(6);
      int created = 0;
      for (int i = 0; i < MAX_ACTIVE_NOTES; i++){
        if (! arrayOccupied[i] && created == 0){
          struct note newNote = {.x = 0, .column = 1, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          created = 1;
        }
        else if (! arrayOccupied[i] && created == 1){
          struct note newNote = {.x = 0, .column = 2, .fallVelocity = velocity, .hit = false};
          notes[i] = newNote;
          activeNotes++;
          arrayOccupied[i] = true;
          break;
        }
      }
    }
    else {
      //Serial.print(0);
    }
  }

    // For creating beatmaps
    /*
    if (gameInputState.buttons[3].state && gameInputState.buttons[0].state){
      Serial.print(4); 
    }
    else if (gameInputState.buttons[3].state && gameInputState.buttons[1].state){
      Serial.print(5);
    }
    else if (gameInputState.buttons[0].state && gameInputState.buttons[1].state){
      Serial.print(6);
    }
    else if (gameInputState.buttons[3].state){
      Serial.print(1);
    }
    else if (gameInputState.buttons[0].state){
      Serial.print(2);
    }
    else if (gameInputState.buttons[1].state){
      Serial.print(3);
    }
    else{
      Serial.print(0);
    }
    Serial.print("\n");
  }*/
  

}

static void checkHit(){
  //Serial.print(gameInputState.buttons[3].isRising);
  //Serial.print(gameInputState.buttons[0].isRising);
  //Serial.print(gameInputState.buttons[1].isRising);
  //Serial.print("\n");
  int healthChange = 0;
	if (gameInputState.buttons[3].isRising){ //Left
    bool miss = true;
    for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
      if (arrayOccupied[i] && notes[i].column == 0){
        if (notes[i].x > 116 && notes[i].x < 120){ //Highest accuracy
          score += 300; //Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 2;
          arrayOccupied[i] = false;
          activeNotes --;
        }
        else if (notes[i].x > 114 && notes[i].x < 122){//Somewhat accurate
          score += 200;//Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 1;
          arrayOccupied[i] = false;
          activeNotes --;
        }
        else if (notes[i].x > 112 && notes[i].x < 124) {//Barely hit the note
          score += 50;//Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          arrayOccupied[i] = false;
          activeNotes --;
        }
      }
    }
    if (miss) {
      Serial.print("Miss\n");
      hitStreak = 0;
      healthChange -= 2;

    }
	}
 
  if (gameInputState.buttons[0].isRising){ //Center
    bool miss = true;
    for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
      if (arrayOccupied[i] && notes[i].column == 1){
        if (notes[i].x > 116 && notes[i].x < 120){
          score += 300;//Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 2;
          arrayOccupied[i] = false;
          activeNotes --;
        }
        else if (notes[i].x > 114 && notes[i].x < 122){
          score += 200;//Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 1;
          arrayOccupied[i] = false;
          activeNotes --;
        }
        else if (notes[i].x > 112 && notes[i].x < 124) {
          score += 50;//Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          arrayOccupied[i] = false;
          activeNotes --;
        }
      }
    }
    if (miss) {
      Serial.print("Miss\n");
      hitStreak = 0;
      healthChange -= 2;
    }
  }
  
  if (gameInputState.buttons[1].isRising){ //Right
    bool miss = true;
    for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
      if (arrayOccupied[i] && notes[i].column == 2){
        if (notes[i].x > 116 && notes[i].x < 120){
          score += 300;//Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 2;
          arrayOccupied[i] = false;
          activeNotes --;
        }
        else if (notes[i].x > 114 && notes[i].x < 122){
          score += 200;//Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          healthChange += 1;
          arrayOccupied[i] = false;
          activeNotes --;
        }
        else if (notes[i].x > 112 && notes[i].x < 124) {
          score += 50;//Serial.print(notes[i].x);
          hitStreak += 1;
          miss = false;
          notes[i].hit = true;
          arrayOccupied[i] = false;
          activeNotes --;
        }
      }
    }
    if (miss) {
      hitStreak = 0;
      Serial.print("Miss\n");
      healthChange -= 2;
    }
  }
  if (life + healthChange <= 100){
    if (life + healthChange > 0){
      life += healthChange;
    }
    else {
      life = 0;
      Serial.print("You have died\n");
      currentState = FailScreen;
    }
  }
  else {
    life = 100;
  }
}

static void updateNoteLocation() { //This function iterates through the array of notes that we have and updates their location
							                     //Note that this is not drawing the notes, it simply changes their x and y coordinates
  for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
  	if (arrayOccupied[i]){
  		notes[i].x = notes[i].x + notes[i].fallVelocity;
  		if (notes[i].x > 128){ //If statement which checks if the note is off the screen, then we can delete it.
  			arrayOccupied[i] = false;
  			activeNotes--;
        if (!notes[i].hit) {
          hitStreak = 0;
          //Serial.print("Lost note\n");
          if (life - 3 > 0){
            life -= 3;
          }
          else {
            Serial.print("You have died\n");
            life = 0;
            currentState = FailScreen;
          }
        }
  		}
  	}
  }
}

static void drawNotes() {
  for (int i = 0; i < MAX_ACTIVE_NOTES; i++) {
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

  //Using The Tiva's LED to display combo status
  if (hitStreak > 15){
      //Serial.print("Red\n");
      digitalWrite(LEDBLUE, LOW);
      digitalWrite(LEDRED, HIGH);

  }
  else if (hitStreak > 10){
    //Serial.print("Blue\n");
      digitalWrite(LEDGREEN, LOW);
      digitalWrite(LEDBLUE, HIGH);
  }
  else if (hitStreak > 5){
    //Serial.print("Green\n");
      digitalWrite(LEDGREEN, HIGH);
  }
  else {
    digitalWrite(LEDBLUE, LOW);
    digitalWrite(LEDGREEN, LOW);
    digitalWrite(LEDRED, LOW);
  }
  
}

