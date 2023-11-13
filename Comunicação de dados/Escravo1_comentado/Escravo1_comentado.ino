//  *********************************************
//                    ESCRAVO 1
//  *********************************************
#include <SoftwareSerial.h>
SoftwareSerial Mestre(6, 7); // RX, TX

// Pino Entrada - Selecao Escravo
int PinFault = 10; // gerar falha 
int LED = 13;

// Variaveis Globais
char Flag = '0';
String readString = " ";
int temp = 0;

struct HDLCFrame {
  char flag[8];         
  char address[8];      
  char control[16];     
  char information[10]; 
  char fcs[32];        
};

 HDLCFrame frame;

void CreateFrame (int Fault) { 

  //**********************************//
  //*********** Monta Flag ***********//  
  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0';      // Preenchendo com '0'  
  }  
  //**********************************//
  //*********** Monta Endereço  *****//
  for (int i = 0; i < 8; i++) {
    frame.address[i] = '0';     // Preenchendo com '0'     
  } 
  //**********************************//
  //*********** Monta Controle  *****//  
  for (int i = 0; i < 16; i++) {
    frame.control[i] = '0';     // Preenchendo com '0'    
  }  
  //**********************************//
  //********** Monta Informação *****//  
  for (int i = 0; i < 10; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }  
  //**********************************//
  //************* Monta fcs *********//
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
    }
  
  if(Fault == 0){
    frame.fcs[20] = '0';	 
  }else{
    frame.fcs[20] = '1';    
  }    
  frame.fcs[32] = '\0';  // Adicionando o caractere nulo no final

  //*********** Imprime Frame - Envia para Info. Mestre ***********//
  Mestre.write(frame.flag); // Eviando o frame para o mestre 
  Mestre.println();
}

int GenerateFault()
{

  int Fault = digitalRead(PinFault); //identifica se o simulador de falha esta selecionado
  return Fault;

}
  
void setup() {  
  Serial.begin(9600);
  Mestre.begin(9600);
  pinMode(PinFault, INPUT);
  pinMode(LED,OUTPUT);
}

void loop() { 

  int GenerateFault1 = GenerateFault();
  
  /********Trocar Serial por Mestre para Simular o Fisico! ************/
  while (Mestre.available()){
    //Serial.println("SERIAL - Aguarda Envio  Mestre!");
    delay(1);  
    if(Mestre.available()>0){      
      readString = Mestre.readString();
    }
  }
  /*********************************************************************/
  /*Frame Recebido - Mostra no Monitor*/
  if(readString != " " & readString[8] == '1'){
    Serial.println();    
    Serial.print(readString);         
  }
  
  /*Verifica se Foi Enviado Flag*/
  if(readString[3] == '1' & GenerateFault1 == 0 ){   

   
    /*Verifica se foi endereçado para esse Escravo*/
    if (readString[8] == '1') { 
      Serial.println();
      Serial.println("SERIAL - Comando Recebido do Mestre");
      digitalWrite(LED, HIGH);
      //Mestre.println();
      if(temp == 0){
        CreateFrame(GenerateFault1);
        delay(1);
        temp = 1;     
      }
      //Recebe Informacao - Liga - Desliga Led

      if (readString[41] == '0') {
        Serial.println("Desliga LED ");
        digitalWrite(LED, LOW);
        temp = 0;
      }else{
        digitalWrite(LED, HIGH);
        Serial.println("Liga LED "); 
      }
      

    } else
    {
      //Escravo N Selecionado.
      digitalWrite(LED, LOW);
    }
  }
   /*Retorno Falha Gerada*/
  if (readString[3] == '1' & readString[8] == '1' & GenerateFault1 == 1){
    Serial.println("#Retorno Falha#");
    CreateFrame(GenerateFault1);    
    digitalWrite(LED, LOW);
    delay(1);
  }

  readString = " ";
  delay(1);

}
