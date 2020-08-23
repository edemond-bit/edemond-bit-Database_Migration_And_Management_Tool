#include <qfile.h>
#include <qthread.h>
#include <math.h>
#include <qwaitcondition.h>
#include <qprogressdialog.h>

//````Thread Class Definition`````[START]
class threadRun : public QThread
{
    QString fileName,saveFile;
    FILE *fp;
    int thID;
    int retVal;
    double o_aa,o_ab,o_bb,noCall,p,q;
    bool mG,mA,mH,mC;
public:
    threadRun(int thID, QString i_fileName, int i_retVal,bool i_mG,bool i_mA,bool i_mH,bool i_mC)
    {
	fileName = i_fileName;
	retVal = i_retVal;
	mG = i_mG; mA = i_mA; mH = i_mH; mC = i_mC;
	o_aa = 0.0, o_ab = 0.0, o_bb = 0.0, noCall = 0.0; p = 0.0; q = 0.0;
	saveFile = QString::number(thID) + fileName;
    }
	
    ~threadRun()
    {
	o_aa = 0.0, o_ab = 0.0, o_bb = 0.0, noCall = 0.0;
    }
   
    void genotypeCalculation();
private:
    virtual void run()
    {
	if(mG)
	    genotypeCalculation();
    }
};
//```Thread Class Definition`````[END]
