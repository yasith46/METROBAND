#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3C

#define chgsense 12

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 'blt', 10x10px
const unsigned char chblt [] PROGMEM = {
	0x00, 0x00, 0x0c, 0x00, 0x0a, 0x00, 0x2b, 0x00, 0x1c, 0x00, 0x1e, 0x00, 0x2b, 0x00, 0x0a, 0x00, 
	0x0c, 0x00, 0x00, 0x00
};
// 'chg', 10x10px
const unsigned char chchg [] PROGMEM = {
	0x00, 0x00, 0x02, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x1f, 0x00, 0x3e, 0x00, 0x0c, 0x00, 0x08, 0x00, 
	0x10, 0x00, 0x00, 0x00
};
// 'b33', 10x10px
const unsigned char chb33 [] PROGMEM = {
	0x00, 0x00, 0x0e, 0x00, 0x1f, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x1f, 0x00, 
	0x1f, 0x00, 0x00, 0x00
};
// 'b66', 10x10px
const unsigned char chb66 [] PROGMEM = {
	0x00, 0x00, 0x0e, 0x00, 0x1f, 0x00, 0x11, 0x00, 0x11, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 
	0x1f, 0x00, 0x00, 0x00
};
// 'b100', 10x10px
const unsigned char chb100 [] PROGMEM = {
	0x00, 0x00, 0x0e, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 
	0x1f, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 240)
const int challArray_LEN = 5;
const unsigned char* challArray[5] = {
	chb100,
	chb33,
	chb66,
	chblt,
	chchg
};




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  float voltage  = analogRead(34)*3.3/4095;
  int percentage = round(voltage/3.3*100);

  Serial.print(voltage);
  Serial.print("V, ");
  // Needs to be changed irl
  // WokWi doesn't allow voltage divider and a potentiometer
  Serial.print(percentage);
  Serial.println("%");

  oledPrint("128", 0, 20, 3);
  oledPrint("bpm", 54, 33, 1);
  display.drawBitmap(0,0,chblt,10,10,WHITE);
  
  int right = 128;
  if (digitalRead(chgsense)) {
    right -= 10;
    display.drawBitmap(118,0,chchg,10,10,WHITE);
  }
  if (percentage>66) {
    display.drawBitmap(right-10,0,chb100,10,10,WHITE);
  } else if ((percentage>33)&&(percentage<=66)) {
    display.drawBitmap(right-10,0,chb66,10,10,WHITE);
  } else {
    display.drawBitmap(right-10,0,chb33,10,10,WHITE);
  }
  int col = right-28;
  if (percentage==100) {
    col -= 6;
  } else if (percentage<10) {
    col += 6;
  }
  oledPrint((String(percentage)+"%"), col, 2, 1);
  
  display.display();
  delay(500); // this speeds up the simulation
  display.clearDisplay();
  display.display();
  delayMicroseconds(0.1);
}

// Function to print a line of text onto OLED
void oledPrint(String text, int column, int row, int size) {
  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row); // (column, row)
  display.println(text);
}
