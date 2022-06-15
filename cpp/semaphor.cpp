/*
 * semaphor.cpp
 *
 *  Created on: May 17, 2021
 *      Author: OS1
 */

#include "semaphor.h"
#include "KSem.h"
#include "aglobal.h"
int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal(){
	myImpl->signal();
}

int Semaphore::val()const{
	return myImpl->getValue();
}

Semaphore::Semaphore(int a){
	lock
	myImpl=new KernelSem(a);
	unlock
}

Semaphore::~Semaphore(){
	lock
	delete myImpl;
	unlock
}


