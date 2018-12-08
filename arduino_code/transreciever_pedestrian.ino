#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>

#include <SPI.h>

//!!!!!!!!!!!!!!!!!!!WORKING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

RF24 radio(7,8);
RF24Network network(radio);

//addresses of all nodes in the network are octal numbers hence the 0 in the beginning
uint16_t node0 = 011;  //this node
uint16_t node1 = 01;
uint16_t node2 = 00;
uint16_t node3 = 02;
uint16_t node4 = 012;
uint16_t node5 = 022;
//end

//program these for individual arduinos accroding to the diagram
uint16_t this_node = xx;
uint16_t neigh_node1 = xx;
uint16_t neigh_node2 = xx; //only one neighbour for this node

//temporary code in place of sensor code
int sensorvalue = 0;
int sensorpin = A0;
// temporary code ends

void setup() {
  Serial.begin(57600);
  Serial.println("Transreciever_pedestrian");
  
  SPI.begin();
  radio.begin();
  network.begin(90,this_node);

  //temp code
  pinMode(sensorpin,INPUT);
  
}

void loop() {

  network.update();

  while(network.available()){
    RF24NetworkHeader header;
    int recieved = -1;
    network.read(header,&recieved,sizeof(recieved));
    Serial.print("Sensor data recieved: \t");
    Serial.println(recieved);
  }

  sensorvalue = analogRead(sensorpin);
  if(sensorvalue>500){  //if pedestrian detected send signal to neighbouring arduinos
    
    Serial.println("Pedestrian detected");
    RF24NetworkHeader header(neigh_node1);
    bool ok1 = network.write(header,&sensorvalue,sizeof(sensorvalue));
    
   
    RF24NetworkHeader header2(neigh_node2);
    bool ok2 = network.write(header2,&sensorvalue,sizeof(sensorvalue));
    bool ok = ok1 && ok2;
   
    
    if(ok){   //change to ok when other code is uncommented
      Serial.println("ok");
    }
    else{
      Serial.println("failed");
    }
  }

}
