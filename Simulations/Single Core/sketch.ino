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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ROT_SW, INPUT);
  pinMode(ROT_DT, INPUT);
  pinMode(ROT_CL, INPUT);
  pinMode(MOT_OUT, OUTPUT);
}

int lastClk = HIGH;
int BPM = 100;

void loop() {
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

  if(digitalRead(ROT_SW)==LOW) {
    float HIGH_T = 0.1;
    float LOW_T = 60.0/BPM - HIGH_T;
    for (;;) {
      digitalWrite(MOT_OUT, HIGH);
      delay(HIGH_T*1000);
      digitalWrite(MOT_OUT, LOW);
      delay(LOW_T*1000);
      Serial.println(digitalRead(ROT_SW));
      if(digitalRead(ROT_SW)==LOW) {
        break;
      }
    }
    delay(1000);
  }
}
