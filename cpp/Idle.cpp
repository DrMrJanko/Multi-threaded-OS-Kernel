/*
 * Idle.cpp
 *
 *  Created on: May 17, 2021
 *      Author: OS1
 */
#include "Idle.h"
#include "PCB.h"

void Idle::run(){
	while(1){}
}

void Idle::start(){
myPCB->state=PCB::READY;
myPCB->createPCB();
}



