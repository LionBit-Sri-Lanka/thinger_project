#include <Arduino.h>
#include <lionbit.h>

#include <ThingerESP32.h>
#include "arduino_secrets.h"
#include <DHT.h>
 #include <Adafruit_Sensor.h>

#define THINGER_SERIAL_DEBUG
#define DEBUG_PRINTER

#define DHTPIN D4

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

float t, h;
unsigned long timeOut = 0;
int slider = 0;
void setup()
{
  // open serial for debugging
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  dht.begin();

  thing.add_wifi(SSID, SSID_PASSWORD);

  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thing["LED"] << digitalPin(LED_BUILTIN);
  thing["ROOM_LIGHT"] << digitalPin(D4);
  thing["FAN"] << digitalPin(D5);

  // resource output example (i.e. reading a sensor value)
  thing["millis"] >> outputValue(millis());
  thing["ldr"] >> outputValue(analogRead(A2));
  thing["temp"] >> outputValue(t);
  thing["hum"] >> outputValue(h);
  thing["soil"] >> outputValue(analogRead(A1));
  thing["dimmer"] << inputValue(slider);

  // more details at http://docs.thinger.io/arduino/
}

void loop()
{
  thing.handle();
  if (millis() - timeOut > 2000UL)
  {
    Serial.println(slider);
    h = dht.readHumidity();

    t = dht.readTemperature();

    timeOut = millis();
  }
}