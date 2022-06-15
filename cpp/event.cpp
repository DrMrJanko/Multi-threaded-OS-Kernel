#include "event.h"

#include "aglobal.h"
#include "KEve.h"
Event::Event(IVTNo n){
	lock
	myImpl=new KernelEv(n);
	unlock
}
Event::~Event(){
	lock
	delete myImpl;
	unlock
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}


