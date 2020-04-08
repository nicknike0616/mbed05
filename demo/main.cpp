#include "mbed.h"
#include "mbed_events.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

InterruptIn btn(SW2);
InterruptIn btn2(SW3);

EventQueue printfQueue;
EventQueue eventQueue;

void blink_led3() {
  led3 = !led3;
}

void print_toggle_led() {
  printf("Toggle LED!\r\n");
}

void btn_fall_irq() {
  led1 = !led1;
  printfQueue.call(&print_toggle_led);
}

void btn_fall_irq2() {
  led2 = !led2;
  printfQueue.call(&print_toggle_led);
}

int main() {
  Thread printfThread(osPriorityLow);
  printfThread.start(callback(&printfQueue, &EventQueue::dispatch_forever));

  Thread eventThread(osPriorityNormal);
  eventThread.start(callback(&eventQueue, &EventQueue::dispatch_forever));

  Ticker ledTicker;

  ledTicker.attach(eventQueue.event(&blink_led3), 0.5f);

  btn.fall(&btn_fall_irq);
  btn2.fall(&btn_fall_irq2);

  while (1) {wait(1);}

}
