#include <stdlib.h>
#include <stdbool.h>


int activeNotes = 0;

static enum GameStates
{
  Welcome          = 0,
  SelectDifficulty = 1,
  PlayGame         = 2,
  FailScreen       = 3,
  WinScreen        = 4,
  SelectGameMode   = 5,
  
} currentState = Welcome;

const uint32_t SwitchCount = 2;
const uint32_t ButtonCount = 4;
const uint32_t Switches[SwitchCount] = { PA_7, PA_6 };
const uint32_t Buttons[ButtonCount] = { PD_2, PE_0, PUSH1, PUSH2 };

int menuDisplay = 0;
bool noteSpeed = false;
bool noteFrequency = false;
bool menuChanged = true;
int gameMenuDisplay = 0;
bool gameMenuChanged = true;

bool previousSpeed = false;
bool previousFrequency = false;

int gameMode = 0;

struct ButtonState {
  bool state;
  bool isRising;
};
/*
struct GameState {
  
} activeGame;
*/
static struct InputState
{
  bool switches[2]; //Array of 2 bools to represent the switch states
  struct ButtonState buttons[4]; //Array of 4 Button state structs.
  
} gameInputState;


//Initialization function called by TivaHero once in Setup()

void GameUIInit() {
  OrbitOledInit();
  OrbitOledClear();
  OrbitOledClearBuffer();
  OrbitOledSetCursor(0,0);

  gameInputState = { 0 };

  for(int i = 0; i < SwitchCount; ++i )
    pinMode(Switches[i], INPUT);
  for(int i = 0; i < ButtonCount; ++i ){
    if (i == 2){
      pinMode(Buttons[i], INPUT_PULLUP);
    }
    else{
      pinMode(Buttons[i], INPUT);
    }
  }

  GPIOPadConfigSet(LED1Port, LED1, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(LED2Port, LED2, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(LED3Port, LED3, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
  GPIOPadConfigSet(LED4Port, LED4, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);

  GPIOPinTypeGPIOOutput(LED1Port, LED1);
  GPIOPinTypeGPIOOutput(LED2Port, LED2);
  GPIOPinTypeGPIOOutput(LED3Port, LED3);
  GPIOPinTypeGPIOOutput(LED4Port, LED4);

  GPIOPinWrite(LED4Port, LED4, LOW);
  GPIOPinWrite(LED3Port, LED3, LOW);
  GPIOPinWrite(LED2Port, LED2, LOW);
  GPIOPinWrite(LED1Port, LED1, LOW);
  
}

bool finishPlayGameInit = false;
//Functions to handle each seperate game state

static void handleStateWelcome()
{
  finishPlayGameInit = false;
  //  OrbitOledMoveTo(30,0);
  //  OrbitOledDrawString("Tiva Hero");
  if(menuChanged){
    OrbitOledClear();
    switch (menuDisplay){
        
        case 0:   
        OrbitOledMoveTo(30,0);
        OrbitOledDrawString("Tiva Hero");
        OrbitOledMoveTo(0,20);
        OrbitOledDrawString("Press any button");
        break; 
        
        case 1: 
        OrbitOledMoveTo(32,0);
        OrbitOledDrawString("Play Game");
        
        break;
    
        case 2: 
        OrbitOledMoveTo(28,0);
        OrbitOledDrawString("Difficulty");
        break;
    
        case 3:
        OrbitOledMoveTo(44,0);
        OrbitOledDrawString("Scores");
        break;
        
        }
  
  
    if(menuDisplay < 3 && menuDisplay > 0){
    OrbitOledMoveTo(0,20); 
    OrbitOledDrawString("Down");
    }
  
    if(menuDisplay > 1){
    OrbitOledMoveTo(110,20); 
    OrbitOledDrawString("Up");
    }
    menuChanged = false; 
    }

    //Input handling
    if (menuDisplay == 0){
      if (gameInputState.buttons[0].isRising || gameInputState.buttons[1].isRising || gameInputState.buttons[2].isRising || gameInputState.buttons[3].isRising){
        menuDisplay ++;
        menuChanged = true;
      }
    }
    else {
      //Center column button pressed to scroll down on menu
      if (gameInputState.buttons[0].isRising){
        if(menuDisplay < 3) {
          menuDisplay += 1;
        menuChanged = true;
        }
        Serial.print(menuDisplay);
      }
      
      if (gameInputState.buttons[1].isRising){
        if(menuDisplay > 1) {
        menuDisplay -= 1;
        menuChanged = true;
        }
        Serial.print(menuDisplay);
      }
  
      if (gameInputState.buttons[2].isRising){
       menuDisplay = 0;
       menuChanged = true;
      }
      
      if(gameInputState.buttons[3].isRising){
        switch(menuDisplay){
          case 1:
          OrbitOledClear();
          menuChanged = true;
          currentState = SelectGameMode;
          break;
          
          case 2:       
          OrbitOledClear();
          menuChanged = true;
          currentState = SelectDifficulty;
          handleSelectDifficulty();
          break;
  
          case 3:
  
          break;
          }
  
      }
    }

    //Example of using Orbit LEDs
    if (millis() % 50 > 25){
      GPIOPinWrite(LED4Port, LED4, LED4);
      GPIOPinWrite(LED3Port, LED3, LED3);
      GPIOPinWrite(LED2Port, LED2, LED2);
      GPIOPinWrite(LED1Port, LED1, LED1);
    }
    else {
      GPIOPinWrite(LED4Port, LED4, LOW);
      GPIOPinWrite(LED3Port, LED3, LOW);
      GPIOPinWrite(LED2Port, LED2, LOW);
      GPIOPinWrite(LED1Port, LED1, LOW);
    }
  
}

static void handleSelectDifficulty() {
  Serial.print("Handle");
  char str[1];
  OrbitOledMoveTo(25,0);
  OrbitOledDrawString("Use Switches");
  OrbitOledMoveTo(0,20);
  if (noteSpeed){
    OrbitOledDrawString("Faster ");
  }
  else {
    OrbitOledDrawString("Slower ");
  }

  OrbitOledMoveTo(80,20);
  if (noteFrequency){
    OrbitOledDrawString("Hard");
  }
  else {
    OrbitOledDrawString("Normal");
  }
  if (gameInputState.switches[1] && gameInputState.switches[1] != previousSpeed){
    previousSpeed = gameInputState.switches[1];
    noteSpeed = true;
    OrbitOledClear();
    
  }
  else if (!gameInputState.switches[1] && gameInputState.switches[1] != previousSpeed) {
    previousSpeed = gameInputState.switches[1];
    noteSpeed = false;
    OrbitOledClear();
    
  }
  if (gameInputState.switches[0] && gameInputState.switches[0] != previousFrequency){
    previousFrequency = gameInputState.switches[0];
    noteFrequency = true;
    OrbitOledClear();
    
  }
  else if (!gameInputState.switches[0] && gameInputState.switches[0] != previousFrequency) {
    previousFrequency = gameInputState.switches[0];
    noteFrequency = false;
    OrbitOledClear();
    
  }
  /*
  OrbitOledMoveTo(50,20);
  sprintf(str, "%d", difficulty);
  OrbitOledDrawString(str);

  if (gameInputState.buttons[1].isRising){
    OrbitOledClear();
    if(difficulty < 10) {
       difficulty += 1;
    }
  }

  if (gameInputState.buttons[0].isRising){
    OrbitOledClear();
    if(difficulty > 1) {
     difficulty -= 1;
    }
  }
  */
  if (gameInputState.buttons[2].isRising){
    gameInputState.buttons[2].isRising = false;
    OrbitOledClear();
    currentState = Welcome;
    menuChanged = true;
    menuDisplay = 1;
    handleStateWelcome();
  }

  if (millis() % 50 > 25){
      GPIOPinWrite(LED4Port, LED4, LED4);
      GPIOPinWrite(LED3Port, LED3, LED3);
      GPIOPinWrite(LED2Port, LED2, LED2);
      GPIOPinWrite(LED1Port, LED1, LED1);
    }
    else {
      GPIOPinWrite(LED4Port, LED4, LOW);
      GPIOPinWrite(LED3Port, LED3, LOW);
      GPIOPinWrite(LED2Port, LED2, LOW);
      GPIOPinWrite(LED1Port, LED1, LOW);
    }
  
}

static void handleSelectGameMode() {
  if (gameMenuChanged){
    
    switch (gameMenuDisplay){
      case 0:
      OrbitOledClear();
      OrbitOledMoveTo(0,0);
      OrbitOledDrawString("Select Game Mode");
      OrbitOledMoveTo(35,20);
      OrbitOledDrawString("Survival");
      gameMenuChanged = false;
      break;
      
      case 1:
      OrbitOledClear();
      OrbitOledMoveTo(0,0);
      OrbitOledDrawString("Select Game Mode");
      OrbitOledMoveTo(35,12);
      OrbitOledDrawString("Shelter");
      OrbitOledMoveTo(0,24);
      OrbitOledDrawString("Porter Robinson");
      gameMenuChanged = false;
      break;

      case 2:
      OrbitOledClear();
      OrbitOledMoveTo(0,0);
      OrbitOledDrawString("Select Game Mode");
      OrbitOledMoveTo(10,12);
      OrbitOledDrawString("Black Beatles");
      OrbitOledMoveTo(12,24);
      OrbitOledDrawString("Raw Sremmurd");
      gameMenuChanged = false;
      break;
    }
  }
  
  if (gameInputState.buttons[0].isRising){
    if(gameMenuDisplay <2) {
      gameMenuDisplay += 1;
    gameMenuChanged = true;
    }
    Serial.print(gameMenuDisplay);
  }
  
  if (gameInputState.buttons[1].isRising){
    if(gameMenuDisplay > 0) {
    gameMenuDisplay -= 1;
    gameMenuChanged = true;
    }
    Serial.print(gameMenuDisplay);
  }

  if (gameInputState.buttons[2].isRising){
    gameInputState.buttons[2].isRising = false;
    OrbitOledClear();
    currentState = Welcome;
    gameMenuChanged = true;
    menuChanged = true;
    menuDisplay = 1;
    handleStateWelcome();
  }
  
  if(gameInputState.buttons[3].isRising){
    switch(gameMenuDisplay){
      case 0:
        OrbitOledClear();
        OrbitOledMoveTo(0,0);
        OrbitOledDrawString("Rotate your ");
        OrbitOledMoveTo(0,12);
        OrbitOledDrawString("controller!!");
        OrbitOledMoveTo(0,24);
        OrbitOledDrawString("<= This side up");   
        OrbitOledUpdate();    
        delay(3000);
        
        gameInputState.buttons[2].isRising = false;
        gameMode = 0;
        OrbitOledClear();
        currentState = PlayGame;
        gameMenuChanged = true;
        menuChanged = true;
        menuDisplay = 1;
      break;
      
      case 1:    
        OrbitOledClear();
        OrbitOledMoveTo(0,0);
        OrbitOledDrawString("Rotate your ");
        OrbitOledMoveTo(0,12);
        OrbitOledDrawString("controller!!");
        OrbitOledMoveTo(0,24);
        OrbitOledDrawString("<= This side up");      
        OrbitOledUpdate();  
        delay(3000);
           
        gameInputState.buttons[2].isRising = false;
        gameMode = 1;
        OrbitOledClear();
        currentState = PlayGame;
        gameMenuChanged = true;
        menuChanged = true;
        menuDisplay = 1;
      break;

      case 2:
        OrbitOledClear();
        OrbitOledMoveTo(0,0);
        OrbitOledDrawString("Rotate your ");
        OrbitOledMoveTo(0,12);
        OrbitOledDrawString("controller!!");
        OrbitOledMoveTo(0,24);
        OrbitOledDrawString("<= This side up");    
        OrbitOledUpdate();  
        delay(3000);
        
        gameInputState.buttons[2].isRising = false;
        gameMode = 2;
        OrbitOledClear();
        currentState = PlayGame;
        gameMenuChanged = true;
        menuChanged = true;
        menuDisplay = 1;
      break;
      }

  }
    

  
}

static void handleGame(int mode, int speed) { //play mode. 0 == endless/random, 1 == Shelter, 2 == Black Beatles
                                                   //Speed. Determines the speed that the notes move at
                                                   //noteFrequency is how often the notes are generated
  if (! finishPlayGameInit){
    playGameInit();
    finishPlayGameInit = true;
  }
  if (gameInputState.buttons[2].isRising){
    OrbitOledClear();
    
  }
  if (mode == 0){
    //Randomly create notes
    if (millis() % 300 == 0){
      if (activeNotes <  MAX_ACTIVE_NOTES){
        createNoteRandom(0, 1, 3); 
      }
    }
    checkHit();
    if (millis() % 50 == 0){ 
      render();
    }
    if (millis() % 100 == 0){
      updateScoreStreak();
    }
  }
  else { //Create timed notes according to a song
    createNoteTimed(3, 185.59996); //Pass it the note velocity and the BPM
    checkHit();
    if (millis() % 30 == 0){ 
      render();
    }
    if (millis() % 100 == 0){
      updateScoreStreak();
    }
  }

  
  
  
}

static void handleFailScreen() {
  OrbitOledClear();
  OrbitOledMoveTo(30,0);
  OrbitOledDrawString("Dead");

}

static void handleWinScreen() {
  
}

//Function to check for Input. Called constantly by GameUITick()

static void InputTick() {
  for(int i = 0; i < SwitchCount; ++i ){
    gameInputState.switches[i] = digitalRead(Switches[i]);
  }
  
  for(int i = 0; i < ButtonCount; ++i ) {
    // Only look for Rising Edge Signals.
    bool previousState = gameInputState.buttons[i].state;
    if (i > 1){
      if (digitalRead(Buttons[i])){
        gameInputState.buttons[i].state = 0;
      }
      else {
        gameInputState.buttons[i].state = 1;
      }
    }
    else {
      gameInputState.buttons[i].state = digitalRead(Buttons[i]);
    }
    
    gameInputState.buttons[i].isRising = (!previousState && gameInputState.buttons[i].state);
  }
}


//Main GameUI function that is called by TivaHero's Main loop

void GameUITick(){
  InputTick();
  //switches[0] is the lower switch on the orbut booster pack, switches[1] is the higher one
  //buttons[0] = left button on booster pack, buttons[1] = right button on booster pack, buttons[2] = top button on Tiva, buttons[3] = bottom button on Tiva

  /*
  Serial.print(gameInputState.buttons[0].state);//Center column
  Serial.print(gameInputState.buttons[1].state);//Right column
  Serial.print(gameInputState.buttons[2].state);
  Serial.print(gameInputState.buttons[3].state); //Left column
  Serial.print(' ');
  Serial.print(gameInputState.buttons[0].isRising);//Center column
  Serial.print(gameInputState.buttons[1].isRising);//Right column
  Serial.print(gameInputState.buttons[2].isRising);
  Serial.print(gameInputState.buttons[3].isRising); //Left column
  Serial.print(' ');
  Serial.print(gameInputState.switches[0]);
  Serial.print(gameInputState.switches[1]);
  Serial.print('\n');
  */

  switch(currentState)
  
  {
  case Welcome:
    handleStateWelcome();
    break;

  case SelectDifficulty:
    handleSelectDifficulty();
    break;
  case SelectGameMode:
    handleSelectGameMode();
    break;
  case PlayGame:
    handleGame(gameMode,0);
    break;

  case FailScreen:
    handleFailScreen();
    break;

  case WinScreen:
    handleWinScreen();
    break;
  }

  
  OrbitOledUpdate();
}

