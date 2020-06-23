#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

const char* ssid     = "CTCP Hung Viet Solution";
const char* password = "hungvietsolution";
const String serverHttp = "http://192.168.1.80:9080";
const String serverHttps = "https://vid-service.herokuapp.com";

WiFiClientSecure *client = new WiFiClientSecure;

TinyGPSPlus gps;
SoftwareSerial ssGps(16, 17);

void setup()
{
  Serial.begin(115200);
  connectWifi();
}

void loop()
{
  if (ssGps.available()) {
    if (gps.encode(ssGps.read())) {
      if (gps.location.isValid()) {
        Serial.print("lat: ");
        Serial.println(String(gps.location.lat(), 6));
        Serial.print("lng: ");
        Serial.println(String(gps.location.lng(), 6));
      }
    }
  }
  
//  Serial.print("connecting to ");
//  Serial.println(serverHttps);
//
//  String payload = httpGet(serverHttps, "/alarm");
//  Serial.println("Payload: ");
//  Serial.println(payload);
////  JSONVar oPayload = JSON.parse(payload);
////
////  Serial.print("name: ");
////  Serial.println(oPayload["name"]);
////  Serial.print("year old: ");
////  Serial.println(oPayload["year_old"]);
////  Serial.print("school: ");
////  Serial.println(oPayload["school"]);
//
//  while (1) {
//    reconnectWifi();  
//  }
}

String httpGet(String server, String api) {
  HTTPClient http;

  http.begin(*client, server + api);

  // Send HTTP request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode == HTTP_CODE_OK) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void connectWifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
}

void reconnectWifi() {
  if (WiFi.status() == WL_DISCONNECTED) {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}
