/*
 * KSem.h
 *
 *  Created on: May 17, 2021
 *      Author: OS1
 */

#ifndef PROJEKAT2_KSEM_H_
#define PROJEKAT2_KSEM_H_
#include "thread.h"
class Queue;

class KernelSem{
public:

KernelSem(int a=1);
~KernelSem();
int wait(Time maxTimeToWait);
void signal();
int val() const;
int getValue()const;
void delPCB(PCB*a);
private:

volatile int value;
Queue* myQueue;
friend class Semaphore;

};



#endif /* PROJEKAT2_KSEM_H_ */
