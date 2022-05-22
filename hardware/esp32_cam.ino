#include <esp32cam.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define WSSID "your wifi SSID"
#define WPASS "your wifi password"

int ldrpin = 12;
int laserpin = 13;
int buzzerpin = 15;
int rstpin = 14;
const int buzzdelay = 500;
String serverName = "http://<herokuapp name>.herokuapp.com/notify";

void handleCapture(){
  auto img = esp32cam::capture();
  img->toJpeg(80);
  delay(500);
  WiFiClient client;
  HTTPClient http;
  if(img != nullptr){
    Serial.println("[HTTP] begin...\n");
    http.begin(client, serverName);
    int httpCode = http.POST(img->data(), img->size());
    if (httpCode == HTTP_CODE_OK){
      Serial.println(http.getString());
    }
    else{
      Serial.println("Can't connect to server: " + String(httpCode));
    }
    http.end();
  }
  else{
    Serial.println("Capture failed");
  }
}

void waitForRst(bool buzz=false){
  Serial.println("Waiting to be reseted");
  while(true){
    if(buzz){
      digitalWrite(buzzerpin, HIGH);
      delay(buzzdelay);
      digitalWrite(buzzerpin, LOW);
      delay(buzzdelay);
    }
    else{
      delay(500);
    }
    if(digitalRead(rstpin) == HIGH){
      digitalWrite(laserpin, HIGH);
      delay(3000);
      digitalWrite(laserpin, LOW);
      delay(500);
      digitalWrite(laserpin, HIGH);
      delay(500);
      Serial.println("reseted");
      break;
    }
  }
}

void setup(){
  Serial.begin(115200);

  // setting up pins
  pinMode(ldrpin, INPUT);
  pinMode(laserpin, OUTPUT);
  pinMode(buzzerpin, OUTPUT);
  pinMode(rstpin, INPUT);

  delay(250);
  digitalWrite(laserpin, HIGH);
  delay(500);
  
  // setting up wifi
  Serial.println("\r\nConnecting to: " + String(WSSID));
  WiFi.begin(WSSID, WPASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());

  // setting up camera
  pinMode(4, OUTPUT); // The flash, if you like to use it
  auto res = esp32cam::Resolution::find(1024, 768);
  esp32cam::Config cfg;
  cfg.setPins(esp32cam::pins::AiThinker); // For any other board reffer to esp32cam library's pins.h definitions
  cfg.setResolution(res);
  esp32cam::Camera.begin(cfg);
}

void loop(){
  if(digitalRead(ldrpin) == HIGH){
    Serial.println("alert");
    digitalWrite(laserpin, LOW);
    handleCapture();
    delay(2000);
    waitForRst(true);
  }
  else if(digitalRead(rstpin) == HIGH){
    Serial.println("please go");
    digitalWrite(laserpin, LOW);
    delay(2000);
    waitForRst();
  }
}
