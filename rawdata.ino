#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_NeoPixel.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEAdvertising.h>


#define BNO055_SAMPLERATE_DELAY_MS (10)
#define PIN      5
#define N_LEDS 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x29, &Wire);

void setup(void)
{
  Serial.begin(115200);

    strip.begin();
    brems(false);

    BLEDevice::init("CykelJakke_RANDOMID2");

    BLEServer *pServer = BLEDevice::createServer();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  pAdvertising->start();   


  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");



  if(!bno.begin())
  {

    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  delay(1000);


  bno.setExtCrystalUse(true);

  Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
}


int xSpeed,ySpeed,zSpeed;
float bremseGrænseværdi=-1.0;
bool bremser=false;
unsigned long bremsetid = 0;
unsigned long bremsetidMinimum=50;
unsigned long count=0;

void loop(void)
{

  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

  xSpeed+=acc.x();
  ySpeed+=acc.y();
  zSpeed+=acc.z();

  if(acc.y()<bremseGrænseværdi && bremser==false) {
    bremser = true;
    bremsetid=millis();
  }
  else if(acc.y()>bremseGrænseværdi) {
    bremser = false;
  }

  if(bremser) count++;
  else count=0;

  if(bremser && millis()-bremsetid>bremsetidMinimum) brems(true);

  else if(!bremser) brems(false);

  
  Serial.print("min:-4.0");
  Serial.print("\tmax:4.0");
  Serial.print("\tBremser:");
  Serial.print(bremser);
    Serial.print("\tcount:");
  Serial.print(count);

  Serial.print("\tY:");
  Serial.print(acc.y());

  Serial.println("");


  delay(BNO055_SAMPLERATE_DELAY_MS);
}

void brems(bool on){
    uint32_t farven=0;
  if (on) farven=strip.Color(255, 0, 0);
  
   for(int i=0;i<N_LEDS;i++)
  {
    strip.setPixelColor(i  , farven);
  }
  strip.show();
}
