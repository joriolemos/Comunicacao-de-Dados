// Biblioteca para Serial
//#include <SoftwareSerial.h>

// Configuração da porta Serial Software (RX, TX)
//SoftwareSerial SerialPort(0, 1);  // Use pinos 0 (RX) e 1 (TX) para a comunicação

int PinTX = 7;
int PinRX = 8;
char Flag = '0';
int PinSel = 10;

struct HDLCFrame {
  char flag[9];     //Envio
  char address[9];  //Enviar 1 ou 2
  char control[17]; //Resposta 
  char information[10]; //Liga  
  char fcs[33];         //indica falha entrega 
};

 HDLCFrame frame;

void CreateFrame (char Flag, int Escravo) { 
  
  //*********** Monta Flag ***********//
  
  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0'; 
  }
  //frame.flag[8] = '1';
  frame.flag[8] = Flag;
  
  if(Flag == '1'){
  	digitalWrite(PinTX, HIGH);
    //delay(1000);
  }
  
  if(Flag == '0'){
  	digitalWrite(PinTX, LOW);
    //delay(1000);
  }  
  //*********** FIM Monta Flag ***********//
  
  //*********** Monta Endereço ***********//
  for (int i = 0; i < 8; i++) {
    frame.address[i] = '0';
    //frame.address[i] = char(Escravo);
    
  }
  
  //frame.address[8] = char(Escravo);
  
  //itoa(3, frame.address + 8, 10);  // Concatenando o valor do endereço
 
  if(Escravo == 0){
  	itoa(1, frame.address + 4, 5);  // Concatenando o valor do controle
   }
  else{
    itoa(2, frame.address + 4, 5);  // Concatenando o valor do controle	 
   }
   
  
  //*********** FIM Monta Endereço ***********//
  
  //*********** Monta Controle ***********//
  
  for (int i = 0; i < 8; i++) {
    frame.control[i] = '0';    
  }
  
  //*********** FIM Monta Controle ***********//
  
  //*********** Monta Informação ***********//
  
  for (int i = 0; i < 9; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }
  frame.information[9] = '\0';  // Adicionando o caractere nulo no final
  
  //*********** FIM Monta Informação ***********//
  
  //*********** Monta fcs ***********//
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
  }
  frame.fcs[32] = '\0';  // Adicionando o caractere nulo no final

  //*********** FIM Monta fcs ***********//
  
  // Acessando os campos da estrutura e imprimindo-os
  
  //SerialPort.print(frame.flag);
  //SerialPort.print(frame.address);
  //SerialPort.print(frame.control);
  //SerialPort.print(frame.information);
  //SerialPort.print(frame.fcs); 
  
  Serial.print(frame.flag);
  Serial.print(frame.address);
  Serial.print(frame.control);
  Serial.print(frame.information);
  Serial.print(frame.fcs); 

  
  //for (int i = 0; i < 8; i++) {
    //if (frame.flag[i] == '0') {digitalWrite(PinTX, LOW);delay(100);}
  //  if (frame.flag[i] == 1) {digitalWrite(PinTX, HIGH);delay(1000);}
  //}  
}
  
  
void setup() {  
  //SerialPort.begin(9600); 
  Serial.begin(9600);
  pinMode(PinTX, OUTPUT);
  pinMode(PinRX, INPUT);
  pinMode(PinSel, INPUT);
  digitalWrite(PinTX, LOW);
  
}

void loop() { 
  
    //Serial.print(1);
  	//delay(10);
  	//Serial.print(0);
  	//delay(10);  
    int Escravo = digitalRead(PinSel);
    // Preenchendo a estrutura sem usar strcpy
  	//CreateFrame();
	if (digitalRead(PinRX)== 0){
      
  		Flag = '1'; 
		CreateFrame(Flag,Escravo);
  		Serial.println();
  		//SerialPort.println();
  		delay(1000);
    }
  	
	if (digitalRead(PinRX) == 1){
  		Flag = '0';
    	CreateFrame(Flag,Escravo);
  		Serial.println();
  		//SerialPort.println();
   		delay(1000);  
  }
    
  
  
  
}
