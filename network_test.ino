#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>

#include <SPI.h>

//WORKING

RF24 radio(7,8);
RF24Network network(radio);

//addresses of all nodes in the network are octal numbers hence the 0 in the beginning
uint16_t node0 = 011;  
uint16_t node1 = 01;
uint16_t node2 = 00;
uint16_t node3 = 02;
uint16_t node4 = 012;
//uint16_t node5 = 022;
//end

uint16_t this_node = 01;

uint16_t reciever_node = 00;
uint16_t transmitter_node = 01;


void setup() {
  Serial.begin(57600);
  Serial.println("Code to Set up a RF24 Network with 5 nodes");
  
  SPI.begin();
  radio.begin();
  network.begin(90,this_node);
  
}

void loop() {

  network.update();
  
  if(this_node == transmitter_node){
    Serial.println("Transmitter Node");
    char text[32] = "hello there";
    RF24NetworkHeader header(reciever_node);
    bool ok = network.write(header,text,sizeof(text));
    if(ok){
      Serial.println("Data Transmitted");
    }
    else{
      Serial.println("failed");
    }
  }
  
  if(this_node == reciever_node){
    Serial.println("reciever node");
    while(network.available()){
      RF24NetworkHeader header;
      char text[32] =" ";
      network.read(header,text,sizeof(text));
      Serial.println(text);
    }
  }
  
}
