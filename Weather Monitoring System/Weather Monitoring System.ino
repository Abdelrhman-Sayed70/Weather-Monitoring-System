#include <ESP8266WiFi.h> // ESP32 = #include <WiFi.h> | NodeMCU = #include <ESP8266WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <SFE_BMP180.h>

// DHT Intializing
#define DHTPIN D1 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// BMB Initializing
#define ALTITUDE 90.0
SFE_BMP180 BMP;

// WiFi Credentials
const char* ssid = "Gaber";
const char* password = "family1988imbabaabdo"; // family1988imbabaabdo

// Pins
const int LDRPin = D0;
const int LEDPin = D5;

void setup() {
  // Preprocessing
  Serial.begin(115200);
  delay(3000);
  Serial.println("_________Preprocessing_________");

  // Connecting WIFI
  ConnectWifi();
  
  // Sensors Running
  dht.begin();
  BMPRunning();
  
  // Initializing Pins
  pinMode(LDRPin, INPUT);  
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  Serial.println("==============================================================================");
  Serial.println("_________Weather Monitoring System_________");
  LDRSensor();
  DHT11sensor();
  BMP180Sensor();
  delay(6000);
}

// =========================================> Connecting To WIFI <======================================
void ConnectWifi(){
  Serial.print("Wifi Connecting To ");
  Serial.print(ssid);
  Serial.println(" Network");

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED){
      delay(1000);
      Serial.print(".");        
  }
  Serial.println("\nWifi Connected Success!");
  Serial.print("NodeMCU IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

// =========================================> LDR [Light] <======================================
void LDRSensor() {
  Serial.println("LDR Sensor [Light Detection]");
  bool value = digitalRead(LDRPin);
  if (value == HIGH) {
    // DARK
    Serial.println("Dark");
    // Turn On Leds
    digitalWrite(LEDPin, HIGH);
  } 
  else {  
    // Light
    Serial.println("Light");
    // Turn Off Leds
    digitalWrite(LEDPin, LOW);
  }
  Serial.println("=================================================");
}

// =========================================> DHT11 [Temperature & Humidity] <======================================
void DHT11sensor() {
  Serial.println("DHT11 Sensor [Temperature & Humidity]");
 
  float HumidityReading = dht.readHumidity();
  float TemperatureReading = dht.readTemperature();

  if (isnan(HumidityReading) || isnan(TemperatureReading)) {
    Serial.println("Failed To Read From DHT Sensor :(");
    return;
  }
  
  Serial.println("Read From DHT is Done Successfully");
  
  Serial.print("DHT Temperature: ");
  Serial.println(TemperatureReading);

  Serial.print("DHT Humidity: ");
  Serial.println(HumidityReading);

  Serial.println("=================================================");
}

// =========================================> BMP180 [Pressure & Altitude] <======================================
void BMP180Sensor(){
  Serial.println("BMP180 Sensor [Pressure & Altitude]");
  char status;
  double T, P, p0, a;

  Serial.print("Altitude: ");
  Serial.print(ALTITUDE, 0);
  Serial.print(" Meters, ");
  Serial.print(ALTITUDE * 3.28084, 0);
  Serial.println(" Feet");
  Serial.println();

  // Pressure Reading 
  status = BMP.startTemperature();
  if (status != 0)
  {
    // Wait For The Temperature Measurement To Complete
    delay(status);
    status = BMP.getTemperature(T);
    if (status != 0)
    {
      Serial.print("BMP Temperature: ");
      Serial.print(T,2);
      Serial.print(" C°, ");
      Serial.print((9.0 / 5.0) * T + 32.0, 2);
      Serial.println(" F°");
      Serial.println();
      
      // Pressure Measurement
      status = BMP.startPressure(3);
      if (status != 0)
      {
        delay(status);
        
        status = BMP.getPressure(P, T);
        if (status != 0)
        {
          Serial.print("BMP Pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P * 0.0295333727, 2);
          Serial.println(" Hg");

          // The pressure sensor returns abolute pressure, which varies with altitude.
          // To remove the effects of altitude, use the sealevel function and your current altitude.
          
          p0 = BMP.sealevel(P,ALTITUDE); // we're at 90 meters (Boulder, CO)
          Serial.print("BMP Relative (Sea-Level) Pressure: ");
          Serial.print(p0 * 0.0295333727, 2);
          Serial.println(" Hg");
          Serial.println();
          // On the other hand, if you want to determine your altitude from the pressure reading,
          // use the altitude function along with a baseline pressure (sea-level or other).
          // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
          // Result: a = altitude in m.

          a = BMP.altitude(P, p0);
          Serial.print("BMP Altitude: ");
          Serial.print(a, 0);
          Serial.println(" Meters");
        }
        else 
          Serial.println("error retrieving pressure measurement");
      }
      else 
        Serial.println("error starting pressure measurement");
    }
    else 
      Serial.println("error retrieving temperature measurement");
  }
  else
    Serial.println("error starting temperature measurement");

  Serial.println("=================================================");
}

void BMPRunning(){
  Serial.println("BMP Running..");
  Wire.begin(D4, D3); // SDA, SCL (From Most Left)
  
  if (BMP.begin()) {
    Serial.println("BMP180 Runs Successfully :)");
  }
  
  else{
    Serial.println("BMP180 Can't Run :(");
    while(1); // Pause Forever.
  }
}
