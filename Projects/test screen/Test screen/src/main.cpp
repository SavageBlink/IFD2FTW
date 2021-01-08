#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <string.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     5

#define LOADING 0
#define IDLE 1


int state = IDLE;

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void oledSetup(){
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  OLED.clearDisplay();
  OLED.setTextSize(1);                      // choix de la taille des caractères
  OLED.setTextWrap(true);                  // disable text wrap
  OLED.setTextColor(WHITE, BLACK);          // set text color to white and black background
  OLED.setCursor(0,0);
  OLED.display();
}

void oledPrint(const char* window, const char* text){
  OLED.clearDisplay();
  OLED.setCursor(0,0);
  OLED.print(window);
  OLED.setCursor(0,20);
  OLED.print(text);
  OLED.display();
}


void setup() {
  oledSetup();
  Serial.begin(115200);
}


void loop(){

  oledPrint("I'd like to interject for a moment","What you're refereing to as LINUX, is in fact ultra GAY");

}



