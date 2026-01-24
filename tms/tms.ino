#include "tms/constants.h"
#include "tms/context.h"
#include "tms/kernel/scheduler.h"

// Devices
#include "tms/devices/led.h"
#include "tms/devices/sonar.h"

// Tasks
#include "tms/tasks/commTask.h"
#include "tms/tasks/logicTask.h"
#include "tms/tasks/sonarTask.h"
#include "tms/tasks/ledTask.h"

// --- Global Pointers ---
Scheduler* scheduler;
Context* context;

// Hardware
Led* lGreen;
Led* lRed;
Sonar* sonar;

// Tasks
SonarTask* tSonar;
CommTask* tComm;
LogicTask* tLogic;
LedTask* tLed;

void setup() {
  Serial.begin(115200);

  // 1. Kernel Init
  scheduler = new Scheduler();
  scheduler->init(50); // Base period 50ms
  context = new Context();

  // 2. Hardware Init
  lGreen = new Led(GREEN_LED_PIN);
  lRed   = new Led(RED_LED_PIN);
  sonar  = new Sonar(TRIG_PIN, ECHO_PIN);

  // 3. Task Allocation
  tSonar = new SonarTask(sonar, context);
  tComm  = new CommTask(context);
  tLogic = new LogicTask(context);
  tLed   = new LedTask(lGreen, lRed, context);

  // 4. Task Init (Timing)
  tSonar->init(200);  // Campiona ogni 200ms
  tComm->init(1000);  // Invia dati/check connessione ogni 1s
  tLogic->init(100);  // Valuta lo stato ogni 100ms
  tLed->init(100);    // Aggiorna LED ogni 100ms

  // 5. Add to Scheduler
  scheduler->addTask(tSonar);
  scheduler->addTask(tComm);
  scheduler->addTask(tLogic);
  scheduler->addTask(tLed);
}

void loop() {
  scheduler->schedule();
}