#ifndef MYTHREADCLASS_H
#define MYTHREADCLASS_H

class MyThreadClass
{
public:
   MyThreadClass() {/* empty */}
   virtual ~MyThreadClass() {/* empty */}

   /** Returns true if the thread was successfully started, false if there was an error starting the thread */
   bool StartInternalThread()
   {
       //struct sched_param mysched_param;
       //memset(&mysched_param,0,sizeof(mysched_param));
       //mysched_param.sched_priority = 50;
       //pthread_setschedparam(_thread,SCHED_FIFO,&mysched_param);
      return (pthread_create(&_thread, NULL, InternalThreadEntryFunc, this) == 0);
   }

   /** Will not return until the internal thread has exited. */
   void WaitForInternalThreadToExit()
   {
      (void) pthread_join(_thread, NULL);
   }

protected:
   /** Implement this method in your subclass with the code you want your thread to run. */
   virtual void InternalThreadEntry() = 0;

private:
   static void * InternalThreadEntryFunc(void * This) {((MyThreadClass *)This)->InternalThreadEntry(); return NULL;}

   pthread_t _thread;
};


#endif // MYTHREADCLASS_H
