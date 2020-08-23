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
#include <qapplication.h>
#include <iostream.h>
#include <stdio.h>
#include <qstring.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qprogressdialog.h>
#include <qmessagebox.h>

void frmdatarearrange::init()
{
    txtkeyfile->setReadOnly(true);
    txtdatafile->setReadOnly(true);
    txtoutfile->setReadOnly(true);
}

void frmdatarearrange::initSettings(QString wDir)
{
    workDir = wDir;
}
void frmdatarearrange::cmdkeyfile_clicked()
{
    QString dirPath;
    //Some prerequisites
    dirPath = workDir + "/data/";
    //Some prerequisites[END]
    
    QString fn = QFileDialog::getOpenFileName(dirPath,"Text (*.txt)",this,"Open file dialog","Choose appropriate key file");
    //cout << fn << endl;
    if(!fn.isEmpty()) 
	txtkeyfile->setText(fn);
}

void frmdatarearrange::cmddatafile_clicked()
{
    QString dirPath;
    //Some prerequisites
    dirPath = workDir + "/data/";
    //Some prerequisites[END]
    
    QString fn = QFileDialog::getOpenFileName(dirPath,"Text (*.txt)",this,"Open file dialog","Choose appropriate data file");
    //cout << fn << endl;
    if(!fn.isEmpty()) 
	txtdatafile->setText(fn);
}


void frmdatarearrange::cmdoutfile_clicked()
{
    QString dirPath;
    //Some prerequisites
    dirPath = workDir + "/data/";
    //Some prerequisites[END]
    
    QString fn = QFileDialog::getSaveFileName(dirPath,"Text (*.txt)",this,"Save file dialog","Choose a filename to save under");
    if(!fn.isEmpty())
    {
	QString strPath = "";
	
	strPath = fn.section('/',-1);
	if(!strPath.endsWith(".txt"))
	    fn += tr(".txt");
	txtoutfile->setText(fn);
    }
}

void frmdatarearrange::processKeyFile(QString &keyFile, int &totKeys)
{
    FILE *fpIn;
    int fieldsCount = 0;
    
    fscanVal = system("tail -1 " + txtkeyfile->text() + " | wc -w > .tempFields.txt");
    fpIn = fopen(".tempFields.txt","r");
    fscanVal = fscanf(fpIn,"%d",&fieldsCount);
    fclose(fpIn);
    fpIn = NULL;
    fscanVal = system("rm -f .tempFields.txt");
    if(fieldsCount > 1)
    {
	keyFile = (txtkeyfile->text()).section('/',0,(txtkeyfile->text()).contains('/') - 1) + "/tempFieldFile.txt"; 
	fscanVal = system("cut -f 2 " + txtkeyfile->text() + " > " + keyFile);
    }
    else
	keyFile = txtkeyfile->text();
    
    fscanVal = system("wc -l " + keyFile + " > .tempFields.txt");
    fpIn = fopen(".tempFields.txt","r");
    fscanVal = fscanf(fpIn,"%d",&totKeys);
    fclose(fpIn);
    fpIn = NULL;
    fscanVal = system("rm -f .tempFields.txt");
}

void frmdatarearrange::initiateOutFile()
{
    fscanVal = system("head -1 " + txtdatafile->text() + " > " + txtoutfile->text());
}

void frmdatarearrange::progressDialogSettings(QProgressDialog **progressDlg, int totalSteps, QString title, QString label)
{
    (*progressDlg) = new QProgressDialog(this,tr(title),true,WDestructiveClose);
    (*progressDlg)->setCaption(title);
    (*progressDlg)->setTotalSteps(totalSteps);
    lblProgress = new QLabel(*progressDlg,label,WDestructiveClose);
    (*progressDlg)->setLabel(lblProgress);
    (*progressDlg)->setAutoReset(true);
}

bool frmdatarearrange::datarearrangement()
{
    QProgressDialog *progressDlg = 0;
    QString keyFile = "", read_Key = "";
    int totKeys = 0, keyCount = 0, progress = 0;
    bool retStat = false;
    
    processKeyFile(keyFile,totKeys);
    initiateOutFile();
    QFile fpreadKey(keyFile);
    if(fpreadKey.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpredKey(&fpreadKey);
	fpredKey.setEncoding(QTextStream::UnicodeUTF8);
	fpredKey >> read_Key; //Read out header
	progressDialogSettings(&progressDlg,(totKeys - 1)/100,tr("Data re-arrangement"),tr("Operation is in progress"));
	while(!fpredKey.atEnd())
	{
	    fpredKey >> read_Key;
	    if(keyCount == 0 || keyCount%100 == 0)
	    {
		progressDlg->setProgress(++progress);
		progressDlg->setLabelText("Searching key: '" + read_Key + "'\n[Completed: " + QString::number(keyCount + 1) + " of " + QString::number(totKeys) + "]");
		progressDlg->show();
		appQ->processEvents();
	    }
	    fscanVal = system("grep '" + read_Key + "' " + txtdatafile->text() + " >> " + txtoutfile->text());
	    ++keyCount;
	    //cout << keyCount << endl;
	}
	progressDlg->close();
	fpreadKey.close();
	fscanVal = system("rm -f " +keyFile);
	retStat = true;
    }
    return retStat;
}

void frmdatarearrange::cmdOK_clicked()
{
    if(txtkeyfile->text() != "" && txtdatafile->text() != "" && txtoutfile->text() != "")
    {
	if(datarearrangement())
	    QMessageBox::information(this,tr("Operation successful"),tr("Dtata rearrangement operation is completed successfully.\nResult has been stored in the given output file"));
	close();
    }
    else
	QMessageBox::information(this,tr("Incomplete input"),tr("First ensure 'key', 'data' and 'out put' files are given. Then click 'OK' button"));
}


void frmdatarearrange::cmdCancel_clicked()
{
    close();
}
