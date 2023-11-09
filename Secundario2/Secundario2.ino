// Biblioteca para Serial
#include <SoftwareSerial.h>

char number  = ' ';
int LED = 2;
int PinRX  = 7;
int PinTX  = 8;
String readString;

// Configuração da porta Serial Software (RX, TX)
SoftwareSerial SerialPort(0,1);  // Use pinos 16 (RX) e 17 (TX) para a comunicação

void setup()
{
  //Serial.begin(9600);  // Inicializa a comunicação Serial Software
  SerialPort.begin(9600); // Inicializa a comunicação Serial Software 
  pinMode(LED, OUTPUT);
  pinMode(PinRX,INPUT);
  pinMode(PinTX,OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(PinTX, LOW); 
}
void loop(){
  
	int READ_MASTER = digitalRead(PinRX);
  	if (READ_MASTER == 0){digitalWrite(LED, LOW);
                          digitalWrite(PinTX, LOW);}
  	if (READ_MASTER == 1){digitalWrite(LED, HIGH);
                          digitalWrite(PinTX, HIGH);}
  
	/* while (SerialPort.available()){
 		//read the input
    	digitalWrite(LED, HIGH);
      	delay(1000);
      	digitalWrite(LED, LOW);
      	delay(1000);
      	//Serial.println('a');
    	char c =SerialPort.read();
    	readString+=c;
      	//SerialPort.print(readString);
      	
      	if(readString[8] == '0'){
          	digitalWrite(LED, LOW);
      		delay(10);
    		//SerialPort.println(readString);
			//if (readString =="ON"){
        	//	digitalWrite(LED, HIGH);
        	//}   		
			//readString=" ";
    } 
      	if(readString[8] == '1'){
          	digitalWrite(LED, HIGH);
      		delay(10);
    		//SerialPort.println(readString);
			//if (readString =="ON"){
        	//	digitalWrite(LED, HIGH);
        	//}   		
			readString=" ";
    }        
      
       
    }*/		

}
  
  

