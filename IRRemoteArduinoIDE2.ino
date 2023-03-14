//#include "IRremote.h"
//#include "PultBeelineRASSE001.h"

#include "RemotePult.h"
#include "ActionTable.h"
#include "Utils.h"

//IRrecv irrecv(2); // указываем вывод, к которому подключен приемник
//RemotePult remotePult(2, PultButtonItem* _pultButtonTable, uint8_t _pultButtonTableSize):IRrecv(_recvpin) {
RemotePult remotePult(2, TABLE(keyTable));

//decode_results results;

void setup() {
  Serial.begin(9600); // выставляем скорость COM порта
  remotePult.enableIRIn(); // запускаем прием
  //irrecv.enableIRIn();
}

void loop() {

    remotePult.operate();

  
  //if ( remotePult.decode()) { // если данные пришли
  
  /*
  if ( remotePult.decode()) { // если данные пришли
    //remotePult.traceDecodedIRData();
    
    
    Serial.print("decodedRawData: "); Serial.println(remotePult.decodedIRData.decodedRawData, HEX);
    //Serial.print("address: "); Serial.println(irrecv.decodedIRData.address, HEX);
    //Serial.print("extra: "); Serial.println(irrecv.decodedIRData.extra);
    //Serial.print("flags: "); Serial.println(irrecv.decodedIRData.flags);
    //Serial.print("numberOfBits: "); Serial.println(irrecv.decodedIRData.numberOfBits);    
    //Serial.print("protocol: "); Serial.println(irrecv.decodedIRData.protocol);
    //Serial.println("----------------------");
    
    
    //remotePult.resume(); // принимаем следующую команду
    remotePult.resume(); // принимаем следующую команду
  }
  */
  
  
}
