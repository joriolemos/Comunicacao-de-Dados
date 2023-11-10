//  *********************************************
//                    MESTRE
//  *********************************************

// Pino Entrada - Selecao Escravo
int PinSend = 9;
int PinSel = 10;

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

void CreateFrame (char Flag, int Escravo) { 

  //**********************************//
  //*********** Monta Flag ***********//  
  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0'; 
  }  
  frame.flag[8] = Flag;  
  //**********************************//
  //*********** Monta Endereço  *****//
  for (int i = 0; i < 8; i++) {
    frame.address[i] = '0';
    
  }  
  if(Escravo == 0){
  	frame.address[7] = '1';
    frame.address[8] = '0';
   }
  else{
    frame.address[7] = '0';
    frame.address[8] = '1';
   }  
  //**********************************//
  //*********** Monta Controle  *****//
  
  for (int i = 0; i < 16; i++) {
    frame.control[i] = '0';    
  }  
  //**********************************//
  //********** Monta Informação *****//
  
  for (int i = 0; i < 9; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }
  frame.information[8] = Flag;
  frame.information[9] = '\0';  // Adicionando o caractere nulo no final  

  
  //**********************************//
  //************* Monta fcs *********//
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
    }
  frame.fcs[32] = '\0';  // Adicionando o caractere nulo no final

  //*********** Imprime Frame ***********//
  Serial.print(frame.flag);
  Serial.print(frame.address);
  Serial.print(frame.control);
  Serial.print(frame.information);
  Serial.print(frame.fcs);
}

int SlaveSel()
{
  int Escravo = digitalRead(PinSel);
  return Escravo;
}
  
void setup() {  
  Serial.begin(9600);
  pinMode(PinSel, INPUT);
  pinMode(PinSend, INPUT);
}

void loop() {

  //Recebe Informações Escravos
  while (Serial.available()){
    readString = Serial.readString();
    //Serial.print(readString);      
  }

  int SlaveSel1 = SlaveSel();
  
  if(digitalRead(PinSend) == 1 & readString[73] == 0){
    Flag = '1'; 
	CreateFrame(Flag,SlaveSel1);
    Serial.println();
    delay(10);
  }else{
    Flag = '0';
    CreateFrame(Flag,SlaveSel1);
    Serial.println();
    delay(10);
  } 
}
