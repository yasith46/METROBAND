// For the rotary encoder theres an example code here:
// https://wokwi.com/projects/304184298969236032
// and the SW pin works just like a normal pushbutton
//
// There were no motors or vibrators here, so I used an LED and a resistor
// in this case


#define ROT_SW 19  // pushbutton of encoder
#define ROT_DT 18  // data pin of encoder
#define ROT_CL 5   // clock of encoder 

#define MOT_OUT 4  // motor out (LED in this case) 

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ROT_SW, INPUT);
  pinMode(ROT_DT, INPUT);
  pinMode(ROT_CL, INPUT);
  pinMode(MOT_OUT, OUTPUT);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500);
}

int lastClk = HIGH;
int BPM = 100;
float HIGH_T = 0.1;
float LOW_T = 0.0;

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  for(;;){
    int newClk = digitalRead(ROT_CL);

    if (newClk != lastClk) {
      // There was a change on the CLK pin
      lastClk = newClk;
      int dtValue = digitalRead(ROT_DT);

      if (newClk == LOW && dtValue == HIGH) {
        BPM += 1;
        Serial.println(BPM);
      }
      if (newClk == LOW && dtValue == LOW) {
        BPM -= 1;
        Serial.println(BPM);
      }
    }
    LOW_T = 60.0/BPM - HIGH_T;
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  for(;;){
    if(digitalRead(ROT_SW)==LOW) {
    for (;;) {
      digitalWrite(MOT_OUT, HIGH);
      delay(HIGH_T*1000);
      digitalWrite(MOT_OUT, LOW);
      delay(LOW_T*1000);
      if(digitalRead(ROT_SW)==LOW) {
        break;
      }
    }
    delay(1000);
  }
  }
}

void loop() {
  
}
