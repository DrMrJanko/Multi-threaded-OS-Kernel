/*
 * KEve.h
 *
 *  Created on: May 25, 2021
 *      Author: OS1
 */

#ifndef PROJEKAT2_KEVE_H_
#define PROJEKAT2_KEVE_H_
#include "PCB.h"
typedef unsigned char IVTNo;

class KernelEv{
public:
KernelEv(IVTNo n);
~KernelEv();
void signal();
void wait();
friend class Event;

private:
PCB* owner;
volatile int blocked;
IVTNo entry;
};

#endif /* PROJEKAT2_KEVE_H_ */
