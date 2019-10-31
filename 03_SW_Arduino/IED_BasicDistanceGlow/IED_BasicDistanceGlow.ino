// Library Import
#include <Adafruit_NeoPixel.h>
#include <MIDIUSB.h>
//#include <HCSR04.h>
//#include <hcsr04.h>
#include <HCSR04.h>


// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------LED Gamma Correction LUT--------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
const int GC[256] = {
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

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Custom Configuration Structs----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Generated Structures (struct)
struct LEDCOL{
  int red;
  int green;
  int blue;
};

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------General Constant Definition-----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// PIN DEFINITION
// Digital Pin Definitions
const int  LED    = 2;
const int  SW_0   = 7;
const int  SW_1   = 8;
const int  CH_0   = 0;
const int  CH_1   = 1;
const int  CH_2   = 2;
const int  CH_3   = 3;
const int  MUX_A  = 3;
const int  MUX_B  = 4;
const int  MUX_C  = 5;
const int  MUX_D  = 6;
const int  SW_Q   = 16;

// Constant Definition
const bool ON                 =   1;         // General ON value
const bool OFF                =   0;         // General OFF value

const bool UP                 =   1;         // General UP Fade Direction value
const bool DOWN               =   0;         // General DOWN Fade Direction value

const int LED_INNER_Q        =   4;         
const int LED_OUTER_Q        =   12;


const int FADESTEPS_C         =   100; // GENERAL FadeSteps
const int FADEDELAY_C         =   25; // GENERAL FadeDelay

LEDCOL GEN_OFF    = {GC[255], GC[255],  GC[255]};  // SW_GENERAL OFF LED COLOR
LEDCOL DIST_ON    = {GC[255], GC[0],    GC[200]};  // SW_SCALE ON LED COLOR
LEDCOL DIST_OFF   = {GC[0],   GC[0],     GC[0]};  // SW_MODE ON LED COLOR

const int INNER_LED[4]  = {5, 6, 9, 10};
const int OUTER_LED[12] = {0, 1, 2, 3, 4, 7, 8, 11, 12, 13, 14, 15};

const int THR_DISTANCE  = 40;

bool distanceStatus = OFF;
bool fadeDirection = UP;

Adafruit_NeoPixel LEDSTR = Adafruit_NeoPixel(SW_Q, LED, NEO_RGB + NEO_KHZ800);

HCSR04 DISTANCE(6,7);//initialisation class HCSR04 (trig pin , echo pin)

void setup() {
  // Baudrate Set
  Serial.begin(115200); 
  // LED Stripe Initialization
  LEDSTR.begin();
  LEDSTR.show();
  // Set Pin Modes
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
  pinMode(MUX_D, OUTPUT);

  //LED_FADE_INIT(0, 0, 0, CH_LED[0][1], CH_LED[0][0], CH_LED[0][2], 10, 80);
}

void loop() {  
  for(int currentStep = 0; currentStep < FADESTEPS_C; currentStep++){
    distanceStatus = get_distanceStatus();
    Serial.println(distanceStatus);
    switch(distanceStatus){
      case OFF:
        //Serial.println("LED OFF");
        LED_GLOWOUT(DIST_OFF, GEN_OFF, FADESTEPS_C, FADEDELAY_C, currentStep, fadeDirection);
        break;
      case ON:     
        //Serial.println("LED ON");
        LED_GLOWOUT(DIST_ON, GEN_OFF, FADESTEPS_C, (FADEDELAY_C/5), currentStep, fadeDirection);
        break;
      break;
    }
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

bool get_distanceStatus(){
  bool newState = distanceStatus;
  switch(distanceStatus){
    case OFF:      
      if(DISTANCE.dist() <= THR_DISTANCE){
        Serial.println("OFF to ON");
        newState = ON;
      }
      break;
    case ON:      
      if(DISTANCE.dist() > THR_DISTANCE){
        newState = OFF;
        Serial.println("ON to OFF");
      }
      break;
  }
  return newState;
}
