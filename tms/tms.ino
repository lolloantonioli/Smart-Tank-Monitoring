#include "constants.h"
#include "kernel/scheduler.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "devices/led.h"
#include "devices/sonar.h"

#include "tasks/commTask.h"
#include "tasks/sonarTask.h"
#include "tasks/ledTask.h"

QueueHandle_t distanceQueue;
volatile TMSState systemState = NOT_WORKING;

Scheduler* schedApp;  
Scheduler* schedComm; 

Led *lGreen, *lRed;
Sonar *sonar;

SonarTask* tSonar;
CommTask* tComm;
LedTask* tLed;

void CommLoop(void * parameter) {
  schedComm = new Scheduler();
  schedComm->init(100); 
  
  tComm = new CommTask(distanceQueue, &systemState);
  tComm->init(FREQUENCE);
  
  schedComm->addTask(tComm);
  
  for(;;) {
    schedComm->schedule();
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}

void setup() {
  Serial.begin(115200);
  distanceQueue = xQueueCreate(1, sizeof(float));

  lGreen = new Led(GREEN_LED_PIN);
  lRed = new Led(RED_LED_PIN);
  sonar = new Sonar(TRIG_PIN, ECHO_PIN);

  schedApp = new Scheduler();
  schedApp->init(50);

  tSonar = new SonarTask(sonar, distanceQueue);
  tSonar->init(FREQUENCE); 

  tLed = new LedTask(lGreen, lRed, &systemState);
  tLed->init(100);

  schedApp->addTask(tSonar);
  schedApp->addTask(tLed);

  xTaskCreatePinnedToCore(
    CommLoop,   // Funzione
    "CommTask", // Nome
    10000,      // Stack size
    NULL,       // Parametri
    1,          // Priorità
    NULL,       // Handle
    0           // CORE ID 0
  );
}

void loop() {
  schedApp->schedule();
}
