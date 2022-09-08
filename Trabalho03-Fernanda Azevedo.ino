// Trabalho 03
// Data: 12/05/2022
// Aluna: Fernanda Azevedo/1912623

#define LED1  4           // LED 1 PINO 4
#define LED2  5           // LED 2 PINO 5
#define LED3  6           // LED 3 PINO 6
#define LED4  7           // LED 4 PINO 7
#define Botao 3           // Botao Controlador de Passo
#define Botao_Interrupt 2 // Botao de Interrupção
 
//variaveis globais:
int tempo_delay = 300;
 
void (*PonteiroDeFuncao)(); //ponteiro de função da máquina de estados.
void (*temp)(); // posição da máquina de estados antes do stop
 
//Funções de Estado
void ESTADO_1(void);          // Passo 1 (Passo completo) e Passo 1 (Meio Passo)
void ESTADO_1_2(void);        // Passo 2 Meio Passo   
void ESTADO_2(void);          // Passo 2 (Passo completo) e Passo 3 (Meio Passo)
void ESTADO_2_3(void);        // Passo 4 (Meio Passo)
void ESTADO_3(void);          // Passo 3 (Passo completo) e Passo 5 (Meio Passo)
void ESTADO_3_4(void);        // Passo 6 (Meio Passo)
void ESTADO_4(void);          // Passo 4 (Passo completo) e Passo 7 (Meio Passo)
void ESTADO_4_1(void);        // Passo 8 (Meio Passo)
void ESTADO_5(void);          // Estado de interrupção
 
void Botao_ISR();             // Função para interrupção do botao
 
void setup() {

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(Botao, INPUT_PULLUP);
  pinMode(Botao_Interrupt, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(Botao_Interrupt), Botao_ISR, CHANGE);
  Serial.begin(9600);

} 
 
void Botao_ISR(){
         if (!digitalRead(Botao_Interrupt)){
            temp = PonteiroDeFuncao;
            PonteiroDeFuncao = ESTADO_5;}
         else{
            PonteiroDeFuncao = temp;}       
} 
 
void ESTADO_1(void)
{
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          if(digitalRead(Botao)){
            PonteiroDeFuncao = ESTADO_2;}
          else
           {PonteiroDeFuncao = ESTADO_1_2;}
}

void ESTADO_1_2(void)
{
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          if(digitalRead(Botao)){
           PonteiroDeFuncao = ESTADO_2;}
          else
           {PonteiroDeFuncao = ESTADO_2;}
}

void ESTADO_2(void)
{
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, LOW);
          if(digitalRead(Botao)){
           PonteiroDeFuncao = ESTADO_3;}
          else
           {PonteiroDeFuncao = ESTADO_2_3;}
}
void ESTADO_2_3(void)
{
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          if(digitalRead(Botao)){
           PonteiroDeFuncao = ESTADO_3;}
          else
           {PonteiroDeFuncao = ESTADO_3;}
}

void ESTADO_3(void)
{
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, LOW);
          if(digitalRead(Botao)){
           PonteiroDeFuncao = ESTADO_4;}
          else
           {PonteiroDeFuncao = ESTADO_3_4;}
}

void ESTADO_3_4(void)
{
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
          digitalWrite(LED4, HIGH);
          if(digitalRead(Botao)){
            PonteiroDeFuncao = ESTADO_4;}
          else
           {PonteiroDeFuncao = ESTADO_4;}
}

void ESTADO_4(void)
{
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          if(digitalRead(Botao)){
           PonteiroDeFuncao = ESTADO_1;}
          else
           {PonteiroDeFuncao = ESTADO_4_1;}
}

void ESTADO_4_1(void)
{
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
          digitalWrite(LED4, HIGH);
          if(digitalRead(Botao)){
            PonteiroDeFuncao = ESTADO_1;}
          else
           {PonteiroDeFuncao = ESTADO_1;}
}


void ESTADO_5(void)
{
           digitalWrite(LED1, LOW);
           digitalWrite(LED2, LOW);
           digitalWrite(LED3, LOW);
           digitalWrite(LED4, LOW);
}
 
void loop() 
{
        PonteiroDeFuncao = ESTADO_1; //aponta para o estado inicial.

        while(1)
        {
            if (Serial.available()>1){
            tempo_delay = Serial.parseInt();
            Serial.println(tempo_delay);}
 
           (*PonteiroDeFuncao)();    //chama a função apontada pelo ponteiro de função 
           delay(tempo_delay);
           
           
        }
}