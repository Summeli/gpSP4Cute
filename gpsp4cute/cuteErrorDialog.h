#ifndef CUTEERRORDIALOG_H
#define CUTEERRORDIALOG_H

#include <QtGui/QDialog>
#include "ui_cuteErrorDialog.h"

class cuteErrorDialog : public QDialog
{
    Q_OBJECT

public:
    cuteErrorDialog( QString message, QWidget *parent = 0);
    ~cuteErrorDialog();

public slots:
	void buttonClicked();
	
signals:
	void notifiedUser();
	
private:
    Ui::cuteErrorDialogClass ui;
};

#endif // CUTEERRORDIALOG_H
