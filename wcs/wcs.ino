#include "wcs/constants.h"
#include "wcs/context.h"
#include "wcs/kernel/scheduler.h"
#include "wcs/devices/button.h"
#include "wcs/devices/potentiometer.h"
#include "wcs/devices/valve.h"
#include "wcs/devices/display.h"
#include "wcs/tasks/commTask.h"
#include "wcs/tasks/logicTask.h"
#include "wcs/tasks/manualInputTask.h"
#include "wcs/tasks/valveTask.h"
#include "wcs/tasks/displayTask.h"

Scheduler* scheduler;
Context* context;

Button* btn;
Potentiometer* pot;
Valve* valve;
Display* lcd;

ManualInputTask* tManual;
CommTask* tComm;
LogicTask* tLogic;
ValveTask* tValve;
DisplayTask* tDisplay;

void setup() {
  Serial.begin(9600);
  scheduler = new Scheduler();
  scheduler->init(50); 
  context = new Context();
  btn = new Button(BUTTON_PIN);
  pot = new Potentiometer(POT_PIN);
  valve = new Valve(SERVO_PIN);
  lcd = new Display(0x27, 20, 4); 
  tManual = new ManualInputTask(btn, pot, context);
  tComm = new CommTask(context);
  tLogic = new LogicTask(context);
  tValve = new ValveTask(valve, context);
  tDisplay = new DisplayTask(lcd, context);
  
  tManual->init(50);
  tComm->init(100);
  tLogic->init(100);
  tValve->init(100);
  tDisplay->init(500);
  
  scheduler->addTask(tManual);
  scheduler->addTask(tComm);
  scheduler->addTask(tLogic);
  scheduler->addTask(tValve);
  scheduler->addTask(tDisplay);
}

void loop() {
  scheduler->schedule();
}
