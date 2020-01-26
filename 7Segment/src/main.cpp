#include <Arduino.h>

//Transcoder bin-> 7seg
//An 8-bit binary number (corresponding to 8 buttons) should be converted to a decimal number in the range 0-255, and then displayed on the LED display.

#define SEGMENT_A   0
#define SEGMENT_B   1
#define SEGMENT_C   2
#define SEGMENT_D   3
#define SEGMENT_E   4
#define SEGMENT_F   5
#define SEGMENT_G   6
#define SEGMENT_DP  7
#define SIGN_0      8
#define SIGN_1      9
#define SIGN_2      10

#define BUTTON_BIT_1        11
#define BUTTON_BIT_2        12
#define BUTTON_BIT_4        13
#define BUTTON_BIT_8        A5
#define BUTTON_BIT_16       A4
#define BUTTON_BIT_32       A3
#define BUTTON_BIT_64       A2
#define BUTTON_BIT_128      A1

enum state {
    STATE__GETTING_INPUT,
    STATE__SHOWING_DISPLAY
};

uint8_t current_state = STATE__GETTING_INPUT;
uint8_t number = 0;
bool    button_state[8];

bool checkButton(uint8_t ButtonID){
    if(digitalRead(ButtonID) == LOW){
          do{
              delay(1);
              if(digitalRead(ButtonID) == HIGH){
                  break;
              }
          }while(true);
          return true;
    };
    return false;
}

void gettingInput(void) {
    if(checkButton(BUTTON_BIT_1) == true){
        button_state[0] = !button_state[0];
    }
    if(checkButton(BUTTON_BIT_2) == true){
        button_state[1] = !button_state[1];
    }
    if(checkButton(BUTTON_BIT_4) == true){
        button_state[2] = !button_state[2];
    }
    if(checkButton(BUTTON_BIT_8) == true){
        button_state[3] = !button_state[3];
    }
    if(checkButton(BUTTON_BIT_16) == true){
        button_state[4] = !button_state[4];
    }
    if(checkButton(BUTTON_BIT_32) == true){
        button_state[5] = !button_state[5];
    }
    if(checkButton(BUTTON_BIT_64) == true){
        button_state[6] = !button_state[6];
    }
    if(checkButton(BUTTON_BIT_128) == true){
        button_state[7] = !button_state[7];
    }
}

void transferButtonsToNumber(void) {
    number = 0;
    for(uint8_t i = 0; i<8; i++){
        if(button_state[i] == true){
              number += 1<<i;
        }
    }
}

void ledsOff(void) {
    digitalWrite(SEGMENT_DP, HIGH);
    digitalWrite(SEGMENT_A, HIGH);
    digitalWrite(SEGMENT_B, HIGH);
    digitalWrite(SEGMENT_C, HIGH);
    digitalWrite(SEGMENT_D, HIGH);
    digitalWrite(SEGMENT_E, HIGH);
    digitalWrite(SEGMENT_F, HIGH);
    digitalWrite(SEGMENT_G, HIGH);
}

void choosePanel(uint8_t panel) {
    switch(panel){
        case 1:
            digitalWrite(SIGN_0, HIGH);
            digitalWrite(SIGN_1, LOW);
            digitalWrite(SIGN_2, LOW);
        break;
        case 2:
            digitalWrite(SIGN_0, LOW);
            digitalWrite(SIGN_1, HIGH);
            digitalWrite(SIGN_2, LOW);
        break;
        case 3:
            digitalWrite(SIGN_0, LOW);
            digitalWrite(SIGN_1, LOW);
            digitalWrite(SIGN_2, HIGH);
        break;
        default:
            digitalWrite(SIGN_0, LOW);
            digitalWrite(SIGN_1, LOW);
            digitalWrite(SIGN_2, LOW);
        break;
    }
}

void displayDigit(uint8_t digit) {
    if(digit  == 1 || digit == 4){
        digitalWrite(SEGMENT_A, HIGH);
    } else {
        digitalWrite(SEGMENT_A, LOW);
    }
    
    if(digit  == 5 || digit == 6){
        digitalWrite(SEGMENT_B, HIGH);
    } else {
        digitalWrite(SEGMENT_B, LOW);
    }
    
    if(digit  == 2){
        digitalWrite(SEGMENT_C, HIGH);
    } else {
        digitalWrite(SEGMENT_C, LOW);
    }
    
    if(digit  == 1 || digit  == 4 || digit == 7){
        digitalWrite(SEGMENT_D, HIGH);
    } else {
        digitalWrite(SEGMENT_D, LOW);
    }

    if(digit  == 1 || digit  == 3 || digit == 4 || digit  == 5 || digit == 7 || digit  == 9){
        digitalWrite(SEGMENT_E, HIGH);
    } else {
        digitalWrite(SEGMENT_E, LOW);
    }

    if(digit  == 1 || digit  == 2 || digit == 3 || digit == 7){
        digitalWrite(SEGMENT_F, HIGH);
    } else {
        digitalWrite(SEGMENT_F, LOW);
    }

    if(digit  == 1 || digit  == 7 || digit == 0){
        digitalWrite(SEGMENT_G, HIGH);
    } else {
        digitalWrite(SEGMENT_G, LOW);
    }
}

void displayNumber(void) {
    uint8_t digit;
    for(uint8_t i = 1; i <= 3; i++){
        digit = number % 10;
        choosePanel(i);
        displayDigit(digit);
        number = (number-digit)/10;
        delay(1);
        ledsOff();
        if(number == 0){
            break;
        }
    }
}

void setup(void) {
    current_state = STATE__GETTING_INPUT;
    number = 0;
  
    pinMode(SEGMENT_A, OUTPUT);
    pinMode(SEGMENT_B, OUTPUT);
    pinMode(SEGMENT_C, OUTPUT);
    pinMode(SEGMENT_D, OUTPUT);
    pinMode(SEGMENT_E, OUTPUT);
    pinMode(SEGMENT_F, OUTPUT);
    pinMode(SEGMENT_G, OUTPUT);
    pinMode(SEGMENT_DP, OUTPUT);
    pinMode(SIGN_0, OUTPUT);
    pinMode(SIGN_1, OUTPUT);
    pinMode(SIGN_2, OUTPUT);
    
    pinMode(BUTTON_BIT_1,   INPUT_PULLUP);
    pinMode(BUTTON_BIT_2,   INPUT_PULLUP);
    pinMode(BUTTON_BIT_4,   INPUT_PULLUP);
    pinMode(BUTTON_BIT_8,   INPUT_PULLUP);
    pinMode(BUTTON_BIT_16,  INPUT_PULLUP);
    pinMode(BUTTON_BIT_32,  INPUT_PULLUP);
    pinMode(BUTTON_BIT_64,  INPUT_PULLUP);
    pinMode(BUTTON_BIT_128, INPUT_PULLUP);
}

void loop(void) {
    switch (current_state) {
        case STATE__GETTING_INPUT:
            gettingInput();
            transferButtonsToNumber();
            current_state = STATE__SHOWING_DISPLAY;
            break;
        case STATE__SHOWING_DISPLAY:
            displayNumber();
            current_state = STATE__GETTING_INPUT;
            break;
        default:
            break;
    }
}