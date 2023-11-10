//  *********************************************
//                    ESCRAVO 2
//  *********************************************

// Pino Entrada - Selecao Escravo
int PinFault = 10;
int LED = 9;

// Variaveis Globais
char Flag = '0';
String readString;

struct HDLCFrame {
  char flag[9];         
  char address[9];      
  char control[17];     
  char information[10];  
  char fcs[33];        
};

 HDLCFrame frame;

void CreateFrame (int Fault) { 

  //**********************************//
  //*********** Monta Flag ***********//  
  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0';    // Preenchendo com '0'
  }  
  //**********************************//
  //*********** Monta Endereço  *****//
  for (int i = 0; i < 8; i++) {
    frame.address[i] = '0';   // Preenchendo com '0' 
  } 
  //**********************************//
  //*********** Monta Controle  *****//  
  for (int i = 0; i < 16; i++) {
    frame.control[i] = '0';    // Preenchendo com '0'   
  }  
  //**********************************//
  //********** Monta Informação *****//  
  for (int i = 0; i < 9; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }
  frame.information[9] = '\0';  // Adicionando o caractere nulo no final  
  //**********************************//
  //************* Monta fcs *********//
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
    }
  
  if(Fault == 0){
    frame.fcs[31] = '0';	 
  }else{
    frame.fcs[31] = '1';    
  }    
  frame.fcs[32] = '\0';  // Adicionando o caractere nulo no final

  //*********** Imprime Frame - Envia para Info. Mestre ***********//
  Serial.print(frame.flag);
  Serial.print(frame.address);
  Serial.print(frame.control);
  Serial.print(frame.information);
  Serial.print(frame.fcs);
}

int GenerateFault()
{

  int Fault = digitalRead(PinFault);
  return Fault;

}

  
void setup() {  
  //SerialPort.begin(9600); 
  Serial.begin(9600);
  pinMode(PinFault, INPUT);
  pinMode(LED,OUTPUT);
}

void loop() { 

  //Recebe Informações Mestre
  while (Serial.available()){
    readString = Serial.readString();
    //Serial.print(readString);      
  }
  if (readString[17] == '1') {
    int GenerateFault1 = GenerateFault();
    CreateFrame(GenerateFault1);
    Serial.println();
    delay(10);

    if (readString[42] == '0') {
      digitalWrite(LED, LOW);
    }else{
      digitalWrite(LED, HIGH);   
    }
  }else
  {
    digitalWrite(LED, LOW);
  } 
}
