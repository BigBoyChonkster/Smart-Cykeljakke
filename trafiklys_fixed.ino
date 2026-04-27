#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

#define PIN      5
#define N_LEDS 30

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

#define NUM_RED_LEDS 7 
#define NUM_YELLOW_LEDS 9
#define NUM_GREEN_LEDS 13

#define RSSI_THRESHOLD -70
#define MAX_DEVICES 20


int redLeds[NUM_RED_LEDS]={29,28,14,12,11,10,9};
int yellowLeds[NUM_YELLOW_LEDS]={27,26,25,17,16,15,8,7,6};
int greenLeds[NUM_GREEN_LEDS]={0,1,2,3,4,5,18,19,20,21,22,23,24};

int jacketCount = 0;
String seenDevices[MAX_DEVICES];
int seenCount = 0;

class MyCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice device) {
String name = String(device.getName());

    if (name.startsWith("CykelJakke") ){

      String mac = device.getAddress().toString();


      for (int i = 0; i < seenCount; i++) {
        if (seenDevices[i] == mac) return;
      }

      if (seenCount < MAX_DEVICES) {
        seenDevices[seenCount++] = mac;
        jacketCount++;
      }
    }
  }
};

int scanForJackets() {
  jacketCount = 0;
  seenCount = 0;

  BLEScan* scan = BLEDevice::getScan();
  scan->setAdvertisedDeviceCallbacks(new MyCallbacks());
  scan->setActiveScan(true);

  scan->start(1, false); // scanner i 1 sekunder

  Serial.print("Antal jakker: ");
  Serial.println(jacketCount);

  return jacketCount;
}

int scanOgDelay(int millisekunder){
  int antal;
  while(millisekunder>999){
  antal = scanForJackets();
  if(antal >= 2)
    {
     return antal;
    }
  millisekunder-=1000;
  }
  return 0;
}

void setup() {
  strip.begin();

  Serial.begin(115200);
  BLEDevice::init("");
}

void loop() {

  int antal = scanForJackets();

    if (antal >= 2) {
        red(true);
        yellow(false);
        green(false);
        scanOgDelay(9000);

        red(true);
        yellow(true);
        green(false);
        scanOgDelay(4000);
    
        red(false);
        yellow(false);
        green(true);

      while(true){
      scanOgDelay(1000);

      int nytAntal = scanForJackets();

      if (nytAntal < 2) {

        red(false);
        yellow(true);
        green(false);
        delay(3000);

        break; 
      }
    }
  }
  else {

    red(true);
    yellow(false);
    green(false);
    scanOgDelay(12000);

    red(true);
    yellow(true);
    green(false);
    scanOgDelay(3000);  

    red(false);
    yellow(false);
    green(true);

    delay(12000);

    while (true) {
      int antal = scanForJackets();

      if (antal >= 2) {
        delay(1000);
      } else {
        break;
      }
    }

    red(false);
    yellow(true);
    green(false);
    scanOgDelay(3000);
 }
}


void red(bool on)
{
  uint32_t farven=0;
  if (on) farven=strip.Color(255, 0, 0);
   for(int i=0;i<NUM_RED_LEDS;i++)
  {
    strip.setPixelColor(redLeds[i]  , farven);
  }
  strip.show();
}



void yellow(bool on)
{
  uint32_t farven=0;
  if (on) farven=strip.Color(255, 208, 0);
   for(int i=0;i<NUM_YELLOW_LEDS;i++)
  {
    strip.setPixelColor(yellowLeds[i]  , farven);
  }
  strip.show();
}

void green(bool on)
{
  uint32_t farven=0;
  if (on) farven=strip.Color(0, 255, 0);
   for(int i=0;i<NUM_GREEN_LEDS;i++)
  {
    strip.setPixelColor(greenLeds[i]  , farven);
  }
  strip.show();
}