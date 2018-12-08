#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* ssid = "my-wifi";
const char* password = "onlymine";

SoftwareSerial sw(D6,D7);//Rx,Tx

void setup() {
  Serial.begin(57600);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  sw.begin(4800);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  

}

void loop() {
  HTTPClient http;
  if(sw.available()){
    Serial.println("Data Recieved");
    int sensordata = int(sw.read());
    String s = String("http://192.168.0.5:8080/");
    String data = String(sensordata);
    Serial.println(s+data);
    Serial.print("[HTTP] begin...\n");
    http.begin(s+data); 
    Serial.print("[HTTP] GET...\n");  
    int httpCode = http.GET();     
    if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);            
    } 
    else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  }
  

        http.end();
  delay(10000);
}
