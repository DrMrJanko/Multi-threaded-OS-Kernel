#ifndef _event_h_
#define _event_h_
typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
   Event (IVTNo ivtNo);
  ~Event ();
  void wait ();
protected:
   friend class KernelEv;
   void signal(); // can call KernelEv
private:
 KernelEv* myImpl;
};

class IVTEntry{
public:
IVTEntry(IVTNo id,void interrupt (*a)(...));
KernelEv *myKEvent;
void interrupt (*old)(...);
void signal();
void callOld();
};


#define PREPAREENTRY(id, old)\
void interrupt inter##id(...); \
IVTEntry newEntry##id(id, inter##id);\
void interrupt inter##id(...) {\
newEntry##id.signal();\
if (old == 1)\
newEntry##id.callOld();\
}

#endif

