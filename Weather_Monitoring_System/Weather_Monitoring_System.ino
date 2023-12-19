#include <ESP8266WiFi.h> // ESP32 = #include <WiFi.h> | NodeMCU = #include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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
const int LEDPin = D2;

float DHT_Temperature, DHT_Humidity, BMP_Temperature, BMP_Amplitude, BMP_Pressure;
String LDR_Lightness;

ESP8266WebServer server(80);
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

  server.on("/" , handle_connected);
  server.onNotFound(handle_notfound);
}

void loop() {
  server.handleClient();
  Serial.println("==============================================================================");
  Serial.println("_________Weather Monitoring System_________");
  LDRSensor();
  DHT11sensor();
  BMP180Sensor();
  delay(1000);
}

void handle_notfound() {
  server.send(404 , "text/plain" , "Not Found");
}

void handle_connected() {
  server.send(200 , "text/html" , sendHTML(DHT_Temperature , DHT_Humidity , BMP_Temperature, BMP_Amplitude, BMP_Pressure, LDR_Lightness));
}

String sendHTML(float DHT_Temperature , float DHT_Humidity , float BMP_Temperature, float BMP_Amplitude, float BMP_Pressure, String LDR_Lightness) {
  String ptr;
  ptr = "<!DOCTYPE html>\n"
  ptr+= "<html lang=\"en\">\n"
  ptr+= "<head>\n"
  ptr+= "    <meta charset=\"UTF-8\">\n"
  ptr+= "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
  ptr+= "    <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap-icons@1.11.2/font/bootstrap-icons.min.css\">\n"
  ptr+= "    <link rel=\"shortcut icon\" href=\"/assets/favicon.png\" type=\"image/x-icon\">\n"
  ptr+= "    <title>Atmoscribe</title>\n"
  ptr+= "</head>\n"
  ptr+= "<style>\n"
  ptr+= "    @import url('https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@700&display=swap');\n"
  ptr+= "    * {\n"
  ptr+= "        margin: 0;\n"
  ptr+= "        padding: 0;\n"
  ptr+= "        box-sizing: border-box;\n"
  ptr+= "        font-family: sans-serif;\n"
  ptr+= "    }\n"
  ptr+= "    video {\n"
  ptr+= "        width: 100vw;\n"
  ptr+= "        height: 100vh;\n"
  ptr+= "        object-fit: cover;\n"
  ptr+= "        position: fixed;\n"
  ptr+= "        left: 0;\n"
  ptr+= "        right: 0;\n"
  ptr+= "        top: 0;\n"
  ptr+= "        bottom: 0;\n"
  ptr+= "        z-index: -1;\n"
  ptr+= "    }\n"
  ptr+= "    main {\n"
  ptr+= "        padding: 20px 20%;\n"
  ptr+= "        height: 100vh;\n"
  ptr+= "        display: flex;\n"
  ptr+= "        flex-direction: column;\n"
  ptr+= "        align-items: center;\n"
  ptr+= "        justify-content: space-evenly;\n"
  ptr+= "    }\n"
  ptr+= "    .mainText {\n"
  ptr+= "        font-family: 'Space Grotesk', sans-serif;\n"
  ptr+= "        color: white;\n"
  ptr+= "        font-size: 7rem;\n"
  ptr+= "        text-align: center;\n"
  ptr+= "        text-shadow: 0 0 10px white;\n"
  ptr+= "    }\n"
  ptr+= "    .secondText {\n"
  ptr+= "        color: rgba(255, 255, 255, 0.7);\n"
  ptr+= "        width: 80%;\n"
  ptr+= "        text-align: center;\n"
  ptr+= "        font-size: 1.4rem;\n"
  ptr+= "    }\n"
  ptr+= "    section:first-child {\n"
  ptr+= "        display: flex;\n"
  ptr+= "        flex-direction: column;\n"
  ptr+= "        align-items: center;\n"
  ptr+= "    }\n"
  ptr+= "    section:last-child {\n"
  ptr+= "        display: grid;\n"
  ptr+= "        width: 100%;\n"
  ptr+= "        gap: 40px;\n"
  ptr+= "        grid-template-columns: repeat(2, auto);\n"
  ptr+= "    }\n"
  ptr+= "    section>div {\n"
  ptr+= "        background-color: rgba(0, 67, 105, 0.7);\n"
  ptr+= "        padding: 20px;\n"
  ptr+= "        border-radius: 10px;\n"
  ptr+= "        outline: 1px solid #70f0fd;\n"
  ptr+= "        outline-offset: 10px;\n"
  ptr+= "    }\n"
  ptr+= "    div>h1 {\n"
  ptr+= "        font-family: 'Space Grotesk', sans-serif;\n"
  ptr+= "        color: white;\n"
  ptr+= "        text-shadow: none;\n"
  ptr+= "        margin-bottom: 10px;\n"
  ptr+= "    }\n"
  ptr+= "    hr {\n"
  ptr+= "        margin: 10px 0;\n"
  ptr+= "        border-color: rgba(255, 255, 255, 0.3);\n"
  ptr+= "    }\n"
  ptr+= "    section>div:nth-child(2) {\n"
  ptr+= "        grid-row: span 2;\n"
  ptr+= "        height: fit-content;\n"
  ptr+= "        align-self: center;\n"
  ptr+= "    }\n"
  ptr+= "    section>div>div {\n"
  ptr+= "        display: flex;\n"
  ptr+= "        align-items: center;\n"
  ptr+= "        justify-content: space-between;\n"
  ptr+= "    }\n"
  ptr+= "    section>div>div>span {\n"
  ptr+= "        display: flex;\n"
  ptr+= "        align-items: center;\n"
  ptr+= "        gap: 5px;\n"
  ptr+= "    }\n"
  ptr+= "    section>div>div>span>i {\n"
  ptr+= "        color: #70f0fd;\n"
  ptr+= "        font-size: 22px;\n"
  ptr+= "    }\n"
  ptr+= "    section>div>div>span>p {\n"
  ptr+= "        color: rgba(255, 255, 255, 0.5);\n"
  ptr+= "    }\n"
  ptr+= "    section>div>div>p {\n"
  ptr+= "        color: white;\n"
  ptr+= "        font-weight: bold;\n"
  ptr+= "        font-size: 20px;\n"
  ptr+= "    }\n"
  ptr+= "</style>\n"
  ptr+= "<body>\n"
  ptr+= "    <video autoplay muted loop>\n"
  ptr+= "        <source src=\"assets/background.mp4\" type=\"video/mp4\">\n"
  ptr+= "    </video>\n"
  ptr+= "    <main>\n"
  ptr+= "        <section>\n"
  ptr+= "            <h1 class=\"mainText\">Atmoscribe</h1>\n"
  ptr+= "            <p class=\"secondText\">Unleash the power of weather with real-time forecasts, personalized insights, and precise\n"
  ptr+= "                atmospheric data at\n"
  ptr+= "                your fingertips\n"
  ptr+= "            </p>\n"
  ptr+= "        </section>\n"
  ptr+= "        <section>\n"
  ptr+= "            <div>\n"
  ptr+= "                <h1>DHT Sensor</h1>\n"
  ptr+= "                <div class=\"temp\">\n"
  ptr+= "                    <span>\n"
  ptr+= "                        <i class=\"bi bi-thermometer-sun\"></i>\n"
  ptr+= "                        <p>Temperature</p>\n"
  ptr+= "                    </span>\n"
  ptr+= "                    <p>"
  ptr+= (String)DHT_Temperature
  ptr+= "°</p>\n"
  ptr+= "                </div>\n"
  ptr+= "                <hr>\n"
  ptr+= "                <div class=\"hum\">\n"
  ptr+= "                    <span>\n"
  ptr+= "                        <i class=\"bi bi-droplet-half\"></i>\n"
  ptr+= "                        <p>Humidity</p>\n"
  ptr+= "                    </span>\n"
  ptr+= "                    <p>"
  ptr+= (String)DHT_Humidity
  ptr+= "%</p>\n"
  ptr+= "                </div>\n"
  ptr+= "            </div>\n"
  ptr+= "            <div>\n"
  ptr+= "                <h1>BMP Sensor</h1>\n"
  ptr+= "                <div class=\"temp\">\n"
  ptr+= "                    <span>\n"
  ptr+= "                        <i class=\"bi bi-thermometer-sun\"></i>\n"
  ptr+= "                        <p>Temperature</p>\n"
  ptr+= "                    </span>\n"
  ptr+= "                    <p>"
  ptr+= (String)BMP_Temperature
  prt+= "°</p>\n"
  ptr+= "                </div>\n"
  ptr+= "                <hr>\n"
  ptr+= "                <div class=\"amp\">\n"
  ptr+= "                    <span>\n"
  ptr+= "                        <i class=\"bi bi-activity\"></i>\n"
  ptr+= "                        <p>Amplitude</p>\n"
  ptr+= "                    </span>\n"
  ptr+= "                    <p>"
  ptr+= (String)BMP_Amplitude
  ptr+= "%</p>\n"
  ptr+= "                </div>\n"
  ptr+= "                <hr>\n"
  ptr+= "                <div class=\"pres\">\n"
  ptr+= "                    <span>\n"
  ptr+= "                        <i class=\"bi bi-arrows-collapse\"></i>\n"
  ptr+= "                        <p>Pressure</p>\n"
  ptr+= "                    </span>\n"
  ptr+= "                    <p>"
  ptr+= (String)BMP_Pressure
  ptr+= "%</p>\n"
  ptr+= "                </div>\n"
  ptr+= "            </div>\n"
  ptr+= "            <div>\n"
  ptr+= "                <h1>LDR Sensor</h1>\n"
  ptr+= "                <div class=\"light\">\n"
  ptr+= "                    <span>\n"
  ptr+= "                        <i class=\"bi bi-lightbulb-fill\"></i>\n"
  ptr+= "                        <p>Light/Dark</p>\n"
  ptr+= "                    </span>\n"
  ptr+= "                    <p>"
  ptr+= (String)LDR_Lightness
  ptr+= "</p>\n"
  ptr+= "                </div>\n"
  ptr+= "            </div>\n"
  ptr+= "        </section>\n"
  ptr+= "    </main>\n"
  ptr+= "</body>\n"
  ptr+= "</html>"
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
    LDR_Lightness = "Dark";
  } 
  else {  
    // Light
    Serial.println("Light");
    // Turn Off Leds
    digitalWrite(LEDPin, LOW);
    LDR_Lightness = "Light";
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
  DHT_Temperature = TemperatureReading;
  DHT_Humidity = HumidityReading;
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
      
      // Pressure & Altitude Measurement
      status = BMP.startPressure(3);
      if (status != 0)
      {
        delay(status);
        
        status = BMP.getPressure(P, T);
        if (status != 0)
        {
          // Pressure
          Serial.print("BMP Pressure: ");
          Serial.print(P,2);
          Serial.print(" mb, ");
          Serial.print(P * 0.0295333727, 2);
          Serial.println(" Hg");

          // Pressure using Sea Level
          p0 = BMP.sealevel(P,ALTITUDE); // we're at 90 meters (Boulder, CO)
          Serial.print("BMP Relative (Sea-Level) Pressure: ");
          Serial.print(p0 * 0.0295333727, 2);
          Serial.println(" Hg");
          Serial.println();

          // Altitude
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
  BMP_Pressure = ;
  BMP_Amplitude = ;
  BMP_Temperature = ;
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
