#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <SPI.h>

//abhishek nair
//17/10/17
//code to test the rF24 Network

RF24 radio(8,7);
RF24Network network(radio);

//addresses of all nodes in the network are octal numbers hence the 0 in the beginning
uint16_t node11 = 011;  
uint16_t node1 = 01;
uint16_t node0 = 00;
uint16_t node2 = 02;
uint16_t node12 = 012;
//uint16_t node22 = 022;

uint16_t this_node = node1;

//assign neighbouring nodes to this node according to the network diagram.
uint16_t neigh_node1 = node1;
uint16_t neigh_node2 = node0;

//sensor pin setup
int sensorpin  = 4;
long int sensorvalue = 0;

//setting up of leds
int ledpin = 9;

//for setting led on time.
unsigned long int ledstart_time = 0;
int led_delay = 5000;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90,this_node);

  pinMode(sensorpin,INPUT);
  pinMode(ledpin,OUTPUT);
  
}

long int sensorRead(){
   return(digitalRead(sensorpin));
}

void loop() {
  network.update();

  sensorvalue = sensorRead();
  
 
  if(sensorvalue ==1 /*human presence detected*/){
    
    analogWrite(ledpin,255);
    ledstart_time = millis();
      
    int detection_status = 1;
    RF24NetworkHeader header1(neigh_node1);
    network.write(header1,&detection_status,sizeof(detection_status));
       
    RF24NetworkHeader header2(neigh_node2);
    network.write(header2,&detection_status,sizeof(detection_status));
    
    
  if(network.available()){
    RF24NetworkHeader header;
    
      int detection_status = 0;
      network.read(header,&detection_status,sizeof(detection_status));
      if(detection_status == 1){
        
      analogWrite(ledpin,255);
      ledstart_time = millis();
      }
  }
  analogWrite(ledpin,127);
   
}
}
