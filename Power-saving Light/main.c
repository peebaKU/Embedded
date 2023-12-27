#include "mbed.h"
#include "DebounceIn.h"


DigitalOut led3(PB_3);
DebounceIn sw1(D3, PullUp);
bool mode_auto = false;
EventQueue queue;
Thread thr_queue;

#define BLINKING_RATE 100ms

void Light(void) {
    if(led3){
        printf("Light is on by switch\n");
        int count = 0;
        while(mode_auto){
            if(count==50){
                led3 = false;
                mode_auto = false;
                printf("Light is off by timeout\n");
                break;
            }
            ThisThread::sleep_for(BLINKING_RATE);
            count++;
        }
    }
    else{
        led3 = false;
        printf("Light is off by switch\n");
    }
}

void sw1_pressed(void) {
    led3 = !led3;
    mode_auto = !mode_auto;
    queue.call(Light);
}

int main(void) {
    printf("App started...\n");
    thr_queue.start(
        callback(&queue,&EventQueue::dispatch_forever)
    );
    sw1.fall(sw1_pressed);
}