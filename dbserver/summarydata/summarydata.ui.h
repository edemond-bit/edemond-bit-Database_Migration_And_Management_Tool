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
#include <iostream.h>
#include <math.h>
#include <time.h>
#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qvbox.h>
#include <qworkspace.h>
#include <qsqldatabase.h>
#include <qsqlcursor.h>
#include <qsqlresult.h>
#include <qdatatable.h>
#include <qtable.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qprogressdialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include "../threadrun/concurrentrun.h"
#include "../crypticsettings/frmcrypticsettings.h"

bool summarydata::closeFlag = false;

void summarydata::init()
{
    sqlDB = 0;
    tableView = 0;
    sampletab = 0;
    
    chkallelefreq->setEnabled(false);
    chkhwe->setEnabled(false);
    cmdcryptic->setEnabled(false);
    //~~~~~~~~~Workspace Setting~~~[START]
    vb = new QVBox(frm1,"vBox",WDestructiveClose);
    vb->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    wSpace = new QWorkspace(vb);
    wSpace->setScrollBarsEnabled(true);
    vb->show();
    //~~~~~~~~~Workspace Setting~~~[STOP]
    startTimer(1000);
}

void summarydata::initSettings(QSqlDatabase *db, QDataTable *table)
{
    tableView = table;
    viewCur = tableView->sqlCursor();
    viewName = viewCur->name();
    sqlDB = db;
}

void summarydata::timerEvent(QTimerEvent *e)
{
    if(tableView && !tableView->isShown())
    {
	viewCur = 0;
	tableView = 0;
    }
    
    if(sampletab && !sampletab->isShown())
	deleteTheResultTable();
}
////////~~~~~~~~~~~Resizing Event~~~~~~~~~~~~~~~[START]
void summarydata::resizeEvent(QResizeEvent *e)
{
    QSize size = e->size();
    
    setGeometry(x(),y(),(size.width() < 655 ? 655 : size.width()),(size.height() < 300 ? 300 : size.height()));    
    frm1->setGeometry(frm1->x(),frm1->y(),width() - 155,height() - 13);
    grbox3->setGeometry(width() - 149,grbox3->y(),grbox3->width(),grbox3->height());
    grbox4->setGeometry(width() - 149,grbox4->y(),grbox4->width(),grbox4->height());
    vb->setGeometry(frm1->x() + 1,5, frm1->width() - 8, frm1->height() - 8);
}
////////~~~~~~~~~~~Resizing Event~~~~~~~~~~~~~~~[STOP]

void summarydata::cmdgenerate_clicked()
{
    int retVal = 0,progress = 0;
    int numRows = 0, numCols = 0;
        
    if(cmdgenerate->text() == "Result" || cmdgenerate->text() == "Start")
    {
	if(!tableView)
	{
	    load_fileIndex = 0;
	    generateResultTable(tableSizeArr[load_fileIndex],(int)columnList.count());
	    loadResult(tableSizeArr[load_fileIndex]);
	    if(load_fileIndex < numThreadReq - 1)
	    {
		cmdgenerate->setText("Next");
		QMessageBox::information(this,tr("Memory overflow"),tr("Due to lack of memory, \nall result can't be shown at a time.\nUse 'Next' button to see next set of result"));
	    }
	}
	else
	    QMessageBox::information(this,tr("Close the table"),tr("Close the table: ") + viewName + tr(" first!"));
    }
    else if(cmdgenerate->text() == "Start")
    {
	if(!tableView)
	{
	    if(load_fileIndex != 0)
	    {
		load_fileIndex = 0;
		generateResultTable(tableSizeArr[load_fileIndex],(int)columnList.count());
		loadResult(tableSizeArr[load_fileIndex]);
	    }
	    if(load_fileIndex < numThreadReq - 1)
	    {
		cmdgenerate->setText("Next");
		QMessageBox::information(this,tr("Memory overflow"),tr("Due to lack of memory, \nall result can't be shown at a time.\nUse 'Next' button to see next set of result"));
	    }
	}
	else
	    QMessageBox::information(this,tr("Close the table"),tr("Close the table: ") + viewName + tr(" first!"));
    }
    else if(cmdgenerate->text() == "Next")
    {
	if(!tableView)
	{
	    ++load_fileIndex;
	    if(tableSizeArr[load_fileIndex] != tableSizeArr[load_fileIndex - 1])
		generateResultTable(tableSizeArr[load_fileIndex],(int)columnList.count());
	    loadResult(tableSizeArr[load_fileIndex]);
	}
	else
	    QMessageBox::information(this,tr("Close the table"),tr("Close the table: ") + viewName + tr(" first!"));
    }
    else if(cmdgenerate->text() == "End")
    {
	if(!tableView)
	{
	    if(load_fileIndex != numThreadReq - 1)
	    {
		load_fileIndex = numThreadReq - 1;
		generateResultTable(tableSizeArr[load_fileIndex],(int)columnList.count());
		loadResult(tableSizeArr[load_fileIndex]);
	    }
	    cmdgenerate->setText("Prev");
	    QMessageBox::information(this,tr("Memory overflow"),tr("Due to lack of memory, \nall result can't be shown at a time.\nUse 'Prev' button to see next set of result"));
	}
	else
	    QMessageBox::information(this,tr("Close the table"),tr("Close the table: ") + viewName + tr(" first!"));
    }
    else if(cmdgenerate->text() == "Prev")
    {
	if(!tableView)
	{
	    --load_fileIndex;
	    generateResultTable(tableSizeArr[load_fileIndex],(int)columnList.count());
	    loadResult(tableSizeArr[load_fileIndex]);
	}
	else
	    QMessageBox::information(this,tr("Close the table"),tr("Close the table: ") + viewName + tr(" first!"));
    }
    else
    {
	if(tableView)
	{
	    retVal = istheTableforGenotypeData();
	    if(retVal)
	    {
		columnList.clear();
		if(rdIndividual->isChecked())
		{
		    columnList.append("IndivID");
		    numCols = 1;
		    if(chkgenotype->isChecked())
		    {
			columnList.append("GC_AA");
			columnList.append("GC_AB");
			columnList.append("GC_BB");
			columnList.append("GC_Total");
			numCols += 4;
		    }
		    else if(chkcrypticreln->isChecked())
		    {
			numCols = 1;
			columnList.clear();
			columnList.append(tr("Individual"));
			QHeader *header = tableView->horizontalHeader();
			for(int i = (retVal == 1 ? 2 : 1); i < tableView->numCols(); ++i,++numCols)
			    columnList.append(header->label(i));
		    }
		    numRows = (retVal == 1 ? tableView->numCols() - 2 : tableView->numCols() - 1);
		}
		else
		{
		    QHeader *header = tableView->horizontalHeader();
		    if(retVal == 1)
		    {
			columnList.append(header->label(0));
			columnList.append(header->label(1));
		    }
		    else
			columnList.append(header->label(0));
		    numCols = (retVal == 1 ? 2 : 1);
		    if(chkgenotype->isChecked())
		    {
			columnList.append("GC_AA");
			columnList.append("GC_AB");
			columnList.append("GC_BB");
			columnList.append("GC_Total");
			columnList.append("GC_noCall");
			numCols += 5;
		    }
		    
		    if(chkallelefreq->isChecked())
		    {
			columnList.append("Allele_Frequency_A");
			columnList.append("Allele_Frequency_B");
			numCols += 2;
		    }
		    
		    if(chkhwe->isChecked())
		    {
			columnList.append("HWE");
			++numCols;
		    }
		    numRows = tableView->numRows();
		}
		//cout << numRows << " " << numCols << endl;
		int k = 0;
		if(rdIndividual->isChecked()) //Individual Wise
		{
		    progressDlg = 0;
		    generateResultTable(numRows,numCols);
		    QHeader *header = tableView->horizontalHeader();
		    int i = (retVal == 1 ? 2 : 1);
		    if(chkgenotype->isChecked() && chkcrypticreln->isChecked())
		    {
			progressDialogSettings(&progressDlg,numRows,tr("Individual wise operation"),tr("Progress Label"));
			for(int j = 0; j < numRows; ++j, ++i)
			{
			    k = 0;
			    sampletab->setText(j,k,header->label(i));
			    genotypeCalculation(i,tableView->numRows(),j,k);
			    progressDlg->setLabelText(tr("Individual: ") + header->label(i) + tr(" under processing...\n") + QString::number(i) + tr(" of ") + QString::number(numRows) + tr(" columns processed..."));
			    progressDlg->setProgress(++progress);
			}
			progressDlg->close();
			delete progressDlg;
			progressDlg = 0;
		    }
		    else if(chkgenotype->isChecked())
		    {
			progressDialogSettings(&progressDlg,numRows,tr("Individual wise operation"),tr("Progress Label"));
			for(int j = 0; j < numRows; ++j, ++i)
			{
			    k = 0;
			    sampletab->setText(j,k,header->label(i));
			    genotypeCalculation(i,tableView->numRows(),j,k);
			    progressDlg->setLabelText(tr("Individual: ") + header->label(i) + tr(" under processing...\n") + QString::number(j + 1) + tr(" of ") + QString::number(numRows) + tr(" columns processed..."));
			    progressDlg->setProgress(++progress);
			}
			progressDlg->close();
			delete progressDlg;
			progressDlg = 0;
		    }
		    else if(chkcrypticreln->isChecked())
		    {
			progressDialogSettings(&progressDlg,tableView->numRows(),tr("Individual wise operation"),tr("Progress Label"));
			progressDlg->setAutoReset(true);
			if(loadbadGenotypes())
			{
			    for(int j = 0; j < numRows; ++j, ++i)
			    {
				k = 0;
				sampletab->setText(j,k,header->label(i));
				crypticrelnCalculation(i,j,k,j + 1,numRows);
			    }
			}
			else
			    QMessageBox::information(this,tr("Empty settings"),tr("You must give threshold value and atleast one undefined genotype value!"));
			progressDlg->close();
			delete progressDlg;
			progressDlg = 0;
		    }
		}
		else //SNIP wise
		{
		    QTime t1,t2;
		    int h = 0, m = 0, s = 0, ms = 0;
		    double timeWidth = double(15*60)/double(909622);
		    int approxTime = (int)ceil(double(timeWidth*(numRows/60.0)));
		    QMessageBox::information(this,tr("Important!"),tr("This operation will take approximately ") + QString::number(approxTime) + " minutes more or less\n depending on available strength of the system\nSo, let system to work without any interruption...");
		    if(numRows > 500000)
			t1 = QTime::currentTime();
		    if(saveTableTemporarily(viewName,"temp_" + viewName,numRows))
		    {
			bool mG,mA,mH,mC;
			int thitr = 0;

			mG = mA = mH = mC = false;
			if(chkgenotype->isChecked())
			    mG = true;
			if(chkallelefreq->isChecked())
			    mA = true;
			if(chkhwe->isChecked())
			    mH = true;
			if(chkcrypticreln->isChecked())
			    mC = true;
			threadRun **thRun;
			thRun = new threadRun *[numThreadReq];
			for(thitr = 0; thitr < numThreadReq - 1; ++thitr)
			{
			    thRun[thitr] = new threadRun(thitr,QString::number(thitr) + "temp_" + viewName,retVal,mG,mA,mH,mC);
			    thRun[thitr]->start();
			}
			thRun[numThreadReq - 1] = new threadRun(numThreadReq - 1,QString::number(numThreadReq - 1) + "temp_" + viewName,retVal,mG,mA,mH,mC);
			thRun[numThreadReq - 1]->start();
			for(thitr = 0; thitr < numThreadReq; ++thitr)
			    thRun[thitr]->wait();
			
			fscanVal = system(tr("rm -f ") + viewName);
			for(thitr = 0; thitr < numThreadReq; ++thitr)
			{
			    fscanVal = system("rm -f [0-9]temp_" + viewName);
			    delete thRun[thitr];
			    fscanVal = system(tr("cat ") + QString::number(thitr) + QString::number(thitr) + tr("temp_") + viewName + tr(" >> ") + viewName);
			}
			
			for(thitr = 0; thitr < numThreadReq; ++thitr)
			    fscanVal = system(tr("rm -f ") + QString::number(thitr) + QString::number(thitr) + tr("temp_") + viewName);
			delete thRun;
			thRun = 0;
			if(numRows > 500000)
			{
			    QString str = "";
			    //Split 'viewName' 1 lacs by 1 lacs[START]
			    numThreadReq = numRows/500000;
			    numThreadReq = (numRows%500000 == 0 ? numThreadReq : numThreadReq + 1);
			    tableSizeArr = new int[numThreadReq];
			    for(thitr = 0; thitr < numThreadReq - 1; ++thitr)
			    {
				str = tr("head -") + QString::number((thitr + 1)*500000) + tr(" ") + viewName + tr(" | tail -500000 - > ") + QString::number(thitr) + tr("temp_") + viewName;
				fscanVal = system(str);
				tableSizeArr[thitr] = 500000;
			    }
			    str = tr("tail -") + QString::number(numRows - (numThreadReq - 1)*500000) + tr(" ") + viewName + tr(" > ") + QString::number(numThreadReq - 1) + tr("temp_") + viewName;
			    fscanVal = system(str);
			    tableSizeArr[numThreadReq - 1] = numRows - (numThreadReq - 1)*500000;
			    //Split 'viewName' 1 lacs by 1 lacs[STOP]
			    t2 = QTime::currentTime();
			    ms = (t2.hour()*360000 + t2.minute()*6000 + t2.second()*100 + t2.msec()) - (t1.hour()*360000 + t1.minute()*6000 + t1.second()*100 + t1.msec());
			    s = ms/100; ms %= 100;
			    m = s/60; s %= 60;
			    h = m/60; m %= 60;
			    QString estTime = tr("Time elapsed: ") + QString::number(h) + tr(":") + QString::number(m) + tr(":") + QString::number(s) + tr(":") + QString::number(ms) + tr("[HH:MM:SS:MS]");
			    cmdgenerate->setText("Result");
			    QMessageBox::information(this,tr("Urgent Alert!"),estTime + tr("\nSince the table: ") + viewName + tr(" consumes huge memory.\nIt contains ") + QString::number(numRows) + tr(" rows...\nTo reduce memory consumption, the table is required to close.\nClose it manually, then click on \'Result\' button to see result."));
			}
			else
			{
			    generateResultTable(numRows,numCols);
			    load_fileIndex = -1;
			    loadResult(numRows);
			}
		    }
		}
	    }
	    else
		QMessageBox::information(this,tr("Valid table?"),tr("The table given may not be the 'Genotype Table'\nKindly choose correct table"));
	}
	else
	    QMessageBox::information(this,tr("Table isn't open!"),tr("The table: ") + viewName + tr(" isn't open."));
    }
}

void summarydata::initiateStatusDialog()
{
    statDlg = new QDialog(0,"Status Dialog",true,WDestructiveClose);
    statDlg->setGeometry(statDlg->x(),statDlg->y(),50,25);
    statDlg->show();
}

int summarydata::istheTableforGenotypeData()
{
    int numcols = 0,val = 0;
    bool ok = false, Flag = false; 
    int nextFlag = 0;
    QString colVal = "";
    register int i = 0;
    
    if(tableView->numRows())
    {
	numcols = tableView->numCols();
	colVal = (tableView->value(0,0)).toString();
	val = colVal.toInt(&ok);
	if(ok) //val contains integer 
	{
	    if(val >= 0) //may be 'index' field
	    {
		colVal = (tableView->value(0,1)).toString(); //second field is expected to be 'snp id' field
		colVal = colVal.upper();
		if((int)colVal.length() > 2) //may be 'SNPID' field
		{
		    //Let's check further
		    if(colVal.contains(tr("SNP")) || colVal.contains(tr("RS")))
			Flag = 1;
		    ok = false;
		    val = colVal.toInt(&ok);
		    if(ok)
			Flag = 1;
		}
	    }

	    if(!Flag)
	    {
		colVal = colVal.upper();
		//if((int)colVal.length() > 2) //may be 'SNPID' field
		//{
		    //Let's check further
		if(colVal.contains(tr("SNP")) || colVal.contains(tr("RS")))
		    Flag = 2;
		val = colVal.toInt(&ok,10);
		if(ok)
		    Flag = 2;
		//}
	    }
	}
	
	if(Flag)
	{
	    for(i = 2; i < numcols; ++i)
	    {
		colVal = (tableView->value(0,i)).toString();
		colVal.remove(" ");
		if((int)colVal.length() == 2) //may be 'genotype value'
		{
		    colVal = colVal.upper();
		    QChar ch1 = colVal.at(0);
		    QChar ch2 = colVal.at(1);
		    if(((ch1 == 'A' || ch1 == 'B' || ch1 == 'T' || ch1 == 'C' || ch1 == 'G' || ch1 == 'X') && (ch2 == 'A' || ch2 == 'B' || ch2 == 'T' || ch2 == 'C' || ch2 == 'G' || ch2 == 'X')) || (colVal.isNull()))
			nextFlag = true;
		    else
			nextFlag = false;
		}
		else if(colVal.isEmpty())
		    nextFlag = true;
		else
		    nextFlag = false;
		if(!nextFlag)
		    break;
	    }
	}
    }
    return (nextFlag ? Flag : 0);
}

bool summarydata::saveTableTemporarily(QString saveTab, QString saveFileName,int numRows)
{
    bool retStat = false;
    
    QString query = "mysql " + sqlDB->databaseName() + " -B -e \"select * from " + saveTab + ";\" | sed 's/\t/,/g'  > " + saveFileName;
    int status = system(query);
    if(status > -1)
    {
	//~~~~Now split the file in 10 parts
	//This part only to set progressDialog's max value[START]
	segmentSize = 0;
	QString query = "", temp_saveFile = "";
	
	temp_saveFile = saveFileName;	
	if(numRows < 10)
	{
	    segmentSize = lastSegmentSize = 1;
	    numThreadReq = numRows;
	}
	else
	{
	    segmentSize = (int)ceil(double(numRows)/10.0);
	    numThreadReq = numRows/segmentSize;
	    lastSegmentSize = (numRows%numThreadReq == 0 ? segmentSize : numRows - segmentSize*numThreadReq);
	    numThreadReq += (numRows%numThreadReq == 0 ? 0 : 1);
	}
	for(int i = 0; i < numThreadReq - 1; i++)
	{
	    query = "tail -" + QString::number(numRows - segmentSize*i) + " " + saveFileName + " | head -" + QString::number(segmentSize) + "  > " + QString::number(i) + temp_saveFile;
	    fscanVal = system(query);
	}
	query = "tail -" + QString::number(numRows - segmentSize*(numThreadReq - 1)) + " " + saveFileName + " > " + QString::number(numThreadReq - 1) + temp_saveFile;
	fscanVal = system(query);
	fscanVal = system("rm -f " + temp_saveFile);
	//This part only to set progressDialog's max value[STOP]
	retStat = true;
    }
    return retStat;
}

void summarydata::generateResultTable(int numRows, int numCols)
{
    if(sampletab)
	deleteTheResultTable();
    sampletab = new QTable(numRows,numCols,wSpace,tr("Summary Table"));
    sampletab->setGeometry(vb->x() + 1, vb->y() + 1, vb->width() - 8, vb->height() - 8);
    sampletab->setVScrollBarMode(QScrollView::AlwaysOn);
    sampletab->setColumnLabels(columnList);
    sampletab->show();
}

void summarydata::deleteTheResultTable()
{
    if(sampletab->isShown())
	sampletab->close();
    delete sampletab;
    sampletab = 0;
}

void summarydata::genotypeCalculation(int index, int numItr, int &row, int &col)
{
    o_aa = 0, o_ab = 0, o_bb = 0, noCall = 0.0;

    QString query = "select " + viewCur->fieldName(index) + " from " + viewName + " where " + viewCur->fieldName(index);
    QSqlQuery squery(query + "='AA';",sqlDB);
    if(squery.isActive())
	o_aa = squery.numRowsAffected();
    if(o_aa == 0)
    {
	if(squery.exec(query + "='A A';") && squery.isActive())
	    o_aa = squery.numRowsAffected();
    }
    if(squery.exec(query + "='AB';") && squery.isActive())
	o_ab = squery.numRowsAffected();
    if(o_ab == 0)
    {
	if(squery.exec(query + "='A B';") && squery.isActive())
	    o_ab = squery.numRowsAffected();
    }
    if(squery.exec(query + "='BB';") && squery.isActive())
	o_bb = squery.numRowsAffected();
    if(o_bb == 0)
    {
	if(squery.exec(query + "='B B';") && squery.isActive())
	    o_bb = squery.numRowsAffected();
    }
    
    noCall = numItr - (o_aa + o_ab + o_bb);
    sampletab->setText(row,++col,QString::number((o_aa/double(numItr))*100.0));
    sampletab->setText(row,++col,QString::number((o_ab/double(numItr))*100.0));
    sampletab->setText(row,++col,QString::number((o_bb/double(numItr))*100.0));
    sampletab->setText(row,++col,QString::number(numItr - noCall));
    //cout << row << endl;
}

void summarydata::allelefreqCalculation(int &row, int &col)
{
    p = (double(2*o_aa + o_ab)/double(2.0*(o_aa + o_ab + o_bb)));
    q = 1.0 - p;
    sampletab->setText(row,++col,QString::number(p*100.0));
    sampletab->setText(row,++col,QString::number(q*100.0));
}

void summarydata::hweCalculation(int &row, int &col)
{
    double n = (o_aa + o_ab + o_bb), e_aa, e_ab, e_bb;
    double chi_sq;

    e_aa = p*p*n;
    e_ab = p*q*n;
    e_bb = q*q*n;
    chi_sq = pow((o_aa - e_aa),2.0)/e_aa + pow((o_ab - e_ab),2.0)/e_ab + pow((o_bb - e_bb),2.0)/e_bb;
    sampletab->setText(row,++col,QString::number(chi_sq));
}

void summarydata::loadResult(int load_Records)
{
    int row = 0;
    int col = 0;
    int progress = 0;
    int response = -1;
    QString readBuf = "",readFileName = "";
    QStringList readbufList;
                
    progressDlg = 0;
    progressDialogSettings(&progressDlg,load_Records,tr("SNP wise operation"),tr("Progress Label"));
    progressDlg->setAutoReset(true);	
    //~~~~~Now load result data from file to table[START]
    if(load_fileIndex == -1)
    {
	//cout << tr("Loading data from file: ") + viewName + tr(" ...") << endl;
	progressDlg->setLabelText(tr("Loading data from file: ") + viewName + tr(" ..."));
	readFileName = viewName;
    }
    else
    {
	progressDlg->setLabelText(tr("Loading data from file: ") + QString::number(load_fileIndex + 1) + tr(" of ") + QString::number(numThreadReq) + tr(" ..."));
	readFileName = QString::number(load_fileIndex) + tr("temp_") + viewName;
    }
    
    QFile fpRead(readFileName);
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	row = 0;
	progress = 0;
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	while(!fpIn.atEnd())
	{
	    progressDlg->setProgress(++progress);
	    progressDlg->show();
	    readBuf = fpIn.readLine();
	    readbufList = QStringList::split('\t',readBuf);
	    col = 0;
	    for(QStringList::iterator it = readbufList.begin(); it != readbufList.end(); ++it, ++col)
		sampletab->setText(row,col,*it);
	    ++row;
	}
	fpRead.close();
    }
    progressDlg->close();
    delete progressDlg;
    progressDlg = 0;
    //~~~~~Now load result data from file to table[STOP]
    
    if(load_fileIndex == numThreadReq - 1 && cmdgenerate->text() == "Next")
    {
	response = QMessageBox::question(this,tr("What to do next?"),tr("We have reached end of table.\n Do you want to see result from "),tr("Start?"), tr("End?"), tr("back to generate?"), 1, 2);
	switch(response)
	{
	case 0:
	    cmdgenerate->setText("Start");
	    break;
	case 1:
	    cmdgenerate->setText("End");
	    break;
	case 2:
	    load_fileIndex = -1;
	    cmdgenerate->setText("Generate");
	    delete tableSizeArr;
	    tableSizeArr = 0;
	    break;
	}
    }
    else if(load_fileIndex == 0 && cmdgenerate->text() == "Prev")
    {
	response = QMessageBox::question(this,tr("What to do next?"),tr("We have reached end of table.\n Do you want to see result from "),tr("Start?"), tr("End?"), tr("back to generate?"), 1, 2);
	switch(response)
	{
	case 0:
	    cmdgenerate->setText("Start");
	    break;
	case 1:
	    cmdgenerate->setText("End");
	    break;
	case 2:
	    load_fileIndex = -1;
	    cmdgenerate->setText("Generate");
	    delete tableSizeArr;
	    tableSizeArr = 0;
	    break;
	}
    }
}

bool summarydata::notSufficientMemory(long int &recSlabSize)
{
    char Mem[12],kB[3];
    int totMem = 0,availMem = 0;
    FILE *fp;
   
    //Memory available in the system[START]
    fscanVal = system("cat /proc/meminfo > .meminfo.txt");
    if((fp = fopen(".meminfo.txt","r")) != NULL)
    {
	fscanVal = fscanf(fp,"%s%d%s\n",Mem,&totMem,kB);
	fscanVal = fscanf(fp,"%s%d%s\n",Mem,&availMem,kB);
	fclose(fp);
	fp = 0;
    }
    //Memory available in the system[STOP]
    //Percentage memory available multiplied by total memory available gives absolute amount of memory available. 
    //For each 1 mb memory available 500 records are allocted
    recSlabSize = (long int)round(((double(availMem)/double(totMem))*100.0)*totMem);
    //cout << "\nrecSlabSize: " << recSlabSize << endl;
    //cout << int((double(availMem)/double(totMem))*100.0) << endl;
    if((double(availMem)/double(totMem))*100.0 > 10.0)
	return false;
    else
	return true;
}

void summarydata::cmdcryptic_clicked()
{
    frmcrypticsettings *crypset = new frmcrypticsettings(this,"crypticsettings",WDestructiveClose);
    crypset->show();
}

void summarydata::crypticrelnCalculation(int index, int rowIndx, int colIndx, int padding,int totalIndiv)
{
    double percentVal = 0.0;
    bool state= true;
    int frequency = 0,rejCount = 0;
    int totalRows = 0, totalColumns = 0;
    int progress = 0;
            
    if((int)badGeno.count())
    {
	QHeader *header = tableView->horizontalHeader();
	totalRows = tableView->numRows();
	totalColumns = tableView->numCols();
	for(int i = 0; i < padding; ++i)
	    sampletab->setText(rowIndx,++colIndx,"NA");
	for(int col = index + 1; col < totalColumns; ++col)
	{
	    for(int row = 0; row < totalRows; ++row)
	    {
		for(QStringList::iterator it = badGeno.begin(); it != badGeno.end(); ++it)
		{
		    if(*it == tableView->text(row,col) || *it == tableView->text(row,index))
		    {
			state = false;
			break;
		    }
		}
		
		if(state && tableView->text(row,col) == tableView->text(row,index))
		    ++frequency;
		else
		    ++rejCount;
		state = true;
		if(row%1000 == 0 || row == totalRows - 1)
		{
		    progressDlg->setLabelText(tr("Individual: ") + header->label(index) + tr("\n") + QString::number(padding) + tr(" of ") + QString::number(totalIndiv) + tr(" individuals under processing...\nCurrent: ") + header->label(index) + tr(" --> ") + header->label(col) + tr("\n") + QString::number(row + 1) + tr(" of ") + QString::number(totalRows));
		    progressDlg->setProgress(++progress);
		}
		else
		    ++progress;
	    }
	    percentVal = (double(frequency)/double(tableView->numRows() - rejCount))*100.0;
	    if(percentVal >= thresholdVal)
		sampletab->setText(rowIndx,++colIndx,QString::number(percentVal));
	    else
		sampletab->setText(rowIndx,++colIndx,QString::number(00));
	    //cout << percentVal << endl;
	    frequency = 0;
	    rejCount = 0;
	    progress = 0;
	}
    }
    else
	QMessageBox::information(this,tr("No undefined genotype"),tr("No undefined genotype value given!"));
}

bool summarydata::loadbadGenotypes()
{
    bool ok = false, flag = false;
    QString rowScan = "";
    
    thresholdVal = 0.0;
    badGeno.clear();
    QFile fpRead(".crypticsettings.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	if(!fpIn.atEnd())
	{
	    flag = false;
	    rowScan = fpIn.readLine();
	    thresholdVal = rowScan.toDouble(&ok);
	    while(!fpIn.atEnd())
	    {
		rowScan = fpIn.readLine();
		badGeno.append(rowScan);
	    }
	    if((int)badGeno.count())
		flag = true;
	}
	fpRead.close();
    }
    return flag;
}

void summarydata::closeAnyOpenTable()
{
    if(tableView)
    {
	tableView->close();
	if(viewCur)
	{
	    viewCur->clear();
	    delete viewCur;
	    viewCur = 0;
	}
	delete tableView;
	fscanVal = system("sleep 5s");
	tableView = 0;
    }
}

void summarydata::cmdsave_clicked()
{
    bool saveStat = false;
    QString readBuf = "";
    int progress = 1;
    QStringList filterList;
    QFileDialog* saveFD = 0;
    QString saveFilename = "",filePath = "";
    
    if(sampletab && sampletab->isShown())
    {
	filterList.append(tr("TGT Files (*.txt)"));
	saveFD = new QFileDialog(this, "Give file name", true);
	saveFD->setMode(QFileDialog::AnyFile);
	saveFD->setFilters(filterList);
	saveFD->setInfoPreviewEnabled(true);
	if(saveFD->exec() == QDialog::Accepted)
	{
	    saveFD->close();
	    saveFilename = saveFD->selectedFile();
	    if(saveFilename.isEmpty())
		QMessageBox::information(this,tr("Table saving status"),tr("No file name given"),QMessageBox::Ok);
	    else
	    {
		QString str = saveFilename.section('/',-1);
		filePath = saveFilename.section('/',0,saveFilename.contains('/') - 1);
		if(str.contains(tr(".txt")) > 0)
		    str.remove(tr(".txt"));
		if(checkTbNameSyntax(str))
		{
		    saveFilename = filePath + "/" + str + tr(".txt");
		    if(rdIndividual->isChecked())
		    {
			QHeader *header = sampletab->horizontalHeader();
			for(int i = 0; i < header->count() - 1; ++i)
			    readBuf += header->label(i) + "\t";
			readBuf += header->label(header->count() - 1) + "\n";
			progressDlg = 0;
			progressDialogSettings(&progressDlg,numThreadReq,tr("Individual wise operation"),tr("Progress Label"));
			QFile fpWrite(saveFilename);
			if(fpWrite.open(IO_WriteOnly | IO_Truncate))
			{
			    QTextStream fpOut(&fpWrite);
			    fpOut.setEncoding(QTextStream::UnicodeUTF8);
			    fpOut << readBuf;
			    for(int i = 0; i < sampletab->numRows(); ++i)
			    {
				for(int j = 0; j < sampletab->numCols() - 1; ++j)
				    fpOut << sampletab->text(i,j) << "\t";
				fpOut << sampletab->text(i,sampletab->numCols() - 1) << "\n";
				progressDlg->setLabelText(tr("Loading data from file: ") + QString::number(i + 1) + tr(" of ") + QString::number(sampletab->numRows()) + tr(" rows completed..."));
				progressDlg->setProgress(++progress);
			    }
			    fpWrite.close();
			    saveStat = true;
			    progressDlg->close();
			    delete progressDlg;
			    progressDlg = 0;
			}
		    }
		    else if(rdsnip->isChecked())
		    {
			QString readBuf = "";
			
			QHeader *header = sampletab->horizontalHeader();
			for(int i = 0; i < header->count() - 1; ++i)
			    readBuf += header->label(i) + "\t";
			readBuf += header->label(header->count() - 1) + "\n";
			QFile fpWrite(saveFilename);
			if(fpWrite.open(IO_WriteOnly | IO_Truncate))
			{
			    QTextStream fpOut(&fpWrite);
			    fpOut.setEncoding(QTextStream::UnicodeUTF8);
			    fpOut << readBuf;
			    fpWrite.close();
			    
			    if(load_fileIndex == -1)
				fscanVal = system(tr("cat ") + viewName + tr(" >> ") + saveFilename);
			    else
			    {
				for(int thitr = 0; thitr < numThreadReq; ++thitr)
				    fscanVal = system(tr("cat ") + QString::number(thitr) + tr("temp_") + viewName + tr(" >> ") + saveFilename);
			    }
			    saveStat = true;
			}
		    }
		}
		else
		    QMessageBox::information(this,tr("Table naming status"),tr("Problem with given name! \nTable name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
		if(saveStat)
		    QMessageBox::information(this,tr("Table import status"),tr("Table has been saved in a tab generated datafile " + saveFilename +"\nunder the directory: '" + filePath + "'\nImport that datafile using standard import method provided"));
	    }
	}
	delete saveFD;
	saveFD = 0;
    }
    else
	QMessageBox::information(this,tr("No table!"),tr("No open table found!"));
}

void summarydata::cmdExport_clicked()
{
    bool saveStat = false;
    QString readBuf = "";
    int progress = 1;
    QStringList filterList;
    QFileDialog* saveFD = 0;
    QString saveFilename = "",filePath = "";
    
    if(sampletab && sampletab->isShown())
    {
	filterList.append(tr("TGT Files (*.txt)"));
	
	saveFD = new QFileDialog(this, "Give file name", true);
	saveFD->setMode(QFileDialog::AnyFile);
	saveFD->setFilters(filterList);
	saveFD->setInfoPreviewEnabled(true);
	if(saveFD->exec() == QDialog::Accepted)
	{
	    saveFD->close();
	    saveFilename = saveFD->selectedFile();
	    if(saveFilename.isEmpty())
		QMessageBox::information(this,tr("Table export status"),tr("No file name given"),QMessageBox::Ok);
	    else
	    {
		QString str = saveFilename.section('/',-1);
		filePath = saveFilename.section('/',0,saveFilename.contains('/') - 1);
		if(str.contains(tr(".txt")) > 0)
		    str.remove(tr(".txt"));
		if(checkTbNameSyntax(str))
		{
		    saveFilename = filePath + "/" + str + tr(".txt");
		    if(rdIndividual->isChecked() && sampletab->isShown())
		    {
			QHeader *header = sampletab->horizontalHeader();
			for(int i = 0; i < header->count() - 1; ++i)
			    readBuf += header->label(i) + "\t";
			readBuf += header->label(header->count() - 1) + "\n";
			progressDlg = 0;
			progressDialogSettings(&progressDlg,numThreadReq,tr("Individual wise operation"),tr("Progress Label"));
			QFile fpWrite(saveFilename);
			if(fpWrite.open(IO_WriteOnly | IO_Truncate))
			{
			    QTextStream fpOut(&fpWrite);
			    fpOut.setEncoding(QTextStream::UnicodeUTF8);
			    fpOut << readBuf;
			    for(int i = 0; i < sampletab->numRows(); ++i)
			    {
				for(int j = 0; j < sampletab->numCols() - 1; ++j)
				    fpOut << sampletab->text(i,j) << "\t";
				fpOut << sampletab->text(i,sampletab->numCols() - 1) << "\n";
				progressDlg->setLabelText(tr("Loading data from file: ") + QString::number(i + 1) + tr(" of ") + QString::number(sampletab->numRows()) + tr(" rows completed..."));
				progressDlg->setProgress(++progress);
			    }
			    fpWrite.close();
			    saveStat = true;
			    progressDlg->close();
			    delete progressDlg;
			    progressDlg = 0;
			}
		    }
		    else if(rdsnip->isChecked() && sampletab->isShown())
		    {
			QString readBuf = "";
			
			QHeader *header = sampletab->horizontalHeader();
			for(int i = 0; i < header->count() - 1; ++i)
			    readBuf += header->label(i) + "\t";
			readBuf += header->label(header->count() - 1) + "\n";
			QFile fpWrite(saveFilename);
			if(fpWrite.open(IO_WriteOnly | IO_Truncate))
			{
			    QTextStream fpOut(&fpWrite);
			    fpOut.setEncoding(QTextStream::UnicodeUTF8);
			    fpOut << readBuf;
			    fpWrite.close();
			    
			    if(load_fileIndex == -1)
				fscanVal = system(tr("cat ") + viewName + tr(" >> ") + saveFilename);
			    else
			    {
				for(int thitr = 0; thitr < numThreadReq; ++thitr)
				    fscanVal = system(tr("cat ") + QString::number(thitr) + tr("temp_") + viewName + tr(" >> ") + saveFilename);
			    }
			    saveStat = true;
			}
		    }
		}
		else
		    QMessageBox::information(this,tr("Table naming status"),tr("\nProblem with given name! \nTable name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
		if(saveStat)
		    QMessageBox::information(this,tr("Table export status"),tr("Table has been saved in required format in the data file " + saveFilename +"\nunder the directory: '" + filePath));
	    }
	}
	delete saveFD;
	saveFD = 0;
    }
    else
	QMessageBox::information(this,tr("No table!"),tr("No open table found!"));
}

bool summarydata::checkTbNameSyntax(QString tabName)
{
    bool ok = true;
    int length = tabName.length();
    QChar ch;
    
    for(register int i = 0; i < length; ++i)
    {
	ch = tabName.at(i);
	if(!((ch >= 'A' && ch <= 'Z') || (ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')))
	{
	    ok = false;
	    break;
	}
    }
    return ok;
}
//~~~~~~~Option Button Controls[START]~~~~~~~~~
void summarydata::rdIndividual_toggled(bool state)
{
    if(state)
    {
	chkallelefreq->setEnabled(false);
	chkhwe->setEnabled(false);
	chkcrypticreln->setEnabled(true);
	chkgenotype->setChecked(false);
	chkallelefreq->setChecked(false);
	chkhwe->setChecked(false);
	chkgenotype->setChecked(false);
    }
}

void summarydata::rdsnip_toggled(bool state)
{
    if(state)
    {
	chkallelefreq->setEnabled(false);
	chkhwe->setEnabled(false);
	chkcrypticreln->setChecked(false);
	chkcrypticreln->setEnabled(false);
	chkgenotype->setChecked(false);
	chkallelefreq->setChecked(false);
	chkhwe->setChecked(false);
	chkgenotype->setChecked(false);
    }
}

void summarydata::chkgenotype_toggled(bool state)
{
    if(rdIndividual->isChecked())
    {
	if(state)
	    chkcrypticreln->setChecked(false);
    }
    else if(rdsnip->isChecked())
    {
	if(state)
	    chkallelefreq->setEnabled(true);
	else
	    chkallelefreq->setEnabled(false);
	chkallelefreq->setChecked(false);
	chkhwe->setEnabled(false);
    }
}

void summarydata::chkallelefreq_toggled(bool state)
{
    if(state)
	chkhwe->setEnabled(true);
    else
	chkhwe->setEnabled(false);
    chkhwe->setChecked(false);
}

void summarydata::chkcrypticreln_toggled(bool state)
{
    if(rdIndividual->isChecked())
    {
	if(state)
	    chkgenotype->setChecked(false);
    }
    
    if(state)
	cmdcryptic->setEnabled(true);
    else
	cmdcryptic->setEnabled(false);
}
//~~~~~~~Option Button Controls[STOP]~~~~~~~~~
//~~~~~Progress Dialog Settings[START]~~~~~~~~~
void summarydata::progressDialogSettings(QProgressDialog **progressDlg,int totalSteps, QString title, QString label)
{
    *progressDlg = new QProgressDialog(0,tr(title),true,WDestructiveClose);
    (*progressDlg)->setTotalSteps(totalSteps);
    lblProgress = new QLabel(*progressDlg,label,WDestructiveClose);
    (*progressDlg)->setLabel(lblProgress);
    (*progressDlg)->setAutoReset(false);
}
//~~~~~~Progress Dialog Settings[STOP]~~~~~~~~~
void summarydata::closeEvent(QCloseEvent *e)
{
    if(sampletab)
    {
	//QMessageBox::information(this,tr("Table closing message"),tr("Time required to close the data table."));
	deleteTheResultTable();
	//Also delete any intermediate file created[START]
	for(int thitr = 0; thitr < numThreadReq; ++thitr)
	    fscanVal = system(tr("rm -f ") + QString::number(thitr) + tr("temp_") + viewName);
	fscanVal = system(tr("rm -f ") + viewName);
	//Also delete any intermediate file created[STOP]
    }
    summarydata::closeFlag = true;
    e->accept();
}
