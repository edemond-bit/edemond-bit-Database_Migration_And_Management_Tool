#include <qfiledialog.h>
#include <qstring.h>
#include <string.h>
#include <qfiledialog.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <iostream.h>
#include <qstringlist.h>
#include <qfile.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qregexp.h>
#include "./filerename/frmfilerename.h"

void frmInput::init()
{
    dlgStatus = false;
    importStat = false;
    indexreqStat = false;
    selectedFieldStat = false;
    transposeStat = false;
    plinkformatStat = false;
    valueChangeStat = false;
    illuminaFlag = false;
    chkOther->setChecked(true);
    chkindexing->setEnabled(false);
    lstFile1->setSelectionMode(QListBox::Extended);
    lstFile1->clear();
    lstFile2->setSelectionMode(QListBox::Extended);
    lstFile2->clear();
    dirPath = workDir + "/data/";
}

void frmInput::cmdBrowse_clicked()
{
    DIR *dip;
    struct dirent *dit;
    int counter;
    
    //Some prerequisites
    lstFile1->clear();
    lstFile2->clear();
    //Some prerequisites[END]
    
    fn = QFileDialog::getExistingDirectory(dirPath,this,"Open directory dialog","Choose appropriate directory");
    pathWDir = fn.section('/',0,fn.contains("/") - 1);
    cleanDirectory(fn);
    if(!fn.isEmpty()) 
    {
	dirPath = fn;
	txtFile->setText(fn);
	if(fn != "" && (dip = opendir(fn)) != NULL)
	{
	    counter = 0;
	    while((dit = readdir(dip)) != NULL)
	    {
		if(!strcmp(dit->d_name,"."))
		    continue;
		else if(!strcmp(dit->d_name,".."))
		    continue;
		else
		    lstFile1->insertItem(dit->d_name,counter++);
	    }
	    lblFileList->setText("(" + QString::number(counter) + ")");
	    closedir(dip);
	    dip = NULL;
	    lstFile1->sort();
	}
    }
}

void frmInput::cleanDirectory(QString dir)
{
    QString query;
    genrateCorrectFormat(&dir);
    query = "rm " + dir + "*~";
    scanVal = system(query);
    query = "rm " + dir + "*-import.txt";
    scanVal = system(query);
}

void frmInput::genrateCorrectFormat(QString *path)
{
    //~~~~~~~~Finding 'space', then mark them with 'escape' charecter [START]
    for(register int pivot = 0; pivot < (int)(*path).length(); ++++pivot)
    {
	pivot = (*path).find(tr(QString(" ")),pivot,false);
	if(pivot < 0)
	    break;
	(*path).insert(pivot,tr(QString("\\")));
	(*path).insert((*path).length() + 1,tr(QString("\0")));
    }
    //~~~~~~~~Finding 'space', then mark them with 'escape' charecter [STOP]
    //~~~~~~~~Finding '(', then mark them with 'escape' charecter [START]
    for(register int pivot = 0; pivot < (int)(*path).length(); ++++pivot)
    {
	pivot = (*path).find(tr(QString("(")),pivot,false);
	if(pivot < 0)
	    break;
	(*path).insert(pivot,tr(QString("\\")));
	(*path).insert((*path).length() + 1,tr(QString("\0")));
    }
    //~~~~~~~~Finding '(', then mark them with 'escape' charecter [STOP]
    //~~~~~~~~Finding ')', then mark them with 'escape' charecter [START]
    for(register int pivot = 0; pivot < (int)(*path).length(); ++++pivot)
    {
	pivot = (*path).find(tr(QString(")")),pivot,false);
	if(pivot < 0)
	    break;
	(*path).insert(pivot,tr(QString("\\")));
	(*path).insert((*path).length() + 1,tr(QString("\0")));
    }
    //~~~~~~~~Finding ')', then mark them with 'escape' charecter [STOP]
    //~~~~~~~~Finding '[', then mark them with 'escape' charecter [START]
    for(register int pivot = 0; pivot < (int)(*path).length(); ++++pivot)
    {
	pivot = (*path).find(tr(QString("[")),pivot,false);
	if(pivot < 0)
	    break;
	(*path).insert(pivot,tr(QString("\\")));
	(*path).insert((*path).length() + 1,tr(QString("\0")));
    }
    //~~~~~~~~Finding '[', then mark them with 'escape' charecter [STOP]
    //~~~~~~~~Finding ']', then mark them with 'escape' charecter [START]
    for(register int pivot = 0; pivot < (int)(*path).length(); ++++pivot)
    {
	pivot = (*path).find(tr(QString("]")),pivot,false);
	if(pivot < 0)
	    break;
	(*path).insert(pivot,tr(QString("\\")));
	(*path).insert((*path).length() + 1,tr(QString("\0")));
    }
    //~~~~~~~~Finding ']', then mark them with 'escape' charecter [STOP]
    //~~~~~~~~Finding '{', then mark them with 'escape' charecter [START]
    for(register int pivot = 0; pivot < (int)(*path).length(); ++++pivot)
    {
	pivot = (*path).find(tr(QString("{")),pivot,false);
	if(pivot < 0)
	    break;
	(*path).insert(pivot,tr(QString("\\")));
	(*path).insert((*path).length() + 1,tr(QString("\0")));
    }
    //~~~~~~~~Finding '{', then mark them with 'escape' charecter [STOP]
    //~~~~~~~~Finding '}', then mark them with 'escape' charecter [START]
    for(register int pivot = 0; pivot < (int)(*path).length(); ++++pivot)
    {
	pivot = (*path).find(tr(QString("}")),pivot,false);
	if(pivot < 0)
	    break;
	(*path).insert(pivot,tr(QString("\\")));
	(*path).insert((*path).length() + 1,tr(QString("\0")));
    }
    //~~~~~~~~Finding '}', then mark them with 'escape' charecter [STOP]
}

void frmInput::cmdAdd_clicked()
{
    itemList.clear();
    unsigned int numItems = lstFile1->count();
    for(register int itr = 0; itr < (int)numItems; ++itr)
    {
	if(lstFile1->isSelected(itr))
	    itemList.append(lstFile1->text(itr));
    }
    lstFile2->insertStringList(itemList,0);
    
    for(QStringList::Iterator it = itemList.begin(); it != itemList.end(); ++it)
	lstFile1->takeItem(lstFile1->findItem(*it));
    itemList.clear();
    lblFileList->setText("(" + QString::number((int)lstFile1->count()) + ")");
    lblFileListS->setText("(" + QString::number((int)lstFile2->count()) + ")");
}

void frmInput::cmdRej_clicked()
{
    itemList.clear();
    unsigned int numItems = lstFile2->count();
    
    for(register int itr = 0; itr < (int)numItems; ++itr)
    {
	if(lstFile2->isSelected(itr))
	    itemList.append(lstFile2->text(itr));
    }
    lstFile1->insertStringList(itemList,0);
   
    for(QStringList::Iterator it = itemList.begin(); it != itemList.end(); ++it)
	lstFile2->takeItem(lstFile2->findItem(*it));
    itemList.clear();
    lblFileList->setText("(" + QString::number((int)lstFile1->count()) + ")");
    lblFileListS->setText("(" + QString::number((int)lstFile2->count()) + ")");
}

void frmInput::chkimport_toggled(bool state)
{
    if(state == true)
    {
	importStat = true;
	chkindexing->setEnabled(true);
	if(chktabledirect->isChecked())
	    chktabledirect->setChecked(false);
    }
    else
    {
	importStat = false;
	chkindexing->setChecked(false);
	chkindexing->setEnabled(false);
    }
}

void frmInput::chkindexing_toggled(bool state)
{
    if(state == true)
	indexreqStat = true;
    else
	indexreqStat = false;
}

void frmInput::chkField_toggled( bool state)
{
    if(state == true)
    {
	selectedFieldStat = true;
	if(chktabledirect->isChecked())
	    chktabledirect->setChecked(false);
    }
    else
	selectedFieldStat = false;
}

void frmInput::chkTranspose_toggled( bool state)
{
    if(state == true)
    {
	transposeStat = true;
	if(chktabledirect->isChecked())
	    chktabledirect->setChecked(false);
    }
    else
	transposeStat = false;
}

void frmInput::chkplink_toggled(bool state)
{
    if(state == true)
    {
	plinkformatStat = true;
	if(chktabledirect->isChecked())
	    chktabledirect->setChecked(false);
    }
    else
	plinkformatStat = false;
}

void frmInput::chkValue_toggled(bool state)
{
    if(state == true)
    {
	valueChangeStat = true;
	if(chktabledirect->isChecked())
	    chktabledirect->setChecked(false);
    }
    else
	valueChangeStat = false;
}

void frmInput::chkillumina_clicked()
{
    frame5->setEnabled(false);
}

void frmInput::chkAffy_clicked()
{
    frame5->setEnabled(false);
}

void frmInput::chkOther_clicked()
{
    frame5->setEnabled(true);
}

void frmInput::chktabledirect_toggled(bool state)
{
    if(state)
    {
	chkField->setChecked(false);
	chkTranspose->setChecked(false);
	chkValue->setChecked(false);
	chkplink->setChecked(false);
	chkimport->setChecked(false);
	chkindexing->setChecked(false);
    }
}

void frmInput::cmdOK_clicked()
{
    bool retStat = true;
    bool status = true;
    
    if(lstFile2->count())
    {
	QString filePath1,filePath2,query;

	if(chktabledirect->isChecked() && lstFile2->count() > 1)
	    QMessageBox::information(this,"File input error","Table direct takes one input at a time");
	else
	{
	    if(retStat)
	    {
		genrateCorrectFormat(&fn);
		QFile fpWrite("datasource.txt");
		if(fpWrite.open(IO_WriteOnly | IO_Truncate))
		{
		    QTextStream fpOut(&fpWrite);
		    fpOut.setEncoding(QTextStream::UnicodeUTF8);
		    
		    unsigned int numItems = lstFile2->count();
		    for(register int itr = 0; itr < (int)numItems; ++itr)
		    {
			filePath1 = filePath2 = (lstFile2->text(itr)).stripWhiteSpace();
			if(filePath2.contains(tr(".txt")) > 0 || filePath2.contains(tr(".ped")) > 0)
			{
			    if(filePath2.contains(tr("_-")) > 0) //To check whether '_-' exist or not
				filePath2 = filePath2.remove(tr("_-"));
			    if(filePath2.contains(tr(".")) > 1) //For replacing '.' with '_'
			    {
				filePath2 = filePath2.remove(tr(".txt"));
				filePath2 = (filePath2.replace(tr("."),tr("_"))).append(".txt");
			    }
			    if(filePath2.contains(tr("-")) == 0) //To check whether '-' exist or not
			    {
				if(filePath2.contains(tr("_")) > 0) //For replacing '_' with '-'
				{
				    int firstOccur = filePath2.find(tr("_"),0,false);
				    filePath2 = filePath2.remove(tr(".txt"));
				    filePath2 = (filePath2.replace(firstOccur,1,tr("-"))).append(".txt");
				}
			    }
			    else if(filePath2.contains(QRegExp("-*_")))
			    {
				int firstOccur = filePath2.find(tr("_"),0,false);
				filePath2 = filePath2.replace(firstOccur,1,tr("-"));
				firstOccur = filePath2.find(tr("-"),0,false);
				filePath2 = filePath2.replace(firstOccur,1,tr("_"));
			    }
			    query = "rename " + filePath1 + " " + filePath2 + " " + fn + filePath1;
			    scanVal = system(query);
			    genrateCorrectFormat(&filePath2);
			    fpOut << fn + filePath2 << endl;
			}
			else
			{
			    status = false;
			    QMessageBox::information(this,tr("File name error!"),tr("Each file name must contains the extension '.txt'"));
			    break;
			}
		    }
		    fpWrite.close();
		    if(status)
		    {
			txtFile->setText("");
			illuminaFlag = (chkillumina->isChecked() ? true : false);
			affyFlag = (chkAffy->isChecked() ? true : false);
			otherFlag = (chkOther->isChecked() ? true : false);
			tDirect = (chktabledirect->isChecked() ? true : false);
			dlgStatus = true;
			accept();
		    }
		}
	    }
	    else
		QMessageBox::information(this,"File Opening Problem!","Can not open \'datasource.txt\' file for writing");
	}
    }
}

bool frmInput::getPlinkCharecterSet()
{
    bool retStat = false,ok = false;
    
    QString text = QInputDialog::getText("Plink Charecter Set", "Enter charecter set[seperated by space only]:", QLineEdit::Normal,QString::null, &ok,this);
    if(ok && !text.isEmpty())
    {
	//user entered something and pressed OK
	if(checkcharSet(text))
	{
	    QStringList strLst = QStringList::split(text," ");
	    text = strLst.join(" ");
	    QFile fpOut(".plinkChars.txt");
	    if(fpOut.open(IO_WriteOnly | IO_Truncate))
	    {
		QTextStream fpO(&fpOut);
		fpO.setEncoding(QTextStream::UnicodeUTF8);
		fpO << text << endl;
		fpOut.close();
		retStat = true;
	    }
	}
	else
	    QMessageBox::information(this,tr("Input Error!"), tr("Problem with given input! \nonly charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and space/s are allowed"));
    }
    return retStat;
}

void frmInput::cmdCancel_clicked()
{
    dlgStatus = false;
    accept();
}

bool frmInput::checkcharSet(QString &charSet)
{
    bool ok = true;
    int length = charSet.length();
    QChar ch;
	    
    for(register int i = 0; i < length; ++i)
    {
	ch = charSet.at(i);
	if(!((ch >= 'A' && ch <= 'Z') || (ch == ' ') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')))
	{
	    ok = false;
	    break;
	}
    }
    return ok;
}

void frmInput::accept()
{
    QDialog::accept();
}

void frmInput::cmdRename_clicked()
{
    if((int)lstFile2->count() > 0)
    {
	QStringList fileNameList;
	frmFileRename *fileRen = new frmFileRename(this,tr("File Rename"),true,0);
	for(int i = 0; i < (int)lstFile2->count(); i++)
	    fileNameList.append(lstFile2->text(i));
	fileRen->initSetting(fileNameList,pathWDir);
	if(fileRen->exec() == QDialog::Accepted)
	{
	    if((int)(fileRen->fileNameLst).count())
	    {
		fileNameList = fileRen->fileNameLst;
		lstFile2->clear();
		lstFile2->insertStringList(fileNameList);
	    }
	    fileRen->close();
	    delete fileRen;
	}
    }
    else
	QMessageBox::information(this,tr("Empty list"), tr("Right list box is empty!"));
}


void frmInput::cmdDelete_clicked()
{
    if((int)lstFile2->count() > 0)
    {
	QString query = "";
	int fscanVal = -1,index = 0;
	QStringList delList;
	bool ok = false;
	
	delList.clear();
	for(int i = 0; i < (int)lstFile2->count(); i++)
	{
	    if(lstFile2->isSelected(i))
	    {
		query = txtFile->text() + lstFile2->text(i);
		query = "rm -f " + query.replace(" ","\\ ");
		//cout << query << endl;
		fscanVal = system(query);
		if(fscanVal > -1)
		    delList.prepend(QString::number(i));
	    }
	}
	
	if((int)delList.count())
	{
	    for(QStringList::iterator it = delList.begin(); it != delList.end(); ++it,++index)
		lstFile2->removeItem((*it).toInt(&ok));
	    QMessageBox::information(this,tr("Deletion successful!"), tr("Selected files are deleted successfully"));
	    lblFileListS->setText("(" + QString::number((int)lstFile2->count()) + ")");
	}
	else
	    QMessageBox::information(this,tr("No selection"), tr("No file name selected!"));
    }
    else
	QMessageBox::information(this,tr("Empty list"), tr("Right list box is empty!"));
}
