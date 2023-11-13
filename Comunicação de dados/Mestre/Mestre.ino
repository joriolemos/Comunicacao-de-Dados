//  *********************************************
//                    MESTRE
//  *********************************************
#include <SoftwareSerial.h>
SoftwareSerial Escravo1(2, 3); // RX, TX
SoftwareSerial Escravo2(4, 5); // RX, TX

// Pino Entrada - Selecao Escravo

int PinSend = 9;
int PinSel = 10;

// Variaveis Globais
char Flag = '0';
char LigaLed = '0';
String readString = " ";
int temp = 0;
int EscravoRetain = 0;
int SlaveSel1 = 0;

struct HDLCFrame {
  char flag[8];         
  char address[8];      
  char control[16];     
  char information[10]; 
  char fcs[32];         
};

 HDLCFrame frame;

void CreateFrame (char Flag, int Escravo, char LigaLed) { 

  //**********************************//
  //*********** Monta Flag ***********//  
  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0'; 
  }  

  //Confirma Envio de Informação para os Escravos.
  frame.flag[3] = Flag;

  //**********************************//
  //*********** Monta Endereço  *****//
  for (int i = 0; i < 8; i++) {
    frame.address[i] = '0';    
  }  

  //Seleção Escravo
  if(Escravo == 0){
  	frame.address[0] = '1';
    frame.address[1] = '0';
   }
  else{
    frame.address[0] = '0';
    frame.address[1] = '1';
   }

  //**********************************//
  //*********** Monta Controle  *****//
  
  for (int i = 0; i < 16; i++) {
    frame.control[i] = '0';    
  }  
  //**********************************//
  //********** Monta Informação *****//
  
  for (int i = 0; i < 10; i++) {
    frame.information[i] = '0';  // Preenchendo com '0'
  }
  frame.information[9] = LigaLed;
  //frame.information[9] = '\0';  // Adicionando o caractere nulo no final  

  
  //**********************************//
  //************* Monta fcs *********//
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
    }
  frame.fcs[32] = '\0';  // Adicionando o caractere nulo no final

  //*********** Imprime Frame ***********//
  Escravo1.write(frame.flag);
  Escravo1.println();
  
  Escravo2.write(frame.flag);
  Escravo2.println();
}

int SlaveSel()
{
  int Escravo = digitalRead(PinSel);  

  if(Escravo == 1 & EscravoRetain == 0){
    Serial.println("### Escravo 2 - Selecionado ###");
    EscravoRetain = 1;
  }else{
    if(Escravo == 1){
    	Serial.println("### Escravo 1 - Selecionado ###");
   		EscravoRetain = 0;
    }
  }
  delay(500);

  return EscravoRetain;
}
  
void setup() {  
  Serial.begin(9600);
  Escravo1.begin(9600);
  Escravo2.begin(9600);
  pinMode(PinSel, INPUT);
  pinMode(PinSend, INPUT);  
}

void loop() {

  SlaveSel1 = SlaveSel(); 

  if(digitalRead(PinSend) == 1){ 

    Flag = '1';
    LigaLed = '1'; 
    Serial.print("Frame Enviado para o  Escravos!");
    Serial.println();
	  CreateFrame(Flag,SlaveSel1,LigaLed);

    /********Le retorno Escravo 01 ************/
    if(SlaveSel1 == 0){      
      //Recebe Informações Escravos
      Serial.print("### Aguarda Retorno Escravo 01! ###");
      delay(2000);
      /********Trocar Serial por Escravo1 para Simular o Fisico! ************/
      while (Escravo1.available()){
        delay(1);   
        if(Escravo1.available()>0){      
          readString = Escravo1.readString();
        }
      }
      /************************************************************************/
    }
    /********Le Retorno Escravo 02 ************/
    else{
      //Recebe Informações Escravos
      Serial.print("### Aguarda Retorno Escravo 02! ###");
      delay(2000);
      /********Trocar Serial por Escravo2 para Simular o Fisico! ************/
      while (Escravo2.available()){
        delay(1);   
        if(Escravo2.available()>0){      
          readString = Escravo2.readString();
        }
      }
    /************************************************************************/
    }    

    /********Verifica Se foi recebido o Frame ************/
    if(readString != " "){
      Serial.println();
      Serial.print("##### Recebido Escravo ####### ");  
      Serial.print(readString);
      Serial.print(" #####");
      }
    else{
      Serial.println();
      Serial.print("##### Falha - Não recebido Informacao Escravo! ####### ");    
      } 

    //Flag = '0'; 
    LigaLed = '0';
    CreateFrame(Flag,SlaveSel1,LigaLed);  
    delay(1000);

    if(readString[62] == '0'){         
      Serial.println();
      Serial.print("##### Comunicacao OK - Fim Comunicacao #####");
      Serial.println();  

    }else{
      Serial.println();
      Serial.print("##### Fault - Erro Retorno Escravo!!!!! #####");
      Serial.println();

    }
    temp = 0;
    readString = " ";
    
  }else{ 
    if(digitalRead(PinSend) == 0 & temp == 0){
      Serial.println("Pressione Botao Enviar"); 
      temp = 1;
      //Frame Inicial.
      Flag = '0';
      LigaLed = '0';
      //CreateFrame(Flag,SlaveSel1,LigaLed);
      readString = " ";
    }   
  }

  

}
