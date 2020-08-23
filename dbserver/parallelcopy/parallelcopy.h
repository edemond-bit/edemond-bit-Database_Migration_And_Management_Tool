#include <qtable.h>
#include <qthread.h>
//#include <qprogressdialog.h>

//````Thread Class Definition`````[START]
class parllCopy : public QThread
{
    QTable *source_tabTag,*dest_tabTag;
    int source_StartRow,source_StartCol,source_numRow,source_numCol;
    int dest_StartRow,dest_StartCol;
    
public:
    parllCopy(QTable *sTab, int s_sRow, int s_sCol, int s_nRow, int s_nCol, QTable *dTab, int d_sRow, int d_sCol)
    {
	source_tabTag = sTab;
	source_StartRow = s_sRow;
	source_StartCol = s_sCol;
	source_numRow = s_nRow;
	source_numCol = s_nCol;
	
	dest_tabTag = dTab;
	dest_StartRow = d_sRow;
	dest_StartCol = d_sCol;
    }
	
    ~parllCopy()
    {
	source_tabTag = 0;
	source_StartRow = 0;
	source_StartCol = 0;
	source_numRow = 0;
	source_numCol = 0;
	
	dest_tabTag = 0;
	dest_StartRow = 0;
	dest_StartCol = 0;
    }
   
private:
    void copyData();
    virtual void run()
    {
	copyData();
    }
};
//```Thread Class Definition`````[END]
