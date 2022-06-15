#ifndef PROJEKAT2_PCB_H_
#define PROJEKAT2_PCB_H_
#include "thread.h"

class KernelSem;
class PCB{
private: static unsigned IDD;
public:
	friend class Global;
	friend class PCBList;
    static PCB* running;
    static PCB* tmpPCB;
    PCBList* waitingForMe;
    PCBList* children;
	enum State{BLOCKED,READY,FINISHED,DEFAULT};
	State state;
	unsigned sp;
	unsigned ss;
	unsigned bp;
    unsigned quant;
    unsigned* stack;
    Thread* myThread;
    unsigned id;
    int infTime;
    int sleepingTime;
    volatile unsigned signalized;
    KernelSem* waitingOn;
    PCB* parentPCB;
    unsigned numOfChilds;
    unsigned waitingCount;
    unsigned waitingForChildren;
    PCB(Thread*const thr,StackSize sts,Time t);
    ~PCB();
    void createPCB();
    static void wrapper();
    StackSize stacksize;
    StackSize stsInElem;
    void start();
    void waitToComplete();
    static ID getRunningId();
    static Thread* getThreadById(ID id);
    ID getId();
    static void interrupt copyStack();
};

#endif /* PROJEKAT2_PCB_H_ */
