/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
//////////////////////////////////////////////////////////
//Key parameters for threading[START]
//numLines_List ~ used in List Search Part
//Key parameters for threading[STOP]
/////////////////////////////////////////////////////////
#define MAXBUFSIZE 100000
#include <qsqldatabase.h>
#include <qsqlcursor.h>
#include <qdatatable.h>
#include <iostream.h>
#include <math.h>
#include <qsqlselectcursor.h>
#include <qevent.h>
#include <qfiledialog.h>
#include <qvbox.h>
#include <qworkspace.h>
#include <qdatatable.h>
#include <qlistview.h>
#include <qpalette.h>
#include <qspinbox.h>
#include <qprogressdialog.h>
#include "./summarydata/summarydata.h"
#include "./editheader/frmeditheader.h"
#include "./pedheader/frmpedheader.h"
#include "./bkgroundproc/bkgroundproc.h"
#include "./columnadd/frmcolumnadd.h"
#include "./colmodification/frmcolmodify.h"
#include <qinputdialog.h>
#include <qmessagebox.h>


class UPDATESTK
{
public:
    int row;
    int col;
    int automode;
    QString value;
    UPDATESTK *next;
    UPDATESTK *prev;
    
    UPDATESTK()
    {
	row = col = -1;
	value = "";
	next = 0;
	prev = 0;
	automode = -1;
    }
};

UPDATESTK *u_headR = 0; //Pointer to first node
UPDATESTK *u_undoR = 0; //Pointer to undo node
UPDATESTK *u_redoR = 0; //Pointer to redo node
UPDATESTK *u_headW = 0; //Pointer to first node
UPDATESTK *u_undoW = 0; //Pointer to undo node
UPDATESTK *u_redoW = 0; //Pointer to redo node
UPDATESTK *u_headF = 0; //Pointer to first node
UPDATESTK *u_undoF = 0; //Pointer to undo node
UPDATESTK *u_redoF = 0; //Pointer to redo node
UPDATESTK *u_headC = 0; //Pointer to first node
UPDATESTK *u_undoC = 0; //Pointer to undo node
UPDATESTK *u_redoC = 0; //Pointer to redo node
UPDATESTK *u_headL = 0; //Pointer to first node
UPDATESTK *u_undoL = 0; //Pointer to undo node
UPDATESTK *u_redoL = 0; //Pointer to redo node
int frmDataAnalyzer::automodeCounter = 0;
	
void frmDataAnalyzer::init()
{
    tabCursor = 0;
    //~~~~~~~~For Analysis Action[START]~~~~~~~~~~
    connect(cmdgenerateR,SIGNAL(clicked()),this,SLOT(openAnalysisDialog()));
    connect(cmdgenerateW,SIGNAL(clicked()),this,SLOT(openAnalysisDialog()));
    connect(cmdgenerateF,SIGNAL(clicked()),this,SLOT(openAnalysisDialog()));
    connect(cmdgenerateC,SIGNAL(clicked()),this,SLOT(openAnalysisDialog()));
    connect(cmdgenerateA,SIGNAL(clicked()),this,SLOT(openAnalysisDialog()));
    connect(cmdgenerateL,SIGNAL(clicked()),this,SLOT(openAnalysisDialog()));
    //~~~~~~~~For Analysis Action[STOP]~~~~~~~~~~
    //~~~~~~~~For Edit Action[START]~~~~~~~~~~
    QStringList editingList;
    editingList.append("Read Only");
    editingList.append("Update");
    editingList.append("Delete");
    editingList.append("Save changes");
    
    cmbactionR->insertStringList(editingList);
    cmbactionR->setEnabled(false);
    cmbactionW->insertStringList(editingList);
    cmbactionW->setEnabled(false);
    cmbactionF->insertStringList(editingList);
    cmbactionF->setEnabled(false);
    cmbactionC->insertStringList(editingList);
    cmbactionC->setEnabled(false);
    cmbactionA->insertStringList(editingList);
    cmbactionA->setEnabled(false);
    cmbactionL->insertStringList(editingList);
    cmbactionL->setEnabled(false);
    //~~~~~~~~~For Edit Action[STOP]~~~~~~~~~~
    for(register int i = 0; i < 6; ++i)
	pageStat[i] = false;
    workSpaceSetting();
    actionSettings();
    startTimer(100);
}

void frmDataAnalyzer::workSpaceSetting()
{
    //```````````````````````````Row Selection Part[SATRT]~~~~~~~~~~~~~~~
    vbR = new QVBox(grviewerR,"vBox",WDestructiveClose);
    vbR->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    wSpaceR = new QWorkspace(vbR);
    wSpaceR->setScrollBarsEnabled(true);
    vbR->show();
    qryresviewerR = 0;
    redoCountR = 0;
    undoCountR = 0;
    cmdFindReplaceR->setHidden(true);
    cmdconfirmupdateR->setHidden(true);
    //```````````````````````````Row Selection Part[END]~~~~~~~~~~~~~~~~~
    //```````````````````````````WildCard Search Part[SATRT]~~~~~~~~~~~~~
    vbW = new QVBox(grviewerW,"vBox",WDestructiveClose);
    vbW->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    wSpaceW = new QWorkspace(vbW);
    wSpaceW->setScrollBarsEnabled(true);
    vbW->show();
    qryresviewerW = 0;
    redoCountW = 0;
    undoCountW = 0;
    cmdFindReplaceW->setHidden(true);
    cmdconfirmupdateW->setHidden(true);
    //```````````````````````````WildCard Search Part[END]~~~~~~~~~~~~~~
    //```````````````````````````Field Search Part[SATRT]~~~~~~~~~~~~~~~~
    vbF = new QVBox(grviewerF,"vBox",WDestructiveClose);
    vbF->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    wSpaceF = new QWorkspace(vbF);
    wSpaceF->setScrollBarsEnabled(true);
    vbF->show();
    txtPrimaryKey->setHidden(true);
    qryresviewerF = 0;
    redoCountF = 0;
    undoCountF = 0;
    cmdFindReplaceF->setHidden(true);
    cmdconfirmupdateF->setHidden(true);
    //```````````````````````````Field Search Part[END]~~~~~~~~~~~~~~~~~
    //`````````````````Comparison Search Part[SATRT]~~~~~~~~~~~~~~~~
    vbC = new QVBox(grviewerC,"vBox",WDestructiveClose);
    vbC->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    wSpaceC = new QWorkspace(vbC);
    wSpaceC->setScrollBarsEnabled(true);
    vbC->show();
    qryresviewerC = 0;
    redoCountC = 0;
    undoCountC = 0;
    cmdFindReplaceC->setHidden(true);
    cmdconfirmupdateC->setHidden(true);
    //`````````````````````Comparison Search Part[END]~~~~~~~~~~~~~~~
    //```````````````````Aggregate Search Part[SATRT]~~~~~~~~~~~~~~~~
    vbA = new QVBox(grviewerA,"vBox",WDestructiveClose);
    vbA->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    wSpaceA = new QWorkspace(vbA);
    wSpaceA->setScrollBarsEnabled(true);
    vbA->show();
    qryresviewerA = 0;
    redoCountA = 0;
    undoCountA = 0;
    //``````````````````````Aggregate Search Part[END]~~~~~~~~~~~~~~~~
    //```````````````````````````List Search Part[SATRT]~~~~~~~~~~~~~
    vbL = new QVBox(grviewerL,"vBox",WDestructiveClose);
    vbL->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    wSpaceL = new QWorkspace(vbL);
    wSpaceL->setScrollBarsEnabled(true);
    vbL->show();
    qryresviewerL = 0;
    redoCountL = 0;
    undoCountL = 0;
    cmdFindReplaceL->setHidden(true);
    cmdconfirmupdateL->setHidden(true);
    numLines_List = 0;
    //```````````````````````````List Search Part[END]~~~~~~~~~~~~~~
}

void frmDataAnalyzer::initSettings(QString wDir, QSqlDatabase *db, QString op_tabName)
{
    workDir = wDir;
    sqlDB = db;
    tableName = op_tabName;
    loadOperation_Table(0);
    loadRichDialog();
}

void frmDataAnalyzer::loadOperation_Table(int mode)
{
    freeOperation_Table(mode);
    tabCursor = new QSqlCursor(tableName,true,sqlDB);
    tabCursor->setMode(mode);
}

void frmDataAnalyzer::freeOperation_Table(int mode)
{
    if(tabCursor)
    {
	tabCursor->clear();
	delete tabCursor;
	tabCursor = 0;
    }
}
////////~~~~~~~~~~~Resizing Event~~~~~~~~~~~~~~~[START]
void frmDataAnalyzer::resizeEvent(QResizeEvent *e)
{
    int extraWidth = 0, extendWidth = 0;
    QSize size = e->size();
    
    setGeometry(x(),y(),(size.width() < 790 ? 790 : size.width()),(size.height() < 520 ? 520 : size.height()));
    tabWidget2->setGeometry(tabWidget2->x(),tabWidget2->y(),width() - (tabWidget2->x() + 2),height() - 2);
    //Row selection[START]
    grviewerR->setGeometry(grviewerR->x(),grviewerR->y(),tabWidget2->width() - (grviewerR->x() + 4),tabWidget2->height() - (grviewerR->y() + 30));
    vbR->setGeometry(grviewerR->x() + 3,15, grviewerR->width() - 10, grviewerR->height() - 20);
    bgr0->setGeometry(tabWidget2->width() - (bgr0->width() + 4),bgr0->y(),bgr0->width(),bgr0->height());
    pallet0->setGeometry(pallet0->x(),pallet0->y(),tabWidget2->width() - (bgr0->width() + 10),pallet0->height());
    extraWidth = pallet0->width() - (rowPoolR->x() + rowPoolR->width() + 4);
    extendWidth = (int)round(8 + extraWidth*0.05);
    actionBoxR->setGeometry(actionBoxR->x(),actionBoxR->y(),tabWidget2->width() - (actionBoxR->x() + 4),actionBoxR->height());
    //Row selection[STOP]
    //Wildcard Search[START]
    grviewerW->setGeometry(grviewerW->x(),grviewerW->y(),tabWidget2->width() - (grviewerW->x() + 4),tabWidget2->height() - (grviewerW->y() + 30));
    vbW->setGeometry(grviewerW->x() + 3,15, grviewerW->width() - 10, grviewerW->height() - 20);
    bgr1->setGeometry(tabWidget2->width() - (bgr1->width() + 4),bgr1->y(),bgr1->width(),bgr1->height());
    pallet1->setGeometry(pallet1->x(),pallet1->y(),tabWidget2->width() - (bgr1->width() + 10),pallet1->height());
    extraWidth = pallet1->width() - (wildcharPoolW->x() + wildcharPoolW->width() + 4);
    extendWidth = (int)round(8 + extraWidth*0.05);
    lblW->setGeometry(((int)round(lblW->x() + extraWidth*0.1) < 4 ? 4 : (int)round(lblW->x() + extraWidth*0.1)),lblW->y(),lblW->width(),lblW->height());
    fieldListW1->setGeometry(((int)round(fieldListW1->x() + extraWidth*0.1) < 4 ? 4 : (int)round(fieldListW1->x() + extraWidth*0.1)),fieldListW1->y(),(int)round(fieldListW1->width() + extraWidth*0.15), fieldListW1->height());
    cmdAddW1->setGeometry(fieldListW1->x() + fieldListW1->width() + (extendWidth < 8 ? 8 : extendWidth),cmdAddW1->y(),cmdAddW1->width(),cmdAddW1->height());
    cmdRejW1->setGeometry(fieldListW1->x() + fieldListW1->width() + (extendWidth < 8 ? 8 : extendWidth),cmdRejW1->y(),cmdRejW1->width(),cmdRejW1->height());
    fieldListW2->setGeometry(cmdRejW1->x() + cmdRejW1->width() + (extendWidth < 8 ? 8 : extendWidth),fieldListW2->y(),(int)round(fieldListW2->width() + extraWidth*0.15), fieldListW2->height());
    cmdMore->setGeometry(fieldListW2->x() + fieldListW2->width() + (extendWidth < 8 ? 8 : extendWidth),cmdMore->y(),cmdMore->width(),cmdMore->height());
    cmdRemove->setGeometry(fieldListW2->x() + fieldListW2->width() + (extendWidth < 8 ? 8 : extendWidth),cmdRemove->y(),cmdRemove->width(),cmdRemove->height());
    wildcharPoolW->setGeometry(cmdMore->x() + cmdMore->width() + (extendWidth < 8 ? 8 : extendWidth),wildcharPoolW->y(),wildcharPoolW->width(),wildcharPoolW->height()); //For setting X value
    wildcharPoolW->setGeometry(wildcharPoolW->x(),wildcharPoolW->y(),((wildcharPoolW->width() + (int)round(extraWidth*0.4)) > (pallet1->width() - (wildcharPoolW->x() + 4)) ? (pallet1->width() - (wildcharPoolW->x() + 4)) : (wildcharPoolW->width() + (int)round(extraWidth*0.4))),wildcharPoolW->height()); //For setting Width
    //buttonGrW->setGeometry(wildcharPoolW->x() + wildcharPoolW->width() - buttonGrW->width(),buttonGrW->y(),buttonGrW->width(),buttonGrW->height());
    actionBoxW->setGeometry(actionBoxW->x(),actionBoxW->y(),tabWidget2->width() - (actionBoxW->x() + 4),actionBoxW->height());
    //Wildcard Search[STOP]
    //Field Search[START]
    grviewerF->setGeometry(grviewerF->x(),grviewerF->y(),tabWidget2->width() - (grviewerF->x() + 4),tabWidget2->height() - (grviewerF->y() + 30));
    vbF->setGeometry(grviewerF->x() + 3,15, grviewerF->width() - 10, grviewerF->height() - 20);
    bgr2->setGeometry(tabWidget2->width() - (bgr2->width() + 4),bgr2->y(),bgr2->width(),bgr2->height());
    pallet2->setGeometry(pallet2->x(),pallet2->y(),tabWidget2->width() - (bgr1->width() + 10),pallet2->height());
    extraWidth = pallet2->width() - (chkAllflds->x() + chkAllflds->width() + 68);
    extendWidth = (int)round(10 + extraWidth*0.05);
    lblF->setGeometry(((int)round(lblF->x() + extraWidth*0.1) < 4 ? 4 : (int)round(lblF->x() + extraWidth*0.1)),lblF->y(),lblF->width(),lblF->height());
    fieldList1->setGeometry(((int)round(fieldList1->x() + extraWidth*0.1) < 4 ? 4 : (int)round(fieldList1->x() + extraWidth*0.1)),fieldList1->y(),(int)round(fieldList1->width() + extraWidth*0.2),fieldList1->height());
    cmdAdd->setGeometry(fieldList1->x() + fieldList1->width() + (extendWidth < 10 ? 10 : extendWidth),cmdAdd->y(),cmdAdd->width(),cmdAdd->height());
    cmdRej->setGeometry(fieldList1->x() + fieldList1->width() + (extendWidth < 10 ? 10 : extendWidth),cmdRej->y(),cmdRej->width(),cmdRej->height());
    fieldList2->setGeometry(cmdAdd->x() + cmdAdd->width() + (extendWidth < 10 ? 10 : extendWidth),fieldList2->y(),(int)round(fieldList2->width() + extraWidth*0.2),fieldList2->height());
    //chkDistinctF->setGeometry(fieldList2->x() + fieldList2->width() + ((pallet2->width() - (fieldList2->x() + fieldList2->width()))/2 - (chkDistinctF->text()).length()/2),chkDistinctF->y(),chkDistinctF->width(),chkDistinctF->height());
    actionBoxF->setGeometry(actionBoxF->x(),actionBoxF->y(),tabWidget2->width() - (actionBoxF->x() + 4),actionBoxF->height());
    chkAllflds->setGeometry(fieldList2->x() + fieldList2->width() + ((pallet2->width() - (fieldList2->x() + fieldList2->width()))/2 - (chkAllflds->text()).length()/2),chkAllflds->y(),chkAllflds->width(),chkAllflds->height());
    //logTrackerF->setGeometry(logTrackerF->x(),logTrackerF->y(),logTrackerF->columnWidth(0),100);
    //Field Search[STOP]
    //Comparison Search[START]
    grviewerC->setGeometry(grviewerC->x(),grviewerC->y(),tabWidget2->width() - (grviewerC->x() + 4),tabWidget2->height() - (grviewerC->y() + 30));
    vbC->setGeometry(grviewerC->x() + 3,15, grviewerC->width() - 10, grviewerC->height() - 20);
    bgr3->setGeometry(tabWidget2->width() - (bgr3->width() + 4),bgr3->y(),bgr3->width(),bgr3->height());
    pallet3->setGeometry(pallet3->x(),pallet3->y(),tabWidget2->width() - (bgr3->width() + 10),pallet3->height());
    extraWidth = pallet3->width() - (comparisonPoolC->x() + comparisonPoolC->width() + 4);
    extendWidth = (int)round(10 + extraWidth*0.1);
    cmbComparison1->setGeometry(((int)round(cmbComparison1->x() + extraWidth*0.1) < 74 ? 74 : (int)round(cmbComparison1->x() + extraWidth*0.1)),cmbComparison1->y(),((int)round(cmbComparison1->width() + extraWidth*0.15) < 50 ? 50 : (int)round(cmbComparison1->width() + extraWidth*0.15)),cmbComparison1->height());
    comboCompField->setGeometry(((int)round(comboCompField->x() + extraWidth*0.1) < 4 ? 4 : (int)round(comboCompField->x() + extraWidth*0.1)),comboCompField->y(),((int)round(comboCompField->width() + extraWidth*0.15) < 120 ? 120 : (int)round(comboCompField->width() + extraWidth*0.15)),comboCompField->height());
    cmbComparison2->setGeometry(((int)round(cmbComparison2->x() + extraWidth*0.1) < 74 ? 74 : (int)round(cmbComparison2->x() + extraWidth*0.1)),cmbComparison2->y(),((int)round(cmbComparison2->width() + extraWidth*0.15) < 50 ? 50 : (int)round(cmbComparison2->width() + extraWidth*0.15)),cmbComparison2->height());
    txtComp1->setGeometry((cmbComparison1->x() + cmbComparison1->width() + (extendWidth < 10 ? 10 : extendWidth)),txtComp1->y(),((int)round(txtComp1->width() + extraWidth*0.15) < 120 ? 120 : (int)round(txtComp1->width() + extraWidth*0.15)),txtComp1->height());
    radioGroupC->setGeometry((cmbComparison1->x() + cmbComparison1->width() + (extendWidth < 10 ? 10 : extendWidth)),radioGroupC->y(),((int)round(radioGroupC->width() + extraWidth*0.15) < 120 ? 120 : (int)round(radioGroupC->width() + extraWidth*0.15)),radioGroupC->height());
    txtComp2->setGeometry((cmbComparison2->x() + cmbComparison2->width() + (extendWidth < 10 ? 10 : extendWidth)),txtComp2->y(),((int)round(txtComp2->width() + extraWidth*0.15) < 120 ? 120 : (int)round(txtComp2->width() + extraWidth*0.15)),txtComp2->height());
    cmdAddC->setGeometry((txtComp1->x() + txtComp1->width() + (extendWidth < 10 ? 10 : extendWidth)),cmdAddC->y(),cmdAddC->width(),cmdAddC->height());
    cmdRemoveC->setGeometry((txtComp2->x() + txtComp2->width() + (extendWidth < 10 ? 10 : extendWidth)),cmdRemoveC->y(),cmdRemoveC->width(),cmdRemoveC->height());
    comparisonPoolC->setGeometry((cmdAddC->x() + cmdAddC->width() + (extendWidth < 10 ? 10 : extendWidth)),comparisonPoolC->y(),comparisonPoolC->width(),comparisonPoolC->height()); //For setting X value
    comparisonPoolC->setGeometry(comparisonPoolC->x(),comparisonPoolC->y(),((comparisonPoolC->width() + (int)round(extraWidth*0.3)) > (pallet3->width() - (comparisonPoolC->x() + 4)) ? (pallet3->width() - (comparisonPoolC->x() + 4)) : (comparisonPoolC->width() + (int)round(extraWidth*0.3))),comparisonPoolC->height()); //For setting Width
    actionBoxC->setGeometry(actionBoxC->x(),actionBoxC->y(),tabWidget2->width() - (actionBoxC->x() + 4),actionBoxC->height());
    //logTrackerC->setGeometry(logTrackerC->x(),logTrackerC->y(),logTrackerC->columnWidth(0),100);
    //Comparison Search[STOP]
    //Aggregate Search[START]
    grviewerA->setGeometry(grviewerA->x(),grviewerA->y(),tabWidget2->width() - (grviewerA->x() + 4),tabWidget2->height() - (grviewerA->y() + 30));
    vbA->setGeometry(grviewerA->x() + 3,15, grviewerA->width() - 10, grviewerA->height() - 20);
    bgr4->setGeometry(tabWidget2->width() - (bgr4->width() + 4),bgr4->y(),bgr4->width(),bgr4->height());
    pallet4->setGeometry(pallet4->x(),pallet4->y(),tabWidget2->width() - (bgr4->width() + 10),pallet4->height());
    actionBoxA->setGeometry(actionBoxA->x(),actionBoxA->y(),tabWidget2->width() - (actionBoxA->x() + 4),actionBoxA->height());
    //logTrackerA->setGeometry(logTrackerA->x(),logTrackerA->y(),logTrackerA->columnWidth(0),100);
    //Aggregate Search[STOP]
    //List Search[START]
    grviewerL->setGeometry(grviewerL->x(),grviewerL->y(),tabWidget2->width() - (grviewerL->x() + 4),tabWidget2->height() - (grviewerL->y() + 30));
    vbL->setGeometry(grviewerL->x() + 3,15, grviewerL->width() - 10, grviewerL->height() - 20);
    bgr5->setGeometry(tabWidget2->width() - (bgr5->width() + 4),bgr5->y(),bgr5->width(),bgr5->height());
    pallet5->setGeometry(pallet5->x(),pallet5->y(),tabWidget2->width() - (bgr5->width() + 10),pallet5->height());
    lblListSearch->setGeometry(lblListSearch->x(),lblListSearch->y(),lblListSearch->width(),lblListSearch->height());
    cmbSearchField->setGeometry(cmbSearchField->x(),cmbSearchField->y(),cmbSearchField->width(),cmbSearchField->height());
    lblValueList->setGeometry(lblValueList->x(),lblValueList->y(),pallet5->width() - 64,lblValueList->height());
    txtValueList->setGeometry(txtValueList->x(),txtValueList->y(),pallet5->width() - 270,txtValueList->height());
    cmdBrowse->setGeometry(txtValueList->x() + txtValueList->width() + 12,cmdBrowse->y(),cmdBrowse->width(),cmdBrowse->height());
    actionBoxL->setGeometry(actionBoxL->x(),actionBoxL->y(),tabWidget2->width() - (actionBoxL->x()+4),actionBoxL->height());
    //List Search[STOP]
}
////////~~~~~~~~~~~Resizing Event~~~~~~~~~~~~~~~[STOP]

void frmDataAnalyzer::loadRichDialog()
{
    int numFields = tabCursor->count();
    int counter1 = 0,counter2 = 0;
    QSqlField *sqfield;
    
    row_wiseSettings();
    wildcardSettings();
    fieldSettings();
    comparisonField.clear();
    aggregateField.clear();
    listField.clear();
        
    txtPrimaryKey->setText(tabCursor->fieldName(0));
    for(register int itr = 0; itr < numFields; ++itr)
    {
	if(itr > 0)
	    fieldList1->insertItem(tabCursor->fieldName(itr),counter1++);
	sqfield = tabCursor->field(itr);
	if(sqfield->type() == QVariant::String)
	    fieldListW1->insertItem(tabCursor->fieldName(itr),counter2++);
	else
	    comparisonField.append(tabCursor->fieldName(itr));
	aggregateField.append(tabCursor->fieldName(itr));
    }

    listField = aggregateField;
    listSettings();
    comparisonSettings();
    aggregateSettings();
    miscellaneousSettings();
    tableName = tabCursor->name();
    viewR = tableName + tr("_viewR");
    viewW = tableName + tr("_viewW");
    viewF = tableName + tr("_viewF");
    viewC = tableName + tr("_viewC");
    viewA = tableName + tr("_viewA");
    viewL = tableName + tr("_viewL");
}

////~~~~~~~~~~~~~~~~~Row-wise Search~~~~~~~~~~[START]
void frmDataAnalyzer::row_wiseSettings()
{
    QStringList collabel;
    
    txtstartrow->setMinValue(1);
    txtstartrow->setMaxValue(2147483647);
    txtendrow->setMinValue(1);
    txtendrow->setMaxValue(2147483647);
    
    collabel.append("Start row");
    collabel.append("End row");
    rowPoolR->setNumCols(2);
    rowPoolR->setColumnLabels(collabel);
    rowPoolR->setNumRows(0);
}

void frmDataAnalyzer::cmdAddR_clicked()
{
    rowPoolR->setNumRows(rowPoolR->numRows() + 1);
    rowPoolR->setText(rowPoolR->numRows() - 1, 0, QString::number(txtstartrow->value()));
    rowPoolR->setText(rowPoolR->numRows() - 1, 1, QString::number(txtendrow->value()));
}

void frmDataAnalyzer::cmdRemoveR_clicked()
{
    for(int i = 0; i < rowPoolR->numRows(); ++i)
    {
	if(rowPoolR->isRowSelected(i))
	    rowPoolR->removeRow(i);
    }
}

void frmDataAnalyzer::cmdExecuteR_clicked()
{
    if(logTrackerR->childCount())
    {
	if(!QMessageBox::question(this, tr("Sesssion status"),("Number of update/delete operations have been made on \nthe result table \ngenerated by the previous query execution. \nIf you click yes then changes made on the result table \nwill be saved \nin the database permanently. \n\tDo you want to proceed?"),tr("Yes"),tr("No"),QString::null,0,1))
	{
	    save_OR_commit_changes(logTrackerR);
	    rowselectionQueryHub();
	}
    }
    else
	rowselectionQueryHub();
}

bool frmDataAnalyzer::rowselectionQueryHub()
{
    if(isDistinctEntry())
	showFilteredData(addPrimaryKey(viewR,createViewBasedOnUserQuery_R(0, viewR, "")));
    else
	QMessageBox::information(this,tr("Query execution status"),tr("Overlapping range of row sub sets!\nRow sub sets must be distinct"));
}

bool frmDataAnalyzer::isDistinctEntry()
{
    bool statFlag = true;
    bool ok;
    
    for(int i = 0; i < rowPoolR->numRows(); ++i)
    {
	for(int j = i + 1; j < rowPoolR->numRows(); ++j)
	{
	    if((rowPoolR->text(j,0)).toInt(&ok) >= (rowPoolR->text(i,0)).toInt(&ok) && (rowPoolR->text(j,0)).toInt(&ok) <= (rowPoolR->text(i,1)).toInt(&ok))
	    {
		statFlag = false;
		break;
	    }
	    else if((rowPoolR->text(j,1)).toInt(&ok) >= (rowPoolR->text(i,0)).toInt(&ok) && (rowPoolR->text(j,1)).toInt(&ok) <= (rowPoolR->text(i,1)).toInt(&ok))
	    {
		statFlag = false;
		break;
	    }
	    else if((rowPoolR->text(j,0)).toInt(&ok) <= (rowPoolR->text(i,0)).toInt(&ok) && (rowPoolR->text(j,1)).toInt(&ok) >= (rowPoolR->text(i,1)).toInt(&ok))
	    {
		statFlag = false;
		break;
	    }
	}
	if(!statFlag)
	    break;
    }
    return statFlag;
}

bool frmDataAnalyzer::createViewBasedOnUserQuery_R(int mode, QString dataSource, QString listQuery) //mode 0 ~ called from "query execute" button, 1 ~ called from "redo" button
{
    QString target, query = "";
    int stRow,endRow;
    bool ok;
    
    dropViewR();
    for(int i = 0; i < rowPoolR->numRows(); ++i)
    {
	if(i == 0)
	{
	    stRow = (rowPoolR->text(i,0)).toInt(&ok);
	    endRow = (rowPoolR->text(i,1)).toInt(&ok);
	    query = "CREATE TABLE " + dataSource + " AS SELECT * FROM " + tableName + " LIMIT " + QString::number(stRow - 1) + "," + QString::number(endRow - stRow + 1);
	    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	}
	else
	{
	    stRow = (rowPoolR->text(i,0)).toInt(&ok);
	    endRow = (rowPoolR->text(i,1)).toInt(&ok);
	    query = "CREATE TABLE " + dataSource + "b AS SELECT * FROM " + tableName + " LIMIT " + QString::number(stRow - 1) + "," + QString::number(endRow - stRow + 1);
	    cout << query << endl;
	    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	    target = dataSource + tr("b");
	    query = tr("INSERT HIGH_PRIORITY INTO ") + dataSource + tr(" SELECT * FROM ") + target;
	    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	    query = tr("DROP TABLE ") + dataSource + tr("b");
	    placeQueryToProcess("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	}
    }
}
////~~~~~~~~~~~~~~~~~Row-wise Search~~~~~~~~~~[STOP]

////~~~~~~~~~~~~~~~~~Wildcard Search~~~~~~~~~~[START]
void frmDataAnalyzer::wildcardSettings()
{
    QStringList wildcolList;
    ////~~~~~~~~~~~~~~~~~Wildcard Search~~~~~~~~~~
    fieldListW1->setSelectionMode(QListBox::Extended);
    fieldListW1->clear();
    fieldListW2->setSelectionMode(QListBox::Extended);
    fieldListW2->clear();
    
    wildcolList.append(tr("Wildcard"));
    wildcolList.append(tr("Search Field"));
    wildcolList.append(tr("Logic Relation"));
    wildcharPoolW->setNumCols(3);
    wildcharPoolW->setColumnLabels(wildcolList);
    wildCharList.clear();
    wildFieldList.clear();
    ////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    lsthrq11->setSelectionMode(QListBox::Extended);
    lsthrq11->clear();
    lsthrq11->insertItem(tr(""),0);
    lsthrq11->insertItem(tr("Field selection"),1);
    lsthrq11->insertItem(tr("\'OR\' Comparison search"),2);
    lsthrq11->insertItem(tr("\'AND\' Comparison search"),3);
    lsthrq11->insertItem(tr("Aggregate search"),4);
    lsthrq11->insertItem(tr("\'OR\' List search"),5);
    lsthrq11->insertItem(tr("\'AND\' List search"),6);
    lsthrq11->insertItem(tr("Miscellaneous search"),7);
}

void frmDataAnalyzer::rdabs_clicked()
{
    rdabs->setChecked(true);
    rdsubstring->setChecked(false);
}

void frmDataAnalyzer::rdsubstring_clicked()
{
    rdabs->setChecked(false);
    rdsubstring->setChecked(true);
}

void frmDataAnalyzer::cmdAddW1_clicked()
{
    QStringList tempList;
    fieldListW.clear();
    
    for(register int itr = 0; itr < (int)fieldListW1->count(); ++itr)
    {
	if(fieldListW1->isSelected(itr))
	    fieldListW.append(fieldListW1->text(itr));
	else
	    tempList.append(fieldListW1->text(itr));
    }
    
    for(register int itr = (int)fieldListW2->count() - 1; itr >= 0; --itr)
	fieldListW.prepend(fieldListW2->text(itr));
    
    fieldListW1->clear();
    fieldListW1->insertStringList(tempList,0);
    fieldListW2->clear();
    fieldListW2->insertStringList(fieldListW,0);
    fieldListW.clear();
    tempList.clear();
}

void frmDataAnalyzer::cmdRejW1_clicked()
{
    QStringList tempList;
    fieldListW.clear();
    
    for(register int itr = 0; itr < (int)fieldListW2->count(); ++itr)
    {
	if(fieldListW2->isSelected(itr))
	    fieldListW.append(fieldListW2->text(itr));
	else
	    tempList.append(fieldListW2->text(itr));
    }
    
    for(register int itr = (int)fieldListW1->count() - 1; itr >= 0 ; --itr)
	fieldListW.prepend(fieldListW1->text(itr));
    
    fieldListW1->clear();
    fieldListW1->insertStringList(fieldListW,0);
    fieldListW2->clear();
    fieldListW2->insertStringList(tempList,0);
    fieldListW.clear();
    tempList.clear();
}

void frmDataAnalyzer::cmdMore_clicked()
{
    if(txtWildcard->text() != tr(""))
    {
	wildCharList.append(rdabs->isChecked() ? txtWildcard->text() : "%" + txtWildcard->text() + "%");
	if(fieldListW2->count())
	{
	    if(fieldListW2->count() == 1)
		wildFieldList.append(fieldListW2->text(0));
	    else
	    {
		int count = fieldListW2->count();
		QStringList strList;
		for(register int index = 0; index < count; ++index)
		    strList.append(fieldListW2->text(index));
		wildFieldList.append(strList.join(tr(",")));
	    }
	    updateTableW();
	    txtWildcard->setText(tr(""));
	    updateFieldPoolW();
	}
    }
}

void frmDataAnalyzer::cmdRemove_clicked()
{
    int numRows = wildcharPoolW->numRows();
    int row = 0;
    
    for(row = 0; row < numRows; ++row)
    {
	if(wildcharPoolW->isRowSelected(row))
	{
	    wildcharPoolW->removeRow(row);
	    if(wildcharPoolW->numRows() == 1)
	    {
		if(wildcharPoolW->text(0,2) == NULL)
		    wildcharPoolW->clearCellWidget(0,2);
	    }
	}
    }
    
    wildCharList.clear();
    wildFieldList.clear();
    numRows = wildcharPoolW->numRows();
    for(row = 0; row < numRows; ++row)
    {
	wildCharList.append(wildcharPoolW->text(row,0));
	wildFieldList.append(wildcharPoolW->text(row,1));
    }
}

void frmDataAnalyzer::updateTableW()
{
    int numRows = wildcharPoolW->numRows();
    int row = 0;
    QStringList cbItem;
    
    for(row = 0; row < numRows; ++row)
	wildcharPoolW->removeRow(row);
    
    numRows = wildCharList.count();
    wildcharPoolW->setNumRows(numRows);
    row = 0;
    cbItem.clear();
    cbItem.append(tr("or"));
    cbItem.append(tr("and"));
    
    QStringList::Iterator it1 = wildFieldList.begin();
    for(QStringList::Iterator it2 = wildCharList.begin(); it2 != wildCharList.end(); ++it1,++it2,++row)
    {
	wildcharPoolW->setText(row,0,*it2);
	wildcharPoolW->setText(row,1,*it1);
	if(row < numRows - 1)
	{
	    QComboBox *cbox = new QComboBox(wildcharPoolW);
	    cbox->insertStringList(cbItem,0);
	    wildcharPoolW->setCellWidget(row,2,cbox);
	}
	else
	    wildcharPoolW->setText(row,2,"");
    }
    wildcharPoolW->adjustColumn(0);
    wildcharPoolW->setColumnReadOnly(0,true);
    wildcharPoolW->adjustColumn(1);
    wildcharPoolW->setColumnReadOnly(1,true);
    wildcharPoolW->adjustColumn(2);
    wildcharPoolW->setColumnReadOnly(2,true);
}

void frmDataAnalyzer::updateFieldPoolW()
{
    int count = fieldListW2->count();
    
    for(register int i = 0; i < count; ++i)
	fieldListW2->setSelected(i,true);
    cmdRejW1_clicked();
}

void frmDataAnalyzer::cmdExecuteW_clicked()
{
    if(logTrackerW->childCount())
    {
	if(!QMessageBox::question(this, tr("Sesssion status"),("Number of update/delete operations have been made on \nthe result table \ngenerated by the previous query execution. \nIf you click yes then changes made on the result table \nwill be saved \nin the database permanently. \n\tDo you want to proceed?"),tr("Yes"),tr("No"),QString::null,0,1))
	{
	    save_OR_commit_changes(logTrackerW);
	    wildcardQueryHub();
	}
    }
    else
	wildcardQueryHub();   
}

bool frmDataAnalyzer::wildcardQueryHub()
{
    QString dataSource;
    
    if(rdresCarry11->isChecked())
    {
	if(cmbCarry11->currentText() == "Row")
	    dataSource = viewR;
	else if(cmbCarry11->currentText() == "Wildcard")
	    dataSource = viewW;
	else if(cmbCarry11->currentText() == "Comparison")
	    dataSource = viewC;
	else if(cmbCarry11->currentText() == "Aggregation")
	    dataSource = viewA;
	else if(cmbCarry11->currentText() == "ListSearch")
	    dataSource = viewL;
    }
    else
	dataSource = tableName;
    
    showFilteredData(addPrimaryKey(viewW,prepareListSearchSubQuery(0,dataSource)));
}

bool frmDataAnalyzer::addPrimaryKey(QString viewName, bool retStat)
{
    if(retStat)
    {
	QString query;
	//~~~~~~~~~~~~Add Primary Key [START]~~~~~~~~~~~~~~~~~
	query = "mysql " + sqlDB->databaseName() + " -e \"alter table " + viewName + " add primary key(";
	QSqlSelectCursor cursor(tr("select * from " + viewName),sqlDB);
	query += cursor.fieldName(0) + ")\"";
	cout << query << endl;
	retStat = system(query);
	if(retStat == 1)
	    QMessageBox::information(this,tr("Query execution status"),tr("\tIncompitable name in the primary key field!\nEither change the field name or give an alias name if possible.\n\tOtherwise select different field name"));
	//~~~~~~~~~~~~Add Primary Key [STOP]~~~~~~~~~~~~~~~~~
	return true;
    }
    else
	return false;
}

void frmDataAnalyzer::showFilteredData(bool returnStat)
{
    if(returnStat)
    {
	QDataTable *qryresviewer = 0;
	QComboBox *cmbaction = 0;
	QSqlCursor *viewCur = 0;
	QString view = "";
	QWorkspace *wSpace = 0;
	QListView *logTracker = 0;
	QVBox *vb = 0;
	QRadioButton *rdresCarry = 0;
	
	switch(tabId)
	{
	case 0:
	    qryresviewer = qryresviewerR;
	    cmbaction = cmbactionR;
	    viewCur = viewCurR;
	    view = viewR;
	    wSpace = wSpaceR;
	    logTracker = logTrackerR;
	    vb = vbR;
	    break;
	case 1:
	    qryresviewer = qryresviewerW;
	    cmbaction = cmbactionW;
	    viewCur = viewCurW;
	    view = viewW;
	    wSpace = wSpaceW;
	    logTracker = logTrackerW;
	    vb = vbW;
	    break;
	case 2:
	    qryresviewer = qryresviewerF;
	    cmbaction = cmbactionF;
	    viewCur = viewCurF;
	    view = viewF;
	    wSpace = wSpaceF;
	    logTracker = logTrackerF;
	    vb = vbF;
	    rdresCarry = rdresCarry12;
	    break;
	case 3:
	    qryresviewer = qryresviewerC;
	    cmbaction = cmbactionC;
	    viewCur = viewCurC;
	    view = viewC;
	    wSpace = wSpaceC;
	    logTracker = logTrackerC;
	    vb = vbC;
	    rdresCarry = rdresCarry13;
	    break;
	case 4:
	    qryresviewer = qryresviewerA;
	    cmbaction = cmbactionA;
	    viewCur = viewCurA;
	    view = viewA;
	    wSpace = wSpaceA;
	    logTracker = logTrackerA;
	    vb = vbA;
	    rdresCarry = rdresCarry14;
	    break;
	case 5:
	    qryresviewer = qryresviewerL;
	    cmbaction = cmbactionL;
	    viewCur = viewCurL;
	    view = viewL;
	    wSpace = wSpaceL;
	    logTracker = logTrackerL;
	    vb = vbL;
	    rdresCarry = rdresCarry15;
	    break;
	}
	
	if(qryresviewer)
	{
	    qryresviewer->close();
	    cmbaction->setEnabled(false);
	}
	
	viewCur = new QSqlCursor(view,true,sqlDB);
	if(viewCur->size())
	{
	    qryresviewer = new QDataTable(viewCur,true,wSpace,tr(view));
	    qryresviewer->setGeometry(logTracker->x() + logTracker->width(),qryresviewer->y(),qryresviewer->width(),vb->height() - 30);
	    qryresviewer->setCaption(view);
	    qryresviewer->setSelectionMode(QTable::Multi);
	    qryresviewer->setAutoEdit(true);
	    viewCur->setMode(0);
	    qryresviewer->refresh(QDataTable::RefreshAll);
	    qryresviewer->show();
	    
	    switch(tabId)
	    {
	    case 0:
		qryresviewerR = qryresviewer;
		viewCurR = viewCur;
		pageStat[0] = true;
		break;
	    case 1:
		qryresviewerW = qryresviewer;
		viewCurW = viewCur;
		pageStat[1] = true;
		break;
	    case 2:
		qryresviewerF = qryresviewer;
		viewCurF = viewCur;
		pageStat[2] = true;
		break;
	    case 3:
		qryresviewerC = qryresviewer;
		viewCurC = viewCur;
		pageStat[3] = true;
		break;
	    case 4:
		qryresviewerA = qryresviewer;
		viewCurA = viewCur;
		pageStat[4] = true;
		break;
	    case 5:
		qryresviewerL = qryresviewer;
		viewCurL = viewCur;
		pageStat[5] = true;
		break;
	    }
	    
	    if(rdresCarry && rdresCarry->isChecked())
		cmbaction->setEnabled(false);
	    else
		cmbaction->setEnabled(true);
	    
	    qryresviewer = 0;
	    cmbaction = 0;
	    viewCur = 0;
	    view = "";
	    wSpace = 0;
	    logTracker = 0;
	    vb = 0;
	    rdresCarry = 0;
	}
    }
    else
	QMessageBox::information(this,tr("Query execution status"),tr("No query to execute!"));
}

bool frmDataAnalyzer::createViewBasedOnUserQuery_W(int mode, QString dataSource, QString listQuery) //mode 0 ~ called from "query execute" button, 1 ~ called from "redo" button
{
    QString query = "",subquery = "";
    int row = 0;
    int status = 0;
    
    if(mode == 0 && wildFieldList.count())
    {
	validDelAction[1] = true;
	validUpdateAction[1] = true;
	//~~~~~~~~~~~~~~Query preparation[START]
	commandW = queryW = ""; //flush the variable
	dropViewW();
	query = tr("CREATE TABLE ") + viewW + tr(" AS (SELECT ");
	if(lsthrq11->isSelected(1) || lsthrq11->isSelected(4))
	{
	    if(lsthrq11->isSelected(1) && !chkAllflds->isChecked()) //Field Search
	    {
		if((int)fieldList2->count())
		{
		    if(!(int)fieldList2->count() || chkAllflds->isChecked())
			validDelAction[1] = true;
		    else
			validDelAction[1] = false;
		    if(lsthrq11->isSelected(7) && chkDistinct->isChecked())
			subquery = tr("DISTINCT ");
		    subquery += txtPrimaryKey->text() + ",";
		    for(register int itr = 0; itr < (int)fieldList2->count(); ++itr)
		    {
			if(itr < (int)fieldList2->count()-1)
			    subquery += fieldList2->text(itr) + ",";
			else
			    subquery += fieldList2->text(itr);
		    }
		    query += subquery;
		}
		else
		{
		    QMessageBox::information(this,tr("Query error!"),tr("No field selected!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
		    return false;
		}
	    }
	    else
	    {
		if(lsthrq11->isSelected(7) && chkDistinct->isChecked())
		    query += "DISTINCT *";
		else
		    query += "*";
	    }
	    
	    if(lsthrq11->isSelected(4)) //Aggregate Search
	    {
		if(aggrList.count())
		{
		    validDelAction[1] = false;
		    validUpdateAction[1] = false;
		    //~~~~~~~~~~~~~~Query preparation[START]
		    if(cmbgroupby->isChecked())
		    {
			if(!subquery.contains(comboGroupField->currentText()))
			{
			    if(subquery.isEmpty())
				query += comboGroupField->currentText() + ",";
			    else
				query += "," + comboGroupField->currentText() + ",";
			}
			else
			    query += ",";
		    }
		    else
			query += ",";
		    
		    for(QStringList::Iterator it1 = aggrList.begin(); it1 != aggrList.end(); ++it1,++row)
		    {
			if(!(comparisonPoolA->text(row,1)).isEmpty())
			    query += *it1 + " as " + comparisonPoolA->text(row,1) + ",";
			else
			    query += *it1 + ",";
		    }
		    query.remove(query.length()-1,1);
		}
		else
		{
		    QMessageBox::information(this,tr("Query error!"),tr("No aggregate search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
		    return false;
		}
	    }
	    subquery = ""; //flush the subquery
	}
	else
	{
	    if(lsthrq11->isSelected(7) && chkDistinct->isChecked())
		query += "distinct * ";
	    else
		query += "*";
	}
	query += " from " + dataSource + " where "; 
	//Wildcard search part
	row = 0;
	QStringList::Iterator it1 = wildFieldList.begin();
	for(QStringList::Iterator it2 = wildCharList.begin(); it2 != wildCharList.end(); ++it1,++it2,++row)
	{
	    if(row > 0)
		subquery += " " + ((QComboBox *)(wildcharPoolW->cellWidget(row-1,2)))->currentText() + " ";    
	    if((*it1).contains(tr(",")) > 0)
	    {
		for(register int i = 0; i <= (int)((*it1).contains(tr(","))); ++i)
		{
		    if(i > 0)
			subquery += " and ";
		    subquery += (*it1).section(",",i,i);
		    
		    if(chknonMatchW->isChecked())
			subquery += " not like "; 
		    else
			subquery += " like ";
		    subquery += "\'" + *it2 + "\'";
		}
	    }
	    else
	    {
		subquery += *it1;
		if(chknonMatchW->isChecked())
		    subquery += " not like ";
		else
		    subquery += " like "; 
		subquery += "\'" + *it2 + "\'";
	    }
	}
		
	if((lsthrq11->isSelected(2) || lsthrq11->isSelected(3)) && compQList.count()) //Comparison Search
	{
	    row = 0;
	    if(lsthrq11->isSelected(2))
	    {
		subquery += " or ";
		if(lsthrq11->isSelected(3))
		    lsthrq11->setSelected(lsthrq11->item(3),false);
	    }
	    else if(lsthrq11->isSelected(3))
	    {
		subquery += " and ";
		if(lsthrq11->isSelected(2))
		    lsthrq11->setSelected(lsthrq11->item(2),false);
	    }
	    
	    if(compQList.count() > 1) //more than one range of comparison
		subquery += tr("(");
	    for(QStringList::Iterator it1 = compQList.begin(); it1 != compQList.end(); ++it1,++row)
	    {
		if(row > 0)
		    subquery += ((QComboBox *)(comparisonPoolC->cellWidget(row-1,1)))->currentText() + " ";
		subquery += tr("(") + *it1 + tr(") ");
	    }
	    subquery.truncate(subquery.length() - 1);
	    if(compQList.count() > 1) //more than one range of comparison
		subquery += tr(")");
	}
	else if(lsthrq11->isSelected(2) || lsthrq11->isSelected(3))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No comparison search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	//List Search[START]
	if(lsthrq11->isSelected(5) || lsthrq11->isSelected(6))
	{
	    if(lsthrq11->isSelected(5))
	    {
		subquery = " or ";
		if(lsthrq11->isSelected(6))
		    lsthrq11->setSelected(lsthrq11->item(6),false);
	    }
	    else if(lsthrq11->isSelected(6))
	    {
		subquery = " and ";
		if(lsthrq11->isSelected(5))
		    lsthrq11->setSelected(lsthrq11->item(5),false);
	    }

	    if(!(txtValueList->text()).isEmpty())
	    {
		subquery += cmbSearchField->currentText();
		if(chknonMatchL->isChecked())
		    subquery += " NOT IN(";
		else
		    subquery += " IN(";
		subquery += listQuery;
	    }
	}
	//List Search[STOP]
	query += subquery;
	subquery = ""; //flush the buffer
	
	if(lsthrq11->isSelected(4) && aggrList.count()) //Aggregate Search
	{
	    validDelAction[1] = false;
	    validUpdateAction[1] = false;
	    if(cmbgroupby->isChecked())
	    {
		query += tr(" GROUP BY ") + comboGroupField->currentText();
		if(aggrHavingList.count())
		{
		    row = 0;
		    subquery = " HAVING ";
		    for(QStringList::Iterator it1 = aggrHavingList.begin(); it1 != aggrHavingList.end(); ++it1,++row)
		    {
			if(row > 0)
			    subquery += ((QComboBox *)(comparisonPoolH->cellWidget(row-1,1)))->currentText() + " ";
			subquery += *it1;
		    }
		    query += subquery;
		    subquery = ""; //flush the buffer
		}
	    }
	}
	else if(lsthrq11->isSelected(4))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No aggregate search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	if(lsthrq11->isSelected(7) && chkSortType->isChecked())
	    query += " order by " + cmborderBy->currentText() + " " + (rdAsc->isChecked() ? tr("Asc") : tr("Desc"));
	query += ");";
	commandW = query;
	placeQueryToProcess("mysql " + sqlDB->databaseName() + " -e \"" + commandW + "\"");
	//status = system("mysql " + sqlDB->databaseName() + " -e \"" + commandW + "\"");
	queryW = (commandW.section("(",1,commandW.contains("(")));
	queryW = tr(" where") + queryW.section("where",1);
	queryW.truncate(queryW.length() - 2);
	//cout << "\nqueryW: " << queryW << endl;
	cout << "\ncommandW: " << commandW << endl;
	if(status == -1)
	    return false;
	else
	    return true;
	//~~~~~~~~~~~~~~Query execution[END]
    }
    else if(mode == 1)
    {
	status = system("mysql " + sqlDB->databaseName() + " -e \"" + commandW + "\"");
	if(status == -1)
	    return false;
	else
	    return true;
    }
    else
	return false;
}

void frmDataAnalyzer::placeQueryToProcess(QString query)
{
    bkgroundproc procTh(query);
    procTh.start();
    procTh.wait();
}
////~~~~~~~~~~~~~~~~~Wildcard Search~~~~~~~~~~[END]
////////~~~~~~~~~~~Field Selection~~~~~~~~~~~~~~~[START]
void frmDataAnalyzer::fieldSettings()
{
    ////////~~~~~~~~~~~Field Selection~~~~~~~~~~~~~~~
    fieldList1->setSelectionMode(QListBox::Extended);
    fieldList1->clear();
    fieldList2->setSelectionMode(QListBox::Extended);
    fieldList2->clear();
    /////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    lsthrq12->setSelectionMode(QListBox::Extended);
    lsthrq12->clear();
    lsthrq12->insertItem(tr(""),0);
    lsthrq12->insertItem(tr("Wildcard search"),1);
    lsthrq12->insertItem(tr("\'OR\' Comparison search"),2);
    lsthrq12->insertItem(tr("\'AND\' Comparison search"),3);
    lsthrq12->insertItem(tr("Aggregate search"),4);
    lsthrq12->insertItem(tr("\'OR\' List search"),5);
    lsthrq12->insertItem(tr("\'AND\' List search"),6);
    lsthrq12->insertItem(tr("Miscellaneous search"),7);
}

void frmDataAnalyzer::cmdAdd_clicked()
{
    QStringList tempList;
    fieldList.clear();
    
    for(register int itr = 0; itr < (int)fieldList1->count(); ++itr)
    {
	if(fieldList1->isSelected(itr))
	    fieldList.append(fieldList1->text(itr));
	else
	    tempList.append(fieldList1->text(itr));
    }
    
    for(register int itr = (int)fieldList2->count() - 1; itr >= 0; --itr)
	fieldList.prepend(fieldList2->text(itr));
    
    fieldList1->clear();
    fieldList1->insertStringList(tempList,0);
    fieldList2->clear();
    fieldList2->insertStringList(fieldList,0);
    fieldList.clear();
    tempList.clear();
}

void frmDataAnalyzer::cmdRej_clicked()
{
    QStringList tempList;
    fieldList.clear();
    
    for(register int itr = 0; itr < (int)fieldList2->count(); ++itr)
    {
	if(fieldList2->isSelected(itr))
	    fieldList.append(fieldList2->text(itr));
	else
	    tempList.append(fieldList2->text(itr));
    }
    
    for(register int itr = (int)fieldList1->count() - 1; itr >= 0; --itr)
	fieldList.prepend(fieldList1->text(itr));
    
    fieldList1->clear();
    fieldList1->insertStringList(fieldList,0);
    fieldList2->clear();
    fieldList2->insertStringList(tempList,0);
    fieldList.clear();
    tempList.clear();
}

void frmDataAnalyzer::cmdExecuteF_clicked()
{
    if(logTrackerF->childCount())
    {
	if(!QMessageBox::question(this, tr("Sesssion status"),("Number of update/delete operations have been made on the result table \ngenerated by the previous query execution. \nIf you click yes then changes made on the result table will be saved \nin the database permanently. \nDo you want to proceed?"),tr("Yes"),tr("No"),QString::null,0,1))
	{
	    save_OR_commit_changes(logTrackerF);
	    fieldSearchQueryHub();
	}
    }
    else
	fieldSearchQueryHub();
}
 
void frmDataAnalyzer::fieldSearchQueryHub()
{
    QString dataSource;
    
    if(rdresCarry12->isChecked())
    {
	if(cmbCarry12->currentText() == "Row")
	    dataSource = viewR;
	else if(cmbCarry12->currentText() == "Wildcard")
	    dataSource = viewW;
	else if(cmbCarry12->currentText() == "Comparison")
	    dataSource = viewC;
	else if(cmbCarry12->currentText() == "Aggregation")
	    dataSource = viewA;
	else if(cmbCarry12->currentText() == "ListSearch")
	    dataSource = viewL;
    }
    else
	dataSource = tableName;
    showFilteredData(addPrimaryKey(viewF,prepareListSearchSubQuery(0,dataSource)));
}

bool frmDataAnalyzer::createViewBasedOnUserQuery_F(int mode,QString dataSource, QString listQuery) //mode 0 ~ called from "query execute" button, 1 ~ called from "redo" button
{
    QString query = "",subquery = "",qcommand = "";
    int status = 0,row = 0;
    
    if(mode == 0 && ((int)fieldList2->count() || chkAllflds->isChecked()))
    {
	if(!(int)fieldList2->count() || chkAllflds->isChecked())
	    validDelAction[2] = true;
	else
	    validDelAction[2] = false;
	validUpdateAction[2] = true;
	//~~~~~~~~~~~~~~Query preparation[START]
	commandF = queryF = ""; //flush the variable
	dropViewF();
	query = "create table " + viewF + " as (select ";
	if(lsthrq12->isSelected(7) && chkDistinct->isChecked())
	    query += "distinct ";
	if(chkAllflds->isChecked())
	    query += "* ";
	else
	{
	    subquery = txtPrimaryKey->text() + ",";
	    for(register int itr = 0; itr < (int)fieldList2->count(); ++itr)
	    {
		if(itr < (int)fieldList2->count()-1)
		    subquery += fieldList2->text(itr) + ",";
		else
		    subquery += fieldList2->text(itr);
	    }
	    query += subquery;
	}
	
	if(lsthrq12->isSelected(4)) //Aggregate Search
	{
	    if(aggrList.count())
	    {
		validDelAction[2] = false;
		validUpdateAction[2] = false;
		//~~~~~~~~~~~~~~Query preparation[START]
		if(cmbgroupby->isChecked())
		{
		    if(!subquery.contains(comboGroupField->currentText()))
		    {
			if(subquery.isEmpty())
			    query += comboGroupField->currentText() + ",";
			else
			    query += "," + comboGroupField->currentText() + ",";
		    }
		    else
			query += ",";
		}
		else
		    query += ",";
		
		for(QStringList::Iterator it1 = aggrList.begin(); it1 != aggrList.end(); ++it1,++row)
		{
		    if(!(comparisonPoolA->text(row,1)).isEmpty())
			query += *it1 + " as " + comparisonPoolA->text(row,1) + ",";
		    else
			query += *it1 + ",";
		}
		query.remove(query.length()-1,1);
	    }
	    else
	    {
		QMessageBox::information(this,tr("Query error!"),tr("No aggregate search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
		return false;
	    }	    
	}
	subquery = ""; //flush the subquery
	query += " from " + dataSource;
	if(lsthrq12->isSelected(1) && wildFieldList.count()) //Wildcard search
	{
	    row = 0;
	    subquery = " where ";
	    QStringList::Iterator it1 = wildFieldList.begin();
	    for(QStringList::Iterator it2 = wildCharList.begin(); it2 != wildCharList.end(); ++it1,++it2,++row)
	    {
		if(row > 0)
		    subquery += " " + ((QComboBox *)(wildcharPoolW->cellWidget(row-1,2)))->currentText() + " ";    
		if((*it1).contains(tr(",")) > 0)
		{
		    for(register int i = 0; i <= (int)((*it1).contains(tr(","))); ++i)
		    {
			if(i > 0)
			    subquery += " and ";
			subquery += (*it1).section(",",i,i);
			if(chknonMatchW->isChecked())
			    subquery += " not like ";
			else
			    subquery += " like ";
			subquery += "\'" + *it2 + "\'";
		    }
		}
		else
		{
		    subquery += *it1;
		    if(chknonMatchW->isChecked())
			subquery += " not like ";
		    else
			subquery += " like ";
		    subquery += "\'" + *it2 + "\'";
		}
	    }
	}
	else if(lsthrq12->isSelected(1))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No wildcard search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}

	if((lsthrq12->isSelected(2) || lsthrq12->isSelected(3)) && compQList.count()) //Comparison Search
	{
	    row = 0;
	    if(subquery.isEmpty())
		subquery = " where ";
	    else
	    {
		if(lsthrq12->isSelected(2))
		{
		    subquery += " or ";
		    if(lsthrq12->isSelected(3))
			lsthrq12->setSelected(lsthrq12->item(3),false);
		}
		else if(lsthrq12->isSelected(3))
		{
		    subquery += " and ";
		    if(lsthrq12->isSelected(2))
			lsthrq12->setSelected(lsthrq12->item(2),false);
		}
	    }
	    if(compQList.count() > 1) //more than one range of comparison
		subquery += tr("(");
	    for(QStringList::Iterator it1 = compQList.begin(); it1 != compQList.end(); ++it1,++row)
	    {
		if(row > 0)
		    subquery += ((QComboBox *)(comparisonPoolC->cellWidget(row-1,1)))->currentText() + " ";
		subquery += tr("(") + *it1 + tr(") ");
	    }
	    subquery.truncate(subquery.length() - 1);
	    if(compQList.count() > 1) //more than one range of comparison
		subquery += tr(")");
	}
	else if(lsthrq12->isSelected(2) || lsthrq12->isSelected(3))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No comparison search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	//List Search[START]
	if(lsthrq12->isSelected(5) || lsthrq12->isSelected(6))
	{
	    if(subquery.isEmpty())
		subquery = " where ";
	    else
	    {
		if(lsthrq12->isSelected(5))
		{
		    subquery += " or ";
		    if(lsthrq12->isSelected(6))
			lsthrq12->setSelected(lsthrq12->item(6),false);
		}
		else if(lsthrq12->isSelected(6))
		{
		    subquery += " and ";
		    if(lsthrq12->isSelected(5))
			lsthrq12->setSelected(lsthrq12->item(5),false);
		}
	    }
	    if(!(txtValueList->text()).isEmpty())
	    {
		subquery += cmbSearchField->currentText();
		if(chknonMatchL->isChecked())
		    subquery += " NOT IN(";
		else
		    subquery += " IN(";
		subquery += listQuery;
	    }
	}
	//List Search[STOP]
	query += subquery;
	
	if(lsthrq12->isSelected(4) && aggrList.count()) //Aggregate Search
	{
	    if(cmbgroupby->isChecked())
	    {
		query += " GROUP BY " + comboGroupField->currentText();
		if(aggrHavingList.count())
		{
		    row = 0;
		    subquery = " HAVING ";
		    for(QStringList::Iterator it1 = aggrHavingList.begin(); it1 != aggrHavingList.end(); ++it1,++row)
		    {
			if(row > 0)
			    subquery += ((QComboBox *)(comparisonPoolH->cellWidget(row-1,1)))->currentText() + " ";
			subquery += *it1;
		    }
		    query += subquery;
		}
	    }
	}
	else if(lsthrq12->isSelected(4))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No aggregate search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	if(lsthrq12->isSelected(7) && chkSortType->isChecked())
	    query += " order by " + cmborderBy->currentText() + " " + (rdAsc->isChecked() ? tr("Asc") : tr("Desc"));
	query += ");";
	commandF = query;
	//~~~~~~~~~~~~~~Query preparation[END]
	//~~~~~~~~~~~~~~Query execution[START]
	qcommand = "mysql " + sqlDB->databaseName() +  " -e \"" + commandF + "\"";
	status = system(qcommand);
	queryF = (commandF.section("(",1,commandF.contains("(")));
	queryF = tr(" where ") + queryF.section("where",1);
	queryF.truncate(queryF.length() - 2);
	//cout << "\nqueryF: " << queryF << endl;
	cout << commandF << " : " << status << endl;
	if(status == 0)
	    return true;
	else
	    return false;
	//~~~~~~~~~~~~~~Query execution[END]
    }
    else if(mode == 1)
    {
	status = system("mysql " + sqlDB->databaseName() + " -e \"" + commandF + "\"");
	if(status == 0)
	    return true;
	else
	    return false;
    }
    else
	return false;
}

void frmDataAnalyzer::rdRefreshqr12_toggled(bool state)
{
    if(state == true)
    {
	lsthrq12->setEnabled(true);
	cmbactionF->setEnabled(true);
	if(qryresviewerF)
	    qryresviewerF->close();
    }
    else
	lsthrq12->setEnabled(false);
}

void frmDataAnalyzer::rdresCarry12_toggled(bool state)
{
    if(state == true)
    {
	cmbCarry12->setEnabled(true);
	destroyGarbageTablesIfAny(logTrackerF);
	cmbactionF->setCurrentItem(0);
	editorAction(0);
	cmbactionF->setEnabled(false);
	if(qryresviewerF)
	    qryresviewerF->close();
    }
    else
	cmbCarry12->setEnabled(false);
}
////////~~~~~~~~~~~Field Selection~~~~~~~~~~~~~~~[END]
////~~~~~~~~~~~~~~~~~Comparison Search~~~~~~~~~~[START]
void frmDataAnalyzer::comparisonSettings()
{
    QStringList compaisonOpLst1,compaisonOpLst2;
    ////~~~~~~~~~~~~~~~~~Comparison Search~~~~~~~~~~
    compcolList.append(tr("Comparison query"));
    compcolList.append(tr("Logic Relation"));
    comparisonPoolC->setNumCols(2);
    comparisonPoolC->setColumnLabels(compcolList);
    compQList.clear();
    ////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    comboCompField->insertStringList(comparisonField,0);
    compaisonOpLst1.append("=");
    compaisonOpLst1.append("!=");
    compaisonOpLst1.append(">");
    compaisonOpLst1.append(">=");
    compaisonOpLst1.append("<");
    compaisonOpLst1.append("<=");
    compaisonOpLst2.append(">");
    compaisonOpLst2.append(">=");
    compaisonOpLst2.append("<");
    compaisonOpLst2.append("<=");
    
    //cmbComparison1->insertItem(tr("All Fields"),0);
    cmbComparison1->insertStringList(compaisonOpLst1,0);
    cmbComparison2->insertStringList(compaisonOpLst2,0);
    cmbComparison1_activated(tr("="));
    grSortType->setEnabled(false);
    
    lsthrq13->setSelectionMode(QListBox::Extended);
    lsthrq13->clear();
    
    lsthrq13->insertItem(tr(""),0);
    lsthrq13->insertItem(tr("\'OR\' Wildcard search"),1);
    lsthrq13->insertItem(tr("\'AND\' Wildcard search"),2);
    lsthrq13->insertItem(tr("Field selection"),3);
    lsthrq13->insertItem(tr("Aggregate search"),4);
    lsthrq13->insertItem(tr("\'OR\' List search"),5);
    lsthrq13->insertItem(tr("\'AND\' List search"),6);
    lsthrq13->insertItem(tr("Miscellaneous search"),7);
}

void frmDataAnalyzer::cmbComparison1_activated(const QString &text)
{
    if((text == tr("=")) || (text == tr("!=")))
    {
	radioGroupC->setEnabled(false);
	cmbComparison2->setEnabled(false);
	txtComp2->setEnabled(false);
    }
    else
    {
	radioGroupC->setEnabled(true);
	cmbComparison2->setEnabled(true);
	txtComp2->setEnabled(true);
    }
}

void frmDataAnalyzer::txtComp1_textChanged( const QString &text)
{
    bool ok;
    int integer = text.toInt(&ok,10);
    if(!ok)
    {
	double realVal = text.toDouble(&ok);
	if(!ok)
	{
	    QString str = txtComp1->text();
	    txtComp1->setText(str.remove(str.length()-1,1));
	}
    }
}

void frmDataAnalyzer::txtComp2_textChanged( const QString &text)
{
    bool ok;
    int integer = text.toInt(&ok,10);
    if(!ok)
    {
	double realVal = text.toDouble(&ok);
	if(!ok)
	{
	    QString str = txtComp2->text();
	    txtComp2->setText(str.remove(str.length()-1,1));
	}
    }
}

void frmDataAnalyzer::cmdAddC_clicked()
{
    QString subQuery = "";

    if((cmbComparison1->currentItem() == 0 || cmbComparison1->currentItem() == 1) && (txtComp1->text() != ""))
    {
	subQuery = comboCompField->currentText() + " " + cmbComparison1->currentText() + " '" + txtComp1->text() + "'";
	compQList.append(subQuery);
	updateTableC();
	txtComp1->setText(tr(""));
    }
    else if(txtComp1->text() != "")
    {
	subQuery = comboCompField->currentText() + " " + cmbComparison1->currentText() + " '" + txtComp1->text() + "'";
	if(txtComp2->text() != "")
	{
	    subQuery += (rdAND->isChecked() ? " AND " : " OR ");
	    subQuery += comboCompField->currentText() + " " + cmbComparison2->currentText() + " '" + txtComp2->text() + "'";
	}
	compQList.append(subQuery);
	updateTableC();
	txtComp1->setText(tr(""));
	txtComp2->setText(tr(""));
    }
}

void frmDataAnalyzer::cmdRemoveC_clicked()
{
    int numRows = comparisonPoolC->numRows();
    int row = 0;

    for(row = 0; row < numRows; ++row)
    {
	if(comparisonPoolC->isRowSelected(row))
	{
	    comparisonPoolC->removeRow(row);
	    if(comparisonPoolC->numRows() == 1)
	    {
		if(comparisonPoolC->text(0,2) == NULL)
		    comparisonPoolC->clearCellWidget(0,2);
	    }
	}
    }
    compQList.clear();
    numRows = comparisonPoolC->numRows();
    for(row = 0; row < numRows; ++row)
	compQList.append(comparisonPoolC->text(row,0));
}

void frmDataAnalyzer::updateTableC()
{
    int numRows = comparisonPoolC->numRows();
    int row = 0;
    QStringList cbItem;

    for(row = 0; row < numRows; ++row)
	comparisonPoolC->removeRow(row);
    numRows = compQList.count();
    comparisonPoolC->setNumRows(numRows);
    row = 0;
    cbItem.clear();
    cbItem.append(tr("or"));
    cbItem.append(tr("and"));
    
    for(QStringList::Iterator it1 = compQList.begin(); it1 != compQList.end(); ++it1,++row)
    {
	comparisonPoolC->setText(row,0,*it1);
	if(row < numRows - 1)
	{
	    QComboBox *cbox = new QComboBox(comparisonPoolC);
	    cbox->insertStringList(cbItem,0);
	    comparisonPoolC->setCellWidget(row,1,cbox);
	}
	else
	    comparisonPoolC->setText(row,1,"");
    }
    comparisonPoolC->adjustColumn(0);
    comparisonPoolC->setColumnReadOnly(0,true);
    comparisonPoolC->adjustColumn(1);
    comparisonPoolC->setColumnReadOnly(1,true);
}

void frmDataAnalyzer::cmdExecuteC_clicked()
{
    if(logTrackerC->childCount())
    {
	if(!QMessageBox::question(this, tr("Sesssion status"),("Number of update/delete operations have been made on the result table \ngenerated by the previous query execution. \nIf you click yes then changes made on the result table will be saved \nin the database permanently. \nDo you want to proceed?"),tr("Yes"),tr("No"),QString::null,0,1))
	{
	    save_OR_commit_changes(logTrackerC);
	    comparisonQueryHub();
	}
    }
    else
	comparisonQueryHub();
}

void frmDataAnalyzer::comparisonQueryHub()
{
    QString dataSource;
    
    if(rdresCarry13->isChecked())
    {
	if(cmbCarry13->currentText() == "Row")
	    dataSource = viewR;
	else if(cmbCarry13->currentText() == "Wildcard")
	    dataSource = viewW;
	else if(cmbCarry13->currentText() == "Field")
	    dataSource = viewF;
	else if(cmbCarry13->currentText() == "Aggregation")
	    dataSource = viewA;
	else if(cmbCarry13->currentText() == "ListSearch")
	    dataSource = viewL;
    }
    else
	dataSource = tableName;
    showFilteredData(addPrimaryKey(viewC,prepareListSearchSubQuery(0,dataSource)));
}

bool frmDataAnalyzer::createViewBasedOnUserQuery_C(int mode, QString &dataSource, QString listQuery) //mode 0 ~ called from "query execute" button, 1 ~ called from "redo" button
{
    QString qcommand = "", query = "", subquery = "";
    int row = 0;
    int status = 0;
    
    if(mode == 0 && compQList.count())
    {
	validDelAction[3] = true;
	validUpdateAction[3] = true;
	//~~~~~~~~~~~~~~Query preparation[START]
	commandC = queryC = "";
	dropViewC();
	query = tr("CREATE TABLE ") + viewC + tr(" AS (SELECT ");
	if(lsthrq13->isSelected(3) || lsthrq13->isSelected(4))
	{
	    if(!(int)fieldList1->count() || chkAllflds->isChecked())
		validDelAction[3] = true;
	    else
		validDelAction[3] = false;
	    if(lsthrq13->isSelected(3) && !chkAllflds->isChecked()) //Field Search
	    {
		if(fieldList2->count())
		{
		    if(lsthrq13->isSelected(7) && chkDistinct->isChecked())
			subquery = tr("DISTINCT ");
		    subquery += txtPrimaryKey->text() + ",";
		    for(register int itr = 0; itr < (int)fieldList2->count(); ++itr)
		    {
			if(itr < (int)fieldList2->count()-1)
			    subquery += fieldList2->text(itr) + ",";
			else
			    subquery += fieldList2->text(itr);
		    }
		    query += subquery;
		}
		else
		{
		    QMessageBox::information(this,tr("Query error!"),tr("No field selected!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
		    return false;
		}
	    }
	    else
	    {
		if(lsthrq13->isSelected(7) && chkDistinct->isChecked())
		    query += "distinct * ";
		else
		    query += "* ";
	    }
	    
	    if(lsthrq13->isSelected(4)) //Aggregate Search
	    {
		if(aggrList.count())
		{
		    validDelAction[3] = false;
		    validUpdateAction[3] = false;
		    //~~~~~~~~~~~~~~Query preparation[START]
		    if(cmbgroupby->isChecked())
		    {
			if(!subquery.contains(comboGroupField->currentText()))
			{
			    if(subquery.isEmpty())
				query += comboGroupField->currentText() + ",";
			    else
				query += "," + comboGroupField->currentText() + ",";
			}
			else
			    query += ",";
		    }
		    else
			query += ",";
		    
		    for(QStringList::Iterator it1 = aggrList.begin(); it1 != aggrList.end(); ++it1,++row)
		    {
			if(!(comparisonPoolA->text(row,1)).isEmpty())
			    query += *it1 + " as " + comparisonPoolA->text(row,1) + ",";
			else
			    query += *it1 + ",";
		    }
		    query.remove(query.length()-1,1);
		}
		else
		{
		    QMessageBox::information(this,tr("Query error!"),tr("No aggregate search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
		    return false;
		}
	    }
	    subquery = "";
	}
	else
	{
	    if(lsthrq13->isSelected(7) && chkDistinct->isChecked())
		query += "distinct * ";
	    else
		query += "* ";
	}
	query += " from " + dataSource + " where ";
	
	//////Comparison Search[START]
	row = 0;
	for(QStringList::Iterator it1 = compQList.begin(); it1 != compQList.end(); ++it1,++row)
	{
	    if(row > 0)
		query += ((QComboBox *)(comparisonPoolC->cellWidget(row-1,1)))->currentText() + " ";
	    query += tr("(") + *it1 + tr(") ");
	}
	//////Comparison Search[END]
	if((lsthrq13->isSelected(1) || lsthrq13->isSelected(2)) && wildFieldList.count()) //Wildcard search
	{
	    row = 0;
	    if(lsthrq13->isSelected(1))
	    {
		subquery += " or ";
		if(lsthrq13->isSelected(2))
		    lsthrq13->setSelected(lsthrq13->item(2),false);
	    }
	    else if(lsthrq13->isSelected(2))
	    {
		subquery += " and ";
		if(lsthrq13->isSelected(1))
		    lsthrq13->setSelected(lsthrq13->item(1),false);
	    }
	    
	    QStringList::Iterator it1 = wildFieldList.begin();
	    for(QStringList::Iterator it2 = wildCharList.begin(); it2 != wildCharList.end(); ++it1,++it2,++row)
	    {
		if(row > 0)
		    subquery += " " + ((QComboBox *)(wildcharPoolW->cellWidget(row-1,2)))->currentText() + " ";    
		if((*it1).contains(tr(",")) > 0)
		{
		    for(register int i = 0; i <= (int)((*it1).contains(tr(","))); ++i)
		    {
			if(i > 0)
			    subquery += " and ";
			subquery += (*it1).section(",",i,i);
			if(chknonMatchW->isChecked())
			    subquery += " not like ";
			else
			    subquery += " like ";
			subquery += "\'" + *it2 + "\'";
		    }
		}
		else
		{
		    subquery += *it1;
		    if(chknonMatchW->isChecked())
			subquery += " not like ";
		    else
			subquery += " like ";
		    subquery += "\'" + *it2 + "\'";
		}
	    }
	    query += subquery;
	}
	else if(lsthrq13->isSelected(1) || lsthrq13->isSelected(2))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No wildcard search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	//List Search[START]
	if(lsthrq13->isSelected(5) || lsthrq13->isSelected(6))
	{
	    if(subquery.isEmpty())
		subquery = " where ";
	    else
	    {
		if(lsthrq13->isSelected(5))
		{
		    subquery = " or ";
		    if(lsthrq13->isSelected(6))
			lsthrq13->setSelected(lsthrq13->item(6),false);
		}
		else if(lsthrq13->isSelected(6))
		{
		    subquery = " and ";
		    if(lsthrq13->isSelected(5))
			lsthrq13->setSelected(lsthrq13->item(5),false);
		}
	    }
	    
	    if(!(txtValueList->text()).isEmpty())
	    {
		subquery += cmbSearchField->currentText();
		if(chknonMatchL->isChecked())
		    subquery += " NOT IN(";
		else
		    subquery += " IN(";
		subquery += listQuery;
	    }
	    query += subquery;
	    subquery = "";
	}
	//List Search[STOP]
	
	if(lsthrq13->isSelected(4) && aggrList.count()) //Aggregate Search
	{
	    if(cmbgroupby->isChecked())
	    {
		 query += " group by " + comboGroupField->currentText();
		 if(aggrHavingList.count())
		 {
		     row = 0;
		     subquery = " having ";
		     for(QStringList::Iterator it1 = aggrHavingList.begin(); it1 != aggrHavingList.end(); ++it1,++row)
		     {
			 if(row > 0)
			     subquery += ((QComboBox *)(comparisonPoolH->cellWidget(row-1,1)))->currentText() + " ";
			 subquery += *it1;
		     }
		     query += subquery;
		 }
	     }
	}
	else if(lsthrq13->isSelected(4))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No aggregate search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	if(lsthrq13->isSelected(7) && chkSortType->isChecked())
	    query += " order by " + cmborderBy->currentText() + " " + (rdAsc->isChecked() ? tr("Asc") : tr("Desc"));
	query += ");";
	commandC = query;
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + commandC + "\"";
	//~~~~~~~~~~~~~~Query preparation[STOP]
	status = system(qcommand);
	queryC = (commandC.section("(",1,commandC.contains("(")));
	queryC = tr(" where ") + queryC.section("where",1);
	queryC.truncate(queryC.length() - 2);
	//cout << "\nqueryC: " << queryC << endl;
	cout << commandC << endl;
	if(status == 0)
	    return true;
	else
	    return false;
    }
    else if(mode == 1)
    {
	status = system("mysql " + sqlDB->databaseName() + " -e \"" + commandC + "\"");
	if(status == 0)
	    return true;
	else
	    return false;
    }
    else
	return false;
}

void frmDataAnalyzer::rdRefreshqr13_toggled(bool state)
{
    if(state == true)
    {
	lsthrq13->setEnabled(true);
	cmbactionC->setEnabled(true);
	if(qryresviewerC)
	    qryresviewerC->close();
    }
    else
	lsthrq13->setEnabled(false);
}

void frmDataAnalyzer::rdresCarry13_toggled(bool state)
{
    if(state == true)
    {
	cmbCarry13->setEnabled(true);
	destroyGarbageTablesIfAny(logTrackerC);
	cmbactionC->setCurrentItem(0);
	editorAction(0);
	cmbactionC->setEnabled(false);
	if(qryresviewerC)
	    qryresviewerC->close();
    }
    else
	cmbCarry13->setEnabled(false);
}
////~~~~~~~~~~~~~~~~~Comparison Search~~~~~~~~~~[END]
////~~~~~~~~~~~~~~~~~Aggregate Search~~~~~~~~~~[START]
void frmDataAnalyzer::aggregateSettings()
{
    QStringList aggrOpList;
    QStringList aggrcolList;
    QStringList aggrHList;
    QStringList aggrOpLst1,aggrOpLst2;
    
    ////~~~~~~~~~~~~~~~~~Aggregate Search~~~~~~~~~~
    aggrcolList.append(tr("Grouping query"));
    aggrcolList.append(tr("Alias name"));
    comparisonPoolA->setNumCols(2);
    comparisonPoolA->setColumnLabels(aggrcolList);
    aggrList.clear();
    
    aggrHList.append(tr("Grouping condition"));
    aggrHList.append(tr("Logic relation"));
    comparisonPoolH->setNumCols(2);
    comparisonPoolH->setColumnLabels(aggrHList);
    aggrHavingList.clear();
    ////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    aggrOpList.append(tr("COUNT"));
    aggrOpList.append(tr("SUM"));
    aggrOpList.append(tr("AVG"));
    aggrOpList.append(tr("MIN"));
    aggrOpList.append(tr("MAX"));
            
    comboAggregate->insertStringList(aggrOpList,0);
    comboaggrField->insertStringList(aggregateField,0);
    comboGroupField->insertStringList(aggregateField,0);
    cmbHaving1->insertStringList(aggregateField,0);
    
    aggrOpLst1.append("=");
    aggrOpLst1.append("!=");
    aggrOpLst1.append(">");
    aggrOpLst1.append(">=");
    aggrOpLst1.append("<");
    aggrOpLst1.append("<=");
    aggrOpLst2.append(">");
    aggrOpLst2.append(">=");
    aggrOpLst2.append("<");
    aggrOpLst2.append("<=");
    cmbCompH1->insertStringList(aggrOpLst1);
    cmbCompH2->insertStringList(aggrOpLst2);
    cmbCompH1_activated(tr("="));
    cmbLogicH->insertItem(tr("AND"),0);
    cmbLogicH->insertItem(tr("OR"),1);
    
    lsthrq14->setSelectionMode(QListBox::Extended);
    lsthrq14->clear();
    
    lsthrq14->insertItem(tr(""),0);
    lsthrq14->insertItem(tr("Wildcard search"),1);
    lsthrq14->insertItem(tr("Field selection"),2);
    lsthrq14->insertItem(tr("\'OR\' Comparison search"),3);
    lsthrq14->insertItem(tr("\'AND\' Comparison search"),4);
    lsthrq14->insertItem(tr("\'OR\' List search"),5);
    lsthrq14->insertItem(tr("\'AND\' List search"),6);
    lsthrq14->insertItem(tr("Miscellaneous search"),7);
    
    cmbgroupby->setChecked(false);
    comboGroupField->setEnabled(false);
    grcomparisonBox->setEnabled(false);
}

void frmDataAnalyzer::cmbgroupby_toggled(bool state)
{
    if(state)
    {
	comboGroupField->setEnabled(true);
	grcomparisonBox->setEnabled(true);
    }
    else
    {
	comboGroupField->setEnabled(false);
	grcomparisonBox->setEnabled(false);
    }
}

void frmDataAnalyzer::comboAggregate_activated( const QString &text)
{
    int numFields = tabCursor->count();
    QStringList compField;
    
    if(text == tr("SUM") || text == tr("AVG"))
    {
	QSqlField *sqfield;
	for(register int itr = 0; itr < numFields; ++itr)
	{
	    sqfield = tabCursor->field(itr);
	    if(sqfield->type() == QVariant::Int || sqfield->type() == QVariant::Double)
		compField.append(tabCursor->fieldName(itr));
	}
    }
    else
    {
	for(register int itr = 0; itr < numFields; ++itr)
	    compField.append(tabCursor->fieldName(itr));
    }
    comboaggrField->clear();
    //cmbComparison1->insertItem(tr("All Fields"),0);
    comboaggrField->insertStringList(compField,1);
}

void frmDataAnalyzer::cmdAddA_clicked()
{
    QString subQuery = "";
    
    subQuery = comboAggregate->currentText() + "(" + comboaggrField->currentText() + ")";
    aggrList.append(subQuery);
    updateTableA();
    update_cmborderBy();
}

void frmDataAnalyzer::cmdRemoveA_clicked()
{
    int numRows = comparisonPoolA->numRows();
    int row = 0;
    
    for(row = 0; row < numRows; ++row)
    {
	if(comparisonPoolA->isRowSelected(row))
	    comparisonPoolA->removeRow(row);
    }
    aggrList.clear();
    numRows = comparisonPoolA->numRows();
    for(row = 0; row < numRows; ++row)
	aggrList.append(comparisonPoolA->text(row,0));
    update_cmborderBy();
}

void frmDataAnalyzer::updateTableA()
{
    int numRows = comparisonPoolA->numRows();
    int row = 0;
    
    for(row = 0; row < numRows; ++row)
	comparisonPoolA->removeRow(row);
    numRows = aggrList.count();
    comparisonPoolA->setNumRows(numRows);
    row = 0;
    for(QStringList::Iterator it1 = aggrList.begin(); it1 != aggrList.end(); ++it1,++row)
	comparisonPoolA->setText(row,0,*it1);
    comparisonPoolA->adjustColumn(0);
    comparisonPoolA->setColumnReadOnly(0,true);
}

void frmDataAnalyzer::comparisonPoolA_valueChanged(int row, int col)
{
    QStringList orderByList;
    int rowNum = comparisonPoolA->numRows();
    for(register int i = 0; i < rowNum; ++i)
    {
	if(!(comparisonPoolA->text(i,1)).isEmpty())
	    orderByList.append(comparisonPoolA->text(i,1));
	else
	    orderByList.append(comparisonPoolA->text(i,0));
    }
    cmborderBy->clear();
    cmborderBy->insertStringList(orderByList);
    cmbHaving1->clear();
    cmbHaving1->insertStringList(orderByList);
}

void frmDataAnalyzer::update_cmborderBy()
{
    QStringList orderByList;
    int rowNum = comparisonPoolA->numRows();
    for(register int i = 0; i < rowNum; ++i)
    {
	if(comparisonPoolA->text(i,1))
	    orderByList.append(comparisonPoolA->text(i,1));
	else
	    orderByList.append(comparisonPoolA->text(i,0));
    }
    cmborderBy->clear();
    cmborderBy->insertStringList(orderByList);
    cmbHaving1->clear();
    cmbHaving1->insertStringList(orderByList);
}

void frmDataAnalyzer::cmbCompH1_activated( const QString &text)
{
    if((text == tr("=")) || (text == tr("!=")))
    {
	cmbLogicH->setEnabled(false);
	cmbCompH2->setEnabled(false);
	txtCompH2->setEnabled(false);
    }
    else
    {
	cmbLogicH->setEnabled(true);
	cmbCompH2->setEnabled(true);
	txtCompH2->setEnabled(true);
    }
}

void frmDataAnalyzer::txtCompH1_textChanged( const QString &text)
{
    bool ok;
    int integer = text.toInt(&ok,10);
    if(!ok)
    {
	double realVal = text.toDouble(&ok);
	if(!ok)
	{
	    QString str = txtCompH1->text();
	    txtCompH1->setText(str.remove(str.length()-1,1));
	}
    }
}

void frmDataAnalyzer::txtCompH2_textChanged( const QString &text)
{
    bool ok;
    int integer = text.toInt(&ok,10);
    if(!ok)
    {
	double realVal = text.toDouble(&ok);
	if(!ok)
	{
	    QString str = txtCompH2->text();
	    txtCompH2->setText(str.remove(str.length()-1,1));
	}
    }
}

void frmDataAnalyzer::cmdAddH1_clicked()
{
    QString subQuery = "";
    if(txtCompH1->text() != "")
    {
	subQuery = cmbHaving1->currentText() + " " + cmbCompH1->currentText() + " \'" + txtCompH1->text() + "\'";
	if(txtCompH2->text() != "")
	{
	    subQuery += " " + cmbLogicH->currentText() + " " + cmbHaving1->currentText() + " " + cmbCompH2->currentText() + " \'" + txtCompH2->text() + "\'";
	}
	aggrHavingList.append(subQuery);
	updateTableH();
    }
}

void frmDataAnalyzer::cmdRemoveH2_clicked()
{
    int numRows = comparisonPoolH->numRows();
    int row = 0;
    
    for(row = 0; row < numRows; ++row)
    {
	if(comparisonPoolH->isRowSelected(row))
	    comparisonPoolH->removeRow(row);
    }
    aggrHavingList.clear();
    numRows = comparisonPoolH->numRows();
    for(row = 0; row < numRows; ++row)
	aggrHavingList.append(comparisonPoolH->text(row,0));
}

void frmDataAnalyzer::updateTableH()
{
    int numRows = comparisonPoolC->numRows();
    int row = 0;
    QStringList cbItem;

    for(row = 0; row < numRows; ++row)
	comparisonPoolH->removeRow(row);
    numRows = aggrHavingList.count();
    comparisonPoolH->setNumRows(numRows);
    row = 0;
    cbItem.clear();
    cbItem.append(tr("OR"));
    cbItem.append(tr("AND"));
    
    for(QStringList::Iterator it1 = aggrHavingList.begin(); it1 != aggrHavingList.end(); ++it1,++row)
    {
	comparisonPoolH->setText(row,0,*it1);
	if(row < numRows - 1)
	{
	    QComboBox *cbox = new QComboBox(comparisonPoolH);
	    cbox->insertStringList(cbItem,0);
	    comparisonPoolH->setCellWidget(row,1,cbox);
	}
	else
	    comparisonPoolH->setText(row,1,"");
    }
    comparisonPoolH->adjustColumn(0);
    comparisonPoolH->setColumnReadOnly(0,true);
    comparisonPoolH->adjustColumn(1);
    comparisonPoolH->setColumnReadOnly(1,true);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void frmDataAnalyzer::cmdExecuteA_clicked()
{
    if(logTrackerA->childCount())
    {
	if(!QMessageBox::question(this, tr("Sesssion status"),("Number of update/delete operations have been made on the result table \ngenerated by the previous query execution. \nIf you click yes then changes made on the result table will be saved \nin the database permanently. \nDo you want to proceed?"),tr("Yes"),tr("No"),QString::null,0,1))
	{
	    save_OR_commit_changes(logTrackerA);
	    aggregateQueryHub();
	}
    }
    else
	aggregateQueryHub();
}

void frmDataAnalyzer::aggregateQueryHub()
{
    QString dataSource;
    
    if(rdresCarry14->isChecked())
    {
	if(cmbCarry14->currentText() == "Row")
	    dataSource = viewR;
	else if(cmbCarry14->currentText() == "Wildcard")
	    dataSource = viewW;
	else if(cmbCarry14->currentText() == "Field")
	    dataSource = viewF;
	else if(cmbCarry14->currentText() == "Comparison")
	    dataSource = viewC;
	else if(cmbCarry14->currentText() == "ListSearch")
	    dataSource = viewL;
    }
    else
	dataSource = tableName;
    showFilteredData(addPrimaryKey(viewA,prepareListSearchSubQuery(0,dataSource)));
}

bool frmDataAnalyzer::createViewBasedOnUserQuery_A(int mode, QString &dataSource, QString listQuery) //mode 0 ~ called from "query execute" button, 1 ~ called from "redo" button
{
    QString qcommand = "", query = "", subquery = "";
    int row = 0;
    int status = 0;
    
    if(mode == 0 && aggrList.count())
    {
	validDelAction[4] = false;
	validUpdateAction[4] = false;
	//~~~~~~~~~~~~~~Query preparation[START]
	commandA = queryA = "";
	dropViewA();
	query = tr("CREATE TABLE ") + viewA + tr(" AS (SELECT ");
	if(lsthrq14->isSelected(7) && chkDistinct->isChecked())
	    query += tr("DISTINCT ");
	
	//Aggregate Search[First Part Start]
	if(cmbgroupby->isChecked())
	    query += comboGroupField->currentText() + ",";
	for(QStringList::Iterator it1 = aggrList.begin(); it1 != aggrList.end(); ++it1,++row)
	{
	    if(!(comparisonPoolA->text(row,1)).isEmpty())
		query += *it1 + tr(" AS ") + comparisonPoolA->text(row,1) + ",";
	    else
		query += *it1 + ",";
	}
	query.remove(query.length()-1,1);
	//Aggregate Search[First Part Stop]
	
	if(lsthrq14->isSelected(2) && fieldList2->count()) //Field Search
	{
	    subquery = ",";
	    subquery += txtPrimaryKey->text() + ",";
	    for(register int itr = 0; itr < (int)fieldList2->count(); ++itr)
	    {
		if(itr < (int)fieldList2->count()-1)
		    subquery += fieldList2->text(itr) + ",";
		else
		    subquery += fieldList2->text(itr);
	    }
	    query += subquery;
	}
	else if(lsthrq14->isSelected(2))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No field selected!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	query += " from " + dataSource;
	
	if(lsthrq14->isSelected(1) && wildFieldList.count()) //Wildcard search
	{
	    row = 0;
	    subquery = " where ";
	    QStringList::Iterator it1 = wildFieldList.begin();
	    for(QStringList::Iterator it2 = wildCharList.begin(); it2 != wildCharList.end(); ++it1,++it2,++row)
	    {
		if(row > 0)
		    subquery += " " + ((QComboBox *)(wildcharPoolW->cellWidget(row-1,2)))->currentText() + " ";    
		if((*it1).contains(tr(",")) > 0)
		{
		    for(register int i = 0; i <= (int)((*it1).contains(tr(","))); ++i)
		    {
			if(i > 0)
			    subquery += " and ";
			subquery += (*it1).section(",",i,i);
			if(chknonMatchW->isChecked())
			    subquery += " not like ";
			else
			    subquery += " like ";
			subquery +=  "\'" + *it2 + "\'";
		    }
		}
		else
		{
		    subquery += *it1;
		    if(chknonMatchW->isChecked())
			subquery += " not like ";
		    else
			subquery += " like ";
		    subquery +=  "\'" + *it2 + "\'";
		}
	    }
	}
	else if(lsthrq14->isSelected(1))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No wildcard search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	if((lsthrq14->isSelected(3) || lsthrq14->isSelected(4)) && compQList.count()) //Comparison Search
	{
	    row = 0;
	    if(subquery.isEmpty())
		subquery = " where ";
	    else
	    {
		if(lsthrq14->isSelected(3))
		{
		    subquery += " or ";
		    if(lsthrq14->isSelected(4))
			lsthrq14->setSelected(lsthrq14->item(4),false);
		}
		else if(lsthrq14->isSelected(4))
		{
		    subquery += " and ";
		    if(lsthrq14->isSelected(3))
			lsthrq14->setSelected(lsthrq14->item(3),false);
		}
	    }
	    
	    for(QStringList::Iterator it1 = compQList.begin(); it1 != compQList.end(); ++it1,++row)
	    {
		if(row > 0)
		    subquery += ((QComboBox *)(comparisonPoolC->cellWidget(row-1,1)))->currentText() + " ";
		subquery += tr("(") + *it1 + tr(") ");
	    }
	}
	else if(lsthrq14->isSelected(3) || lsthrq14->isSelected(4))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No comparison search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	//List Search[START]
	if(lsthrq14->isSelected(5) || lsthrq14->isSelected(6))
	{
	    if(subquery.isEmpty())
		subquery = " where ";
	    else
	    {
		if(lsthrq14->isSelected(5))
		{
		    subquery += " or ";
		    if(lsthrq14->isSelected(6))
			lsthrq14->setSelected(lsthrq14->item(6),false);
		}
		else if(lsthrq14->isSelected(6))
		{
		    subquery += " and ";
		    if(lsthrq14->isSelected(5))
			lsthrq14->setSelected(lsthrq14->item(5),false);
		}
	    }
	    
	    if(!(txtValueList->text()).isEmpty())
	    {
		subquery += cmbSearchField->currentText();
		if(chknonMatchL->isChecked())
		    subquery += " NOT IN(";
		else
		    subquery += " IN(";
		subquery += listQuery;
	    }
	}
	//List Search[STOP]
	query += subquery;
	
	//Aggregate Search[Second Part]
	if(cmbgroupby->isChecked())
	{
	    query += " group by " + comboGroupField->currentText();
	    if(aggrHavingList.count())
	    {
		row = 0;
		subquery = " having ";
		for(QStringList::Iterator it1 = aggrHavingList.begin(); it1 != aggrHavingList.end(); ++it1,++row)
		{
		    if(row > 0)
			subquery += ((QComboBox *)(comparisonPoolH->cellWidget(row-1,1)))->currentText() + " ";
		    subquery += *it1;
		}
		query += subquery;
	    }
	}
	//Aggregate Search[Second Part]
	
	if(lsthrq14->isSelected(7) && chkSortType->isChecked())
	    query += " order by " + cmborderBy->currentText() + " " + (rdAsc->isChecked() ? tr("Asc") : tr("Desc"));
	query += ");";
	commandA = query;
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + commandA + "\"";
	//~~~~~~~~~~~~~~Query preparation[STOP]
	status = system(qcommand);
	queryA = (commandA.section("(",1,commandA.contains("(")));
	queryA = tr(" where ") + queryA.section("where",1);
	queryA.truncate(queryA.length() - 2);
	//cout << "\nqueryA: " << queryA << endl;
	cout << commandA << endl;
	if(status == 0)
	    return true;
	else
	    return false;
    }
    else
	return false;
}

void frmDataAnalyzer::rdRefreshqr14_toggled(bool state)
{
    if(state == true)
    {
	lsthrq14->setEnabled(true);
	cmbactionA->setEnabled(true);
	if(qryresviewerA)
	    qryresviewerA->close();
    }
    else
	lsthrq14->setEnabled(false);
}

void frmDataAnalyzer::rdresCarry14_toggled(bool state)
{
    if(state == true)
    {
	cmbCarry14->setEnabled(true);
	cmbactionA->setCurrentItem(0);
	cmbactionA->setEnabled(false);
	if(qryresviewerA)
	    qryresviewerA->close();
    }
    else
	cmbCarry14->setEnabled(false);
}
////~~~~~~~~~~~~~~~~~Aggregate Search~~~~~~~~~~[END]
////////~~~~~~~~~~~List Search~~~~~~~~~~~~~~~[START]
void frmDataAnalyzer::listSettings()
{
    cmbSearchField->insertStringList(listField);
    txtValueList->setText("");
    
    lsthrq15->setSelectionMode(QListBox::Extended);
    lsthrq15->clear();
    
    lsthrq15->insertItem(tr(""),0);
    lsthrq15->insertItem(tr("\'OR\' Wildcard search"),1);
    lsthrq15->insertItem(tr("\'AND\' Wildcard search"),2);
    lsthrq15->insertItem(tr("Field selection"),3);
    lsthrq15->insertItem(tr("\'OR\' Comparison search"),4);
    lsthrq15->insertItem(tr("\'AND\' Comparison search"),5);
    lsthrq15->insertItem(tr("Aggregate search"),6);
    lsthrq15->insertItem(tr("Miscellaneous search"),7);
}

void frmDataAnalyzer::cmdBrowse_clicked()
{
    QString fn = QFileDialog::getOpenFileName(workDir,"*.txt",this,"File open dialog","Choose appropriate file containing values");
    if(!fn.isEmpty())
    {
	char data_key[MAXBUFSIZE];
	ulong size = 0;
	QString tempBuf = "";
	QString key_Buf = "";
	
	txtValueList->clear();
	txtValueList->setAutoFormatting(QTextEdit::AutoNone);
	QFile fpread(fn);
	if(fpread.open(IO_ReadOnly | IO_Truncate))
	{
	    QTextStream fpred(&fpread);
	    fpred.setEncoding(QTextStream::UnicodeUTF8);
	    while(!fpred.atEnd())
	    {
		size = fpread.readBlock(data_key,sizeof(data_key));
		tempBuf.sprintf("%s",data_key);
		if(size < MAXBUFSIZE)
		    tempBuf.truncate(size);
		
		if(tempBuf.contains('\r') > 0)
		    tempBuf.remove('\r');
		if(tempBuf.right(1) != '\n')
		    tempBuf.truncate(tempBuf.findRev('\n',-1,false));
		txtValueList->append(tempBuf);
	    }
	    fpread.close();
	    //Check number of entries in the Listsearch view[START]
	    QString editText = txtValueList->text();
	    QStringList editList;
	    if(editText.contains(','))
		editList = QStringList::split(',',editText,false);
	    else if(editText.contains('\t'))
		editList = QStringList::split('\t',editText,false);
	    else if(editText.contains('\n'))
		editList = QStringList::split('\n',editText,false);
	    else if(editText.contains(" "))
		editList = QStringList::split(" ",editText,false);
	    else
		editList.append(editText);
	    //Check number of entries in the Listsearch view[START]
	    if((int)editList.count() > 7000)
	    {
		chknonMatchL->setChecked(false);
		chknonMatchL->setEnabled(false);
	    }
	    else
		chknonMatchL->setEnabled(true);
	}
	
	/*FILE *gpIn;
	QString scanBuf;
	
	//This part only to set progressDialog's max value[START]
	scanBuf = "wc -l " + fn + " > numLines.txt";
	fscanVal = system(scanBuf);
	gpIn = fopen("numLines.txt","r");
	fscanVal = fscanf(gpIn,"%d %[^\n]",&numLines_List,scanBuf.ascii());
	fclose(gpIn);
	gpIn = NULL;
	fscanVal = system("rm -f numLines.txt");
	//This part only to set progressDialog's max value[STOP]
	txtValueList->clear();
	txtValueList->append(tr("Input file: ") + fn + tr("\nNumber of entries: ") + QString::number(numLines_List));*/
    }
    else
	QMessageBox::information( this,"File input problem!","No file given!");
}

void frmDataAnalyzer::cmdExecuteL_clicked()
{
    if(logTrackerL->childCount())
    {
	if(!QMessageBox::question(this, tr("Sesssion status"),("Number of update/delete operations have been made on the result table \ngenerated by the previous query execution. \nIf you click yes then changes made on the result table will be saved \nin the database permanently. \nDo you want to proceed?"),tr("Yes"),tr("No"),QString::null,0,1))
	{
	    save_OR_commit_changes(logTrackerL);
	    listSearchQueryHub();
	}
    }
    else
	listSearchQueryHub();
}

void frmDataAnalyzer::listSearchQueryHub()
{
    QString dataSource;
    if(rdresCarry15->isChecked())
    {
	if(cmbCarry15->currentText() == "Row")
	    dataSource = viewR;
	else if(cmbCarry15->currentText() == "Wildcard")
	    dataSource = viewW;
	else if(cmbCarry15->currentText() == "Comparison")
	    dataSource = viewC;
	else if(cmbCarry15->currentText() == "Aggregation")
	    dataSource = viewA;
	else if(cmbCarry15->currentText() == "ListSearch")
	    dataSource = viewL;
    }
    else
	dataSource = tableName;
    showFilteredData(addPrimaryKey(viewL,prepareListSearchSubQuery(0,dataSource)));
}

bool frmDataAnalyzer::prepareListSearchSubQuery(int mode,QString dataSource)
{
    int retStat = false, retStat1 = false;
    
    if(mode == 0 && !(txtValueList->text()).isEmpty())
    {
	//Check number of entries in the Listsearch view[START]
	QString editText = txtValueList->text();
	QStringList editList;
	if(editText.contains(','))
	    editList = QStringList::split(',',editText,false);
	else if(editText.contains('\t'))
	    editList = QStringList::split('\t',editText,false);
	else if(editText.contains('\n'))
	    editList = QStringList::split('\n',editText,false);
	else if(editText.contains(" "))
	    editList = QStringList::split(" ",editText,false);
	else
	    editList.append(editText);
	//Check number of entries in the Listsearch view[START]
	//Necessary parameter initialization[START]
	QProgressDialog *progressDlg = 0;
	int progress = 0;
	QString view_temp,view;
	QString query,qcommand;
	int status = -1;
	int loopLength = 0;
	int listCount = 0;
	int numLoop = (int)editList.count()/7000;
	switch(tabId)
	{
	case 0:
	    view_temp = viewR;
	    dropViewR();
	    break;
	case 1:
	    view_temp = viewW;
	    dropViewW();
	    break;
	case 2:
	    view_temp = viewF;
	    dropViewF();
	    break;
	case 3:
	    view_temp = viewC;
	    dropViewC();
	    break;
	case 4:
	    view_temp = viewA;
	    dropViewA();
	    break;
	case 5:
	    view_temp = viewL;
	    dropViewL();
	    break;
	}

	//Necessary parameter initialization[STOP]
	numLoop = ((int)editList.count()%7000 == 0 ? numLoop : numLoop + 1);
	progressDialogSettings(4,&progressDlg,numLoop,"List search","Progress label");
	progressDlg->show();
	QStringList::iterator it = editList.begin();
	for(int itr = 1; itr <= numLoop; ++itr)
	{
	    progressDlg->setProgress(++progress);
	    //progressDlg->setLabelText(QString::number(itr) + " of " + QString::number(numLoop) + " steps completed");
	    progressDlg->show();
	    //append key values in the query string[START]
	    query = "";
	    loopLength = (numLoop == 1 ? (int)editList.count() - 1 : (itr < numLoop ? 6999 : (int)editList.count() - (itr - 1)*7000 - 1));
	    for(listCount = 0; listCount < loopLength; ++it,++listCount)
		query += "'" + *it + "',";
	    query += "'" + *it + "')";
	    if(it != editList.end())
		++it;
	    //append key values in the query string[STOP]
	    //Idenify main query panel[START]
	    switch(tabId)
	    {
	    case 0:
		view = viewR = (itr == 1 ? view_temp + tr("a") : view_temp + tr("b"));
		retStat = createViewBasedOnUserQuery_R(mode,dataSource,query);
		break;
	    case 1:
		view = viewW = (itr == 1 ? view_temp + tr("a") : view_temp + tr("b"));
		retStat = createViewBasedOnUserQuery_W(mode,dataSource,query);
		break;
	    case 2:
		view = viewF = (itr == 1 ? view_temp + tr("a") : view_temp + tr("b"));
		retStat = createViewBasedOnUserQuery_F(mode,dataSource,query);
		break;
	    case 3:
		view = viewC = (itr == 1 ? view_temp + tr("a") : view_temp + tr("b"));
		retStat = createViewBasedOnUserQuery_C(mode,dataSource,query);
		break;
	    case 4:
		view = viewA = (itr == 1 ? view_temp + tr("a") : view_temp + tr("b"));
		retStat = createViewBasedOnUserQuery_A(mode,dataSource,query);
		break;
	    case 5:
		view = viewL = (itr == 1 ? view_temp + tr("a") : view_temp + tr("b"));
		retStat = createViewBasedOnUserQuery_L(mode,dataSource,query);
		break;
	    }
	    //Idenify main query panel[STOP]
	    
	    if(retStat)
	    {
		if(itr != 1)
		{		    
		    query = tr("INSERT HIGH_PRIORITY INTO ") + view_temp + tr("a SELECT * FROM ") + view;
		    qcommand = tr("mysql ") + sqlDB->databaseName() +  tr(" -e \"") + query + tr("\"");
		    status = system(qcommand);
		    if(status == 0)
		    {
			query = tr("DROP TABLE ") + view;
			qcommand = tr("mysql ") + sqlDB->databaseName() +  tr(" -e \"") + query + tr("\"");
			status = system(qcommand);
			if(status != 0)
			{
			    QMessageBox::information(this,tr("Table deletion error"),tr("Fail to drop the table '") + view + tr("'. \nInform the developer"),QMessageBox::Ok);
			    break;
			}
		    }
		    else
		    {
			QMessageBox::information(this,tr("Table insertion error"),tr("Last insertion operation in the table: '") + view_temp + tr("a' \ndoes not completed successfully.\nInform the developer"),QMessageBox::Ok);
			break;
		    }
		}
	    }
	    else
	    {
		QMessageBox::information(this,tr("Query execution error!"),tr("System fails to execute last query on table: '") + view_temp + tr("a' [Iteration: ") + QString::number(itr) + ("]\nInform the developer"),QMessageBox::Ok);
		break;
	    }
	}
	
	progressDlg->close();
	switch(tabId)
	{
	case 0:
	    view = viewR = view_temp;
	    break;
	case 1:
	    view = viewW = view_temp;
	    break;
	case 2:
	    view = viewF = view_temp;
	    break;
	case 3:
	    view = viewC = view_temp;
	    break;
	case 4:
	    view = viewA = view_temp;
	    break;
	case 5:
	    view = viewL = view_temp;
	    break;
	}
	retStat1 = finalizeResultTable_ListSearch(view,view_temp + tr("a"));
	if(!retStat1)
	    QMessageBox::information(this,tr("Table creation error!"),tr("System fails to create the final table: '") + view + tr("'\nInform the developer"),QMessageBox::Ok);
	retStat = retStat1;
    }
    else
    {
	switch(tabId)
	{
	case 0:
	    retStat = createViewBasedOnUserQuery_R(mode,dataSource,"");
	    break;
	case 1:
	    retStat = createViewBasedOnUserQuery_W(mode,dataSource,"");
	    break;
	case 2:
	    retStat = createViewBasedOnUserQuery_F(mode,dataSource,"");
	    break;
	case 3:
	    retStat = createViewBasedOnUserQuery_C(mode,dataSource,"");
	    break;
	case 4:
	    retStat = createViewBasedOnUserQuery_A(mode,dataSource,"");
	    break;
	case 5:
	    retStat = createViewBasedOnUserQuery_L(mode,dataSource,"");
	    break;
	}
    }
    return retStat;
}

bool frmDataAnalyzer::createViewBasedOnUserQuery_L(int mode,QString dataSource, QString listQuery) //mode 0 ~ called from "query execute" button, 1 ~ called from "redo" button
{
    QString query = "",subquery = "",qcommand = "";
    int status = 0,row = 0;
    
    if(mode == 0 && !(txtValueList->text()).isEmpty())
    {
	if(!(int)fieldList2->count() || chkAllflds->isChecked())
	    validDelAction[5] = true;
	else
	    validDelAction[5] = false;
	validUpdateAction[5] = true;
	//~~~~~~~~~~~~~~Query preparation[START]
	commandL = queryL = ""; //flush the variable
	query = "create table " + viewL + " as (select ";
	if(lsthrq15->isSelected(7) && chkDistinct->isChecked())
	    query += "distinct ";
	
	if(lsthrq15->isSelected(3)) //Field Search
	{
	    if(chkAllflds->isChecked())
		query += "* ";
	    else
	    {
		subquery = txtPrimaryKey->text() + ",";
		for(register int itr = 0; itr < (int)fieldList2->count(); ++itr)
		{
		    if(itr < (int)fieldList2->count()-1)
			subquery += fieldList2->text(itr) + ",";
		    else
			subquery += fieldList2->text(itr);
		}
		query += subquery;
	    }
	}
	else
	    query += "* ";
	
	if(lsthrq15->isSelected(6)) //Aggregate Search
	{
	    if(aggrList.count())
	    {
		validDelAction[5] = false;
		validUpdateAction[5] = false;
		//~~~~~~~~~~~~~~Query preparation[START]
		if(cmbgroupby->isChecked())
		{
		    if(!subquery.contains(comboGroupField->currentText()))
		    {
			if(subquery.isEmpty())
			    query += comboGroupField->currentText() + ",";
			else
			    query += "," + comboGroupField->currentText() + ",";
		    }
		    else
			query += ",";
		}
		else
		    query += ",";
		
		for(QStringList::Iterator it1 = aggrList.begin(); it1 != aggrList.end(); ++it1,++row)
		{
		    if(!(comparisonPoolA->text(row,1)).isEmpty())
			query += *it1 + " as " + comparisonPoolA->text(row,1) + ",";
		    else
			query += *it1 + ",";
		}
		query.remove(query.length()-1,1);
	    }
	    else
	    {
		QMessageBox::information(this,tr("Query error!"),tr("No aggregate search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
		return false;
	    }
	}
	subquery = "";
	query += " from " + dataSource;
	
	//List Search[START]
	if(!(txtValueList->text()).isEmpty())
	{
	    subquery = " where ";
	    subquery += cmbSearchField->currentText();
	    if(chknonMatchL->isChecked())
		subquery += " NOT IN(";
	    else
		subquery += " IN(";
	    
	    subquery += listQuery;
	}
	//List Search[STOP]
	
	if((lsthrq15->isSelected(1) || lsthrq15->isSelected(2)) && wildFieldList.count()) //Wildcard search
	{
	    row = 0;
	    if(subquery.isEmpty())
		subquery = " where ";
	    else
	    {
		if(lsthrq15->isSelected(1))
		{
		    subquery += " or (";
		    if(lsthrq15->isSelected(2))
			lsthrq15->setSelected(lsthrq15->item(2),false);
		}
		else if(lsthrq15->isSelected(2))
		{
		    subquery += " and (";
		    if(lsthrq15->isSelected(1))
			lsthrq15->setSelected(lsthrq15->item(1),false);
		}
	    }

	    QStringList::Iterator it1 = wildFieldList.begin();
	    for(QStringList::Iterator it2 = wildCharList.begin(); it2 != wildCharList.end(); ++it1,++it2,++row)
	    {
		if(row > 0)
		    subquery += " " + ((QComboBox *)(wildcharPoolW->cellWidget(row-1,2)))->currentText() + " ";    
		if((*it1).contains(tr(",")) > 0)
		{
		    for(register int i = 0; i <= (int)((*it1).contains(tr(","))); ++i)
		    {
			if(i > 0)
			    subquery += " and ";
			subquery += (*it1).section(",",i,i);
			if(chknonMatchW->isChecked())
			    subquery += " not like ";
			else
			    subquery += " like ";
			subquery += "\'" + *it2 + "\'";
		    }
		}
		else
		{
		    subquery += *it1;
		    if(chknonMatchW->isChecked())
			subquery += " not like ";
		    else
			subquery += " like ";
		    subquery +=  "\'" + *it2 + "\'";
		}
	    }
	    subquery += ")";
	}
	else if(lsthrq15->isSelected(1))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No wildcard search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	
	if((lsthrq15->isSelected(4) || lsthrq15->isSelected(5)) && compQList.count()) //Comparison Search
	{
	    row = 0;
	    if(subquery.isEmpty())
		subquery = " where ";
	    else
	    {
		if(lsthrq15->isSelected(4))
		{
		    subquery += " or ";
		    if(lsthrq15->isSelected(5))
			lsthrq15->setSelected(lsthrq15->item(5),false);
		}
		else if(lsthrq15->isSelected(5))
		{
		    subquery += " and ";
		    if(lsthrq15->isSelected(4))
			lsthrq15->setSelected(lsthrq15->item(4),false);
		}
	    }
	    if(compQList.count() > 1) //more than one range of comparison
		subquery += tr("(");
	    for(QStringList::Iterator it1 = compQList.begin(); it1 != compQList.end(); ++it1,++row)
	    {
		if(row > 0)
		    subquery += ((QComboBox *)(comparisonPoolC->cellWidget(row-1,1)))->currentText() + " ";
		subquery += tr("(") + *it1 + tr(") ");
	    }
	    subquery.truncate(subquery.length() - 1);
	    if(compQList.count() > 1) //more than one range of comparison
		subquery += tr(")");
	}
	else if(lsthrq15->isSelected(4) || lsthrq15->isSelected(5))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No comparison search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}
	query += subquery;
	
	if(lsthrq15->isSelected(6) && aggrList.count()) //Aggregate Search
	{
	    if(cmbgroupby->isChecked())
	    {
		query += " group by " + comboGroupField->currentText();
		if(aggrHavingList.count())
		{
		    row = 0;
		    subquery = " having ";
		    for(QStringList::Iterator it1 = aggrHavingList.begin(); it1 != aggrHavingList.end(); ++it1,++row)
		    {
			if(row > 0)
			    subquery += ((QComboBox *)(comparisonPoolH->cellWidget(row-1,1)))->currentText() + " ";
			subquery += *it1;
		    }
		    query += subquery;
		}
	    }
	}
	else if(lsthrq15->isSelected(6))
	{
	    QMessageBox::information(this,tr("Query error!"),tr("No aggregate search query given!!!\nQuery execution unsuccessful!"),QMessageBox::Ok);
	    return false;
	}

	if(lsthrq15->isSelected(7) && chkSortType->isChecked())
	    query += " order by " + cmborderBy->currentText() + " " + (rdAsc->isChecked() ? tr("Asc") : tr("Desc"));
	query += ");";
	commandL = query;
	//~~~~~~~~~~~~~~Query preparation[END]
	//~~~~~~~~~~~~~~Query execution[START]
	qcommand = "mysql " + sqlDB->databaseName() +  " -e \"" + commandL + "\"";
	status = system(qcommand);
	queryL = (commandL.section("(",1,commandL.contains("(")));
	queryL = tr(" where ") + queryL.section("where",1);
	queryL.truncate(queryL.length() - 2);
	//cout << "\nqueryL: " << queryL << endl;
	cout << commandL << " : " << status << endl;
	if(status == 0)
	    return true;
	else
	    return false;
	//~~~~~~~~~~~~~~Query execution[END]
    }
    else if(mode == 1)
    {
	status = system("mysql " + sqlDB->databaseName() + " -e \"" + commandL + "\"");
	if(status == 0)
	    return true;
	else
	    return false;
    }
    else
	return false;
}

bool frmDataAnalyzer::finalizeResultTable_ListSearch(QString tableTo, QString tableFrom)
{
    QString query = "", qcommand = "";
    int status = 0;
    bool retStat = false;
    
    query = "CREATE TABLE " + tableTo + " AS SELECT ";
    if(lsthrq15->isSelected(7) && chkDistinct->isChecked())
	query += "DISTINCT ";
    query +=" * FROM " + tableFrom;
    if(lsthrq15->isSelected(7) && chkSortType->isChecked())
	query += " ORDER BY " + cmborderBy->currentText() + " " + (rdAsc->isChecked() ? tr("Asc") : tr("Desc"));
    query += ";";
    //cout << query << endl;
    qcommand = "mysql " + sqlDB->databaseName() +  " -e \"" + query + "\"";
    status = system(qcommand);
    if(status != 0)
    {
	QMessageBox::information(this,tr("Table creation error!"),tr("Fail to create final table!\nInform the developer"),QMessageBox::Ok);
	retStat = false;
    }
    else
    {
	query = tr("DROP TABLE ") + tableFrom;
	qcommand = tr("mysql ") + sqlDB->databaseName() +  tr(" -e \"") + query + tr("\"");
	status = system(qcommand);
	retStat = true;
    }
    return retStat;
}
////////~~~~~~~~~~~List Search~~~~~~~~~~~~~~~[END]
///////~~~~~~~~~~Miscellaneous Settings~~~~~~[START]
void frmDataAnalyzer::miscellaneousSettings()
{
    chkDistinct->setChecked(false);
    chkSortType->setChecked(false);
    cmborderBy->insertStringList(aggregateField,0);
}

void frmDataAnalyzer::chkSortType_toggled( bool state)
{
    if(state == true)
	grSortType->setEnabled(true);
    else
	grSortType->setEnabled(false);
}
///////~~~~~~~~~~Miscellaneous Settings~~~~~~[STOP]

void frmDataAnalyzer::tabWidget2_currentChanged(QWidget *w)
{
    int index = 0;
    QStringList pgStatList,queryOnList;
    pgStatList.clear();
    
    pgStatList.append("Row");
    pgStatList.append("Wildcard");
    pgStatList.append("Field");
    pgStatList.append("Comparison");
    pgStatList.append("Aggregate");
    pgStatList.append("List");

    queryOnList.clear();
    queryOnList.append("Only Current");
    queryOnList.append("Including: Wildcard");
    queryOnList.append("Including: Field");
    queryOnList.append("Including: Comparison");
    queryOnList.append("Including: Aggregate");
    queryOnList.append("Including: List");
    queryOnList.append("Including: Wildcard,Field");
    queryOnList.append("Including: Wildcard,Comparison");
    queryOnList.append("Including: Wildcard,Aggregate");
    queryOnList.append("Including: Wildcard,List");
    
    tabId = 0;
    if(tabWidget2->tabLabel(w) == QString("Wildcard search"))
    {
	cmbCarry11->clear();
	QStringList::Iterator it = pgStatList.begin();
	for(register int i = 0; i < 1; ++i,++it)
	{
	    if(i < 1 && pageStat[i])
		cmbCarry11->insertItem(*it,index++);
	    else if(i > 1 && pageStat[i])
		cmbCarry11->insertItem(*it,index++);
	}
	tabId = 1;
    }
    else if(tabWidget2->tabLabel(w) == QString("Field selection"))
    {
	cmbCarry12->clear();
	QStringList::Iterator it = pgStatList.begin();
	for(register int i = 0; i < 2; ++i,++it)
	{
	    if(i < 2 && pageStat[i])
		cmbCarry12->insertItem(*it,index++);
	    else if(i > 2 && pageStat[i])
		cmbCarry12->insertItem(*it,index++);
	}
	tabId = 2;
    }
    else if(tabWidget2->tabLabel(w) == QString("Comparison search"))
    {
	cmbCarry13->clear();
	QStringList::Iterator it = pgStatList.begin();
	for(register int i = 0; i < 3; ++i,++it)
	{
	    if(i < 3 && pageStat[i])
		cmbCarry13->insertItem(*it,index++);
	    else if(i > 3 && pageStat[i])
		cmbCarry13->insertItem(*it,index++);
	}
	tabId = 3;
    }
    else if(tabWidget2->tabLabel(w) == QString("Aggregate search"))
    {
	cmbCarry14->clear();
	QStringList::Iterator it = pgStatList.begin();
	for(register int i = 0; i < 4; ++i,++it)
	{
	    if(i < 4 && pageStat[i])
		cmbCarry14->insertItem(*it,index++);
	    else if(i > 4 && pageStat[i])
		cmbCarry14->insertItem(*it,index++);
	}
	tabId = 4;
    }
    else if(tabWidget2->tabLabel(w) == QString("List search"))
    {
	cmbCarry15->clear();
	QStringList::Iterator it = pgStatList.begin();
	for(register int i = 0; i < 5; ++i,++it)
	{
	    if(i < 5 && pageStat[i])
		cmbCarry15->insertItem(*it,index++);
	    else if(i > 5 && pageStat[i])
		cmbCarry15->insertItem(*it,index++);
	}
	tabId = 5;
    }
}
///////////////////////Action Section[START]~~~~~~~~~~~~~~~~
//~~~~~~~~~~~Analysis Action[START]~~~~~~~~~~~~~~~~
void frmDataAnalyzer::openAnalysisDialog()
{
    QWorkspace *wSpace = 0;
    QDataTable *tableSave = 0;
    QPushButton *cmdgenerate = 0;
    QComboBox *cmbSave = 0;
    QComboBox *cmbaction = 0;
        
    switch(tabId)
    {
    case 0:
	if(qryresviewerR)
	{
	    wSpace = wSpaceR;
	    tableSave = qryresviewerR;
	    cmdgenerate = cmdgenerateR;
	    cmbSave = cmbSaveR;
	    cmbaction = cmbactionR;
	}
	else
	{
	    wSpace = 0;
	    tableSave = 0;
	    cmdgenerate = 0;
	    cmbSave = 0;
	    cmbaction = 0;
	}
	break;
    case 1:
	if(qryresviewerW)
	{
	    wSpace = wSpaceW;
	    tableSave = qryresviewerW;
	    cmdgenerate = cmdgenerateW;
	    cmbSave = cmbSaveW;
	    cmbaction = cmbactionW;
	}
	else
	{
	    wSpace = 0;
	    tableSave = 0;
	    cmdgenerate = 0;
	    cmbSave = 0;
	    cmbaction = 0;
	}
	break;
    case 2:
	if(qryresviewerF)
	{
	    wSpace = wSpaceF;
	    tableSave = qryresviewerF;
	    cmdgenerate = cmdgenerateF;
	    cmbSave = cmbSaveF;
	    cmbaction = cmbactionF;
	}
	else
	{
	    wSpace = 0;
	    tableSave = 0;
	    cmdgenerate = 0;
	    cmbSave = 0;
	    cmbaction = 0;
	}
	break;
    case 3:
	if(qryresviewerC)
	{
	    wSpace = wSpaceC;
	    tableSave = qryresviewerC;
	    cmdgenerate = cmdgenerateC;
	    cmbSave = cmbSaveC;
	    cmbaction = cmbactionC;
	}
	else
	{
	    wSpace = 0;
	    tableSave = 0;
	    cmdgenerate = 0;
	    cmbSave = 0;
	    cmbaction = 0;
	}
	break;
    case 4:
	if(qryresviewerA)
	{
	    wSpace = wSpaceA;
	    tableSave = qryresviewerA;
	    cmdgenerate = cmdgenerateA;
	    cmbSave = cmbSaveA;
	    cmbaction = cmbactionA;
	}
	else
	{
	    wSpace = 0;
	    tableSave = 0;
	    cmdgenerate = 0;
	    cmbSave = 0;
	    cmbaction = 0;
	}
	break;
    case 5:
	if(qryresviewerL)
	{
	    wSpace = wSpaceL;
	    tableSave = qryresviewerL;
	    cmdgenerate = cmdgenerateL;
	    cmbSave = cmbSaveL;
	    cmbaction = cmbactionL;
	}
	else
	{
	    wSpace = 0;
	    tableSave = 0;
	    cmdgenerate = 0;
	    cmbSave = 0;
	    cmbaction = 0;
	}
	break;
    }
    
    if(wSpace)
    {
	freeOperation_Table(0);
	cmdgenerate->setEnabled(false);
	cmbSave->setEnabled(false);
	cmbaction->setEnabled(false);
	summarydata *sumdat = new summarydata(wSpace,tr("Analysis"),WDestructiveClose);
	sumdat->initSettings(sqlDB,tableSave);
	sumdat->show();
    }
    else
	QMessageBox::information(this,tr("No Table!"),tr("There is no data for analysis!"));
    
    tableSave = 0;
    wSpace = 0;
    cmdgenerate = 0;
    cmbSave = 0;
    cmbaction = 0;
}
//~~~~~~~~~~~~Analysis Action[STOP]~~~~~~~~~~~~~~~

void frmDataAnalyzer::actionSettings()
{
    //~~~~~~~~~~~~~~~~~~Save/Export Action[START]~~~~~~~~~~~~~~~~~~~~~
    QStringList cmbsaveList;
    cmbsaveList.append("Save in Database");
    cmbsaveList.append("Export to Disk");
    cmbsaveList.append("Export to PLINK");
    
    cmbSaveR->insertStringList(cmbsaveList);
    cmbSaveW->insertStringList(cmbsaveList);
    cmbSaveF->insertStringList(cmbsaveList);
    cmbSaveC->insertStringList(cmbsaveList);
    cmbSaveA->insertStringList(cmbsaveList);
    cmbSaveL->insertStringList(cmbsaveList);
    
    connect(cmbSaveR,SIGNAL(activated(int )),this,SLOT(selectSaveAction(int )));
    connect(cmbSaveW,SIGNAL(activated(int )),this,SLOT(selectSaveAction(int )));
    connect(cmbSaveF,SIGNAL(activated(int )),this,SLOT(selectSaveAction(int )));
    connect(cmbSaveC,SIGNAL(activated(int )),this,SLOT(selectSaveAction(int )));
    connect(cmbSaveA,SIGNAL(activated(int )),this,SLOT(selectSaveAction(int )));
    connect(cmbSaveL,SIGNAL(activated(int )),this,SLOT(selectSaveAction(int )));
    //~~~~~~~~~~~~~~~~~~Save/Export Action[STOP]~~~~~~~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~~Editor Action[START]~~~~~~~~~~~~~~~~~~~~~
    connect(cmbactionR,SIGNAL(activated(int )),this,SLOT(editorAction(int )));
    cmdredoR->setHidden(true);
    cmdundoR->setHidden(true);
    connect(cmbactionW,SIGNAL(activated(int )),this,SLOT(editorAction(int )));
    cmdredoW->setHidden(true);
    cmdundoW->setHidden(true);
    connect(cmbactionF,SIGNAL(activated(int )),this,SLOT(editorAction(int )));
    cmdredoF->setHidden(true);
    cmdundoF->setHidden(true);
    connect(cmbactionC,SIGNAL(activated(int )),this,SLOT(editorAction(int )));
    cmdredoC->setHidden(true);
    cmdundoC->setHidden(true);
    connect(cmbactionA,SIGNAL(activated(int )),this,SLOT(editorAction(int )));
    cmdredoA->setHidden(true);
    cmdundoA->setHidden(true);
    connect(cmbactionL,SIGNAL(activated(int )),this,SLOT(editorAction(int )));
    cmdredoL->setHidden(true);
    cmdundoL->setHidden(true);
    //~~~~~~~~~~~~~~~~~Editor Action[STOP]~~~~~~~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~Log Tracker Settings[START]~~~~~~~~~~~~~~~~
    logTrackerR = new QListView(wSpaceR,tr("logTrackerR"),0);
    logTrackerR->setGeometry(logTrackerR->x(),logTrackerR->y(),50,100);
    logTrackerR->setPaletteForegroundColor("blue");
    logTrackerR->setCaption(tr("Edit Log"));
    logTrackerR->addColumn(tr("Save Point"));
    logTrackerR->addColumn(tr("Redo"));
    (logTrackerR->header())->setResizeEnabled(false,1);
    logTrackerR->hideColumn(1);
    logTrackerR->addColumn(tr("Undo"));
    (logTrackerR->header())->setResizeEnabled(false,2);
    logTrackerR->hideColumn(2);
    logTrackerR->addColumn(tr("Status"));
    (logTrackerR->header())->setResizeEnabled(false,3);
    logTrackerR->hideColumn(3);
    logTrackerR->setSorting(-1);
    logTrackerR->show();
    
    logTrackerW = new QListView(wSpaceW,tr("logTrackerW"),0);
    logTrackerW->setGeometry(logTrackerW->x(),logTrackerW->y(),50,100);
    logTrackerW->setPaletteForegroundColor("blue");
    logTrackerW->setCaption(tr("Edit Log"));
    logTrackerW->addColumn(tr("Save Point"));
    logTrackerW->addColumn(tr("Redo"));
    (logTrackerW->header())->setResizeEnabled(false,1);
    logTrackerW->hideColumn(1);
    logTrackerW->addColumn(tr("Undo"));
    (logTrackerW->header())->setResizeEnabled(false,2);
    logTrackerW->hideColumn(2);
    logTrackerW->addColumn(tr("Status"));
    (logTrackerW->header())->setResizeEnabled(false,3);
    logTrackerW->hideColumn(3);
    logTrackerW->setSorting(-1);
    logTrackerW->show();
    
    logTrackerF = new QListView(wSpaceF,tr("logTrackerF"),0);
    logTrackerF->setGeometry(logTrackerF->x(),logTrackerF->y(),50,100);
    logTrackerF->setCaption(tr("Edit Log"));
    logTrackerF->addColumn(tr("Save Point"));
    logTrackerF->addColumn(tr("Redo"));
    (logTrackerF->header())->setResizeEnabled(false,1);
    logTrackerF->hideColumn(1);
    logTrackerF->addColumn(tr("Undo"));
    (logTrackerF->header())->setResizeEnabled(false,2);
    logTrackerF->hideColumn(2);
    logTrackerF->addColumn(tr("Status"));
    (logTrackerF->header())->setResizeEnabled(false,3);
    logTrackerF->hideColumn(3);
    logTrackerF->setSorting(-1);
    logTrackerF->show();
    
    logTrackerC = new QListView(wSpaceC,tr("logTrackerC"),0);
    logTrackerC->setGeometry(logTrackerC->x(),logTrackerC->y(),50,100);
    logTrackerC->setCaption(tr("Edit Log"));
    logTrackerC->addColumn(tr("Save Point"));
    logTrackerC->addColumn(tr("Redo"));
    (logTrackerC->header())->setResizeEnabled(false,1);
    logTrackerC->hideColumn(1);
    logTrackerC->addColumn(tr("Undo"));
    (logTrackerC->header())->setResizeEnabled(false,2);
    logTrackerC->hideColumn(2);
    logTrackerC->addColumn(tr("Status"));
    (logTrackerC->header())->setResizeEnabled(false,3);
    logTrackerC->hideColumn(3);
    logTrackerC->setSorting(-1);
    logTrackerC->show();
    
    logTrackerA = new QListView(wSpaceA,tr("logTrackerA"),0);
    logTrackerA->setGeometry(logTrackerA->x(),logTrackerA->y(),50,100);
    logTrackerA->setCaption(tr("Edit Log"));
    logTrackerA->addColumn(tr("Save Point"));
    logTrackerA->addColumn(tr("Redo"));
    (logTrackerA->header())->setResizeEnabled(false,1);
    logTrackerA->hideColumn(1);
    logTrackerA->addColumn(tr("Undo"));
    (logTrackerA->header())->setResizeEnabled(false,2);
    logTrackerA->hideColumn(2);
    logTrackerA->addColumn(tr("Status"));
    (logTrackerA->header())->setResizeEnabled(false,3);
    logTrackerA->hideColumn(3);
    logTrackerA->setSorting(-1);
    logTrackerA->show();
    
    logTrackerL = new QListView(wSpaceL,tr("logTrackerL"),0);
    logTrackerL->setGeometry(logTrackerL->x(),logTrackerL->y(),50,100);
    logTrackerL->setCaption(tr("Edit Log"));
    logTrackerL->addColumn(tr("Save Point"));
    logTrackerL->addColumn(tr("Redo"));
    (logTrackerL->header())->setResizeEnabled(false,1);
    logTrackerL->hideColumn(1);
    logTrackerL->addColumn(tr("Undo"));
    (logTrackerL->header())->setResizeEnabled(false,2);
    logTrackerL->hideColumn(2);
    logTrackerL->addColumn(tr("Status"));
    (logTrackerL->header())->setResizeEnabled(false,3);
    logTrackerL->hideColumn(3);
    logTrackerL->setSorting(-1);
    logTrackerL->show();
    //~~~~~~~~~~~~~~~~Log Tracker Settings[STOP]~~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~Delete Editor Settings[START]~~~~~~~~~~~~~~~
    connect(cmdredoR,SIGNAL(clicked()),this,SLOT(cmdredo_clicked()));
    connect(cmdredoW,SIGNAL(clicked()),this,SLOT(cmdredo_clicked()));
    connect(cmdredoF,SIGNAL(clicked()),this,SLOT(cmdredo_clicked()));
    connect(cmdredoC,SIGNAL(clicked()),this,SLOT(cmdredo_clicked()));
    connect(cmdredoA,SIGNAL(clicked()),this,SLOT(cmdredo_clicked()));
    connect(cmdredoL,SIGNAL(clicked()),this,SLOT(cmdredo_clicked()));
    
    connect(cmdundoR,SIGNAL(clicked()),this,SLOT(cmdundo_clicked()));
    connect(cmdundoW,SIGNAL(clicked()),this,SLOT(cmdundo_clicked()));
    connect(cmdundoF,SIGNAL(clicked()),this,SLOT(cmdundo_clicked()));
    connect(cmdundoC,SIGNAL(clicked()),this,SLOT(cmdundo_clicked()));
    connect(cmdundoA,SIGNAL(clicked()),this,SLOT(cmdundo_clicked()));
    connect(cmdundoL,SIGNAL(clicked()),this,SLOT(cmdundo_clicked()));
    //~~~~~~~~~~~~~~~~Delete Editor Settings[STOP]~~~~~~~~~~~~~~~~
    //~~~~~~~~~~~~~~~~Update Editor Settings[START]~~~~~~~~~~~~~~
    cmdundoR_U->setHidden(true);
    cmdredoR_U->setHidden(true);
    cmdundoW_U->setHidden(true);
    cmdredoW_U->setHidden(true);
    cmdundoF_U->setHidden(true);
    cmdredoF_U->setHidden(true);
    cmdundoC_U->setHidden(true);
    cmdredoC_U->setHidden(true);
    cmdundoL_U->setHidden(true);
    cmdredoL_U->setHidden(true);
    replaceAllFlag = false;
       
    connect(cmdFindReplaceR,SIGNAL(clicked()),this,SLOT(FindReplace_Clicked()));
    connect(cmdFindReplaceW,SIGNAL(clicked()),this,SLOT(FindReplace_Clicked()));
    connect(cmdFindReplaceF,SIGNAL(clicked()),this,SLOT(FindReplace_Clicked()));
    connect(cmdFindReplaceC,SIGNAL(clicked()),this,SLOT(FindReplace_Clicked()));
    connect(cmdFindReplaceL,SIGNAL(clicked()),this,SLOT(FindReplace_Clicked()));
    
    connect(cmdredoR_U,SIGNAL(clicked()),this,SLOT(cmdredo_U_clicked()));
    connect(cmdredoW_U,SIGNAL(clicked()),this,SLOT(cmdredo_U_clicked()));
    connect(cmdredoF_U,SIGNAL(clicked()),this,SLOT(cmdredo_U_clicked()));
    connect(cmdredoC_U,SIGNAL(clicked()),this,SLOT(cmdredo_U_clicked()));
    connect(cmdredoL_U,SIGNAL(clicked()),this,SLOT(cmdredo_U_clicked()));
    
    connect(cmdundoR_U,SIGNAL(clicked()),this,SLOT(cmdundo_U_clicked()));
    connect(cmdundoW_U,SIGNAL(clicked()),this,SLOT(cmdundo_U_clicked()));
    connect(cmdundoF_U,SIGNAL(clicked()),this,SLOT(cmdundo_U_clicked()));
    connect(cmdundoC_U,SIGNAL(clicked()),this,SLOT(cmdundo_U_clicked()));
    connect(cmdundoL_U,SIGNAL(clicked()),this,SLOT(cmdundo_U_clicked()));
    
    connect(cmdconfirmupdateR,SIGNAL(clicked()),this,SLOT(confirmupdate_Clicked()));
    connect(cmdconfirmupdateW,SIGNAL(clicked()),this,SLOT(confirmupdate_Clicked()));
    connect(cmdconfirmupdateF,SIGNAL(clicked()),this,SLOT(confirmupdate_Clicked()));
    connect(cmdconfirmupdateC,SIGNAL(clicked()),this,SLOT(confirmupdate_Clicked()));
    connect(cmdconfirmupdateL,SIGNAL(clicked()),this,SLOT(confirmupdate_Clicked()));
    //~~~~~~~~~~~~~~~~Update Editor Settings[STOP]~~~~~~~~~~~~~~~    
}

//~~~~~~~~~~~~~~Save/Export operation~~~~~~~~[START]
void frmDataAnalyzer::selectSaveAction(int index)
{
    switch(index)
    {
    case 0:
	saveInDatabase();
	break;
    case 1:
	exportToDisk();
	break;
    case 2:
	exportInPlinkFormat();
	break;
    }
}

void frmDataAnalyzer::saveInDatabase()
{
    QDataTable *tableSave = 0;
    
    switch(tabId)
    {
    case 0:
	if(qryresviewerR)
	    tableSave = qryresviewerR;
	else
	    tableSave = 0;
	break;
    case 1:
	if(qryresviewerW)
	    tableSave = qryresviewerW;
	else
	    tableSave = 0;
	break;
    case 2:
	if(qryresviewerF)
	    tableSave = qryresviewerF;
	else
	    tableSave = 0;
	break;
    case 3:
	if(qryresviewerC)
	    tableSave = qryresviewerC;
	else
	    tableSave = 0;
	break;
    case 4:
	if(qryresviewerA)
	    tableSave = qryresviewerA;
	else
	    tableSave = 0;
	break;
    case 5:
	if(qryresviewerL)
	    tableSave = qryresviewerL;
	else
	    tableSave = 0;
	break;
    }

    if(tableSave)
    {
	bool ok = false;
	QString query = "";
	QString saveTableName = QInputDialog::getText("Table name for saving data","Enter table name:",QLineEdit::Normal,QString::null,&ok,this);
	
	if(ok && !saveTableName.isEmpty()) 
	{
	    if(checkTableNameSyntax(saveTableName))
	    {
		if(checkIfAlreadyTableExistOfThisName(saveTableName))
		{
		    query = tr("mysql ") + sqlDB->databaseName() + tr(" -e \"create table ") +  saveTableName + tr(" as(select * from ") + tableSave->caption() + tr(")\";");
		    fscanVal = system(query);
		    if(fscanVal > -1)
		    {
			//~~To add primary key to the save as table~~[START]
			int recsafter = 0;
			QString pkName = "";
			QStringList::iterator it = listField.begin();
			pkName = *it;
			query = tr("mysql ") + sqlDB->databaseName() + tr(" -e \"ALTER TABLE ") + saveTableName + tr(" ADD PRIMARY KEY(") + pkName + tr(")\";");
			cout << query << endl;
			fscanVal = system(query);
			//~~To add primary key to the save as table~~[STOP]
			QMessageBox::information(this,"Saving successful!","Table has been saved in the database.");
		    }
		    else
			QMessageBox::information(this,"Saving unsuccessful!","Sorry! database can't create table \nwith the given name!");
		}
	    }
	}
    }
    else
	QMessageBox::information(this,tr("No table!"),tr("There is no table for saving!"));
    tableSave = 0;
}

bool frmDataAnalyzer::checkTableNameSyntax(QString &tableName)
{
    bool ok = true;
    int length = tableName.length();
    QChar ch;
    
    for(register int i = 0; i < length; ++i)
    {
	ch = tableName.at(i);
	if(!((ch >= 'A' && ch <= 'Z') || (ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')))
	{
	    ok = false;
	    break;
	}
    }
    return ok;
}

bool frmDataAnalyzer::checkIfAlreadyTableExistOfThisName(QString &tableName)
{
    bool okStat = false, retStat = false;
    QStringList tabList;
    
    tabList = sqlDB->tables();
    
    for(QStringList::iterator it = tabList.begin(); it != tabList.end(); ++it)
    {
	if((*it).upper() == tableName.upper())
	{
	    okStat = true;
	    break;
	}
    }
    
    if(okStat == false)
	retStat = true;
    else
    {
	if(!QMessageBox::question(this, tr("Table exist!"),("Sorry! table with this name already exist!.\nDo you want to overwrite the existing table?"),tr("Yes"),tr("No"),QString::null,0,1))
	{
	    QString query = tr("mysql ") + sqlDB->databaseName() + tr(" -e \"DROP TABLE ") +  tableName + tr("\";");
	    fscanVal = system(query);
	    if(fscanVal > -1)
		retStat = true;
	    else
	    {
		QMessageBox::information(this,tr("Table deletion error"),tr("System can not delete the table!\nInform the developer"));
		retStat = false;
	    }
	}
	else
	    retStat = false;
    }
    return retStat;
}

void frmDataAnalyzer::exportToDisk()
{
    QDataTable *tableSave = 0;
    QStringList filterList;
    QFileDialog* saveFD = 0;
    QString saveFilename = "",filterName = "";
    QString charset = "";
  
    switch(tabId)
    {
    case 0:
	if(qryresviewerR)
	    tableSave = qryresviewerR;
	else
	    tableSave = 0;
	break;
    case 1:
	if(qryresviewerW)
	    tableSave = qryresviewerW;
	else
	    tableSave = 0;
	break;
    case 2:
	if(qryresviewerF)
	    tableSave = qryresviewerF;
	else
	    tableSave = 0;
	break;
    case 3:
	if(qryresviewerC)
	    tableSave = qryresviewerC;
	else
	    tableSave = 0;
	break;
    case 4:
	if(qryresviewerA)
	    tableSave = qryresviewerA;
	else
	    tableSave = 0;
	break;
    case 5:
	if(qryresviewerL)
	    tableSave = qryresviewerL;
	else
	    tableSave = 0;
	break;
    }

    if(tableSave)
    {
	filterList.append(tr("CSV Files (*.csv)"));
	filterList.append(tr("TGT Files (*.txt)"));
	filterList.append(tr("CST Files (*.txt)"));
	filterList.append(tr("HTML Files (*.html)"));
	filterList.append(tr("XML Files (*.xml)"));
    
	saveFD = new QFileDialog(this, "Give file name", true);
	saveFD->setMode(QFileDialog::AnyFile);
	saveFD->setFilters(filterList);
	saveFD->setInfoPreviewEnabled(true);
	if(saveFD->exec() == QDialog::Accepted)
	{
	    saveFD->close();
	    saveFilename = saveFD->selectedFile();
	    if(saveFilename.isEmpty())
		QMessageBox::information(this,tr("File saving status"),tr("No file name given"),QMessageBox::Ok);
	    else
	    {
		QString query = saveFilename.section('/',-1);
		saveFilename = saveFilename.section('/',0,saveFilename.contains('/') - 1);
		filterName = saveFD->selectedFilter();
		int counter = 0;
		for(QStringList::iterator it = filterList.begin(); it != filterList.end(); ++it,++counter)
		{
		    if(*it == filterName)
			break;
		}
		
		switch(counter)
		{
		case 0:
		    if(query.contains(tr(".csv")) > 0)
			query.remove(tr(".csv"));
		    saveFilename += "/" + query;
		    saveInCSVFormat(tableSave->caption(), saveFilename + ".csv");
		    break;
		case 1:
		    if(query.contains(tr(".txt")) > 0)
			query.remove(tr(".txt"));
		    saveFilename += "/" + query;
		    saveInTabGenFormat(tableSave->caption(), saveFilename +".txt");
		    break;
		case 2:
		    if(query.contains(tr(".txt")) > 0)
			query.remove(tr(".txt"));
		    saveFilename += "/" + query;
		    saveInCommaGenFormat(tableSave->caption(), saveFilename +".txt");
		    break;
		case 3:
		    if(query.contains(tr(".html")) > 0)
			query.remove(tr(".html"));
		    saveFilename += "/" + query;
		    saveInHTLMFormat(tableSave->caption(), saveFilename + ".html");
		    break;
		case 4:
		    if(query.contains(tr(".xml")) > 0)
			query.remove(tr(".xml"));
		    saveFilename += "/" + query;
		    saveInXMLFormat(tableSave->caption(), saveFilename + ".xml");
		    break;
		}
	    }
	    delete saveFD;
	    saveFD = 0;
	}
    }
    else
	QMessageBox::information(this,tr("No table!"),tr("There is no table to export!"));
    tableSave = 0;
}

void frmDataAnalyzer::saveInCSVFormat(QString saveTab, QString saveFilename)
{
    //   s/\t/","/g ~ replace 'tab' with ","   //     s/^/"/ ~ quote at the begining 
    QString query = "mysql " + sqlDB->databaseName() + " -B -e \"select * from " + saveTab + ";\" | sed 's/\t/\",\"/g;s/^/\"/;s/$/\"/;'  > " + saveFilename;	 //   s/$/"/ ~ quote at the end
    int status = system(query);
    if(status > -1)
	QMessageBox::information(this,tr("File saving status"),tr("File has been saved successfully"),QMessageBox::Ok);
    else
	QMessageBox::information(this,tr("File saving status"),tr("File saving operation is unsuccessful"),QMessageBox::Ok);
}

void frmDataAnalyzer::saveInTabGenFormat(QString saveTab, QString saveFilename)
{
    QString query = "mysql " + sqlDB->databaseName() + " -B -e \"select * from \\`" + saveTab + "\\`;\"" + " > " + saveFilename;
    int status = system(query);
    if(status > -1)
	QMessageBox::information(this,tr("File saving status"),tr("File has been saved successfully"),QMessageBox::Ok);
    else
	QMessageBox::information(this,tr("File saving status"),tr("File saving operation is unsuccessful"),QMessageBox::Ok);
}

void frmDataAnalyzer::saveInCommaGenFormat(QString saveTab, QString saveFilename)
{
    QString query = "mysql " + sqlDB->databaseName() + " -B -e \"select * from " + saveTab + ";\" | sed 's/\t/,/g'  > " + saveFilename;
    int status = system(query);
    if(status > -1)
	QMessageBox::information(this,tr("File saving status"),tr("File has been saved successfully"),QMessageBox::Ok);
    else
	QMessageBox::information(this,tr("File saving status"),tr("File saving operation is unsuccessful"),QMessageBox::Ok);
}

void frmDataAnalyzer::saveInHTLMFormat(QString saveTab, QString saveFilename)
{
    QString query = "mysql " + sqlDB->databaseName() + " -H -e \"select * from \\`" + saveTab + "\\`;\"" + " > " + saveFilename;
    int status = system(query);
    if(status > -1)
	QMessageBox::information(this,tr("File saving status"),tr("File has been saved successfully"),QMessageBox::Ok);
    else
	QMessageBox::information(this,tr("File saving status"),tr("File saving operation is unsuccessful"),QMessageBox::Ok);
}

void frmDataAnalyzer::saveInXMLFormat(QString saveTab, QString saveFilename)
{
    QString query = "mysql " + sqlDB->databaseName() + " -X -e \"select * from \\`" + saveTab + "\\`;\"" + " > " + saveFilename;
    int status = system(query);
    if(status > -1)
	QMessageBox::information(this,tr("File saving status"),tr("File has been saved successfully"),QMessageBox::Ok);
    else
	QMessageBox::information(this,tr("File saving status"),tr("File saving operation is unsuccessful"),QMessageBox::Ok);
}

void frmDataAnalyzer::exportInPlinkFormat()
{
    QWorkspace *wSpace = 0;
    QSqlCursor *viewCur = 0;
    QDataTable *tableSave = 0;
    QFileDialog* saveFD = 0;
    QString charset = "", saveFilename = "", filterName = "";
    QStringList headerList,headerIndexList,filterList;
    
    switch(tabId)
    {
    case 0:
	if(qryresviewerR)
	{
	    wSpace = wSpaceR;
	    tableSave = qryresviewerR;
	    viewCur = viewCurR;
	}
	else
	    tableSave = 0;
	break;
    case 1:
	if(qryresviewerW)
	{
	    wSpace = wSpaceW;
	    tableSave = qryresviewerW;
	    viewCur = viewCurW;
	}
	else
	    tableSave = 0;
	break;
    case 2:
	if(qryresviewerF)
	{
	    wSpace = wSpaceF;
	    tableSave = qryresviewerF;
	     viewCur = viewCurF;
	}
	else
	    tableSave = 0;
	break;
    case 3:
	if(qryresviewerC)
	{
	    wSpace = wSpaceC;
	    tableSave = qryresviewerC;
	    viewCur = viewCurC;
	}
	else
	    tableSave = 0;
	break;
    case 4:
	if(qryresviewerA)
	{
	    wSpace = wSpaceA;
	    tableSave = qryresviewerA;
	    viewCur = viewCurA;
	}
	else
	    tableSave = 0;
	break;
    case 5:
	if(qryresviewerL)
	{
	    wSpace = wSpaceL;
	    tableSave = qryresviewerL;
	    viewCur = viewCurL;
	}
	else
	    tableSave = 0;
	break;
    }
    
    if(tableSave)
    {
	if(selectPEDFields(wSpace,tableSave,headerList,headerIndexList))
	{
	    filterList.append(tr("Plink Files (*.map)"));
	    filterList.append(tr("Plink Files (*.ped)"));
	    saveFD = new QFileDialog(this, "Give file name", true);
	    saveFD->setMode(QFileDialog::AnyFile);
	    saveFD->setFilters(filterList);
	    saveFD->setInfoPreviewEnabled(true);
	    if(saveFD->exec() == QDialog::Accepted)
	    {
		saveFD->close();
		saveFilename = saveFD->selectedFile();
		if(saveFilename.isEmpty())
		    QMessageBox::information(this,tr("File saving status"),tr("No file name given"),QMessageBox::Ok);
		else
		{
		    QString query = saveFilename.section('/',-1);
		    saveFilename = saveFilename.section('/',0,saveFilename.contains('/') - 1);
		    filterName = saveFD->selectedFilter();
		    int counter = 0;
		    for(QStringList::iterator it = filterList.begin(); it != filterList.end(); ++it,++counter)
		    {
			if(*it == filterName)
			    break;
		    }			    
		    if(counter == 0)
		    {
			if(query.contains(tr(".map")) > 0)
			    query.remove(tr(".map"));
			saveFilename += "/" + query;
			if(saveInPLINK_MAPFormat(tableSave->caption(), saveFilename + ".map1") > -1)
			{
			    saveInPLINK_cutHeader(saveFilename + ".map1", saveFilename + ".map2");
			    if(saveInPLINK_cutSelectedFields(saveFilename + ".map2", saveFilename + ".map",headerIndexList))
				QMessageBox::information(this,tr("Plink export status"),tr("Table has been saved in the name ") + saveFilename + tr(".map under the specified directory"),QMessageBox::Ok);
			    else
				QMessageBox::information(this,tr("Plink export status"),tr("Table saving operation is unsuccessful"),QMessageBox::Ok);
			}
		    }
		    else if(counter == 1)
		    {
			if(askForTableContainingPEDHeaderInfo())
			{
			    loadSelectedHeader(tableSave,headerList,headerIndexList);
			    if(query.contains(tr(".ped")) > 0)
				query.remove(tr(".ped"));
			    saveFilename += "/" + query;
			    charset = getCharSet_PlinkFormat();
			    if(!charset.isEmpty())
			    {
				if(saveInPLINK_PEDFormat(tableSave->caption(), saveFilename + ".ped1", charset) > -1)
				{
				    saveInPLINK_cutHeader(saveFilename + tr(".ped1"), saveFilename + tr(".ped2"));
				    if(saveInPLINK_cutSelectedFields(saveFilename + tr(".ped2"), saveFilename + tr(".ped3"),headerIndexList))
					QMessageBox::information(this,tr("Plink export status"),tr("Table has been saved in the name ") + saveFilename + tr(".ped3 under the specified directory\nThis is 'PED' file's body part."),QMessageBox::Ok);
				    else
					QMessageBox::information(this,tr("Plink export status"),tr("Table saving operation is unsuccessful"),QMessageBox::Ok);
				    //At this stage a copy of snip values are saved in a temporary ped file
				    //Now close the table and open the 'PED' header table to save selected individual's information
				    if(tableSave)
				    {
					QString tableCaption = tableSave->caption();
					//Release cursor and data table widget from the table with name in 'tableCaption'
					if(viewCur)
					{
					    viewCur->clear();
					    delete viewCur;
					    viewCur = 0;
					}
					if(!tableSave->isHidden())
					    tableSave->close();
					delete tableSave;
					tableSave = 0;
					//Release cursor and data table widget from the table with name in 'tableCaption'
					QSqlQuery sq = sqlDB->exec("DROP TABLE " + tableCaption); //drop table
					if(sq.isActive())
					{
					    if(openPEDHeaderTable(wSpace,&viewCur,&tableSave,tableCaption,headerList)) // refPEDTable and refPEDField are global
					    {
						if(selectPEDFields(wSpace,tableSave,headerList,headerIndexList))
						{
						    loadSelectedHeader(tableSave,headerList,headerIndexList);
						    if(saveInPLINK_MAPFormat(tableSave->caption(), saveFilename + ".ped1") > -1)
						    {
							saveInPLINK_cutHeader(saveFilename + ".ped1", saveFilename + ".ped2");
							if(saveInPLINK_cutSelectedFields(saveFilename + ".ped2", saveFilename + ".ped4",headerIndexList))
							{
							    QMessageBox::information(this,tr("Plink export status"),tr("Table has been saved in the name ") + saveFilename + tr(".ped4 under the specified directory\nThis is 'PED' file's header part."),QMessageBox::Ok);
							    if(transposePEDHeaderTableANDSave((int)headerIndexList.count(),saveFilename + tr(".ped4"), saveFilename + tr(".ped")))
							    {
								if(mergePED_snp_AND_header(saveFilename + tr(".ped3"), saveFilename + tr(".ped")))
								    QMessageBox::information(this,tr("Need transpose!"),tr("'PED' file is now in semi-complete form.\nIt is saved in the desired location.\nLocation: ") + saveFilename + tr(".ped") + "\nKindly transpose the file using\nthe tabular data file 'transpose' facility\nprovided in our application to obtain desired 'PED' file");
							    }
							}
							else
							    QMessageBox::information(this,tr("Plink export status"),tr("Table saving operation is unsuccessful"),QMessageBox::Ok);
						    }
						}
						else
						{
						    fscanVal = system(tr("rm -f ") + saveFilename + tr(".ped2"));
						    QMessageBox::information(this,tr("PED file creation problem!"),tr("Sorry...\nWithout header information 'PED' file can't be created!"));
						}
					    }
					    else
						QMessageBox::information(this,tr("Table opening problem!"),tr("May be the table") + tableCaption + tr(" does not exist!\nTry again!!!"));
					}
					else
					    QMessageBox::information(this,tr("Table deletion problem!"),tr("Temporary table deletion peroblem.\nTable name: ") + tableCaption + tr("\n Please inform software developer"));
				    }
				}
			    }
			}
			else
			    QMessageBox::information(this,tr("Additional table missing!"),tr("No table is selected containing additional information of the selected 'Individuals'\nfor generating 'PED' file (Example: Family ID, Individual ID, Paternal ID, \nMaternal ID, Sex, Phenotype)"));
		    }
		}
		delete saveFD;
		saveFD = 0;
	    }
	}
    }
    else
	QMessageBox::information(this,tr("No table!"),tr("There is no table to export!"));
    
    switch(tabId)
    {
    case 0:
	qryresviewerR = tableSave;
	viewCurR = viewCur;
	break;
    case 1:
	qryresviewerW = tableSave;
	viewCurW = viewCur;
	break;
    case 2:
	qryresviewerF = tableSave;
	viewCurF = viewCur;
	break;
    case 3:
	qryresviewerC = tableSave;
	viewCurC = viewCur;
	break;
    case 4:
	qryresviewerA = tableSave;
	viewCurA = viewCur;
	break;
    case 5:
	qryresviewerL = tableSave;
	viewCurL = viewCur;
	break;
    }
    wSpace = 0;
    viewCur = 0;
    tableSave = 0;
}

QString frmDataAnalyzer::getCharSet_PlinkFormat()
{
    bool ok;
    QString charset = "";
    //Load previous charset given[START]
    QFile fpRead(".charsetPlink2.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	fpIn >> charset;
	fpRead.close();
	charset.replace(tr("|"),tr(","),false);
    }
    //Load previous charset given[STOP]
    while(1)
    {
	//Taking 'Cahrecter set' for Plink Formatting from user
	charset = QInputDialog::getText("Charecter set Input","Enter cahrecter set of the Genotypes\nfor converting to Plink format[example: a,A,B] case sensitive:",QLineEdit::Normal,charset,&ok,this);
	if(ok && !charset.isEmpty()) 
	{
	    bool okStat = false;
	    if(charset.contains(tr(",")) > 0)
	    {
		QStringList charList = QStringList::split(charset,",");
		for(QStringList::iterator it = charList.begin(); it != charList.end(); ++it)
		{
		    if((int)((*it).length()) > 1)
		    {
			okStat = true;
			break;
		    }
		}
		if(okStat == true)
		    QMessageBox::information(this,tr("Charecter set input status"),tr("\nInput error!\nType charecters(case sensitive) separated by comma"));
	    }
	    else
	    {
		QMessageBox::information(this,tr("Charecter set input status"),tr("\nInput error!\nType charecters(case sensitive) separated by comma"));
		okStat = true;
	    }

	    if(okStat == false)
	    {
		charset.replace(tr(","),tr("|"),false);
		//Save charset given in file[START]
		QFile fpWrite(".charsetPlink2.txt");
		if(fpWrite.open(IO_WriteOnly | IO_Truncate))
		{
		    QTextStream fpOut(&fpWrite);
		    fpOut.setEncoding(QTextStream::UnicodeUTF8);
		    fpOut << charset << endl;
		    fpWrite.close();
		}
		//Save charset given in file[STOP]
		break;
	    }    
	}
	else
	    QMessageBox::information(this,tr("Charecter set input status"),tr("\nInput error!\nInput may be blank!"));
    }
    return charset;
}

int frmDataAnalyzer::saveInPLINK_MAPFormat(QString saveTab, QString saveFilename)
{
    QString query = "mysql " + sqlDB->databaseName() + " -B -e \"select * from \\`" + saveTab + "\\`;\"" + " > " + saveFilename;
    fscanVal = system(query);
    return fscanVal;
}

int frmDataAnalyzer::saveInPLINK_PEDFormat(QString saveTab, QString saveFilename, QString charset)
{
    //   s/\t/","/g ~ replace 'tab' with ","   //     s/^/"/ ~ quote at the begining 
    QString query = tr("mysql ") + sqlDB->databaseName() + tr(" -B -e \"select * from ") + saveTab + tr(";\" | sed 's/\\([") + charset + tr("]\\)\\([") + charset + tr("]\\)/\\1 \\2/g'  > ") + saveFilename;
    //cout << query << endl;
    fscanVal = system(query);
    return fscanVal;
}

void frmDataAnalyzer::saveInPLINK_cutHeader(QString saveFrom, QString saveTo)
{
    QString query = "";
    char scanBuf[500];
    FILE *fpIn = NULL;
    int numLines = 0;
    
    fscanVal = system(tr("wc -l ") + saveFrom + tr(" > tempPlink_Lines.txt"));
    fpIn = fopen("tempPlink_Lines.txt","r");
    fscanVal = fscanf(fpIn,"%d %[^\n]",&numLines,scanBuf);
    fclose(fpIn);
    fpIn = NULL;
    fscanVal = system("rm -f tempPlink_Lines.txt");
    query = tr("tail -") + QString::number(numLines - 1) + tr(" ") + saveFrom + tr(" > ") + saveTo;
    fscanVal = system(query);
    fscanVal = system(tr("rm -f ") + saveFrom);
}

bool frmDataAnalyzer::saveInPLINK_cutSelectedFields(QString saveFrom, QString saveTo, QStringList headerList)
{
    QString query = "";
    int counter = 0;
    bool retStat = false;
    
    query = tr("cut -f ");
    for(QStringList::iterator it = headerList.begin(); it != headerList.end(); ++it, ++counter)
    {
	if(counter < (int)headerList.count() - 1)
	    query += *it + tr(",");
	else
	    query += *it + tr(" ");
    }
    query += saveFrom + tr(" > ") + saveTo;
    //cout << query << endl;
    fscanVal = system(query);
    retStat = (fscanVal > -1 ? true : false);
    fscanVal = system(tr("rm -f ") + saveFrom);
    return retStat;
}

bool frmDataAnalyzer::selectPEDFields(QWorkspace *wSpace, QDataTable *tableSave, QStringList &headerList, QStringList &headerIndexList)
{
    bool retStat = false;
    
    headerList.clear();
    headerIndexList.clear();
    
    int numFields = tableSave->numCols();
    
    QHeader *head = tableSave->horizontalHeader();
    
    for(int i = 0; i < numFields; ++i)
	headerList.append(head->label(i));
    
    frmeditheader *header = new frmeditheader(wSpace,tr("Header Editor"),true,0);
    header->headList = headerList;
    header->initSettings();
    if(header->exec() == QDialog::Accepted)
    {
	if(header->exitStatus)
	{
	    headerIndexList = header->headList;
	    header->close();
	    delete header;
	    header = 0;
	    retStat = true;
	}
	else
	    retStat = false;
    }
    return retStat;
}

bool frmDataAnalyzer::askForTableContainingPEDHeaderInfo()
{
    bool statFlag = false;
    QStringList tableList = sqlDB->tables(QSql::Tables);
        
    refPEDTable = "";
    frmpedheader *pedhead = new frmpedheader(this,"pedheader",true,0);
    pedhead->tableList = tableList;
    pedhead->initSettings(sqlDB);
    if(pedhead->exec() == QDialog::Accepted)
    {
	statFlag = pedhead->statFlag;
	refPEDTable = pedhead->refTable;
	refPEDField = pedhead->refField;
	pedhead->close();
	delete pedhead;
	pedhead = 0;
    }
    return statFlag;
}

void frmDataAnalyzer::loadSelectedHeader(QDataTable *tableSave, QStringList &headerList, QStringList &headerIndexList)
{
    //Take a copy of the selected column's (ex. individual name) label[START]
    headerList.clear();
    int index = -1; //Temporary variable
    bool ok = false;
    
    QHeader *head = tableSave->horizontalHeader();
    for(QStringList::iterator it = headerIndexList.begin(); it != headerIndexList.end(); ++it)
    {
	ok = false;
	index = (*it).toInt(&ok,10);
	if(ok)
	    headerList.append(head->label(index - 1));
    }
    //Take a copy of the selected column's (ex. individual name) label[STOP]
}

bool frmDataAnalyzer::openPEDHeaderTable(QWorkspace *wSpace, QSqlCursor **viewCur, QDataTable **tableSave, QString tableCaption, QStringList headerList)
{
    int counter = 0, status = -9;
    QString query = "create table " + tableCaption + " as(select * from " + refPEDTable;
    query +=  " where " + refPEDField + " IN(";
    for(QStringList::iterator it = headerList.begin(); it != headerList.end(); ++it,++counter)
    {
	if(counter < (int)headerList.count() - 1)
	    query += "'" + *it + "',";
	else
	    query += "'" + *it + "'));";
    }
    //cout << query << endl;
    status = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
    if(status == -1)
	return false;
    else
    {
	*viewCur = new QSqlCursor(tableCaption,true,sqlDB);
	*tableSave = new QDataTable(*viewCur,true,wSpace,tr(tableCaption));
	(*tableSave)->setCaption(tableCaption);
	(*tableSave)->refresh(QDataTable::RefreshAll);
	(*tableSave)->show();
	return true;
    }
}

bool frmDataAnalyzer::transposePEDHeaderTableANDSave(int numFields, QString saveFrom, QString saveTo)
{
    bool retStat = false;
    
    QString tempFile = tr(".tmpInter");
    QString str1 = tr("cut -f ");
    QString str2 = tr(" ") + saveFrom + tr(" > ") + tempFile;
    QString str3 = tr("tr '\\n' '\\t' < ") + tempFile; //replace '\n' ~ '\t'
    QString str5 = tr("tr -d '\\r' < ") + tempFile; //delete '\r' if any
    QString str4 =  tr(" | sed 's/\\t$/\\n/'"); //replace '\t' in the last line with '\n'
    QString str6 =  tr(" | tr '\\n' '\\t' | sed 's/\\t$/\\n/'");
    QString str7 = tr(" > "); //write mode
    QString str8 = tr(" >> "); //append mode
    
    QString query = "";
    for(int i = 1; i <= numFields; ++i)
	query += str1 + QString::number(i) + str2 + QString::number(i) + (i < numFields ? " | " : " ; ");
    for(int i = 1; i <= numFields; ++i)
	query += (i < numFields ? str3 : str5) + QString::number(i) + (i < numFields ? str4 : str6) + (i > 1 ? str8 : str7) + saveTo + tr(";");
    fscanVal = system(query);
    retStat = (fscanVal == -1 ? false : true);
    //Remove intermediate files[START]
    fscanVal = system(tr("rm -f ") + saveFrom);
    for(int i = 1; i <= numFields; ++i)
	fscanVal = system(tr("rm -f ") + tempFile + tr("*"));
    //Remove intermediate files[STOP]
    return retStat;
}
bool frmDataAnalyzer::mergePED_snp_AND_header(QString saveFrom, QString saveTo)
{
    fscanVal = system(tr("cat ") + saveFrom + tr(" >> ") + saveTo);
    if(fscanVal > -1)
    {
	fscanVal = system(tr("rm -f ") + saveFrom);
	return true;
    }
    else
	return false;
}
//~~~~~~~~~~~~~~Save/Export operation~~~~~~~~[STOP]

void frmDataAnalyzer::editorAction(int id)
{
    QSqlCursor *viewCur = 0;
    QListView *logTracker = 0;
    
    switch(tabId)
    {
    case 0:
	if(qryresviewerR)
	{
	    viewCur = viewCurR;
	    logTracker = logTrackerR;
	    //activate/inactivate some buttons[START]
	    cmdFindReplaceR->setHidden(true);
	    cmdconfirmupdateR->setHidden(true);
	    cmdundoR_U->setHidden(true);
	    cmdredoR_U->setHidden(true);
	    qryresviewerR->setConfirmUpdate(false);
	    //activate/inactivate some buttons[STOP]
	}
	break;
    case 1:
	if(qryresviewerW)
	{
	    viewCur = viewCurW;
	    logTracker = logTrackerW;
	    //activate/inactivate some buttons[START]
	    cmdFindReplaceW->setHidden(true);
	    cmdconfirmupdateW->setHidden(true);
	    cmdundoW_U->setHidden(true);
	    cmdredoW_U->setHidden(true);
	    qryresviewerW->setConfirmUpdate(false);
	    //activate/inactivate some buttons[STOP]
	}
	break;
    case 2:
	if(qryresviewerF)
	{
	    viewCur = viewCurF;
	    logTracker = logTrackerF;
	    //activate/inactivate some buttons[START]
	    cmdFindReplaceF->setHidden(true);
	    cmdconfirmupdateF->setHidden(true);
	    cmdundoF_U->setHidden(true);
	    cmdredoF_U->setHidden(true);
	    qryresviewerF->setConfirmUpdate(false);
	    //activate/inactivate some buttons[STOP]
	}
	break;
    case 3:
	if(qryresviewerC)
	{
	    viewCur = viewCurC;
	    logTracker = logTrackerC;
	    //activate/inactivate some buttons[START]
	    cmdFindReplaceC->setHidden(true);
	    cmdconfirmupdateC->setHidden(true);
	    cmdundoC_U->setHidden(true);
	    cmdredoC_U->setHidden(true);
	    qryresviewerC->setConfirmUpdate(false);
	    //activate/inactivate some buttons[STOP]
	}
	break;
    case 4:
	if(qryresviewerA)
	    viewCur = viewCurA;
	break;
    case 5:
	if(qryresviewerL)
	{
	    viewCur = viewCurL;
	    logTracker = logTrackerL;
	    //activate/inactivate some buttons[START]
	    cmdFindReplaceL->setHidden(true);
	    cmdconfirmupdateL->setHidden(true);
	    cmdundoL_U->setHidden(true);
	    cmdredoL_U->setHidden(true);
	    qryresviewerL->setConfirmUpdate(false);
	    //activate/inactivate some buttons[STOP]
	}
	break;
    }
    
    if(viewCur)
    {
	if(id == 0)
	    viewCur->setMode(0);
	else if(id == 1)
	{
	    if(viewCur == viewCurR)
	    {
		cmdFindReplaceR->setHidden(false);
		qryresviewerR->setConfirmUpdate(true);
	    }
	    else if(viewCur == viewCurW)
	    {
		cmdFindReplaceW->setHidden(false);
		qryresviewerW->setConfirmUpdate(true);
	    }
	    else if(viewCur == viewCurF)
	    {
		cmdFindReplaceF->setHidden(false);
		qryresviewerF->setConfirmUpdate(true);
	    }
	    else if(viewCur == viewCurC)
	    {
		cmdFindReplaceC->setHidden(false);
		qryresviewerC->setConfirmUpdate(true);
	    }
	    else if(viewCur == viewCurL)
	    {
		cmdFindReplaceL->setHidden(false);
		qryresviewerL->setConfirmUpdate(true);
	    }
	    else
		QMessageBox::information(this,tr("Update action status"),tr("Updatation possible only for \n\'Wildcard\',\'Field Search\' \n \'Comparison\' and \'List search\' query"));
	    viewCur->setMode(2);
	}
	else if(id == 2)
	    performDeleteOperation();
	else if(id == 3)
	    save_OR_commit_changes(logTracker);
    }
    viewCur = 0;
}

void frmDataAnalyzer::performDeleteOperation()
{
    switch(tabId)
    {
    case 0:
	if(qryresviewerR && validDelAction[0])
	    prepareQueryANDdeleteRec();
	else
	    QMessageBox::information(this,tr("Delete action status"),tr("Deletion possible only for \n\'Row selection\', \'Wildcard\', \'Field Search\'[when all fields are selected]  and \'Comparison\' query"));
	break;
    case 1:
	if(qryresviewerW && validDelAction[1])
	    prepareQueryANDdeleteRec();
	else
	    QMessageBox::information(this,tr("Delete action status"),tr("Deletion possible only for \n\'Row selection\', \'Wildcard\', \'Field Search\'[when all fields are selected]  and \'Comparison\' query"));
	break;
    case 2:
	if(qryresviewerF && validDelAction[2])
	    prepareQueryANDdeleteRec();
	else
	    QMessageBox::information(this,tr("Delete action status"),tr("Deletion possible only for \n\'Row selection\', \'Wildcard\', \'Field Search\'[when all fields are selected]  and \'Comparison\' query"));
	break;
    case 3:
	if(qryresviewerC && validDelAction[3])
	    prepareQueryANDdeleteRec();
	else
	    QMessageBox::information(this,tr("Delete action status"),tr("Deletion possible only for \n\'Row selection\', \'Wildcard\', \'Field Search\'[when all fields are selected]  and \'Comparison\' query"));
	break;
    case 4:
	if(qryresviewerA && validDelAction[4])
	    prepareQueryANDdeleteRec();
	else
	    QMessageBox::information(this,tr("Delete action status"),tr("Deletion possible only for \n\'Row selection\', \'Wildcard\', \'Field Search\'[when all fields are selected]  and \'Comparison\' query"));
	break;
    case 5:
	if(qryresviewerL && validDelAction[5])
	    prepareQueryANDdeleteRec();
	else
	    QMessageBox::information(this,tr("Delete action status"),tr("Deletion possible only for \n\'Row selection\', \'Wildcard\', \'Field Search\'[when all fields are selected]  and \'Comparison\' query"));
	break;
    }
}

bool frmDataAnalyzer::prepareQueryANDdeleteRec()
{
    static int delIndx = 1;
    QString qcommand;
    int status = -1;
    int numRecs = 0;
    QDataTable *qryresviewer = 0;
    QString backUpTab = "",query = "",command = "";
    QListView *logTracker = 0;
    int redoCount = 0,undoCount = 0;
    QToolButton *cmdundo = 0;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	backUpTab = tr("deleteR");
	query = queryR;
	command = commandR;
	logTracker = logTrackerR;
	redoCount = redoCountR;
	undoCount = undoCountR;
	cmdundo = cmdundoR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	backUpTab = tr("deleteW");
	query = queryW;
	command = commandW;
	logTracker = logTrackerW;
	redoCount = redoCountW;
	undoCount = undoCountW;
	cmdundo = cmdundoW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	backUpTab = tr("deleteF");
	query = queryF;
	command = commandF;
	logTracker = logTrackerF;
	redoCount = redoCountF;
	undoCount = undoCountF;
	cmdundo = cmdundoF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	backUpTab = tr("deleteC");
	query = queryC;
	command = commandC;
	logTracker = logTrackerC;
	redoCount = redoCountC;
	undoCount = undoCountC;
	cmdundo = cmdundoC;
	break;
    case 4:
	break;
    case 5:
	qryresviewer = qryresviewerL;
	backUpTab = tr("deleteL");
	query = queryL;
	command = commandL;
	logTracker = logTrackerL;
	redoCount = redoCountL;
	undoCount = undoCountL;
	cmdundo = cmdundoL;
	break;
    }
    
    numRecs = qryresviewer->numRows();
    qcommand = "mysql " + sqlDB->databaseName() + " -e \"create table " + backUpTab + QString::number(delIndx) + " as(select * from " + qryresviewer->caption() + ")\";"; //Create & populate backup table
    //cout << qcommand << endl;
    status = system(qcommand);
    if(status > -1)
    {
	status = -1;
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"delete from " + (qryresviewer->caption()).section("_",0,int((qryresviewer->caption()).contains("_") - 1)) + query + "\";";
	//cout << qcommand << endl;
	status = system(qcommand); //Delete from Original table
	if(status > -1)
	{
	    status = -1;
	    qcommand = "mysql " + sqlDB->databaseName() + " -e \"delete from " + qryresviewer->caption() + query + "\";";
	    //cout << qcommand << endl;
	    status = system(qcommand); //Delete from view
	    if(status > -1)
	    {
		qryresviewer->refresh(QDataTable::RefreshAll);
		QListViewItem *lvi = new QListViewItem(logTracker,tr(backUpTab + QString::number(delIndx)),query,command,tr("u"));
		lvi->setPixmap(0,QImage(tr(workDir + "/images/undocol.png")));
		logTracker->insertItem(lvi);
		lvi = 0;
		logTracker->hideColumn(1);
		logTracker->hideColumn(2);
		logTracker->hideColumn(3);
		QMessageBox::information(this,tr("Delete action status"),QString ::number(numRecs) + tr(" records are deleted successfully"));
		++delIndx;
		moveNot_EnableListItems_to_End(logTracker);
		++undoCount;
		if(cmdundo->isHidden())
		    cmdundo->setHidden(false);
	    }
	}
    }
    qryresviewer = 0;
    backUpTab = "";
    query = "";
    command = "";
    cmdundo = 0;
    switch(tabId)
    {
    case 0:
	logTrackerR = logTracker;
	undoCountR = undoCount;
	redoCountR = redoCount;
	break;
    case 1:
	logTrackerW = logTracker;
	undoCountW = undoCount;
	redoCountW = redoCount;
	break;
    case 2:
	logTrackerF = logTracker;
	undoCountF = undoCount;
	redoCountF = redoCount;
	break;
    case 3:
	logTrackerC = logTracker;
	undoCountC = undoCount;
	redoCountC = redoCount;
	break;
    case 4:
	break;
    case 5:
	logTrackerL = logTracker;
	undoCountL = undoCount;
	redoCountL = redoCount;
	break;
    }
    logTracker = 0;
}

void frmDataAnalyzer::moveNot_EnableListItems_to_End(QListView *itm)
{
    QListViewItem *tempItem = 0,*firstItem = itm->lastItem();
    for(register int i = 0; i < itm->childCount(); ++i)
    {
	if(firstItem->text(3) == tr("r"))
	{
	    tempItem = firstItem;
	    firstItem = firstItem->itemAbove();
	    itm->takeItem(tempItem);
	    itm->insertItem(tempItem);
	}
	else
	    firstItem = firstItem->itemAbove();
    }
    tempItem = 0;
    firstItem = 0;
}

void frmDataAnalyzer::save_OR_commit_changes(QListView *logTracker)
{
    if(logTracker->childCount())
    {	
	switch(tabId)
	{
	case 0:
	    dropViewR();
	    destroyGarbageTablesIfAny(logTrackerR);
	    break;
	case 1:
	    dropViewW();
	    destroyGarbageTablesIfAny(logTrackerW);
	    break;
	case 2:
	    dropViewF();
	    destroyGarbageTablesIfAny(logTrackerF);
	    break;
	case 3:
	    dropViewC();
	    destroyGarbageTablesIfAny(logTrackerC);
	    break;
	case 5:
	    dropViewL();
	    destroyGarbageTablesIfAny(logTrackerL);
	}
    }
    else
	QMessageBox::information(this,tr("Save action status"),tr("No changes have been made on current table data.\n\tNothing to save!"));
}

void frmDataAnalyzer::cmdredo_clicked()
{
    QString qcommand = "";
    int status = -1;
    QListView *logTracker = 0;
    QListViewItem *firstItem = 0;
    int undoCount = 0, redoCount = 0;
    QToolButton *cmdredo = 0,*cmdundo = 0;
    QDataTable *qryresviewer = 0;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	logTracker = logTrackerR;
	redoCount = redoCountR;
	undoCount = undoCountR;
	cmdredo = cmdredoR;
	cmdundo = cmdundoR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	logTracker = logTrackerW;
	redoCount = redoCountW;
	undoCount = undoCountW;
	cmdredo = cmdredoW;
	cmdundo = cmdundoW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	logTracker = logTrackerF;
	redoCount = redoCountF;
	undoCount = undoCountF;
	cmdredo = cmdredoF;
	cmdundo = cmdundoF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	logTracker = logTrackerC;
	redoCount = redoCountC;
	undoCount = undoCountC;
	cmdredo = cmdredoC;
	cmdundo = cmdundoC;
	break;
    case 4:
	break;
    case 5:
	qryresviewer = qryresviewerL;
	logTracker = logTrackerL;
	redoCount = redoCountL;
	undoCount = undoCountL;
	cmdredo = cmdredoL;
	cmdundo = cmdundoL;
	break;
    }
    
    firstItem = logTracker->lastItem();
    for(register int i = 0; i < logTracker->childCount(); ++i)
    {
	if(firstItem->text(3) == tr("r"))
	    break;
	firstItem = firstItem->itemAbove();
    }
    firstItem->setText(3,tr("u"));
    firstItem->setPixmap(0,QImage(tr(workDir + "/images/undocol.png")));
    
    if((firstItem->text(0)).contains(tr("update")) && performTableUpdate(firstItem->text(2))) //Update
    {
	--redoCount;
	if(redoCount == 0)
	    cmdredo->setHidden(true);
	++undoCount;
	if(cmdundo->isHidden())
	    cmdundo->setHidden(false);
	logTracker->hideColumn(1);
	logTracker->hideColumn(2);
	logTracker->hideColumn(3);
    }
    else if((firstItem->text(0)).contains(tr("delete"))) //Delete
    {
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + "create table " + firstItem->text(0) + " as(select * from " + (qryresviewer->caption()).section("_",0,int((qryresviewer->caption()).contains("_") - 1)) + firstItem->text(1) + ")\";";
	//cout << qcommand << endl;
	status = system(qcommand);
	if(status > -1)
	{
	    status = -1;
	    qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + "delete from " + (qryresviewer->caption()).section("_",0,int((qryresviewer->caption()).contains("_") - 1)) + firstItem->text(1) + "\";";
	    //cout << qcommand << endl;
	    status = system(qcommand); //Delete from Original table
	    if(status > -1)
	    {
		status = -1;
		qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + "delete from " + qryresviewer->caption() + "\";";
		//cout << qcommand << endl;
		status = system(qcommand); //Delete from view
		if(status > -1)
		{
		    qryresviewer->refresh(QDataTable::RefreshAll);
		    //QMessageBox::information(this,tr("Delete Action Status"),QString ::number(numRecs) + tr(" records are deleted successfully"));
		    --redoCount;
		    if(redoCount == 0)
			cmdredo->setHidden(true);
		    ++undoCount;
		    if(cmdundo->isHidden())
			cmdundo->setHidden(false);
		    logTracker->hideColumn(1);
		    logTracker->hideColumn(2);
		    logTracker->hideColumn(3);
		}
	    }
	}
    }
    firstItem = 0;
    qryresviewer = 0;
    cmdredo = 0;
    cmdundo = 0;
    switch(tabId)
    {
    case 0:
	logTrackerR = logTracker;
	undoCountR = undoCount;
	redoCountR = redoCount;
	break;
    case 1:
	logTrackerW = logTracker;
	undoCountW = undoCount;
	redoCountW = redoCount;
	break;
    case 2:
	logTrackerF = logTracker;
	undoCountF = undoCount;
	redoCountF = redoCount;
	break;
    case 3:
	logTrackerC = logTracker;
	undoCountC = undoCount;
	redoCountC = redoCount;
	break;
    case 4:
	break;
    case 5:
	logTrackerL = logTracker;
	undoCountL = undoCount;
	redoCountL = redoCount;
	break;
    }
    logTracker = 0;
}

void frmDataAnalyzer::cmdundo_clicked()
{
    QListView *logTracker = 0;
    QListViewItem *lastItem = 0;
    int undoCount = 0, redoCount = 0;
    QToolButton *cmdundo = 0,*cmdredo = 0;
    
    switch(tabId)
    {
    case 0:
	logTracker = logTrackerR;
	undoCount = undoCountR;
	redoCount = redoCountR;
	cmdundo = cmdundoR;
	cmdredo = cmdredoR;
	break;
    case 1:
	logTracker = logTrackerW;
	undoCount = undoCountW;
	redoCount = redoCountW;
	cmdundo = cmdundoW;
	cmdredo = cmdredoW;
	break;
    case 2:
	logTracker = logTrackerF;
	undoCount = undoCountF;
	redoCount = redoCountF;
	cmdundo = cmdundoF;
	cmdredo = cmdredoF;
	break;
    case 3:
	logTracker = logTrackerC;
	undoCount = undoCountC;
	redoCount = redoCountC;
	cmdundo = cmdundoC;
	cmdredo = cmdredoC;
	break;
    case 4:
	break;
    case 5:
	logTracker = logTrackerL;
	undoCount = undoCountL;
	redoCount = redoCountL;
	cmdundo = cmdundoL;
	cmdredo = cmdredoL;
	break;
    }
    lastItem = logTracker->firstChild();
    for(register int i = 0; i < logTracker->childCount(); ++i)
    {
	if(lastItem->text(3) == tr("u"))
	    break;
	lastItem = lastItem->itemBelow();
    }
    
    if(((lastItem->text(0)).contains(tr("update")) && performTableUpdate(lastItem->text(0))) || ((lastItem->text(0)).contains(tr("delete")) && performTableMerge(lastItem)))
    {
	lastItem->setText(3,tr("r"));
	lastItem->setPixmap(0,QImage(tr(workDir + "/images/redocol.png")));
	--undoCount;
	if(undoCount == 0)
	    cmdundo->setHidden(true);
	++redoCount;
	if(cmdredo->isHidden())
	    cmdredo->setHidden(false);
	logTracker->hideColumn(1);
	logTracker->hideColumn(2);
	logTracker->hideColumn(3);
	lastItem = 0;
    }
    
    cmdundo = 0;
    cmdredo = 0;
    switch(tabId)
    {
    case 0:
	logTrackerR = logTracker;
	undoCountR = undoCount;
	redoCountR = redoCount;
	break;
    case 1:
	logTrackerW = logTracker;
	undoCountW = undoCount;
	redoCountW = redoCount;
	break;
    case 2:
	logTrackerF = logTracker;
	undoCountF = undoCount;
	redoCountF = redoCount;
	break;
    case 3:
	logTrackerC = logTracker;
	undoCountC = undoCount;
	redoCountC = redoCount;
	break;
    case 4:
	break;
    case 5:
	logTrackerL = logTracker;
	undoCountL = undoCount;
	redoCountL = redoCount;
	break;
    }
    logTracker = 0;
}

bool frmDataAnalyzer::performTableMerge(QListViewItem *lastItem)
{
    int status = -1;
    QString dataSource = "";
    QString qcommand = "mysql " + sqlDB->databaseName() + " -e \"insert into " + tableName + "(";
    QSqlSelectCursor cursor(tr("select * from " + tableName),sqlDB);
    for(register int i = 0; i < (int)cursor.count(); ++i)
    {
	qcommand += cursor.fieldName(i);
	if(i < (int)cursor.count() - 1)
	    qcommand += ",";
    }
    qcommand += ") select * from " + lastItem->text(0) + ";"  + "\";";
    status = system(qcommand); //Get back data to original table
    if(status > -1)
    {
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"drop table " + lastItem->text(0) + ";"  + "\";";
	status = system(qcommand); //drop the 'tableToMarge'
	switch(tabId)
	{
	case 0:
	    dropViewR();
	    commandR = lastItem->text(2);
	    dataSource = tableName;
	    showFilteredData(addPrimaryKey(viewR,createViewBasedOnUserQuery_R(1,"",""))); //rePopulate the view
	    break;
	case 1:
	    dropViewW();
	    commandW = lastItem->text(2);
	    dataSource = tableName;
	    showFilteredData(addPrimaryKey(viewW,createViewBasedOnUserQuery_W(1,"",""))); //rePopulate the view
	    break;
	case 2:
	    dropViewF();
	    commandF = lastItem->text(2);
	    if(rdresCarry12->isChecked())
	    {
		if(cmbCarry12->currentText() == "Wildcard")
		    dataSource = viewW;
		else if(cmbCarry12->currentText() == "Comparison")
		    dataSource = viewC;
		else if(cmbCarry12->currentText() == "Aggregation")
		    dataSource = viewA;
		else if(cmbCarry12->currentText() == "ListSearch")
		    dataSource = viewL;
	    }
	    else
		dataSource = tableName;
	    showFilteredData(addPrimaryKey(viewF,createViewBasedOnUserQuery_F(1,dataSource,""))); //rePopulate the view
	    break;
	case 3:
	    dropViewC();
	    commandC = lastItem->text(2);
	    if(rdresCarry13->isChecked())
	    {
		if(cmbCarry13->currentText() == "Wildcard")
		    dataSource = viewW;
		else if(cmbCarry13->currentText() == "Field")
		    dataSource = viewC;
		else if(cmbCarry13->currentText() == "Aggregation")
		    dataSource = viewA;
		else if(cmbCarry13->currentText() == "ListSearch")
		    dataSource = viewL;
	    }
	    else
		dataSource = tableName;
	    showFilteredData(addPrimaryKey(viewC,createViewBasedOnUserQuery_C(1,dataSource,""))); //rePopulate the view
	    break;
	case 4:
	    break;
	case 5:
	    dropViewL();
	    commandL = lastItem->text(2);
	    if(rdresCarry15->isChecked())
	    {
		if(cmbCarry15->currentText() == "Wildcard")
		    dataSource = viewL;
		else if(cmbCarry15->currentText() == "Comparison")
		    dataSource = viewL;
		else if(cmbCarry15->currentText() == "Aggregation")
		    dataSource = viewL;
	    }
	    else
		dataSource = tableName;
	    showFilteredData(addPrimaryKey(viewL,createViewBasedOnUserQuery_L(1,dataSource,""))); //rePopulate the view
	    break;
	}
    }
    if(status == -1)
	return false;
    else
	return true;
}

bool frmDataAnalyzer::performTableUpdate(QString backUpTab)
{
    int numCols = 0;
    bool changeFlag = false;
    //~~~~~~~This part update original table by 'qryresviewerW' table [START]
    //Use cursor to update value[START]
    QSqlCursor curs(backUpTab,true,sqlDB);
    //tabCursor ~ 'QSqlCursor on the original table'
    tabCursor->setMode(2); //Update mode
    QSqlIndex pk = tabCursor->primaryIndex();
    curs.select(); // We'll retrieve every record
    while(curs.next())
    {
	numCols = (int)curs.count();
	tabCursor->setValue(0,QVariant((curs.value(0)).toString()));
	if(tabCursor->select( pk, pk ))
	{
	    if(tabCursor->next())
	    {
		QSqlRecord *buffer  = tabCursor->primeUpdate();
		for(register int col = 1; col < numCols; ++col)
		{
		    if((buffer->value(curs.fieldName(col))).toString() != (curs.value(col)).toString())
		    {
			buffer->setValue(curs.fieldName(col),(curs.value(col)).toString());
			changeFlag = true;
		    }
		}
		
		if(changeFlag && !tabCursor->update())
		    cout << "\nSQL Update failure error\n";
		buffer = 0;
		changeFlag = false;
	    }
	}
    }
    tabCursor->setMode(0); //ReadOnly mode
    //Use cursor to update value[STOP]
    //~~~~~~~This part update original table by 'qryresviewerW' table [STOP]
    //Load updated data [in original table] in Table for viewing[START]
    QString view = "";
    
    switch(tabId)
    {
    case 0:
	dropViewR();
	view = viewR;
	break;
    case 1:
	dropViewW();
	view = viewW;
	break;
    case 2:
	dropViewF();
	view = viewF;
	break;
    case 3:
	dropViewC();
	view = viewC;
	break;
    case 4:
	break;
    case 5:
	dropViewL();
	view = viewL;
	break;
    }
    
    QString query = "create table " + view + " as (select * from " + backUpTab + ");";
    int status = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
    if(status > -1)
	showFilteredData(addPrimaryKey(view,true));
    else
	cout << "\nupdate query execution error in \'performTableUpdateF\' function\n";
    //Load updated data [in original table] in Table for viewing[STOP]
    return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~UPDATE~~~~~~~~~~ [START]
void frmDataAnalyzer::FindReplace_Clicked()
{
    QStringList viewcolList;
    QWorkspace *wSpace = 0;
    QDataTable *qryresviewer = 0;
    QPushButton *cmdFindReplace = 0,*cmdconfirmupdate = 0, *cmdgenerate = 0;
    QComboBox *cmbaction = 0;
    QToolButton *cmdundo = 0, *cmdredo = 0;
    QComboBox *cmbSave = 0;

    switch(tabId)
    {
    case 0:
	wSpace = wSpaceR;
	qryresviewer = qryresviewerR;
	cmdFindReplace = cmdFindReplaceR;
	cmdconfirmupdate = cmdconfirmupdateR;
	cmbaction = cmbactionR;
	cmdundo = cmdundoR;
	cmdredo = cmdredoR;
	cmdgenerate = cmdgenerateR;
	cmbSave = cmbSaveR;
	bgr0->setEnabled(false);
	break;
    case 1:
	wSpace = wSpaceW;
	qryresviewer = qryresviewerW;
	cmdFindReplace = cmdFindReplaceW;
	cmdconfirmupdate = cmdconfirmupdateW;
	cmbaction = cmbactionW;
	cmdundo = cmdundoW;
	cmdredo = cmdredoW;
	cmdgenerate = cmdgenerateW;
	cmbSave = cmbSaveW;
	bgr1->setEnabled(false);
	break;
    case 2:
	wSpace = wSpaceF;
	qryresviewer = qryresviewerF;
	cmdFindReplace = cmdFindReplaceF;
	cmdconfirmupdate = cmdconfirmupdateF;
	cmbaction = cmbactionF;
	cmdundo = cmdundoF;
	cmdredo = cmdredoF;
	cmdgenerate = cmdgenerateF;
	cmbSave = cmbSaveF;
	bgr2->setEnabled(false);
	break;
    case 3:
	wSpace = wSpaceC;
	qryresviewer = qryresviewerC;
	cmdFindReplace = cmdFindReplaceC;
	cmdconfirmupdate = cmdconfirmupdateC;
	cmbaction = cmbactionC;
	cmdundo = cmdundoC;
	cmdredo = cmdredoC;
	cmdgenerate = cmdgenerateC;
	cmbSave = cmbSaveC;
	bgr3->setEnabled(false);
	break;
    case 4:
	break;
    case 5:
	wSpace = wSpaceL;
	qryresviewer = qryresviewerL;
	cmdFindReplace = cmdFindReplaceL;
	cmdconfirmupdate = cmdconfirmupdateL;
	cmbaction = cmbactionL;
	cmdundo = cmdundoL;
	cmdredo = cmdredoL;
	cmdgenerate = cmdgenerateL;
	cmbSave = cmbSaveL;
	bgr5->setEnabled(false);
	break;
    }
    
    cmdgenerate->setEnabled(false);
    cmbSave->setEnabled(false);
    findreplace = new frmfindreplace(wSpace,tr("findandreplace"),WDestructiveClose);
    QSqlSelectCursor cursor(tr("select * from " + qryresviewer->caption()),sqlDB);
    for(register int i = 0; i < (int)cursor.count(); ++i)
	viewcolList.append(cursor.fieldName(i));
    connect(findreplace->cmdFind,SIGNAL(clicked()),this,SLOT(findStringInTable()));
    connect(findreplace->cmdFindAll,SIGNAL(clicked()),this,SLOT(findAllStringInTable()));
    connect(findreplace->cmdReplace,SIGNAL(clicked()),this,SLOT(replaceStringInTable()));
    connect(findreplace->cmdReplaceAll,SIGNAL(clicked()),this,SLOT(replaceStringInTableAll()));
    connect(findreplace->cmdClose,SIGNAL(clicked()),this,SLOT(activateFindReplace()));
    connect(findreplace->cmdClose,SIGNAL(clicked()),this,SLOT(confirmupdate_Clicked()));
    findreplace->initSettings(viewcolList,qryresviewer->numRows());
    findreplace->show();
    cmdFindReplace->setHidden(true);
    cmdconfirmupdate->setGeometry(cmdFindReplace->x(),cmdFindReplace->y(),cmdconfirmupdate->width(),cmdconfirmupdate->height());
    cmdconfirmupdate->setHidden(false);
    cmbaction->setEnabled(false);
    if(!cmdundo->isHidden())
	cmdundo->setEnabled(false);
    if(!cmdredo->isHidden())
	cmdredo->setEnabled(false);
    
    wSpace = 0;
    qryresviewer = 0;
    cmdFindReplace = 0;
    cmdconfirmupdate = 0;
    cmdgenerate = 0;
    cmbSave = 0;
}

void frmDataAnalyzer::activateFindReplace()
{
    findreplaceFlag = true;
    switch(tabId)
    {
    case 0:
	cmdFindReplaceR->setHidden(false);
	cmdconfirmupdateR->setHidden(true);
	break;
    case 1:
	cmdFindReplaceW->setHidden(false);
	cmdconfirmupdateW->setHidden(true);
	break;
    case 2:
	cmdFindReplaceF->setHidden(false);
	cmdconfirmupdateF->setHidden(true);
	break;
    case 3:
	cmdFindReplaceC->setHidden(false);
	cmdconfirmupdateC->setHidden(true);
	break;
    case 4:
	break;
    case 5:
	cmdFindReplaceL->setHidden(false);
	cmdconfirmupdateL->setHidden(true);
	break;
    }
}

void frmDataAnalyzer::findStringInTable()
{
    bool selectionOK = false, stop_replaceAllFlag = false;
    static int row = 0;
    static int col = -1;
    QString tempStr = "";
    static QString strToSearch = "*#$@";
    QDataTable *qryresviewer = 0;

    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	break;
    case 4:
	break;
    case 5:
	qryresviewer = qryresviewerL;
	break;
    }
    
    setCursor(WaitCursor);
    qryresviewer->clearSelection();
    if(findreplace->chkmatchcase->isChecked()) //Weather match case
	tempStr = findreplace->cmbsearchfor->currentText();
    else
	tempStr = (findreplace->cmbsearchfor->currentText()).upper();
    if(tempStr != strToSearch)
    {
	strToSearch = tempStr;
	row = 0; col = -1;
    }
    
    if(findreplace->chkbeg->isChecked()) //Weather search from Begining
	row = 0, col = -1;
    
    if(findreplace->rdall->isChecked())
    {
	if(findreplace->rdforward->isChecked()) //forward search
	{
	    if(row == qryresviewer->numRows() - 1 && col == qryresviewer->numCols() - 1)
		row = 0;
	    else if(col == qryresviewer->numCols() - 1)
		++row;
	    col = (col == (qryresviewer->numCols() - 1) ? 0 : col + 1);
	    for(;row < qryresviewer->numRows(); ++row)
	    {
		for(;col < qryresviewer->numCols(); ++col)
		{
		    //Stop point checking for 'Replace All' mode [START]
		    if(replaceAllFlag == true && startRow_replaceAll == row && startCol_replaceAll == col)
		    {
			stop_replaceAllFlag = true;
			break;
		    }
		    //Stop point checking for 'Replace All' mode [STOP]
		    if(findreplace->chkentireword->isChecked()) //Weather match whole word
		    {
			if(findreplace->chkmatchcase->isChecked()) //Weather match case
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
			    {
				qryresviewer->setCurrentCell(row,col);
				qryresviewer->selectCells(row,col,row,col);
				qryresviewer->ensureCellVisible(row,col);
				selectionOK = true;
				break;
			    }
			}
			else
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).upper() == strToSearch))
			    {
				qryresviewer->setCurrentCell(row,col);
				qryresviewer->selectCells(row,col,row,col);
				qryresviewer->ensureCellVisible(row,col);
				selectionOK = true;
				break;
			    }
			}
		    }
		    else
		    {
			if(findreplace->chkmatchcase->isChecked()) //Weather match case
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
			    {
				qryresviewer->setCurrentCell(row,col);
				qryresviewer->selectCells(row,col,row,col);
				qryresviewer->ensureCellVisible(row,col);
				selectionOK = true;
				break;
			    }
			}
			else
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
			    {
				qryresviewer->setCurrentCell(row,col);
				qryresviewer->selectCells(row,col,row,col);
				qryresviewer->ensureCellVisible(row,col);
				selectionOK = true;
				break;
			    }
			}
		    }
		}
		if(stop_replaceAllFlag)
		    break;
		col = (col  == qryresviewer->numCols() ? 0 : col);
		if(selectionOK)
		    break;
	    }
	}
	else //backward search
	{	    	    
	    col = (col <= 0 ? (findreplace->chkbeg->isChecked() ? 0 : qryresviewer->numCols() - 1) : col - 1);
	    for(;row >= 0; --row)
	    {
		for(;col >= 0; --col)
		{
		    //Stop point checking for 'Replace All' mode [START]
		    if(replaceAllFlag == true && startRow_replaceAll == row && startCol_replaceAll == col)
		    {
			stop_replaceAllFlag = true;
			break;
		    }
		    //Stop point checking for 'Replace All' mode [STOP]
		    if(findreplace->chkentireword->isChecked()) //Weather match whole word
		    {
			if(findreplace->chkmatchcase->isChecked()) //Weather match case
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
			    {
				qryresviewer->setCurrentCell(row,col);
				qryresviewer->selectCells(row,col,row,col);
				qryresviewer->ensureCellVisible(row,col);
				selectionOK = true;
				break;
			    }
			}
			else
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).upper() == strToSearch))
			    {
				qryresviewer->setCurrentCell(row,col);
				qryresviewer->selectCells(row,col,row,col);
				qryresviewer->ensureCellVisible(row,col);
				selectionOK = true;
				break;
			    }
			}
		    }
		    else
		    {
			if(findreplace->chkmatchcase->isChecked()) //Weather match case
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
			    {
				qryresviewer->setCurrentCell(row,col);
				qryresviewer->selectCells(row,col,row,col);
				qryresviewer->ensureCellVisible(row,col);
				selectionOK = true;
				break;
			    }
			}
			else
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
			    {
				qryresviewer->setCurrentCell(row,col);
				qryresviewer->selectCells(row,col,row,col);
				qryresviewer->ensureCellVisible(row,col);
				selectionOK = true;
				break;
			    }
			}
		    }
		}
		if(stop_replaceAllFlag)
		    break;
		col = (col <= 0 ? qryresviewer->numCols() - 1 : col);
		if(selectionOK)
		    break;
	    }
	    if(row <= 0 && col == qryresviewer->numCols() - 1)
		row = qryresviewer->numRows() - 1;
	}
    }
    else if(findreplace->rdcolwise->isChecked())
    {
	col = findreplace->chkcolwise->currentItem();
	if(findreplace->rdforward->isChecked()) //forward search
	{
	    for(;row < qryresviewer->numRows(); ++row)
	    {
		//Stop point checking for 'Replace All' mode [START]
		if(replaceAllFlag == true && startRow_replaceAll == row && startCol_replaceAll == col)
		{
		    stop_replaceAllFlag = true;
		    break;
		}
		//Stop point checking for 'Replace All' mode [STOP]
		if(findreplace->chkentireword->isChecked()) //Weather match whole word
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col).upper()) == strToSearch))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		}
		else
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		}
	    }
	    row = (row + 1 >= qryresviewer->numRows() ? 0 : row + 1);
	}
	else //backward search
	{
	    for(;row >= 0; --row)
	    {
		//Stop point checking for 'Replace All' mode [START]
		if(replaceAllFlag == true && startRow_replaceAll == row && startCol_replaceAll == col)
		{
		    stop_replaceAllFlag = true;
		    break;
		}
		//Stop point checking for 'Replace All' mode [STOP]
		if(findreplace->chkentireword->isChecked()) //Weather match whole word
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).upper() == strToSearch))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		}
		else
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		}
	    }
	    row = (row <= 0 ? qryresviewer->numRows() - 1 : row - 1);
	}
    }
    else if(findreplace->rdrowwise->isChecked())
    {
	row = findreplace->spinrowwise->value() - 1;
	if(findreplace->rdforward->isChecked()) //forward search
	{
	    ++col;
	    for(;col < qryresviewer->numCols(); ++col)
	    {
		//Stop point checking for 'Replace All' mode [START]
		if(replaceAllFlag == true && startRow_replaceAll == row && startCol_replaceAll == col)
		{
		    stop_replaceAllFlag = true;
		    break;
		}
		//Stop point checking for 'Replace All' mode [STOP]
		if(findreplace->chkentireword->isChecked()) //Weather match whole word
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).upper() == strToSearch))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		}
		else
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		}
	    }
	    col = (col + 1 >= qryresviewer->numCols() ? -1 : col);
	}
	else //backward search
	{
	    col = (col <= 0 ? (findreplace->chkbeg->isChecked() ? 0 : qryresviewer->numCols() - 1) : col - 1);
	    for(;col >= 0; --col)
	    {
		//Stop point checking for 'Replace All' mode [START]
		if(replaceAllFlag == true && startRow_replaceAll == row && startCol_replaceAll == col)
		{
		    stop_replaceAllFlag = true;
		    break;
		}
		//Stop point checking for 'Replace All' mode [STOP]
		if(findreplace->chkentireword->isChecked()) //Weather match whole word
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).upper() == strToSearch))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		}
		else
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
			{
			    qryresviewer->setCurrentCell(row,col);
			    qryresviewer->selectCells(row,col,row,col);
			    qryresviewer->ensureCellVisible(row,col);
			    selectionOK = true;
			    break;
			}
		    }
		}
	    }
	    col = (col <= 0 ? qryresviewer->numCols() - 1 : col);
	}
    }
    
    if(stop_replaceAllFlag)
	replaceFlag = false;
    else if(selectionOK)
    {
	if(!replaceAllFlag)
	{
	    findreplace->cmdReplace->setEnabled(true);
	    findreplace->cmdReplaceAll->setEnabled(true);
	}
	replaceFlag = true;
    }
    setCursor(ArrowCursor);
    selectionOK = false;
    if(findreplace->chkbeg->isChecked()) //Weather search from Begining
	findreplace->chkbeg->setChecked(false);
    qryresviewer = 0;
}

void frmDataAnalyzer::findAllStringInTable()
{
    bool selectionOK = false, stop_replaceAllFlag = false;
    static int row = 0;
    static int col = -1;
    QString tempStr = "";
    static QString strToSearch = "*#$@";
    QDataTable *qryresviewer = 0;
    int matchFound = 0;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	break;
    case 4:
	qryresviewer = qryresviewerA;
	break;
    case 5:
	qryresviewer = qryresviewerL;
	break;
    }
    
    findreplace->cmdFind->setEnabled(false);
    findreplace->cmdFindAll->setEnabled(false);
    findreplace->cmdReplace->setEnabled(false);
    findreplace->cmdReplaceAll->setEnabled(false);
    qryresviewer->clearSelection();
    
    setCursor(WaitCursor);
    if(findreplace->chkmatchcase->isChecked()) //Weather match case
	tempStr = findreplace->cmbsearchfor->currentText();
    else
	tempStr = (findreplace->cmbsearchfor->currentText()).upper();
    
    if(tempStr != strToSearch)
    {
	strToSearch = tempStr;
	row = 0; col = -1;
    }
    
    if(findreplace->rdall->isChecked())
    {
	row = 0, col = -1; //Search all from Begining
	if(findreplace->rdforward->isChecked()) //forward search
	{
	    if(row == qryresviewer->numRows() - 1 && col == qryresviewer->numCols() - 1)
		row = 0;
	    else if(col == qryresviewer->numCols() - 1)
		++row;
	    col = (col == (qryresviewer->numCols() - 1) ? 0 : col + 1);
	    for(;row < qryresviewer->numRows(); ++row)
	    {
		for(;col < qryresviewer->numCols(); ++col)
		{
		    if(findreplace->chkentireword->isChecked()) //Weather match whole word
		    {
			if(findreplace->chkmatchcase->isChecked()) //Weather match case
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
				matchFound++;
			}
			else
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).upper() == strToSearch))
				matchFound++;
			}
		    }
		    else
		    {
			if(findreplace->chkmatchcase->isChecked()) //Weather match case
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
				matchFound++;
			}
			else
			{
			    if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
				matchFound++;
			}
		    }
		}
		col = (col  == qryresviewer->numCols() ? 0 : col);
	    }
	}
    }
    else if(findreplace->rdcolwise->isChecked())
    {
	row = 0;
	col = findreplace->chkcolwise->currentItem();
	if(findreplace->rdforward->isChecked()) //forward search
	{
	    for(;row < qryresviewer->numRows(); ++row)
	    {
		if(findreplace->chkentireword->isChecked()) //Weather match whole word
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
			    matchFound++;
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col).upper()) == strToSearch))
			    matchFound++;
		    }
		}
		else
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
			    matchFound++;
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
			    matchFound++;
		    }
		}
	    }
	    row = (row + 1 >= qryresviewer->numRows() ? 0 : row + 1);
	}
    }
    else if(findreplace->rdrowwise->isChecked())
    {
	row = findreplace->spinrowwise->value() - 1;
	col = -1;
	if(findreplace->rdforward->isChecked()) //forward search
	{
	    ++col;
	    for(;col < qryresviewer->numCols(); ++col)
	    {
		if(findreplace->chkentireword->isChecked()) //Weather match whole word
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && qryresviewer->text(row,col) == strToSearch))
			    matchFound++;
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).upper() == strToSearch))
			    matchFound++;
		    }
		}
		else
		{
		    if(findreplace->chkmatchcase->isChecked()) //Weather match case
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch)))
			    matchFound++;
		    }
		    else
		    {
			if((qryresviewer->text(row,col) == "" && strToSearch == "") || (qryresviewer->text(row,col) != "" && strToSearch != "" && (qryresviewer->text(row,col)).contains(strToSearch,false)))
			    matchFound++;
		    }
		}
	    }
	    col = (col + 1 >= qryresviewer->numCols() ? -1 : col);
	}
    }
    
    findreplace->cmdFind->setEnabled(true);
    findreplace->cmdFindAll->setEnabled(true);
    qryresviewer = 0;
    setCursor(ArrowCursor);
    if(matchFound)
	QMessageBox::information(this,tr("Search result"),tr("Match found: ") + QString ::number(matchFound));
    else
	QMessageBox::information(this,tr("Search result"),tr("Match found: ") + QString ::number(matchFound));
}

void frmDataAnalyzer::replaceStringInTable()
{
    QDataTable *qryresviewer = 0;
    QToolButton *cmdundo = 0;
    UPDATESTK *u_head = 0,*u_undo = 0,*u_redo = 0;
    bool changeFlag = false;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	cmdundo = cmdundoR_U;
	u_head = u_headR;
	u_undo = u_undoR;
	u_redo = u_redoR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	cmdundo = cmdundoW_U;
	u_head = u_headW;
	u_undo = u_undoW;
	u_redo = u_redoW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	cmdundo = cmdundoF_U;
	u_head = u_headF;
	u_undo = u_undoF;
	u_redo = u_redoF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	cmdundo = cmdundoC_U;
	u_head = u_headC;
	u_undo = u_undoC;
	u_redo = u_redoC;
	break;
    case 4:
	break;
    case 5:
	qryresviewer = qryresviewerL;
	cmdundo = cmdundoL_U;
	u_head = u_headL;
	u_undo = u_undoL;
	u_redo = u_redoL;
	break;
    }
    
    int row = qryresviewer->currentRow();
    int col = qryresviewer->currentColumn();
    
    if(findreplace->cmbreplacewith->currentText() != qryresviewer->text(row,col))
    {
	//Take backup of the data (to remove)  in STACK [START]
	if(u_head == 0)
	{
	    u_head = new UPDATESTK[1];
	    u_head->row = row; u_head->col = col;
	    u_head->value = qryresviewer->text(row,col);
	    if(replaceAllFlag) //Indicate 'Replace All' mode
		u_head->automode = automodeCounter;
	    u_undo = u_head;
	}
	else
	{
	    UPDATESTK *u_node = new UPDATESTK[1];
	    u_node->row = row; u_node->col = col;
	    u_node->value = qryresviewer->text(row,col);
	    if(replaceAllFlag) //Indicate 'Replace All' mode
		u_node->automode = automodeCounter;
	    if(u_undo)
		u_undo->next = u_node;
	    else
		u_head = u_node;
	    u_node->prev = u_undo;u_node->next = u_redo;
	    if(u_redo)
		u_redo->prev = u_node;
	    u_undo = u_node;
	}
	u_redo = u_undo->next;
	cmdundo->setHidden(false);
	//Take backup of the data (to remove)  in STACK [STOP]
	//Use cursor to update value[START]
	QSqlCursor *curs = qryresviewer->sqlCursor();
	QSqlIndex pk = curs->primaryIndex();
	curs->setValue(0,QVariant(qryresviewer->text(row,0)));
	if(curs->select( pk, pk ))
	{
	    if(curs->next())
	    {
		QSqlRecord *buffer  = curs->primeUpdate();
		if((buffer->value(col)).toString() != findreplace->cmbreplacewith->currentText())
		{
		    buffer->setValue(col,findreplace->cmbreplacewith->currentText());
		    changeFlag = true;
		}
		if(changeFlag && !curs->update())
		    cout << "\nSQL Update failure error\n";
		buffer = 0;
		changeFlag = false;
	    }
	}
	curs = 0;
	//Use cursor to update value[STOP]
	qryresviewer->refresh(QDataTable::RefreshAll);
	qryresviewer->selectCells(row,col,row,col);
	qryresviewer->setCurrentCell(row,col);
	replaceFlag = false;
	findStringInTable();
	if(!replaceFlag)
	{
	    findreplace->cmdReplace->setEnabled(false);
	    if(replaceAllFlag)
		++automodeCounter;
	}
    }
    cmdundo = 0;
    switch(tabId)
    {
    case 0:
	qryresviewerR = qryresviewer;
	u_headR = u_head;
	u_undoR = u_undo;
	u_redoR = u_redo;
	break;
    case 1:
	qryresviewerW = qryresviewer;
	u_headW = u_head;
	u_undoW = u_undo;
	u_redoW = u_redo;
	break;
    case 2:
	qryresviewerF = qryresviewer;
	u_headF = u_head;
	u_undoF = u_undo;
	u_redoF = u_redo;
	break;
    case 3:
	qryresviewerC = qryresviewer;
	u_headC = u_head;
	u_undoC = u_undo;
	u_redoC = u_redo;
	break;
    case 4:
	break;
    case 5:
	qryresviewerL = qryresviewer;
	u_headL = u_head;
	u_undoL = u_undo;
	u_redoL = u_redo;
	break;
    }
    qryresviewer = 0;
    u_head = 0;
    u_undo = 0;
    u_redo = 0;
}

void frmDataAnalyzer::replaceStringInTableAll()
{
    QDataTable *qryresviewer = 0;
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	break;
    case 4:
	break;
    case 5:
	qryresviewer = qryresviewerL;
	break;
    }
    
    //replaceFlag = true ~ 'a match found'
    replaceAllFlag = true; //true ~ replace all mode
    startRow_replaceAll = qryresviewer->currentRow();
    startCol_replaceAll = qryresviewer->currentColumn();
    while(replaceFlag)
    {
	replaceStringInTable();
    };
    replaceAllFlag = false;
    findreplace->cmdReplaceAll->setEnabled(false);
    qryresviewer = 0;
}

void frmDataAnalyzer::cmdredo_U_clicked()
{
    QString tempVal = "";
    bool loopStat = false;
    QComboBox *cmbaction = 0;
    QDataTable *qryresviewer = 0;
    QToolButton *cmdundo = 0,*cmdredo = 0;
    UPDATESTK *u_undo = 0,*u_redo = 0;
    
    switch(tabId)
    {
    case 0:
	cmbaction = cmbactionR;
	qryresviewer = qryresviewerR;
	cmdundo = cmdundoR_U;
	cmdredo = cmdredoR_U;
	u_undo = u_undoR;
	u_redo = u_redoR;
	break;
    case 1:
	cmbaction = cmbactionW;
	qryresviewer = qryresviewerW;
	cmdundo = cmdundoW_U;
	cmdredo = cmdredoW_U;
	u_undo = u_undoW;
	u_redo = u_redoW;
	break;
    case 2:
	cmbaction = cmbactionF;
	qryresviewer = qryresviewerF;
	cmdundo = cmdundoF_U;
	cmdredo = cmdredoF_U;
	u_undo = u_undoF;
	u_redo = u_redoF;
	break;
    case 3:
	cmbaction = cmbactionC;
	qryresviewer = qryresviewerC;
	cmdundo = cmdundoC_U;
	cmdredo = cmdredoC_U;
	u_undo = u_undoC;
	u_redo = u_redoC;
	break;
    case 4:
	break;
    case 5:
	cmbaction = cmbactionL;
	qryresviewer = qryresviewerL;
	cmdundo = cmdundoL_U;
	cmdredo = cmdredoL_U;
	u_undo = u_undoL;
	u_redo = u_redoL;
	break;
    }
    
    if(cmbaction->currentText() == tr("Update")) //Update
    {
	do
	{
	    UPDATESTK *node = u_redo;
	    tempVal = qryresviewer->text(node->row,node->col);
	    //Use cursor to update value[START]
	    QSqlCursor *curs = qryresviewer->sqlCursor();
	    QSqlIndex pk = curs->primaryIndex();
	    curs->setValue(0,QVariant(qryresviewer->text(node->row,0)));
	    if(curs->select( pk, pk ))
	    {
		if(curs->next())
		{
		    QSqlRecord *buffer  = curs->primeUpdate();
		    if((buffer->value(node->col)).toString() != node->value)
		    {
			buffer->setValue(node->col,node->value);
			if(!curs->update())
			    cout << "\nSQL Update failure error\n";
		    }
		    buffer = 0;
		}
	    }
	    curs = 0;
	    //Use cursor to update value[STOP]
	    qryresviewer->refresh(QDataTable::RefreshAll);
	    qryresviewer->selectCells(node->row,node->col,node->row,node->col);
	    qryresviewer->setCurrentCell(node->row,node->col);
	    node->value = tempVal; //restore previous value
	    //Change undo~redo position
	    u_undo = node;
	    u_redo = u_undo->next;
	    node = 0;
	    if(u_redo)
	    {
		if(u_redo->prev)
		    loopStat = (u_redo->automode == -1 ? false : (u_redo->automode == u_redo->prev->automode ? true : false));
		else
		    loopStat = false;
	    }
	    else
		loopStat = false;
	    //undo-redo button active/inactive
	    cmdundo->setHidden(false);
	    if(u_redo == 0)
		cmdredo->setHidden(true);
	}while(loopStat);
    }
    cmbaction = 0;
    cmdundo = 0;
    switch(tabId)
    {
    case 0:
	qryresviewerR = qryresviewer;
	u_undoR = u_undo;
	u_redoR = u_redo;
	break;	
    case 1:
	qryresviewerW = qryresviewer;
	u_undoW = u_undo;
	u_redoW = u_redo;
	break;
    case 2:
	qryresviewerF = qryresviewer;
	u_undoF = u_undo;
	u_redoF = u_redo;
	break;
    case 3:
	qryresviewerC = qryresviewer;
	u_undoC = u_undo;
	u_redoC = u_redo;
	break;
    case 4:
	break;
    case 5:
	qryresviewerL = qryresviewer;
	u_undoL = u_undo;
	u_redoL = u_redo;
	break;
    }
    qryresviewer = 0;
    u_undo = 0;
    u_redo = 0;
}

void frmDataAnalyzer::cmdundo_U_clicked()
{
    QString tempVal = "";
    bool loopStat = false;
    QComboBox *cmbaction = 0;
    QDataTable *qryresviewer = 0;
    QToolButton *cmdredo = 0,*cmdundo = 0;
    UPDATESTK *u_undo = 0,*u_redo = 0;
    
    switch(tabId)
    {
    case 0:
	cmbaction = cmbactionR;
	qryresviewer = qryresviewerR;
	cmdredo = cmdredoR_U;
	cmdundo = cmdundoR_U;
	u_undo = u_undoR;
	u_redo = u_redoR;
	break;
    case 1:
	cmbaction = cmbactionW;
	qryresviewer = qryresviewerW;
	cmdredo = cmdredoW_U;
	cmdundo = cmdundoW_U;
	u_undo = u_undoW;
	u_redo = u_redoW;
	break;
    case 2:
	cmbaction = cmbactionF;
	qryresviewer = qryresviewerF;
	cmdredo = cmdredoF_U;
	cmdundo = cmdundoF_U;
	u_undo = u_undoF;
	u_redo = u_redoF;
	break;
    case 3:
	cmbaction = cmbactionC;
	qryresviewer = qryresviewerC;
	cmdredo = cmdredoC_U;
	cmdundo = cmdundoC_U;
	u_undo = u_undoC;
	u_redo = u_redoC;
	break;
    case 4:
	break;
    case 5:
	cmbaction = cmbactionL;
	qryresviewer = qryresviewerL;
	cmdredo = cmdredoL_U;
	cmdundo = cmdundoL_U;
	u_undo = u_undoL;
	u_redo = u_redoL;
	break;
    }
    
    if(cmbaction->currentText() == tr("Update")) //Update
    {
	do
	{
	    UPDATESTK *node = u_undo;
	    tempVal = qryresviewer->text(node->row,node->col);
	    //Use cursor to update value[START]
	    QSqlCursor *curs = qryresviewer->sqlCursor();
	    QSqlIndex pk = curs->primaryIndex();
	    curs->setValue(0,QVariant(qryresviewer->text(node->row,0)));
	    if(curs->select( pk, pk ))
	    {
		if(curs->next())
		{
		    QSqlRecord *buffer  = curs->primeUpdate();
		    if((buffer->value(node->col)).toString() != node->value)
		    {
			buffer->setValue(node->col,node->value);
			if(!curs->update())
			    cout << "\nSQL Update failure error\n";
		    }
		    buffer = 0;
		}
	    }
	    curs = 0;
	    //Use cursor to update value[STOP]
	    qryresviewer->refresh(QDataTable::RefreshAll);
	    qryresviewer->selectCells(node->row,node->col,node->row,node->col);
	    qryresviewer->setCurrentCell(node->row,node->col);
	    node->value = tempVal; //restore next value
	    //Change undo~redo position
	    u_redo = node;
	    u_undo = u_redo->prev;
	    node = 0;
	    if(u_undo)
	    {
		if(u_undo->next)
		    loopStat = (u_undo->automode == -1 ? false : (u_undo->automode == u_undo->next->automode ? true : false));
		else
		    loopStat = false;
	    }
	    else
		loopStat = false;
	    //undo-redo button active/inactive
	    cmdredo->setHidden(false);
	    if(u_undo == 0)
		cmdundo->setHidden(true);
	}while(loopStat);
    }
    
    cmbaction = 0;
    cmdredo = 0;
    switch(tabId)
    {
    case 0:
	qryresviewerR = qryresviewer;
	u_undoR = u_undo;
	u_redoR = u_redo;
	break;
    case 1:
	qryresviewerW = qryresviewer;
	u_undoW = u_undo;
	u_redoW = u_redo;
	break;
    case 2:
	qryresviewerF = qryresviewer;
	u_undoF = u_undo;
	u_redoF = u_redo;
	break;
    case 3:
	qryresviewerC = qryresviewer;
	u_undoC = u_undo;
	u_redoC = u_redo;
	break;
    case 4:
	break;
    case 5:
	qryresviewerL = qryresviewer;
	u_undoL = u_undo;
	u_redoL = u_redo;
	break;
    }
    qryresviewer = 0;
    u_undo = 0;
    u_redo = 0;
}

void frmDataAnalyzer::confirmupdate_Clicked()
{
    QToolButton *cmdredo_U = 0,*cmdundo_U = 0,*cmdredo = 0,*cmdundo = 0;
    QComboBox *cmbSave = 0;
    QPushButton *cmdFindReplace = 0, *cmdgenerate = 0;
    QComboBox *cmbaction = 0;
    UPDATESTK *u_head = 0;
    
    switch(tabId)
    {
    case 0:
	cmdredo_U = cmdredoR_U;
	cmdundo_U = cmdundoR_U;
	cmdFindReplace = cmdFindReplaceR;
	cmdredo = cmdredoR;
	cmdundo = cmdundoR;
	cmbaction = cmbactionR;
	u_head = u_headR;
	cmdgenerate = cmdgenerateR;
	cmbSave = cmbSaveR;
	bgr0->setEnabled(true);
	break;
    case 1:
	cmdredo_U = cmdredoW_U;
	cmdundo_U = cmdundoW_U;
	cmdFindReplace = cmdFindReplaceW;
	cmdredo = cmdredoW;
	cmdundo = cmdundoW;
	cmbaction = cmbactionW;
	u_head = u_headW;
	cmdgenerate = cmdgenerateW;
	cmbSave = cmbSaveW;
	bgr1->setEnabled(true);
	break;
    case 2:
	cmdredo_U = cmdredoF_U;
	cmdundo_U = cmdundoF_U;
	cmdFindReplace = cmdFindReplaceF;
	cmdredo = cmdredoF;
	cmdundo = cmdundoF;
	cmbaction = cmbactionF;
	u_head = u_headF;
	cmdgenerate = cmdgenerateF;
	cmbSave = cmbSaveF;
	bgr2->setEnabled(true);
	break;
    case 3:
	cmdredo_U = cmdredoC_U;
	cmdundo_U = cmdundoC_U;
	cmdFindReplace = cmdFindReplaceC;
	cmdredo = cmdredoC;
	cmdundo = cmdundoC;
	cmbaction = cmbactionC;
	u_head = u_headC;
	cmdgenerate = cmdgenerateC;
	cmbSave = cmbSaveC;
	bgr3->setEnabled(true);
	break;
    case 4:
	break;
    case 5:
	cmdredo_U = cmdredoL_U;
	cmdundo_U = cmdundoL_U;
	cmdFindReplace = cmdFindReplaceL;
	cmdredo = cmdredoL;
	cmdundo = cmdundoL;
	cmbaction = cmbactionL;
	u_head = u_headL;
	cmdgenerate = cmdgenerateL;
	cmbSave = cmbSaveL;
	bgr5->setEnabled(true);
	break;
    }
    
    if(!QMessageBox::question(this, tr("Confirm update"),tr("Do you want to save current set of changes?"),tr("Yes"),tr("No"),QString::null,0,1))
    {
	automodeCounter = 0;
	cmdundo_U->setHidden(true);
	cmdredo_U->setHidden(true);
	if(u_head)
	{
	    performUpdateOperation();
	    deleteUpdateStack();
	}
	cmdFindReplace->setHidden(true);
	if(findreplaceFlag)
	{
	    cmbaction->setEnabled(true);
	    if(!cmdredo->isHidden())
		cmdredo->setEnabled(true);
	    if(!cmdundo->isHidden())
		cmdundo->setEnabled(true);
	    findreplaceFlag = false;
	}
    }
    else
    {
	if(findreplaceFlag)
	{
	    while(!cmdundo_U->isHidden())
	    {
		cmdundo_U_clicked();
	    };
	    automodeCounter = 0;
	    deleteUpdateStack();
	    cmdundo_U->setHidden(true);
	    cmdredo_U->setHidden(true);
	    cmdFindReplace->setHidden(true);
	    cmbaction->setEnabled(true);
	    if(!cmdredo->isHidden())
		cmdredo->setEnabled(true);
	    if(!cmdundo->isHidden())
		cmdundo->setEnabled(true);
	    findreplaceFlag = false;
	}
    }
    
    cmdgenerate->setEnabled(true);
    cmbSave->setEnabled(true);
    
    cmdredo_U = 0;
    cmdundo_U = 0;
    cmdFindReplace = 0;
    cmdredo = 0;
    cmdundo = 0;
    cmbaction = 0;
    u_head = 0;
    cmdgenerate = 0;
    cmbSave = 0;
}

void frmDataAnalyzer::performUpdateOperation()
{
    switch(tabId)
    {
    case 0:
	if(qryresviewerR && validUpdateAction[0])
	    prepareQueryANDupdateRec();
	else
	    QMessageBox::information(this,tr("Update action status"),tr("Updatation possible only for \n\'Row selection\',\'Wildcard\',\'Field Search\' and \'Comparison\' query"));
	break;
    case 1:
	if(qryresviewerW && validUpdateAction[1])
	    prepareQueryANDupdateRec();
	else
	    QMessageBox::information(this,tr("Update action status"),tr("Updatation possible only for \n\'Row selection\',\'Wildcard\',\'Field Search\' and \'Comparison\' query"));
	break;
    case 2:
	if(qryresviewerF && validUpdateAction[2])
	    prepareQueryANDupdateRec();
	else
	    QMessageBox::information(this,tr("Update action status"),tr("Updatation possible only for \n\'Row selection\',\'Wildcard\',\'Field Search\' and \'Comparison\' query"));
	break;
    case 3:
	if(qryresviewerC && validUpdateAction[3])
	    prepareQueryANDupdateRec();
	else
	    QMessageBox::information(this,tr("Update action status"),tr("Updatation possible only for \n\'Row selection\',\'Wildcard\',\'Field Search\' and \'Comparison\' query"));
	break;
    case 4:
	if(qryresviewerA && validUpdateAction[4])
	    prepareQueryANDupdateRec();
	else
	    QMessageBox::information(this,tr("Update action status"),tr("Updatation possible only for \n\'Row selection\',\'Wildcard\',\'Field Search\' and \'Comparison\' query"));
	break;
    case 5:
	if(qryresviewerL && validUpdateAction[5])
	    prepareQueryANDupdateRec();
	else
	    QMessageBox::information(this,tr("Update action status"),tr("Updatation possible only for \n\'Row selection\',\'Wildcard\',\'Field Search\' and \'Comparison\' query"));
	break;
    }
}

bool frmDataAnalyzer::prepareQueryANDupdateRec()
{
    static int upIndx = 1;
    QString qcommand = "";
    int status = -1;
    QDataTable *qryresviewer = 0;
    QString backTab = "",backTabB = "",query = "";
    bool validDelAct = false;
    QListView *logTracker = 0;
    int redoCount = 0, undoCount = 0;
    QToolButton *cmdundo = 0;
    int minRow = 0,maxRow = -1;
    bool changeFlag = false;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	backTab = "updateR";
	validDelAct = validDelAction[0];
	query = queryR;
	backTabB = "updateRb";
	logTracker = logTrackerR;
	redoCount = redoCountR;
	undoCount = undoCountR;
	cmdundo = cmdundoR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	backTab = "updateW";
	validDelAct = validDelAction[1];
	query = queryW;
	backTabB = "updateWb";
	logTracker = logTrackerW;
	redoCount = redoCountW;
	undoCount = undoCountW;
	cmdundo = cmdundoW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	backTab = "updateF";
	validDelAct = validDelAction[2];
	query = queryF;
	backTabB = "updateFb";
	logTracker = logTrackerF;
	redoCount = redoCountF;
	undoCount = undoCountF;
	cmdundo = cmdundoF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	backTab = "updateC";
	validDelAct = validDelAction[3];
	query = queryC;
	backTabB = "updateCb";
	logTracker = logTrackerC;
	redoCount = redoCountC;
	undoCount = undoCountC;
	cmdundo = cmdundoC;
	break;
    case 4:
	break;
    case 5:
	qryresviewer = qryresviewerL;
	backTab = "updateL";
	validDelAct = validDelAction[5];
	query = queryL;
	backTabB = "updateLb";
	logTracker = logTrackerL;
	redoCount = redoCountL;
	undoCount = undoCountL;
	cmdundo = cmdundoL;
	break;
    }
    
    findMinMaxRowNum(minRow,maxRow); //This is done to reduce update time
    int numRecs = qryresviewer->numRows();
    qcommand = "mysql " + sqlDB->databaseName() + " -e \"create table " + backTab + QString::number(upIndx) + " as(";
    qcommand += "select * from " + (qryresviewer->caption()).section("_",0,int((qryresviewer->caption()).contains("_") - 1)) + query + ")\";";
    status = system(qcommand); //Create & populate backup table: original data at this stage
    if(status > -1)
    {
	status = -1;
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"create table " + backTabB + QString::number(upIndx) + " as(select * from " + qryresviewer->caption() + ")\";";
	status = system(qcommand); //Create & populate backup table: updated data at this stage
	if(status > -1)
	{
	    //~~~~~~~This part update original table by 'qryresviewerW' table [START]
	    //Use cursor to update value[START]
	    QSqlCursor *curs = qryresviewer->sqlCursor();
	    //tabCursor ~ 'QSqlCursor on the original table'
	    tabCursor->setMode(2); //Update mode
	    for(register int row = minRow; row <= maxRow; ++row)
	    {
		QSqlIndex pk = tabCursor->primaryIndex();
		tabCursor->setValue(0,QVariant(qryresviewer->text(row,0)));
		if(tabCursor->select( pk, pk ))
		{
		    if(tabCursor->next())
		    {
			QSqlRecord *buffer  = tabCursor->primeUpdate();
			for(register int col = 1; col < qryresviewer->numCols(); ++col)
			{
			    if((buffer->value(curs->fieldName(col))).toString() != qryresviewer->text(row,col))
			    {
				buffer->setValue(curs->fieldName(col),qryresviewer->text(row,col));
				changeFlag = true;
			    }
			}
			if(changeFlag && !tabCursor->update())
			    cout << "\nSQL Update failure error\n";
			buffer = 0;
		    }
		}
		changeFlag = false;
	    }
	    tabCursor->setMode(0); //ReadOnly mode
	    curs = 0;
	    //Use cursor to update value[STOP]
	    //~~~~~~~This part update original table by 'qryresviewerW' table [STOP]
	    QListViewItem *lvi = new QListViewItem(logTracker,tr(backTab + QString::number(upIndx)),"",backTabB + QString::number(upIndx),tr("u"));
	    lvi->setPixmap(0,QImage(tr(workDir + "/images/undocol.png")));
	    logTracker->insertItem(lvi);
	    lvi = 0;
	    logTracker->hideColumn(1);
	    logTracker->hideColumn(2);
	    logTracker->hideColumn(3);
	    QMessageBox::information(this,tr("Update action status"),QString ::number(numRecs) + tr(" records are updated successfully"));
	    ++upIndx;
	    moveNot_EnableListItems_to_End(logTracker);
	    ++undoCount;
	    if(cmdundo->isHidden())
		cmdundo->setHidden(false);
	}
    }
    
    switch(tabId)
    {
    case 0:
	qryresviewerR = qryresviewer;
	logTrackerR = logTracker;
	redoCountR = redoCount;
	undoCountR = undoCount;
	cmdundoR = cmdundo;
	break;
    case 1:
	qryresviewerW = qryresviewer;
	logTrackerW = logTracker;
	redoCountW = redoCount;
	undoCountW = undoCount;
	cmdundoW = cmdundo;
	break;
    case 2:
	qryresviewerF = qryresviewer;
	logTrackerF = logTracker;
	redoCountF = redoCount;
	undoCountF = undoCount;
	cmdundoF = cmdundo;
	break;
    case 3:
	qryresviewerC = qryresviewer;
	logTrackerC = logTracker;
	redoCountC = redoCount;
	undoCountC = undoCount;
	cmdundoC = cmdundo;
	break;
    case 4:
	break;
    case 5:
	qryresviewerL = qryresviewer;
	logTrackerL = logTracker;
	redoCountL = redoCount;
	undoCountL = undoCount;
	cmdundoL = cmdundo;
	break;
    }
    qryresviewer = 0;
    logTracker = 0;
}

void frmDataAnalyzer::findMinMaxRowNum(int &minRow, int &maxRow)
{
    UPDATESTK *u_head = 0,*u_redo = 0;
    
    switch(tabId)
    {
    case 0:
	u_head = u_headR;
	u_headR = 0;
	u_undoR = 0;
	u_redo = u_redoR;
	u_redoR = 0;
	break;
    case 1:
	u_head = u_headW;
	u_headW = 0;
	u_undoW = 0;
	u_redo = u_redoW;
	u_redoW = 0;
	break;
    case 2:
	u_head = u_headF;
	u_headF = 0;
	u_undoF = 0;
	u_redo = u_redoF;
	u_redoF = 0;
	break;
    case 3:
	u_head = u_headC;
	u_headC = 0;
	u_undoC = 0;
	u_redo = u_redoC;
	u_redoC = 0;
	break;
    case 4:
	break;
    case 5:
	u_head = u_headL;
	u_headL = 0;
	u_undoL = 0;
	u_redo = u_redoL;
	u_redoL = 0;
	break;
    }
    
    if(u_head && u_head != u_redo)
    {
	maxRow = minRow = u_head->row;
	u_head = u_head->next;
	while(u_head && u_head != u_redo)
	{
	    maxRow = (maxRow < u_head->row ? u_head->row : maxRow);
	    minRow = (minRow > u_head->row ? u_head->row : minRow);
	    u_head = u_head->next;
	}
    }
    u_head = 0;
    u_redo = 0;
}

void frmDataAnalyzer::deleteUpdateStack()
{
    UPDATESTK *u_head = 0;
    
    switch(tabId)
    {
    case 0:
	u_head = u_headR;
	u_headR = 0;
	u_undoR = 0;
	u_redoR = 0;
	break;
    case 1:
	u_head = u_headW;
	u_headW = 0;
	u_undoW = 0;
	u_redoW = 0;
	break;
    case 2:
	u_head = u_headF;
	u_headF = 0;
	u_undoF = 0;
	u_redoF = 0;
	break;
    case 3:
	u_head = u_headC;
	u_headC = 0;
	u_undoC = 0;
	u_redoC = 0;
	break;
    case 4:
	break;
    case 5:
	u_head = u_headL;
	u_headL = 0;
	u_undoL = 0;
	u_redoL = 0;
	break;
    }
    
    while(u_head)
    {
	UPDATESTK *node = u_head;
	u_head = u_head->next;
	node->next = 0; 
	delete [] node;
	node = 0;
	if(u_head)
	    u_head->prev = 0;
    }
    u_head = 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~UPDATE~~~~~~~~~~[STOP]
///////////////////////Action Section[STOP]~~~~~~~~~~~~~~~~~

void frmDataAnalyzer::timerEvent(QTimerEvent *e)
{
    if(qryresviewerR && !qryresviewerR->isShown())
    {
	if(viewCurR)
	{
	    if(viewCurR->isValid())
	    {
		viewCurR->clear();
		delete viewCurR;
		delete qryresviewerR;
	    }
	    viewCurR = 0;
	}
	qryresviewerR = 0;
	cmbactionR->setEnabled(false);
    }
    
    if(qryresviewerW && !qryresviewerW->isShown())
    {
	if(viewCurW)
	{
	    if(viewCurW->isValid())
	    {
		viewCurW->clear();
		delete viewCurW;
		delete qryresviewerW;
	    }
	    viewCurW = 0;
	}
	qryresviewerW = 0;
	cmbactionW->setEnabled(false);
    }

    if(qryresviewerF && !qryresviewerF->isShown())
    {
	if(viewCurF)
	{
	    if(viewCurF->isValid())
	    {
		viewCurF->clear();
		delete viewCurF;
		delete qryresviewerF;
	    }
	    viewCurF = 0;
	}
	qryresviewerF = 0;
	cmbactionF->setEnabled(false);
    }

    if(qryresviewerC && !qryresviewerC->isShown())
    {
	if(viewCurC)
	{
	    if(viewCurC->isValid())
	    {
		viewCurC->clear();
		delete viewCurC;
		delete qryresviewerC;
	    }
	    viewCurC = 0;
	}
	qryresviewerC = 0;
	cmbactionC->setEnabled(false);
    }

    if(qryresviewerA && !qryresviewerA->isShown())
    {
	if(viewCurA)
	{
	    if(viewCurA->isValid())
	    {
		viewCurA->clear();
		delete viewCurA;
		delete qryresviewerA;
	    }
	    viewCurA = 0;
	}
	qryresviewerA = 0;
	cmbactionA->setEnabled(false);
    }
    
    if(qryresviewerL && !qryresviewerL->isShown())
    {
	if(viewCurL)
	{
	    if(viewCurL->isValid())
	    {
		viewCurL->clear();
		delete viewCurL;
		delete qryresviewerL;
	    }
	    viewCurL = 0;
	}
	qryresviewerL = 0;
	cmbactionL->setEnabled(false);
    }
    
    if(summarydata::closeFlag)
    {
	loadOperation_Table(0);
	switch(tabId)
	{
	case 0:
	    cmdgenerateR->setEnabled(true);
	    cmbSaveR->setEnabled(true);
	    cmbactionR->setEnabled(true);
	    break;
	case 1:
	    cmdgenerateW->setEnabled(true);
	    cmbSaveW->setEnabled(true);
	    cmbactionW->setEnabled(true);
	    break;
	case 2:
	    cmdgenerateF->setEnabled(true);
	    cmbSaveF->setEnabled(true);
	    cmbactionF->setEnabled(true);
	    break;
	case 3:
	    cmdgenerateC->setEnabled(true);
	    cmbSaveC->setEnabled(true);
	    cmbactionC->setEnabled(true);
	    break;
	case 4:
	    cmdgenerateA->setEnabled(true);
	    cmbSaveA->setEnabled(true);
	    cmbactionA->setEnabled(true);
	    break;
	case 5:
	    cmdgenerateL->setEnabled(true);
	    cmbSaveL->setEnabled(true);
	    cmbactionL->setEnabled(true);
	    break;
	}
	summarydata::closeFlag = false;
    }
}

//////////Close and Deletion Events~~~~~~~~~~~~~~~~
void frmDataAnalyzer::closeEvent(QCloseEvent *e)
{
    closeAnyOpenTable();
    
    dropViewR();
    dropViewW();
    dropViewF();
    dropViewC();
    dropViewA();
    dropViewL();
    
    destroyGarbageTablesIfAny(logTrackerR);
    destroyGarbageTablesIfAny(logTrackerW);
    destroyGarbageTablesIfAny(logTrackerF);
    destroyGarbageTablesIfAny(logTrackerC);
    destroyGarbageTablesIfAny(logTrackerL);
    e->accept();
}

void frmDataAnalyzer::dropViewR()
{
    int status = 0;
    QString qcommand = "", query = "";
    QStringList tabList = sqlDB->tables();
    QString tabStr = tabList.join(tr(","));

    if(tabStr.contains(viewR))
    {
	closeOpenTableR();
	query = "drop table " + viewR + ";";
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + query + "\"";
	status = system(qcommand);
    }
}

void frmDataAnalyzer::dropViewW()
{
    int status = 0;
    QString qcommand = "", query = "";
    QStringList tabList = sqlDB->tables();
    QString tabStr = tabList.join(tr(","));

    if(tabStr.contains(viewW))
    {
	closeOpenTableW();
	query = "drop table " + viewW + ";";
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + query + "\"";
	status = system(qcommand);
    }
}

void frmDataAnalyzer::dropViewF()
{
    int status = 0;
    QString qcommand = "", query = "";
    QStringList tabList = sqlDB->tables();
    QString tabStr = tabList.join(tr(","));
    
    if(tabStr.contains(viewF))
    {
	closeOpenTableF();
	query = "drop table " + viewF + ";";
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + query + "\"";
	status = system(qcommand);
    }
}

void frmDataAnalyzer::dropViewC()
{
    int status = 0;
    QString qcommand = "", query = "";
    QStringList tabList = sqlDB->tables();
    QString tabStr = tabList.join(tr(","));
    
    if(tabStr.contains(viewC))
    {
	closeOpenTableC();
	query = "drop table " + viewC + ";";
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + query + "\"";
	status = system(qcommand);
    }
}

void frmDataAnalyzer::dropViewA()
{
    int status = 0;
    QString qcommand = "", query = "";
    QStringList tabList = sqlDB->tables();
    QString tabStr = tabList.join(tr(","));
    
    if(tabStr.contains(viewA))
    {
	closeOpenTableA();
	query = "drop table " + viewA + ";";
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + query + "\"";
	status = system(qcommand);
    }
}

void frmDataAnalyzer::dropViewL()
{
    int status = 0;
    QString qcommand = "", query = "";
    QStringList tabList = sqlDB->tables();
    QString tabStr = tabList.join(tr(","));
    
    if(tabStr.contains(viewL))
    {
	closeOpenTableL();
	query = "drop table " + viewL + ";";
	qcommand = "mysql " + sqlDB->databaseName() + " -e \"" + query + "\"";
	status = system(qcommand);
    }
}

void frmDataAnalyzer::closeAnyOpenTable()
{
    closeOpenTableR();
    closeOpenTableW();
    closeOpenTableF();
    closeOpenTableC();
    closeOpenTableA();
    closeOpenTableL();
}

void frmDataAnalyzer::closeOpenTableR()
{
    if(qryresviewerR)
    {
	if(viewCurR)
	{
	    viewCurR->clear();
	    delete viewCurR;
	    viewCurR = 0;
	}
	if(!qryresviewerR->isHidden())
	    qryresviewerR->close();
	delete qryresviewerR;
	qryresviewerR = 0;
    }
}

void frmDataAnalyzer::closeOpenTableW()
{
    if(qryresviewerW)
    {
	if(viewCurW)
	{
	    viewCurW->clear();
	    delete viewCurW;
	    viewCurW = 0;
	}
	if(!qryresviewerW->isHidden())
	    qryresviewerW->close();
	delete qryresviewerW;
	qryresviewerW = 0;
    }
}

void frmDataAnalyzer::closeOpenTableF()
{
    if(qryresviewerF)
    {
	if(viewCurF)
	{
	    viewCurF->clear();
	    delete viewCurF;
	}
	viewCurF = 0;
	if(!qryresviewerF->isHidden())
	    qryresviewerF->close();
	delete qryresviewerF;
	qryresviewerF = 0;
    }
}

void frmDataAnalyzer::closeOpenTableC()
{
    if(qryresviewerC)
    {
	if(viewCurC)
	{
	    viewCurC->clear();
	    delete viewCurC;
	}
	viewCurC = 0;
	if(!qryresviewerC->isHidden())
	    qryresviewerC->close();
	delete qryresviewerC;
	qryresviewerC = 0;
    }
}

void frmDataAnalyzer::closeOpenTableA()
{
    if(qryresviewerA)
    {
	if(viewCurA)
	{
	    viewCurA->clear();
	    delete viewCurA;
	}
	viewCurA = 0;
	if(!qryresviewerA->isHidden())
	    qryresviewerA->close();
	delete qryresviewerA;
	qryresviewerA = 0;
    }
}

void frmDataAnalyzer::closeOpenTableL()
{
    if(qryresviewerL)
    {
	if(viewCurL)
	{
	    viewCurL->clear();
	    delete viewCurL;
	}
	viewCurL = 0;
	if(!qryresviewerL->isHidden())
	    qryresviewerL->close();
	delete qryresviewerL;
	qryresviewerL = 0;
    }
}

void frmDataAnalyzer::destroyGarbageTablesIfAny(QListView *logTracker)
{
    QListViewItem *item = 0;
    
    if(logTracker->childCount())
    {
	item = logTracker->lastItem();
	QStringList strlst = sqlDB->tables(QSql::Tables);
	QString strtables = strlst.join(tr(","));
	for(register int i = 0; i < logTracker->childCount(); ++i)
	{
	    if((item->text(0)).contains(tr("update")))
	    {
		fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"drop table " + item->text(0) + ";\"");
		fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"drop table " + item->text(2) + ";\"");
	    }
	    else if((item->text(0)).contains(tr("delete")))
	    {
		if(item->text(3) == tr("u"))
		    cascadeDeletion(item->text(1));
		if(strtables.contains(item->text(0)))
		    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"drop table " + item->text(0) + ";\"");
	    }
	    item = item->itemAbove();
	}
	item = 0;
	logTracker->clear();
    }
    
    if(logTracker == logTrackerR)
    {
	cmdredoR->setHidden(true);
	cmdundoR->setHidden(true);
    }
    else if(logTracker == logTrackerW)
    {
	cmdredoW->setHidden(true);
	cmdundoW->setHidden(true);
    }
    else if(logTracker == logTrackerF)
    {
	cmdredoF->setHidden(true);
	cmdundoF->setHidden(true);
    }
    else if(logTracker == logTrackerC)
    {
	cmdredoC->setHidden(true);
	cmdundoC->setHidden(true);
    }
    else if(logTracker == logTrackerL)
    {
	cmdredoL->setHidden(true);
	cmdundoL->setHidden(true);
    }
    logTracker = 0;
}

void frmDataAnalyzer::cascadeDeletion(QString subQuery)
{
    QString query = "";
    QStringList tableList,tempList,cascadeList;
    
    tableList.append(tableName);
    QSqlCursor cascadeCur(tr("pk_fk_relations"),true,sqlDB);
    while(1)
    {
	for(QStringList::Iterator it = tableList.begin(); it != tableList.end(); ++it)
	{
	    if(cascadeCur.select())
	    {
		while(cascadeCur.next())
		{
		    if((cascadeCur.value(2)).toString() == *it)
		    {
			query = "delete from " + (cascadeCur.value(1)).toString() + subQuery + ";";
			fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
			tempList.append((cascadeCur.value(1)).toString());
			cascadeList.append("'" + (cascadeCur.value(1)).toString() + "'");
		    }
		}
	    }
	}

	if(tempList.isEmpty())
	    break;
	else
	{
	    tableList = tempList;
	    tempList.clear();
	}
    }
    if(!cascadeList.isEmpty())
	QMessageBox::information(this,tr("Cascade deletion"),"Cascaded deletion is executed\nin the following tables: " + cascadeList.join(tr("\n")));
}

//Columns Add, Modification, Del Events[START]
void frmDataAnalyzer::addColumn()
{
    QDataTable *qryresviewer = 0;
    QSqlCursor *viewCur = 0;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	viewCur = viewCurR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	viewCur = viewCurW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	viewCur = viewCurF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	viewCur = viewCurC;
	break;
    case 4:
	qryresviewer = qryresviewerA;
	viewCur = viewCurA;
	break;
    case 5:
	qryresviewer = qryresviewerL;
	viewCur = viewCurL;
	break;
    }

    if(qryresviewer)
    {
	//Prepare column list[START]
	QStringList colList;
	QString query = "";
	
	for(int i = 0; i < qryresviewer->numCols(); ++i)
	    colList.append(viewCur->fieldName(i));
	//Prepare column list[STOP]
	frmcolumnadd *coladd = new frmcolumnadd(0,"colmod",true);
	coladd->initSettings(colList);
	if(coladd->exec() == QDialog::Accepted && coladd->position != -1)
	{
	    query = tr("ALTER TABLE ") + qryresviewer->caption() + tr(" ADD ") + coladd->colName + tr(" ") + coladd->dttype + tr("(") + coladd->Length + tr(")");
	
	    switch(coladd->position)
	    {
	    case 0:
		query += tr(" FIRST;");
		break;
	    case 1:
		query += tr(" AFTER ") + coladd->selCol + tr(";");
		break;
	    case 2:
		query += tr(";");
	    }
	    coladd->close();
	    delete coladd;
	    coladd = 0;
	    
	    if(executeQueryInDatabase(query))
		showFilteredData(true);
	    else
		QMessageBox::information(this,tr("Execution failure"),tr("Fail to execute the given query!\nInform the developer"),QMessageBox::Ok);
	}	
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
    qryresviewer = 0;
    viewCur = 0;
}

void frmDataAnalyzer::modifyColumn()
{
    QDataTable *qryresviewer = 0;
    QSqlCursor *viewCur = 0;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	viewCur = viewCurR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	viewCur = viewCurW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	viewCur = viewCurF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	viewCur = viewCurC;
	break;
    case 4:
	qryresviewer = qryresviewerA;
	viewCur = viewCurA;
	break;
    case 5:
	qryresviewer = qryresviewerL;
	viewCur = viewCurL;
	break;
    }
    
    if(qryresviewer)
    {
	//Prepare column list[START]
	QStringList colList,colList2,dttypeList,lenList;
	QString query = "";
	bool stat = true;
	
	for(int i = 0; i < qryresviewer->numCols(); ++i)
	    colList.append(viewCur->fieldName(i));
	query = tr("mysql ") + sqlDB->databaseName() + tr(" -e 'desc ") + qryresviewer->caption() +tr("' > .fieldInfo.txt");
	fscanVal = system(query);
	//Fetch field information [START]
	QString buffer;
	QFile fpRead(".fieldInfo.txt");
	if(fpRead.open(IO_ReadOnly | IO_Truncate))
	{
	    QTextStream fpIn(&fpRead);
	    fpIn.setEncoding(QTextStream::UnicodeUTF8);
	    
	    buffer = fpIn.readLine();
	    while(!fpIn.atEnd())
	    {
		buffer = fpIn.readLine();
		QStringList list = QStringList::split("\t",buffer);
		QStringList::iterator it = list.begin();
		++it;
		buffer = *it;
		buffer.remove(")");
		dttypeList.append(buffer.section('(',0,0));
		lenList.append(buffer.section('(',-1));
	    }
	    fpRead.close();
	    
	    frmcolmodify *colmod = new frmcolmodify(0,"colmod",true);
	    colmod->initSettings(colList,dttypeList,lenList);
	    if(colmod->exec() == QDialog::Accepted && colmod->retStat)
	    {
		colList2 = colmod->colNameList; //dttype list is used here to store new column names
		lenList = colmod->lengthList;
		int listLength = (int)colList.count();
		QStringList::iterator it1 = colList.begin();
		QStringList::iterator it2 = colList2.begin();
		QStringList::iterator it3 = dttypeList.begin();
		QStringList::iterator it4 = lenList.begin();
		for(int i = 0; i < listLength; ++i)
		{
		    if(*it1 != *it2)
		    {
			query = tr("ALTER TABLE ") + qryresviewer->caption() + tr(" CHANGE ") + *it1 + tr(" ") + *it2 + tr(" ") + *it3 + tr("(") + *it4 + tr(")");
			if(!executeQueryInDatabase(query))
			{
			    QMessageBox::information(this,tr("Execution failure"),tr("Fail to execute the given query!\nInform the developer"),QMessageBox::Ok);
			    stat = false;
			    break;
			}
			it1++;
			it2++;
			it3++;
			it4++;
		    }
		    else
		    {
			query = tr("ALTER TABLE ") + qryresviewer->caption() + tr(" MODIFY ") + *it1 + tr(" ") + *it3 + tr("(") + *it4 + tr(")");
			if(!executeQueryInDatabase(query))
			{
			    QMessageBox::information(this,tr("Execution failure"),tr("Fail to execute the given query!\nInform the developer"),QMessageBox::Ok);
			    stat = false;
			    break;
			}
			it1++;
			it2++;
			it3++;
			it4++;
		    }
		}
		showFilteredData(true);
		delete colmod;
		colmod = 0;
	    }
	    fscanVal = system("rm -f .fieldInfo.txt");
	}
	//Fetch field information [STOP]
	//Prepare column list[STOP]
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
    qryresviewer = 0;
    viewCur = 0;
}

void frmDataAnalyzer::dropColumn()
{
    QDataTable *qryresviewer = 0;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	break;
    case 4:
	qryresviewer = qryresviewerA;
	break;
    case 5:
	qryresviewer = qryresviewerL;
	break;
    }

    if(qryresviewer)
    {
	QStringList dropColList;
	QString query;
	bool stat = true;
	
	QHeader *h = qryresviewer->horizontalHeader();
	for(int i = 0; i < qryresviewer->numCols(); ++i) //identify columns to drop
	{
	    if(qryresviewer->isColumnSelected(i))
		dropColList.append(h->label(i));
	}
	
	for(QStringList::iterator it = dropColList.begin(); it != dropColList.end(); ++it)
	{
	    query = tr("ALTER TABLE ") + qryresviewer->caption() + tr(" DROP COLUMN ") + *it;
	    if(!executeQueryInDatabase(query))
	    {
		QMessageBox::information(this,tr("Execution failure"),tr("Fail to execute the given query!\nInform the developer"),QMessageBox::Ok);
		stat = false;
		break;
	    }
	    
	    if(stat)
		showFilteredData(true);
	}
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
    qryresviewer = 0;
}

void frmDataAnalyzer::renameOpenTable()
{
    QDataTable *qryresviewer = 0;
    
    switch(tabId)
    {
    case 0:
	qryresviewer = qryresviewerR;
	break;
    case 1:
	qryresviewer = qryresviewerW;
	break;
    case 2:
	qryresviewer = qryresviewerF;
	break;
    case 3:
	qryresviewer = qryresviewerC;
	break;
    case 4:
	qryresviewer = qryresviewerA;
	break;
    case 5:
	qryresviewer = qryresviewerL;
	break;
    }

    if(qryresviewer)
    {
	bool ok = false;
	QString tbName = QInputDialog::getText("New table name","Enter new name:",QLineEdit::Normal,QString::null,&ok,this);
	if(ok && !tbName.isEmpty()) 
	{
	    if(checkTableNameSyntax(tbName))
	    {
		QString query = tr("RENAME TABLE ") + qryresviewer->caption() + tr(" TO ") + tbName;
		if(executeQueryInDatabase(query))
		{
		    switch(tabId)
		    {
		    case 0:
			viewW = tbName;
			break;
		    case 1:
			viewF = tbName;
			break;
		    case 2:
			viewC = tbName;
			break;
		    case 3:
			viewA = tbName;
			break;
		    case 4:
			viewL = tbName;
			break;
		    }
		    showFilteredData(true);
		}
		else
		    QMessageBox::information(this,tr("Execution failure"),tr("Query execution failure!\nInform the developer"),QMessageBox::Ok);
	    }
	}
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
    qryresviewer = 0; 
}

bool frmDataAnalyzer::executeQueryInDatabase(QString query)
{
    int status = -1;
    
    QString command = tr("mysql ") + sqlDB->databaseName() + tr(" -e \"") + query + tr("\"");
    status = system(command);
    if(status == -1)
	return false;
    else
	return true;
}
//Columns Add, Modification, Del Events[STOP]

void frmDataAnalyzer::progressDialogSettings(int wSpaceID,QProgressDialog **progressDlg,int totalSteps, QString title, QString label)
{
    switch(wSpaceID)
    {
    case 0:
	*progressDlg = new QProgressDialog(wSpaceR,tr(title),false,WDestructiveClose);
	break;
    case 1:
	*progressDlg = new QProgressDialog(wSpaceW,tr(title),false,WDestructiveClose);
	break;
    case 2:
	*progressDlg = new QProgressDialog(wSpaceF,tr(title),false,WDestructiveClose);
	break;
    case 3:
	*progressDlg = new QProgressDialog(wSpaceC,tr(title),false,WDestructiveClose);
	break;
    case 4:
	*progressDlg = new QProgressDialog(wSpaceA,tr(title),false,WDestructiveClose);
	break;
    case 5:
	*progressDlg = new QProgressDialog(wSpaceL,tr(title),false,WDestructiveClose);
	break;
    }
    (*progressDlg)->setCaption(title);
    (*progressDlg)->setTotalSteps(totalSteps);
    lblProgress = new QLabel(*progressDlg,label,WDestructiveClose);
    (*progressDlg)->setLabel(lblProgress);
    (*progressDlg)->setAutoReset(false);
}
