#include <QtGui> 
#include "FiaWindow.h"
#include "FreeImageAlgorithms_IO.h"
 
FiaWindow::FiaWindow()
{
    setupUi(this);

    this->view = new FiaViewer();
    this->horizontalLayout->addWidget(this->view);

//    this->centralwidget->add(this->FiaViewer);

    //openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    //openAct->setShortcuts(QKeySequence::Open);
    //openAct->setStatusTip(tr("Open an existing file"));
    
    connect(this->openAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
}

void FiaWindow::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty()) {

	QByteArray ba = fileName.toLatin1();
	const char *path = ba.data(); 

	this->viewer()->setImage(FIA_LoadFIBFromFile(path));
    }
}
