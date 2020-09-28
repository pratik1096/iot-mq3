#include <DHT.h>
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
const char* ssid = "ssid";
const char* password = "password";
const char* serverName = "0.0.0.0:0000/myfile";
const char* cloudserverName = "0.0.0.0:0000/myfile";
String apiKeyValue = "**********";
String productcode = "**************";
String uniquekey = "*******";
String modelname = "TES";
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
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    float h = dht.readHumidity();
    float t = dht.readTemperature();
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
     http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  } 
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
    http.begin(cloudserverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    float h = dht.readHumidity();
    float t = dht.readTemperature();
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
  delay(500);
}

void loop() {
  localserver();
  cloudserver();
}
