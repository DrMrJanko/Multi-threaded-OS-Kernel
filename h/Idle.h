#ifndef BCC_IDLE_H_
#define BCC_IDLE_H_

#include "thread.h"


class Idle:public Thread{
public:
	Idle():Thread(){}
	void start();
	~Idle(){waitToComplete();}
protected:
	virtual void run();

};


#endif /* BCC_IDLE_H_ */
