//Com7
#include "SoftwareSerial.h"
int rxPinRemote = D1;
int txPinRemote = D2; 
int rxPinHeatPump = D5;
int txPinHeatPump = D6;
SoftwareSerial swSerHeatPump(rxPinHeatPump, txPinHeatPump, true, 256);
SoftwareSerial swSerRemote(rxPinRemote,txPinRemote , true, 256);

bool sendDataToHeatPump = false;
unsigned long timerHeatPump = 0;
int packageCounterHeatPump = 0;
int packageHeatPump1 = 0;
int packageHeatPump2 = 0;
int packageHeatPump3 = 0;
int packageHeatPump4 = 0;

bool sendDataToRemote = false;
unsigned long timerRemote = 0;
int packageCounterRemote = 0;
int packageRemote1 = 0;
int packageRemote2 = 0;
int packageRemote3 = 0;
int packageRemote4 = 0;

void setup() {  
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Starting...");
  swSerHeatPump.begin(960);
  swSerRemote.begin(960);
}

void loop() {
  
  if(sendDataToHeatPump){
    sendDataToHeatPump = false;
    //delay(50);
    Serial.print("Send Package To HeatPump: ");
    Serial.print(packageRemote1);    
    Serial.print(packageRemote2);    
    Serial.print(packageRemote3);    
    Serial.println(packageRemote4);  
    
    swSerHeatPump.write(packageRemote1);
    swSerHeatPump.write(packageRemote2);
    swSerHeatPump.write(packageRemote3);
    swSerHeatPump.write(packageRemote4);    
  }  

  if(sendDataToRemote){
    sendDataToRemote = false;
    Serial.print("Send Package To Remote: ");
    Serial.print(packageHeatPump1);    
    Serial.print(packageHeatPump2);    
    Serial.print(packageHeatPump3);    
    Serial.println(packageHeatPump4);        
    
    swSerRemote.write(packageHeatPump1);
    swSerRemote.write(packageHeatPump2);
    swSerRemote.write(packageHeatPump3);
    swSerRemote.write(packageHeatPump4);    
  }    

  while (swSerHeatPump.available() > 0) {    
    unsigned int packagedata = swSerHeatPump.read();   
    if(millis() - timerHeatPump > 30){
      packageCounterHeatPump = 1;      
    }    
    if(packageCounterHeatPump == 1 && packagedata != 85){
      Serial.print("swSerHeatPump Wrong Package Received");
      Serial.println(packagedata);  
      timerHeatPump = millis();
      yield();
      continue;
    }
    Serial.print("swSerHeatPump Correct Received");
    Serial.println(packagedata);  
    if(packageCounterHeatPump==1){
      packageHeatPump1 = packagedata;
    }
    if(packageCounterHeatPump==2){
      packageHeatPump2 = packagedata;
    }    
    if(packageCounterHeatPump==3){
      packageHeatPump3 = packagedata;
    }  
    if(packageCounterHeatPump==4){
      packageHeatPump4 = packagedata;
      sendDataToRemote = true;
    }     
    timerHeatPump = millis();    
    packageCounterHeatPump++;
    yield();
  }

  while (swSerRemote.available() > 0) {    
    unsigned int packagedata = swSerRemote.read();   
    if(millis() - timerRemote > 30){
      packageCounterRemote = 1;
    }
    if(packageCounterRemote == 1 && packagedata != 170){
      Serial.print("swSerRemote Wrong Package Received");
      Serial.println(packagedata);  
      timerRemote = millis();
      yield();
      continue;
    }
    Serial.print("swSerRemote Correct Received");
    Serial.println(packagedata);     
    if(packageCounterRemote==1){
      packageRemote1 = packagedata;
    }
    if(packageCounterRemote==2){
      packageRemote2 = packagedata;
    }    
    if(packageCounterRemote==3){
      packageRemote3 = packagedata;
    }  
    if(packageCounterRemote==4){
      packageRemote4 = packagedata;
      sendDataToHeatPump = true;
    }     
    timerRemote = millis();    
    packageCounterRemote++;
    yield();
  }  

}
