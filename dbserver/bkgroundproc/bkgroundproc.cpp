#include "bkgroundproc.h"
#include <stdlib.h>
#include <qmutex.h>

void bkgroundproc::backgroundProcess()
{
    fscanVal = system(query);
}

void timerClass::haltStateTask()
{
    if(proc->finished())
    {
	t->stop();
	delete t;
    }
    cout << "aa\n";
    sleep(1);
}
