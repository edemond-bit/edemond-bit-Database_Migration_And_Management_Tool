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
#include <qlistbox.h>
#include <qstringlist.h>
#include <iostream.h>

static QStringList searchfor = "";
static QStringList replacewith = "";

void frmfindreplace::init()
{
    cmbsearchfor->setEditable(true);
    cmbsearchfor->setListBox(new QListBox(cmbsearchfor,tr("lb1"),WDestructiveClose));
    cmbsearchfor->insertStringList(searchfor,0);
    cmbreplacewith->setEditable(true);
    cmbreplacewith->setListBox(new QListBox(cmbreplacewith,tr("lb2"),WDestructiveClose));
    cmbreplacewith->insertStringList(replacewith,0);
    cmdReplace->setEnabled(false);
    cmdReplaceAll->setEnabled(false);
    closeOK = false;
}

void frmfindreplace::initSettings(QStringList viewcolList, int numRows)
{
    chkcolwise->insertStringList(viewcolList,0);
    spinrowwise->setMinValue(1);
    spinrowwise->setMaxValue(numRows);
}

void frmfindreplace::rdall_clicked()
{
    chkcolwise->setEnabled(false);
    spinrowwise->setEnabled(false);
}

void frmfindreplace::rdcolwise_clicked()
{
    chkcolwise->setEnabled(true);
    spinrowwise->setEnabled(false);
}

void frmfindreplace::rdrowwise_clicked()
{
    chkcolwise->setEnabled(false);
    spinrowwise->setEnabled(true);
}

void frmfindreplace::cmdClose_clicked()
{
    closeOK = true;
    close();
}

void frmfindreplace::closeEvent(QCloseEvent *e)
{
    if(closeOK)
    {
	searchfor.clear();
	for(register int i = 0; i < (int)cmbsearchfor->count(); ++i)
	    searchfor.append(cmbsearchfor->text(i));
	replacewith.clear();
	for(register int i = 0; i < (int)cmbreplacewith->count(); ++i)
	    replacewith.append(cmbreplacewith->text(i));
	e->accept();
    }
    else
	e->ignore();
}
