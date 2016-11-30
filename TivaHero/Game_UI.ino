    #include <stdlib.h>
    #include <stdbool.h>
    
    
    
    static enum GameStates
    {
      Welcome          = 0,
      SelectDifficulty = 1,
      PlayGame         = 2,
      FailScreen       = 3,
      WinScreen        = 4,
      
    } currentState = Welcome;
    
    const uint32_t SwitchCount = 2;
    const uint32_t ButtonCount = 4;
    const uint32_t Switches[SwitchCount] = { PA_7, PA_6 };
    const uint32_t Buttons[ButtonCount] = { PD_2, PE_0, PUSH1, PUSH2 };
    
    int menuDisplay = 0;
    int difficulty = 1;
    int menuChanged = true;
    
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
          break; 
          
          case 1: 
          OrbitOledMoveTo(5,0);
          OrbitOledDrawString("Difficulty");
          break;
      
          case 2: 
          OrbitOledMoveTo(5,0);
          OrbitOledDrawString("Play Game");
          break;
      
          case 3:
          OrbitOledMoveTo(5,0);
          OrbitOledDrawString("Scores");
          break;
          
          }
    
    
      if(menuDisplay < 3){
      OrbitOledMoveTo(0,20); 
      OrbitOledDrawString("Down");
      }
    
      if(menuDisplay > 0){
      OrbitOledMoveTo(80,20); 
      OrbitOledDrawString("Up");
      }
      menuChanged = 0; 
      }
    
          //Center column button pressed to scroll down on menu
        if (gameInputState.buttons[0].isRising){
          if(menuDisplay < 3) {
            menuDisplay += 1;
          menuChanged = 1;
          }
          Serial.print(menuDisplay);
        }
        
        if (gameInputState.buttons[1].isRising){
          if(menuDisplay > 0) {
          menuDisplay -= 1;
          menuChanged = 1;
          }
          Serial.print(menuDisplay);
        }
        
        if(gameInputState.buttons[3].isRising){
          switch(menuDisplay){
  
            case 1:       
            OrbitOledClear();
            currentState = SelectDifficulty;
            handleSelectDifficulty();
            break;
            
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
          char str[1];
          OrbitOledMoveTo(30,0);
          OrbitOledDrawString("Difficulty");
          OrbitOledMoveTo(50,20);
          sprintf(str, "%d", difficulty);
          OrbitOledDrawString(str);
    
          if (gameInputState.buttons[1].isRising){
          if(difficulty < 10) {
             difficulty += 1;
          }else difficulty == 9;
          Serial.print(difficulty);
        }
        
        if (gameInputState.buttons[0].isRising){
          if(difficulty > 1) {
           difficulty -= 1;
  
          }
          Serial.print(difficulty);
  
        }
          
      
    }
    
    static void handleGame() {
      if (! finishPlayGameInit){
        playGameInit();
        finishPlayGameInit = true;
      }
      checkHit();
      if (millis() % 50 == 0){ 
        render();
      }
      if (millis() % 100 == 0){
        updateScoreStreak();
      }
      
      
      
    }
    
    static void handleFailScreen() {
      
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
    
      case PlayGame:
        handleGame();
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

