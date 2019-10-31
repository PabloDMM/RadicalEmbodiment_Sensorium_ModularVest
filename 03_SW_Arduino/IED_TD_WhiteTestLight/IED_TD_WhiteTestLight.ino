// Libraries
#include <Adafruit_NeoPixel.h>

// Pin definition
const int LED_PIN    = 2;
const int DIR_00_PIN = 3;
const int DIR_01_PIN = 4;
const int DIR_02_PIN = 5;
const int DIR_03_PIN = 6;
const int DIR_04_PIN = 7;
const int DIR_05_PIN = 8;
const int DIR_06_PIN = 10;
const int DIR_07_PIN = 11;

// Program constant values
const int LED_Q      = 10;
// Generated Structures (struct)
struct LEDCOL{
  int red;
  int green;
  int blue;
};
const bool ON                 =   1;         // General ON value
const bool OFF                =   0;         // General OFF value

const bool UP                 =   1;         // General UP Fade Direction value
const bool DOWN               =   0;         // General DOWN Fade Direction value

const int LED_INNER_Q        =   1;         
const int LED_OUTER_Q        =   9;

const int FADESTEPS_C         =   100; // GENERAL FadeSteps
const int FADEDELAY_C         =   100; // GENERAL FadeDelay

const int INNER_LED[LED_INNER_Q]  = {5};
const int OUTER_LED[LED_OUTER_Q]  = {0, 1, 2, 3, 4, 6, 7, 8, 9};

// Gamma Correction (GC) LUT
const byte GC[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

LEDCOL GEN_ON    = {GC[255], GC[255],  GC[255]};  
LEDCOL GEN_OFF   = {GC[0],   GC[0],     GC[0]}; 


// Global Variables
Adafruit_NeoPixel LEDSTR = Adafruit_NeoPixel(LED_Q, LED_PIN, NEO_GRB + NEO_KHZ800);
bool fadeDirection = UP;


// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Serial.begin(115200); // Baudrate Set
  LEDSTR.begin();
  LEDSTR.show(); // Initialize all pixels to 'off'
}

void loop() {
  for(int currentStep = 0; currentStep < FADESTEPS_C; currentStep++){
    LED_GLOWOUT(GEN_ON, GEN_OFF, FADESTEPS_C, (FADEDELAY_C/5), currentStep, fadeDirection);
  }
  fadeDirection = !fadeDirection;   // Switch Fade Glow Direction
}

void LED_GLOWOUT(LEDCOL COL_ON, LEDCOL COL_OFF, int FS, int FD, int Step, bool fadeDir){
  int RDiff = (COL_OFF.red-COL_ON.red);
  int GDiff = (COL_OFF.green-COL_ON.green);
  int BDiff = (COL_OFF.blue-COL_ON.blue);
  //for(int Step = 0; Step < FS; Step++){
  if(fadeDir == UP){
    for(int i = 0; i < LED_INNER_Q; i++){
      //Serial.println(i);
      LEDSTR.setPixelColor(INNER_LED[i], (COL_OFF.green - ((GDiff/FS)*(FS-Step))), (COL_OFF.red - ((RDiff/FS)*(FS-Step))), (COL_OFF.blue - ((BDiff/FS)*(FS-Step))));
    }
    for(int i = 0; i < LED_OUTER_Q; i++){
      //Serial.println(i);
      LEDSTR.setPixelColor(OUTER_LED[i], (COL_OFF.green - ((GDiff/FS)*(Step))), (COL_OFF.red - ((RDiff/FS)*(Step))), (COL_OFF.blue - ((BDiff/FS)*(Step))));
    }
    LEDSTR.show(); // Update LEDs 
    delay(FD);
  }

  else if(fadeDir == DOWN){
    for(int i = 0; i < LED_INNER_Q; i++){
      //Serial.println(i);
      LEDSTR.setPixelColor(INNER_LED[i], (COL_OFF.green - ((GDiff/FS)*(Step))), (COL_OFF.red - ((RDiff/FS)*(Step))), (COL_OFF.blue - ((BDiff/FS)*(Step))));
    }
    for(int i = 0; i < LED_OUTER_Q; i++){
      //Serial.println(i);
      LEDSTR.setPixelColor(OUTER_LED[i], (COL_OFF.green - ((GDiff/FS)*(FS-Step))), (COL_OFF.red - ((RDiff/FS)*(FS-Step))), (COL_OFF.blue - ((BDiff/FS)*(FS-Step))));
    }
    LEDSTR.show(); // Update LEDs 
    delay(FD);
  }
}
