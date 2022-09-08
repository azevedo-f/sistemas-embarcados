// Trabalho 4
// Data: 02/06/2022
// Nome: Fernanda_Azevedo/1912623

#include "Arduino_FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LED1 7 // define pino do LED1 tarefa2
#define LED2 8 // define pino do LED2 tarefa3
#define entrada_potenciometro A0 // define entrada analógica do potenciometro tarefa
#define botao 2  // define pino do botao

/* Estrutura para os parâmetros do LED */
typedef struct piscar_LED {

  uint16_t intervalo;
  uint8_t pino;

} pLED;

/* Valores parâmetros do LED1 com 1 Hz; LED2 com 0,5 Hz */
pLED led1 = {500, LED1};
pLED led2 = {1000, LED2};

/* Variáveis para armazenamento do handle das tasks */
TaskHandle_t xTarefa1Handle;
TaskHandle_t xTarefa2Handle;
TaskHandle_t xTarefa3Handle;

SemaphoreHandle_t xSemaforoContador;

/*Protótipos das Tarefas*/
void vTarefa1 (void *pvParameters);
void vTarefa_piscarLED (void *pvParameters);

/* Função de aquisição do botao */
void funcao_ISR (){ 
  
    xSemaphoreGiveFromISR(xSemaforoContador, pdTRUE);     
    portYIELD_FROM_ISR();   
 
}

void setup() {

  Serial.begin(9600);
  pinMode(botao,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(botao), funcao_ISR, FALLING); // Função de aquisição do botão
  xSemaforoContador = xSemaphoreCreateCounting(32,0);

  xTaskCreate(vTarefa1, "vTarefa1",256, NULL, 2, &xTarefa1Handle);  
  xTaskCreate(vTarefa_piscarLED, "vTarefa2", 128, (void *)&led1, 2, &xTarefa2Handle); // Função instanciada em vTarefa_piscarLED
  xTaskCreate(vTarefa_piscarLED, "vTarefa3", 128, (void *)&led2, 2, &xTarefa3Handle); // Função instanciada em vTarefa_piscarLED

  /* Inicia ambas as tarefas suspensas*/
  vTaskSuspend(xTarefa2Handle); 
  vTaskSuspend(xTarefa3Handle);

}

void loop() {

//As funções são executadas nas tarefas

}

void vTarefa1(void *pvParameters){

  UBaseType_t ticket_semaforo;
  int valor_pot=0;
  int count = 0;
    
  while(1)
  {

    xSemaphoreTake(xSemaforoContador, portMAX_DELAY);
    

    /* Controle execução da tarefa 2 ou tarefa 3 */
    if (analogRead(entrada_potenciometro) <= 500) // valor da entrada do potenciometro <= 500 suspende tarefa 2 e ativa tarefa 3
     {
       
      Serial.println("Tarefa 2 suspensa, Tarefa 3 ativa");
      vTaskSuspend(xTarefa2Handle); // Suspende a tarefa 2
      vTaskResume(xTarefa3Handle); // Ativa a tarefa 3

    }
    
    else if (analogRead(entrada_potenciometro)> 500) // valor da entrada do potenciometro > 500 suspende tarefa 3 e ativa tarefa 2
    {

      Serial.println("Tarefa 3 suspensa, Tarefa 2 ativa");
      vTaskSuspend(xTarefa3Handle); // Suspende a tarefa 3
      vTaskResume(xTarefa2Handle);  // Ativa a tarefa 2

    }
    
    /* Realiza a aquisição e imprime o valor atual na serial */
    for (count = 0; count < 4; count++) {

      valor_pot = analogRead(entrada_potenciometro);
      vTaskDelay(pdMS_TO_TICKS(500)); // Realiza a leitura de 2 amostras por segundo
      Serial.println("Valor ADC atual: " + String(valor_pot));

    }  
    
    /* Imprime a quantidade  de requisições faltantes */
    Serial.println("Requisições faltantes: ");
    ticket_semaforo = uxSemaphoreGetCount(xSemaforoContador);
    Serial.print(ticket_semaforo);
    Serial.print ("\n");
    
  }
}

/* Função das tarefa 2 e 3  */
void vTarefa_piscarLED(void *pvParameters) {

  pLED *led = (pLED *)pvParameters;

  pinMode(led->pino, OUTPUT); 

  while(1)
  {

    digitalWrite(led->pino ,!digitalRead(led->pino)); // Executa a função para piscar o led conforme parâmetro
    vTaskDelay(pdMS_TO_TICKS(led->intervalo)); // Define a frequência do led conforme parâmetro

  }
}