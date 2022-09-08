// Trabalho 5
// Data: 23/06/2022
// Aluna: Fernanda Azevedo\1912623

#include "Arduino_FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

#define LED0 4
#define potenciometro A0
#define botao 2

/* Variáveis para armazenamento do Handle dos timers*/
TimerHandle_t xTimer2;

/* Variáveis para armazenamento do Handle das tasks*/
TaskHandle_t xTarefa1Handle;
TaskHandle_t xTarefa2Handle;
TaskHandle_t xTarefa3Handle;

/* Variáveis para armazenamento do Handle da fila */
QueueHandle_t xFila1;

void vTarefa1 (void *pvParameters);
void funcaoTimer2(TimerHandle_t xTimer2);
void vTarefa2(void *pvParameters);
void vTarefa3(void *pvParameters);

int valor_potenciometro=0;

void setup() {
  
  Serial.begin(9600);
  pinMode(botao,INPUT_PULLUP);  
  pinMode(LED0,OUTPUT);
  
  xTaskCreate(vTarefa1,"vTarefa1",128,NULL,1,&xTarefa1Handle); // Cria a tarefa 1
  xTaskCreate(vTarefa2,"vTarefa2",128,NULL,1,&xTarefa2Handle); // Cria a tarefa 2
  xTaskCreate(vTarefa3,"vTarefa3",128,NULL,2,&xTarefa3Handle); // Cria a tarefa 3


  xFila1=xQueueCreate(5,sizeof(int)); // Cria a fila da tarefa 1 com 5 elementos
  xTimer2=xTimerCreate("TIMER2",pdMS_TO_TICKS(250),pdTRUE,0,funcaoTimer2); // Cria o timer da tarefa 2 com freq. de 2Hz

}

void loop() {
  
  // Código executado nas tarefas

}


void vTarefa1(void *pvParameters){
   
   int valor=0;
   int valor_atual=0;
   while(1){

     if(xQueueReceive(xFila1,&valor,pdMS_TO_TICKS(1000))==pdTRUE){
        if(valor!=valor_atual){ // Verifica se há um novo valor na entrada
          
          Serial.print("Iniciando Tarefa 1...");
          Serial.println("Valor da entrada analógica: " + String(valor)); // Imprime na serial a entrada analógica         
          valor_atual=valor;

        }
        
     }

   } 

}

// Função para piscar o LED
void funcaoTimer2(TimerHandle_t xTimer2){

    digitalWrite(LED0,!digitalRead(LED0)); // Pisca o LED na frequência de 2Hz
}
void vTarefa2(void *pvParameters){  
   
   while(1){
     
     if(ulTaskNotifyTake(pdTRUE,portMAX_DELAY)==pdTRUE){
      
      Serial.println("Iniciando Tarefa 2...");
      xTimerStart(xTimer2,0);  // Inicia o timer da tarefa 2     

     }  
    else{

      xTimerStop(xTimer2,0);

    }
   }
   
}
void vTarefa3(void *pvParameters){

  uint8_t debouncingContagem=0;
  uint8_t x=0;
  
  while(1){

    // Verifica as condições do botao e envia para a tarefa 2   
    if(digitalRead(botao)==LOW and x==0){
       debouncingContagem++;

      if(debouncingContagem>5){
         debouncingContagem=0;
         Serial.println("Tarefa 3: botão pressionado !");
         xTaskNotifyGive(xTarefa2Handle);          
         x=1;        
       }

       vTaskDelay(pdMS_TO_TICKS(10));
       
     }
    else{
         
         debouncingContagem=0;
     
       }

    if(digitalRead(botao)==HIGH){

       x=0;
       
     }

     // Envia leitura do potenciometro para a Tarefa1
     valor_potenciometro=analogRead(potenciometro);
     valor_potenciometro=map(valor_potenciometro,0,1023,0,100);         
     xQueueSend(xFila1,&valor_potenciometro,pdMS_TO_TICKS(1000));

 }
}
