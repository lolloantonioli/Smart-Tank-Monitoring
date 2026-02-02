#include "constants.h"
#include "kernel/scheduler.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// Devices
#include "devices/led.h"
#include "devices/sonar.h"

// Tasks
#include "tasks/commTask.h"
#include "tasks/sonarTask.h"
#include "tasks/ledTask.h"

// --- DATI CONDIVISI (Shared Memory) ---
// volatile dice al compilatore: "Attenzione, questo valore può cambiare 
// improvvisamente (perché toccato dall'altro Core), rileggilo sempre dalla RAM!"
QueueHandle_t distanceQueue;
volatile TMSState systemState = NOT_WORKING;

// --- Schedulers ---
Scheduler* schedApp;  // Girerà sul Core 1 (Default Arduino)
Scheduler* schedComm; // Girerà sul Core 0 (Radio/WiFi)

// --- Hardware & Tasks ---
Led *lGreen, *lRed;
Sonar *sonar;

SonarTask* tSonar;
CommTask* tComm;
LedTask* tLed;

// Loop dedicato al Core 0
void CommLoop(void * parameter) {
  schedComm = new Scheduler();
  schedComm->init(100); 
  
  // Passiamo i puntatori alle variabili globali
  tComm = new CommTask(distanceQueue, &systemState);
  tComm->init(FREQUENCE);
  
  schedComm->addTask(tComm);
  
  for(;;) {
    schedComm->schedule();
    // Un piccolo delay è essenziale nei task FreeRTOS per non bloccare il Watchdog
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}

void setup() {
  Serial.begin(115200);
  distanceQueue = xQueueCreate(1, sizeof(float));

  // 1. Init Hardware
  lGreen = new Led(GREEN_LED_PIN);
  lRed   = new Led(RED_LED_PIN);
  sonar  = new Sonar(TRIG_PIN, ECHO_PIN);

  // 2. Scheduler Applicativo (Core 1)
  schedApp = new Scheduler();
  schedApp->init(50);

  // Il Sonar scrive nella variabile distance (puntatore)
  tSonar = new SonarTask(sonar, distanceQueue);
  tSonar->init(FREQUENCE); 

  // Il LED legge la variabile isConnected (puntatore)
  tLed = new LedTask(lGreen, lRed, &systemState);
  tLed->init(100);

  schedApp->addTask(tSonar);
  schedApp->addTask(tLed);

  // 3. Avvio Task Comunicazione su Core 0
  // Questo permette di usare i due core "in modo giusto" come richiesto
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
  // Core 1 (App Loop)
  schedApp->schedule();
}