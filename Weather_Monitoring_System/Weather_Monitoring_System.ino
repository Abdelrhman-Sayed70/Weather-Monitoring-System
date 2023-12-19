#include <ESP8266WiFi.h> // ESP32 = #include <WiFi.h> | NodeMCU = #include <ESP8266WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <SFE_BMP180.h>
#include<BlynkSimpleEsp8266.h>

// DHT Intializing
#define DHTPIN D1 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// BMB Initializing
#define ALTITUDE 90.0
SFE_BMP180 BMP;

// Blynk Credentials
#define BLYNK_TEMPLATE_ID "TMPL2BX3cycI1"
#define BLYNK_TEMPLATE_NAME "Weather Monitoring System"
#define BLYNK_AUTH_TOKEN "PFYI5vZ2flvQA0HS4J5rmzz-8n28zxL6"
char auth[] = BLYNK_AUTH_TOKEN;

// WIFI Credentials
const char* ssid = "Gaber";
const char* pass = "family1988imbabaabdo"; // family1988imbabaabdo

// Pins
const int LDRPin = D0;
const int LEDPin = D2;

void setup() {
  // Preprocessing
  Serial.begin(115200);
  delay(3000);
  Serial.println("_________Preprocessing_________");

  // Connecting WIFI
  ConnectWifi();

  // Connecting to Blynk
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
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
  Blynk.run();
  delay(1000);
}

// =========================================> Connecting To WIFI <======================================
void ConnectWifi(){
  Serial.print("Wifi Connecting To ");
  Serial.print(ssid);
  Serial.println(" Network");

  WiFi.begin(ssid, pass);
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
    
    // Send Darkness to Blynk
    Blynk.virtualWrite(V4, "It's Dark");
    
    // Turn On Leds
    digitalWrite(LEDPin, HIGH);
  } 
  else {  
    // Light
    Serial.println("Light");
    
    // Send Light to Blynk
    Blynk.virtualWrite(V4, "It's Light");
    
    // Turn Off Leds
    digitalWrite(LEDPin, LOW);
  }
  Serial.println("=================================================");
}

// =========================================> DHT11 [Temperature & Humidity] <======================================
void DHT11sensor() {
  Serial.println("DHT11 Sensor [Temperature & Humidity]");

  // Read & Check
  float HumidityReading = dht.readHumidity();
  float TemperatureReading = dht.readTemperature();
  if (isnan(HumidityReading) || isnan(TemperatureReading)) {
    Serial.println("Failed To Read From DHT Sensor :(");
    return;
  }
  Serial.println("Read From DHT is Done Successfully");

  // Print Readings to Serial
  Serial.print("DHT Temperature: ");
  Serial.println(TemperatureReading);
  
  Serial.print("DHT Humidity: ");
  Serial.println(HumidityReading);
  
  Serial.println("=================================================");
  
  // Send Readings to Blynk
  Blynk.virtualWrite(V0, TemperatureReading);
  Blynk.virtualWrite(V1, HumidityReading);
}

// =========================================> BMP180 [Pressure & Altitude] <======================================
void BMP180Sensor(){
  Serial.println("BMP180 Simple Sensor [Pressure & Altitude]");
  char status;
  double TemperatureC, TemperatureF, Pressure, PressureSeaLevel, Altitude;

  // Get BMP Temperature
  status =  BMP.startTemperature();
  if (status == 0){
    Serial.println("Can't Read BMP Temprature");
    return;
  }
  delay(status);
  status = BMP.getTemperature(TemperatureC);
  TemperatureF = (9.0 / 5.0) * TemperatureC + 32.0;

  // Get BMP Pressure in mb
  status = BMP.startPressure(3);// 0 to 3
  if (status == 0){
    Serial.println("Can't Read BMP Pressure");
    return;
  }
  delay(status);
  status = BMP.getPressure(Pressure, TemperatureC);
  if (status == 0){
    Serial.println("Can't Read BMP Pressure");
    return;
  }
  
  // Get BMP Altitude
  PressureSeaLevel = BMP.sealevel(Pressure, ALTITUDE);
  Altitude = BMP.altitude(Pressure, PressureSeaLevel);

  // Print Results in Serial
  Serial.print("Temperature in C: ");
  Serial.print(TemperatureC);
  Serial.print(" °C, Temprature in °F = ");
  Serial.println(TemperatureF);

  Serial.print("Pressure: ");
  Serial.print(Pressure);
  Serial.println(" mb");

  Serial.print("Altitude: ");
  Serial.print(Altitude);
  Serial.println(" Meters");
  Serial.println("=================================================");

  // Send Readings to Blynk
  Blynk.virtualWrite(V2, Pressure);
  Blynk.virtualWrite(V3, Altitude);
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
