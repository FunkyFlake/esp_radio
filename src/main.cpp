#include <Arduino.h>
#include <vs1053.hpp>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <mp3_example.h>
#include <mp3_example2.h>
#include <stdlib.h>
#include <website.h>

//Enter WiFi setup here:
const char *SSID = "UPC230D88";
const char *PASSWORD = "WtznsWw8ffve";

WiFiClient client;

constexpr uint8_t XCS_PIN = D1;
constexpr uint8_t XDCS_PIN = D0;
constexpr uint8_t DREQ_PIN = D2;

VS1053 MP3(XCS_PIN, XDCS_PIN, DREQ_PIN);   

uint8_t mp3Buffer[32]; 

ESP8266WebServer server(80);

String newStation = "0";
String station = "0";

String station_urls[] = {"s3-webradio.rockantenne.de", "radiostreaming.ert.gr", 
                         "mp3.harmonyfm.de", "stream.antenne1.de"};
String station_paths[] = {"/rockantenne/stream/mp3", "/ert-kosmos",
                          "/harmonyfm/hqlivestream.mp3", "/a1stg/livestream2.mp3"};
  
void handleRoot() {
  newStation = server.arg("station"); 
  Serial.print("Station:");
  Serial.println(station);
  server.send(200, "text/html", website);
}

int stationIdx(String &station) {
  return atoi(station.c_str());
}

void connectStation(String &stationID) {
  client.stop();
  int status = 0;
  while(status != 1) {
    status = client.connect(station_urls[stationIdx(stationID)], 80);
  } 
  Serial.println("Connected to radio station!");
  // Request stream
  client.print("GET " + String(station_paths[stationIdx(stationID)]) + " HTTP/1.1\r\nHost: " + String(station_urls[stationIdx(stationID)]) + "\r\nConnection: close\r\n\r\n");
}

void setup() {
  Serial.begin(9600, SERIAL_8N1);
  delay(2000);

  uint16_t samplerate = 44100;
  MP3.init(samplerate);
  MP3.set_volume(80);

  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  while(WiFi.status() != WL_CONNECTED){
    Serial.println("..connecting to WiFi");
    delay(500);
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");

  connectStation(station);
}

void loop() {
  if (client.available() > 0) {
    uint8_t bytesread = client.read(mp3Buffer, 32);
    MP3.playback(mp3Buffer, bytesread);
  }

  server.handleClient();

  if(station != newStation) {
    connectStation(newStation);    
    station = newStation;
  }
  //MP3.playback(mp3_data, sizeof(mp3_data));
  //delay(3000);
  //MP3.playback(mp3_data2, sizeof(mp3_data2));
  //delay(3000);
}
