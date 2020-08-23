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
#include <stdio.h>
#include <iostream.h>
#include <qstring.h>
#include <qmessagebox.h>

void frmFileRename::init()
{
    lstFile1->clear();
    lstFile1->setReadOnly(true);
    //lstFile1->setSelectionMode(QListBox::Extended);
    lstFile2->clear();
    //lstFile2->setSelectionMode(QListBox::Extended);
    lstFile3->clear();
    //~~~~~~~~~~~~~~~~[START]
    FILE *fpIn;
    int numRec = 0;
    char str[100];
    QString strList;
    
    fpIn = fopen("strtoreplace.txt","r");
    if(fpIn != NULL)
    {
	fscanf(fpIn,"%d",&numRec);
	if(numRec)
	{
	    strList = "";
	    for(int i = 0; i < numRec; ++i)
	    {
		fscanf(fpIn,"%s",str);
		strList += QString(str) + "\n";
	    }
	    lstFile2->clear();
	    lstFile2->append(strList);
	}
	fclose(fpIn);
	fpIn = NULL;
    }
    //~~~~~~~~~~~~~~~~[STOP]
}

void frmFileRename::initSetting(QStringList fileNameList, QString pDir)
{
    QString str = fileNameList.join("\n");
    lstFile1->append(str);
    lblfileList->setText("(" + QString::number((int)lstFile1->lines()) + ")");
    pathDir = pDir;
    pathDir.replace(" ","\\ "); 
}

void frmFileRename::cmdAdd_clicked()
{
    QStringList strList;
    QString Text = lstFile1->selectedText();
    
    for(int i = 0; i < lstFile2->lines(); ++i)
    {
	if(lstFile2->text(i) != " ")
	    strList.append(lstFile2->text(i));
    }
    strList.append(Text);
    Text = strList.join("\n");
    lstFile2->setText(Text);
}

void frmFileRename::cmdChange_clicked()
{
    QString str1, str2, fileName;
    QStringList reFileList;
    
    lstFile3->clear();
    reFileList.clear();
    for(int i = 0; i < lstFile1->lines(); ++i)
    {
	fileName = lstFile1->text(i);
	for(int j = 0; j < lstFile2->lines(); ++j)
	{
	    str1 = lstFile2->text(j);
	    str2 = str1.stripWhiteSpace();
	    fileName.remove(str2);
	    fileName = fileName.stripWhiteSpace();
	    if(fileName.contains(".txt") == 0)
		fileName.append(".txt");
	}
	reFileList.append(fileName);
    }
    lstFile3->insertStringList(reFileList);
    lblRenamedList->setText("(" + QString::number((int)lstFile3->count()) + ")");
}

void frmFileRename::cmdConfirm_clicked()
{
    QString str = "",str1,str2;
    pathDir.replace("(","\(");
    
    for(int i = 0; i < lstFile1->lines(); ++i)
    {
	str1 = lstFile1->text(i);
	str1 = str1.stripWhiteSpace();
	str1.replace("(","\\(");str1.replace("{","\\{");
	str1.replace(")","\\)");str1.replace("}","\\}");
	str2 = lstFile3->text(i);
	str2 = str2.stripWhiteSpace();
	str2.replace("(","\\(");str2.replace("{","\\{");
	str2.replace(")","\\)");str2.replace("}","\\}");
	str = tr("rename ") + pathDir + tr("/") + str1 + tr(" ") + pathDir + tr("/") + str2 + tr(" ") + pathDir + tr("/") + str1;
	fscanVal = system(str);
    }
    //~~~~~~~~~~~~~~~~[START]
    FILE *fpOut;
    fpOut = fopen("strtoreplace.txt","w");
    fprintf(fpOut,"%d\n",lstFile2->lines());
    for(int i = 0; i < lstFile2->lines(); ++i)
	fprintf(fpOut,"%s\n",(lstFile2->text(i)).ascii());
    fclose(fpOut);
    fpOut = NULL;
    //~~~~~~~~~~~~~~~~[STOP]
    QMessageBox::information(this,"File rename","All files given renamed successfully");
}

void frmFileRename::cmdOK_clicked()
{
    fileNameLst.clear();
    for(int i = 0; i < (int)lstFile3->count(); ++i)
	fileNameLst.append(lstFile3->text(i));
    accept();
}

void frmFileRename::cmdCancel_clicked()
{
    fileNameLst.clear();
    accept();
}
