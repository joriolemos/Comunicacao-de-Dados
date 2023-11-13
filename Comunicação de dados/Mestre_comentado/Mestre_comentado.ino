//  *********************************************
//                    MESTRE
//  *********************************************
#include <SoftwareSerial.h> // Biblioteca que permite a comunicação via serial com dispositivos externos. 
SoftwareSerial Escravo1(2, 3); // RX, TX
SoftwareSerial Escravo2(4, 5); // RX, TX
// Usei porta diferente do definido (rx-0, tx-1) para os dados que estão sendo transmitidos não atrapalharem a leitura do monitor serial
// Quero enviar apenas o frame
// Pino Entrada - Selecao Escravo

int PinSend = 9; // Botão de envio
int PinSel = 10; // Botão de seleção dos escravos

// Variaveis Globais
char Flag = '0'; // Quando o frame é enviado vai pra 1. Serve p o escravo identifcar que deve fazer algo
char LigaLed = '0'; // Eviar o comando junto com o frame transmitido
String readString = " "; // O frame lido do escravo 
int temp = 0; // Não mostrar a mensagem "Aperte o botão" varias vezes 
int EscravoRetain = 0; // Ultima seleção (toggle do botão de seleção)
int SlaveSel1 = 0; // Leitura do pino selecionado (escravo)

struct HDLCFrame { // Estrutura utilizada para construir e enviar quadros para os escravos 
  char flag[8];         
  char address[8];      
  char control[16];     
  char information[10]; 
  char fcs[32];         
};

 HDLCFrame frame; // Criou um frame com a estrutura acima

void CreateFrame (char Flag, int Escravo, char LigaLed) { // Função que vai ser chamada no loop 

  //**********************************//
  //*********** Monta Flag ***********//  
  for (int i = 0; i < 8; i++) {
    frame.flag[i] = '0'; // Inicializa todos os endereços do vetor da flag em 0.  
  }  

  //Confirma Envio de Informação para os Escravos.
  frame.flag[3] = Flag; // No endereço 3 

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
  
  //**********************************//
  //************* Monta fcs *********//
  
  for (int i = 0; i < 32; i++) {
    frame.fcs[i] = '0';
    }
  frame.fcs[32] = '\0';  // Adicionando o caractere nulo no final

  //*********** Imprime Frame ***********//
  Escravo1.write(frame.flag); // imprime a estrutura inteira do frame 
  Escravo1.println();
  
  Escravo2.write(frame.flag);
  Escravo2.println();
}

int SlaveSel() // Função para seleção de escravo
{
  // Quando o frame é enviado pelo botação de envio, ele é direcionado ao escravo 1
  // Logo a função abaixo seleciona o escravo dois para envio quando o botão estava anteriormente
  //inicializado em zero e foi apertado
  //Para selecionar o escravo 1 novamente eu devo apertar o botão de envio.
  int Escravo = digitalRead(PinSel);  

  if(Escravo == 1 & EscravoRetain == 0){ // EscravoRetain variavel inicializada em zero
    Serial.println("### Escravo 2 - Selecionado ###");
    EscravoRetain = 1; // muda para 1 para que na proxima vez caia no if abaixo 
  }else{
    if(Escravo == 1){
    	Serial.println("### Escravo 1 - Selecionado ###");
   		EscravoRetain = 0;
    }
  }
  delay(500);

  return EscravoRetain;
}
  
void setup() {  // roda 1 vez para configurar os pinos
  Serial.begin(9600);
  Escravo1.begin(9600);
  Escravo2.begin(9600);
  pinMode(PinSel, INPUT);
  pinMode(PinSend, INPUT);  
}

void loop() {

  SlaveSel1 = SlaveSel(); //SlaveSel1 armazena o valor retornado pela função SlaveSel (SlaveSel = seleção de escravo)

  if(digitalRead(PinSend) == 1){ // digitalRead é o pino para enviar o frame 
    //Se o botão for apertado o frame é enviado

    Flag = '1';
    LigaLed = '1'; 
    Serial.print("Frame Enviado para o  Escravos!");
    Serial.println();
	  CreateFrame(Flag,SlaveSel1,LigaLed);

    /********Le retorno Escravo 01 ************/
    if(SlaveSel1 == 0){ // SlaveSel1 == 0 Espero resposta do escravo1   
      //Recebe Informações Escravos
      Serial.print("### Aguarda Retorno Escravo 01! ###");
      delay(2000);
      /********Trocar Serial por Escravo1 para Simular o Fisico! ************/
      while (Escravo1.available()){ // available serve para ficar lendo as informações enviadas pelo escravo
        delay(1);   
        if(Escravo1.available()>0){ // Se for maior que 0 ele está recebendo informações 
          readString = Escravo1.readString(); // Ele le o pacote e armazena na variavel readString
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
      Serial.print(readString); // print na informação recebida 
      Serial.print(" #####");
      }
    else{
      Serial.println();
      Serial.print("##### Falha - Não recebido Informacao Escravo! ####### ");    
      } 
       
    LigaLed = '0';
    CreateFrame(Flag,SlaveSel1,LigaLed); // Envio novamente para desligar o led
    delay(1000);

    if(readString[62] == '0'){ // Posição 62 é para gerar falhas e dizer que o pacote não chegou completamente       
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
