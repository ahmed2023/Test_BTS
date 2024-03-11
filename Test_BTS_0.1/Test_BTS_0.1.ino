// Controlling Position of DC Motor Using Position Encoder.
#define CLK A5
#define DT A4
#define RPMW 5
#define LPMW 6
#define CLK1 A1
#define DT1 A0
#define SW A3
int Present_Push_Button;
long lastDebounceTime = 0;
long Last_Print = 0;
long Last_Toggle_Direction = 0;
bool Direction = true;
int Print_Time = 250;
int Current_State_Of_CLK;
int Last_State_Of_CLK;
int Current_State_Of_DT;
int Last_State_Of_DT = HIGH;
int Current_State_Of_CLK1;
int Last_State_Of_CLK1;
int Current_State_Of_DT1;
int Last_State_Of_DT1;
int Counter_Resolution = 0;
int Counter_Rotation[2] = {0 ,80};
long lastRotateTime = 0;
long lastRotateTime1 = 0;
int debounceDelay = 50;
void Limiting_Values(){
  if(Counter_Rotation[0] > 150){
    Counter_Rotation[0] = 150;
  }
  if (Counter_Rotation[0] < 0){
    Counter_Rotation[0] = 0;
  }
  if(Counter_Rotation[1] > 150){
    Counter_Rotation[1] = 150;
  }
  if(Counter_Rotation[1] < 0){
    Counter_Rotation[1] = 0;
  }
}
void setup(){
  PCICR |= B00000010;
  PCMSK1 |= B00110011;
  Serial.begin(9600);
  pinMode(RPMW, OUTPUT);
  pinMode(LPMW, OUTPUT);
  analogWrite(RPMW, 0);
  analogWrite(LPMW, 0);
  pinMode(SW, INPUT_PULLUP);
}
void loop(){
  if((millis() - Last_Print) > Print_Time){
    Serial.print(Counter_Rotation[0]);
    Serial.print(" ");
    Serial.print(Counter_Rotation[1]);
    Serial.println(" ");
    Last_Print = millis();
  }
  // if(Direction){
    if (Counter_Rotation[0] == Counter_Rotation[1]){
      analogWrite(RPMW, 0);
      analogWrite(LPMW, 0);
    }
    else if(Counter_Rotation[0] > Counter_Rotation[1]){
      analogWrite(RPMW, 255);
      analogWrite(LPMW, 0);
    }
    else if(Counter_Rotation[0] < Counter_Rotation[1]){
      analogWrite(RPMW, 0);
      analogWrite(LPMW, 255);
    }
  // }
}
ISR(PCINT1_vect) {
  Current_State_Of_CLK = digitalRead(CLK);
  if (Current_State_Of_CLK != Last_State_Of_CLK && Current_State_Of_CLK == 1) {
    if ((millis() - lastRotateTime) > debounceDelay) {
      if (digitalRead(DT) != Current_State_Of_CLK) {
        Counter_Rotation[Counter_Resolution]--;
      } else if (digitalRead(DT) == Current_State_Of_CLK) {
        Counter_Rotation[Counter_Resolution]++;
      }
      lastRotateTime = millis();
    }
  }
  Last_State_Of_CLK = Current_State_Of_CLK;
  Current_State_Of_CLK1 = digitalRead(CLK1);
  if (Current_State_Of_CLK1 != Last_State_Of_CLK1 && Current_State_Of_CLK1 == 1) {
    if ((millis() - lastRotateTime1) > debounceDelay) {
      if (digitalRead(DT1) != Current_State_Of_CLK1) {
        Counter_Rotation[Counter_Resolution + 1]--;
      } else if (digitalRead(DT1) == Current_State_Of_CLK1) {
        Counter_Rotation[Counter_Resolution + 1]++;
      }
      lastRotateTime1 = millis();
    }
  }
  Limiting_Values();
  Last_State_Of_CLK1 = Current_State_Of_CLK1;
}