// Lucas Morgan - www.enviral-design.com //


#include <Adafruit_NeoPixel.h>
#include <hcsr04.h>

#define TRIG_PIN 6
#define ECHO_PIN 7

//define variables
long duration;
int distance;

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);

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
#define LED_NUM       16
#define LED_PIN       2

Adafruit_NeoPixel LEDSTR = Adafruit_NeoPixel(LED_NUM, LED_PIN, NEO_RGB + NEO_KHZ800);


char inputBuffer[LED_NUM * 3];

// ------------------- Setup -------------------- //
void setup() {
  //LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
  //LEDS.setBrightness(255);
  //delay(500);
  Serial.begin(115200);
  //Serial.begin(9600);
  Serial.setTimeout(500);
  //LEDS.show();
  LEDSTR.begin();
  LEDSTR.show();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

// ------------------- Main Loop -------------------- //
void loop() {
  
  ledsTD();
  //distanceSensor();  
}

void ledsTD() {
  // Sequence of recieving the RGB data of the 4x4 Matrix in RGB
  if(Serial.available() > 0) {
    Serial.readBytes(inputBuffer, (LED_NUM * 3));
  }
  for (int i = 0; i < LED_NUM; i++){
    //LEDCOL LED_rgbRecieved = {GC[inputBuffer[(i*3)+1]], GC[inputBuffer[(i*3)]], GC[inputBuffer[(i*3)+2]]};
    LEDCOL LED_rgbRecieved = {inputBuffer[(i*3)+1], inputBuffer[(i*3)], inputBuffer[(i*3)+2]};
    LEDSTR.setPixelColor(i, LED_rgbRecieved.green, LED_rgbRecieved.red, LED_rgbRecieved.blue);
  }
  LEDSTR.show();
  //delay(333); //Borrar luego
}

void distanceSensor() {
  //Example sensor HCSR04 (abrir monitor)
  
  // Clears the trigPin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the TRIG_PIN on HIGH state for 10 micro seconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Reads the ECHO_PIN, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.println((String)"A1="+distance);
  delay(50);
}
