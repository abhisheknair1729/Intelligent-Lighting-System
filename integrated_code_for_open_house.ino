//Code that integrates both types of data transmisson through the network.
//Also has the code for interfacing with sensors and dimming module.
//Works only with pcb based nrfs

#include <SoftwareSerial.h>

#include <SPI.h>

#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>

#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8);
RF24Network network(radio);

//addresses of all nodes in the network are octal numbers hence the 0 in the beginning
uint16_t node11 = 011;  
uint16_t node1 = 01;
uint16_t node0 = 00;
uint16_t node2 = 02;
uint16_t node12 = 012;
uint16_t node22 = 022;

uint16_t this_node = node0;

//assign neighbouring nodes to this node according to the network diagram.
uint16_t neigh_node1 = node1;
uint16_t neigh_node2 = node2;

//set node0 as the reciever of all data. It is the central node.
uint16_t reciever_node = node0;

//time interval after which data is to be sent to the central node
int time_interval = 30000;
unsigned int time_now = 0;
unsigned int time_prev = 0;

//sensor pin setup
int sensorpin = A0;
int sensorvalue = 0;

//setting up of leds
int ledpin = 9;

//for setting led on time.
unsigned int ledstart_time = 0;
int led_delay = 5000;

//for setting up serial communication with the NodeMcu
SoftwareSerial sw(2,3); //Rx,Tx

void setup() {
  Serial.begin(57600);
  
  Serial.println("Code to Set up a RF24 Network with 6 nodes");
  Serial.print("Node No:");
  Serial.println(this_node);
  
  SPI.begin();
  radio.begin();
  network.begin(90,this_node);

  pinMode(sensorpin,INPUT);
  pinMode(ledpin,OUTPUT);

  sw.begin(4800);

}

void loop() {
  network.update();

  sensorvalue = analogRead(sensorpin);
  
  if(sensorvalue>500 /*human presence detected*/){
    
    analogWrite(ledpin,255);
    ledstart_time = millis();
      
    int detection_status = 1;
    RF24NetworkHeader header1(neigh_node1,'S');//"S" used to indicate a particular type of signal namely pedestrian detected
    bool ok1 = network.write(header1,&detection_status,sizeof(detection_status));
       
    RF24NetworkHeader header2(neigh_node2,'S');
    bool ok2 = network.write(header2,&detection_status,sizeof(detection_status));
    bool ok = ok1 && ok2;
        
    if(ok){   //change to ok when other code is uncommented
      Serial.println("pedestrian detected signal sent");
    }
    else{
      Serial.println("pedestrian detected signal failed");
    }
  }

  
  //data is to be sent every 10 seconds
  if(this_node != node0){
    int data = sensorvalue; //sensor value is data to  be sent
    time_now = millis();
    if(time_now-time_prev>time_interval){
      time_prev = time_now;
      RF24NetworkHeader header(reciever_node,'D');
      bool ok = network.write(header,&data,sizeof(data));
      if(ok){
        Serial.println("Data Transmitted");
      }
      else{
        Serial.println("Data Transmisson Failed");
      }
    }
  }

  if(network.available()){
    RF24NetworkHeader header;
    network.peek(header);
    if(header.type == 'S'){
      int detection_status = 0;
      network.read(header,&detection_status,sizeof(detection_status));
      Serial.print("status:");
      Serial.println(detection_status);
      //deal with leds
      analogWrite(ledpin,255);
      ledstart_time = millis();
           
    }
    else if(header.type == 'D'){
      int data = 0;
      network.read(header,&data,sizeof(data));
      Serial.println(data);
      if(sw.write(data)){ //send data to the NodeMcu
        Serial.println("data sent to nodemcu");
      }
    }
  }

  if(millis() - ledstart_time > led_delay){
    analogWrite(ledpin,1);
  }
  
}
