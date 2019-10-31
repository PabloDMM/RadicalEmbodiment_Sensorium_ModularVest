/*
 * @file      SEN_ModuleI2cMaser.ino
 * 
 * @authors   Pablo dMM, Gabriel GR (based on Nicholas Zambetti <http://www.zambetti.com> )
 * @date      June 2019
 * @brief     This code correspond to the I2C Slave Master of the Sensorium project, please refer to the repository for more information: https://github.com/PabloDMM/Sensorium_ModularVest
 */

// Libraries
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

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

const byte LEDWHITEINIT       = 50;     // INIT White LED value
const byte LEDWHITE_MINVAR    = 5;      // Minimum variation of LED value
const byte LEDWHITE_MAXVAR    = 50;     // Maximum variation of LED value
const byte LEDWHITE_MINDELAY  = 150;    // Minimum delay between steps
const byte LEDWHITE_MAXDELAY  = 300;    // Mazimum delay between steps

const int FADESTEPS_C         =   25; // GENERAL FadeSteps
const int FADEDELAY_C         =   500; // GENERAL FadeDelay

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


const byte LED_Q        = 10;
const byte MAX_MODULE   = 40;

const byte MIN_I2CDIR   = 8;

const byte ON    = 1;
const byte OFF   = 0;

// Generated Structures (struct)
struct LEDCOL{
  byte red;
  byte green;
  byte blue;
};

// Generated Structures (struct)
struct MODULE{
  byte i2cDir;
  byte detected;
};

// Global Variables
Adafruit_NeoPixel LEDSTR = Adafruit_NeoPixel(LED_Q, LED_PIN, NEO_GRB + NEO_KHZ800);
byte modulesPresent = 1;
//char inputBuffer[LED_Q * MAX_MODULE];
MODULE devices[MAX_MODULE];
LEDCOL GEN_ON    = {GC[255], GC[255],  GC[255]};  
byte ledWhite[LED_Q] = {LEDWHITEINIT, LEDWHITEINIT, LEDWHITEINIT, LEDWHITEINIT, LEDWHITEINIT, LEDWHITEINIT,LEDWHITEINIT,LEDWHITEINIT,LEDWHITEINIT,LEDWHITEINIT};


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);
  
  // Initialice devices to OFF
  for(int i = 0; i < MAX_MODULE; i++){
    devices[i].i2cDir = (MIN_I2CDIR + i);
    devices[i].detected = OFF;
  }
  LEDSTR.begin();
  LEDSTR.show(); // Initialize all pixels to 'off'
  Wire.begin(); // join i2c bus (address optional for master)
}


void loop(){
  char inputBuffer[LED_Q * modulesPresent];
  if(Serial.available() > 0) {
    Serial.readBytes(inputBuffer, (LED_Q * modulesPresent));
  }

  // Turn on self lights
  for(int ledIndex = 0; ledIndex < LED_Q; ledIndex ++){
    GEN_ON.red = inputBuffer[ledIndex];
    GEN_ON.green = inputBuffer[ledIndex];
    GEN_ON.blue = inputBuffer[ledIndex];
    LEDSTR.setPixelColor(ledIndex, GC[GEN_ON.green], GC[GEN_ON.red], GC[GEN_ON.blue]);
  }
  LEDSTR.show(); // Update LEDs 

  int modulesCount = 0;
  while(modulesCount < (modulesPresent-1)){
    for(int i2cAddr = 0; i2cAddr < MAX_MODULE; i2cAddr++){
      if(devices[i2cAddr].detected == ON){
          Wire.beginTransmission(i2cAddr); // transmit to device #13
          for(int ledIndex = 0; ledIndex < LED_Q; ledIndex++){
            Wire.write(inputBuffer[ledIndex + (LED_Q * (modulesCount+1))]);
          }
          Wire.endTransmission();    // stop transmitting
          modulesCount++;
      }
    }
  }
    
  modulesPresent = i2cScanner(modulesPresent);
  Serial.println((String)"A1="+modulesPresent);
  delay(10);
}


byte i2cScanner(byte modulesDetected){
  byte nDevices = 0;
  byte error = 0;
  for(int i2cAddr = 0; i2cAddr < MAX_MODULE; i2cAddr++){
      Wire.beginTransmission(i2cAddr);
      int error = Wire.endTransmission();
      if (error == 0)
      {
        if(devices[i2cAddr].detected == OFF){
          devices[i2cAddr].detected = ON;
          modulesDetected++;
        }
      }else{
        if(devices[i2cAddr].detected == ON){
          devices[i2cAddr].detected = OFF;
          modulesDetected--;
        }
      }
  }
  return modulesDetected;
}

void LED_WHITERANDOM(int minVariation, int maxVariation, int FD){
  int variation = 0;
  for(int i = 0; i < LED_Q; i++){
      variation = minVariation + random(maxVariation - minVariation);
      if(random(1)){
          ledWhite[i] = ledWhite[i] - variation;
      }else{
          ledWhite[i] = ledWhite[i] + variation;
      }
      LEDSTR.setPixelColor(i, GC[ledWhite[i]], GC[ledWhite[i]], GC[ledWhite[i]]);
  }
  LEDSTR.show(); // Update LEDs 
  delay(FD);
}
