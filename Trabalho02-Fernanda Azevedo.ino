
#define LED1 4 // Define LED no pino 4
#define LED2 5 // Define LED no pino 5
#define LED3 6 // Define LED no pino 6
#define LED4 7 // Define LED no pino 7
#define botao 2 // Define botao no pino 2

void setup(){

pinMode(LED1,OUTPUT); // configuração do pino do led
pinMode(LED2,OUTPUT);
pinMode(LED3,OUTPUT);
pinMode(LED4,OUTPUT);
pinMode(botao,INPUT_PULLUP); // configuração do pino do botao

}

void(*PonteirodeFuncao)();

void ESTADO_1(void);
void ESTADO_2(void);
void ESTADO_3(void);
void ESTADO_4(void);

void ESTADO_1(void){

       digitalWrite(LED1,HIGH);
       digitalWrite(LED2,LOW);
       digitalWrite(LED3,LOW);
       digitalWrite(LED4,LOW);
       if(digitalRead(botao)){
          PonteirodeFuncao=ESTADO_2;
       }
       else{
          PonteirodeFuncao=ESTADO_4;
       }
         
}
void ESTADO_2(void){

       digitalWrite(LED1,LOW);
       digitalWrite(LED2,HIGH);
       digitalWrite(LED3,LOW);
       digitalWrite(LED4,LOW);
       if(digitalRead(botao)){
          PonteirodeFuncao=ESTADO_3;
       }
       else{
          PonteirodeFuncao=ESTADO_1;
       }
         
}
void ESTADO_3(void){

       digitalWrite(LED1,LOW);
       digitalWrite(LED2,LOW);
       digitalWrite(LED3,HIGH);
       digitalWrite(LED4,LOW);
       if(digitalRead(botao)){
          PonteirodeFuncao=ESTADO_4;
       }
       else{
          PonteirodeFuncao=ESTADO_2;
       }
         
}
void ESTADO_4(void){

       digitalWrite(LED1,LOW);
       digitalWrite(LED2,LOW);
       digitalWrite(LED3,LOW);
       digitalWrite(LED4,HIGH);
       if(digitalRead(botao)){
          PonteirodeFuncao=ESTADO_1;
       }
       else{
          PonteirodeFuncao=ESTADO_3;
       }
         
}
void loop(){
      // Aponta para o estado inicial, nunca esquecer de informar o estado inicial
      // senão nesse caso especifico pode haver um erro fatal
      PonteirodeFuncao=ESTADO_1;

      while(1){
         
         (*PonteirodeFuncao)();
          delay(300);
      }
}