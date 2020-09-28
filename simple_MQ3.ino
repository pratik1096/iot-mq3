#include <DHT.h>
#include <DHT_U.h>

#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#endif
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
WiFiClient espClient;
PubSubClient client(espClient);
long now = millis();
long lastMeasure = 0;
#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#else

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#endif

//#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>

// Replace with your network credentials


// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
const char* ssid = "AUKCSC_SMARTOFFICE";
const char* password = "PASSWORD2020";
const char* serverName = "http://52.237.118.211:8000/iotdevicedata/TES";
const char* cloudserverName = "http://40.78.60.113:8000/iotdevicedata/TES";
String apiKeyValue = "tPmAT5Ab3j7F9";
String productcode = "AukproDevice11024";
String uniquekey = "!AiOtAtAuKpRo25";
String modelname = "TES";
#define SEALEVELPRESSURE_HPA (1013.25)
//dht DHT;

#define DHTPIN 4
int ethsensor = A0;
int webled = D3;
int light;
float t;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
void setup(void)
{
  dht.begin();
  Serial.begin(9600);
  pinMode(ethsensor, INPUT);
  pinMode(webled, OUTPUT);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());

}
void localserver() {
  digitalWrite(webled, LOW);
  float sensorVoltage;
  float sensorValue;

  sensorValue = analogRead(ethsensor);
  sensorVoltage = sensorValue / 1024 * 5.0;

  Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    ///    light=analogRead(ldr);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    // Prepare your HTTP POST request data
    String httpRequestData =  "api_key=" + apiKeyValue + "&product_code=" + productcode + "&model_name=" + modelname + "&unique_key=" + uniquekey + "&temperature=" +  t + "&humidity=" +  h
                              +  "&ETH=" + sensorVoltage +  "" ;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    if (httpResponseCode == 200) {
      digitalWrite(webled, HIGH);
      delay(300);
    }
    else {
      digitalWrite(webled, LOW);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(500);
}
void cloudserver() {
  digitalWrite(webled, LOW);
  float sensorVoltage;
  float sensorValue;

  sensorValue = analogRead(ethsensor);
  sensorVoltage = sensorValue / 1024 * 5.0;

  Serial.print("sensor voltage = ");
  Serial.print(sensorVoltage);
  Serial.println(" V");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(cloudserverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    ///    light=analogRead(ldr);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    // Prepare your HTTP POST request data
    String httpRequestData =  "api_key=" + apiKeyValue + "&product_code=" + productcode + "&model_name=" + modelname + "&unique_key=" + uniquekey + "&temperature=" +  t + "&humidity=" +  h
                              +  "&ETH=" + sensorVoltage +  "" ;
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    if (httpResponseCode == 200) {
      digitalWrite(webled, HIGH);
      delay(300);
    }
    else {
      digitalWrite(webled, LOW);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(500);
}

void loop() {
  localserver();
  cloudserver();
}
