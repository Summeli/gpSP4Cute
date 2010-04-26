#include "cuteErrorDialog.h"
#include "cutedebug.h"

cuteErrorDialog::cuteErrorDialog( QString message, QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	ui.errorLabel->setText( message );
	connect(ui.OkButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	
}

cuteErrorDialog::~cuteErrorDialog()
{

}

void cuteErrorDialog::buttonClicked()
{
	__DEBUG_IN
	emit( notifiedUser() );
}
