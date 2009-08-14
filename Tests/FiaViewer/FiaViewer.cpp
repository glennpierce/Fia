#include <QtGui> 
#include "FiaViewer.h"
#include "FreeImageAlgorithms_IO.h"

#include <iostream>

const float ZOOM_MIN = 0.1f;
const float ZOOM_MAX = 100.0f;
const float ZOOM_INC = 1.20f;
const float ZOOM_DEC = 1 / 1.20f;
 
FiaViewer::FiaViewer()
{
    setupUi(this);

    //openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    //openAct->setShortcuts(QKeySequence::Open);
    //openAct->setStatusTip(tr("Open an existing file"));
    connect(this->openAction, SIGNAL(triggered()), this, SLOT(fileOpen()));

    this->zoomFactor = 1.0f; 
    this->scene = new QGraphicsScene(this->graphicsView);
    this->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}


void FiaViewer::fitImageToViewer(bool fit)
{
    if(fit == true)
        this->scene->setSceneRect(this->pixmap.rect());
    else
        this->scene->setSceneRect(this->rect());
}

void FiaViewer::zoom(float zoom)
{
    this->zoomFactor = zoom;
    this->graphicsView->scale(zoom,zoom);
}

void FiaViewer::setImage(QPixmap pix)
{
    this->pixmap = pix.copy();
    this->scene->setSceneRect(this->pixmap.rect());
    this->scene->addPixmap(this->pixmap);
    this->graphicsView->setScene(this->scene);
}

void FiaViewer::setImage(FIBITMAP *fib)
{
    uchar *data = FreeImage_GetBits(fib);
    int width = FreeImage_GetWidth(fib);
    int height = FreeImage_GetHeight(fib);
    int pitch = FreeImage_GetPitch(fib);
    int bpp = FreeImage_GetBPP(fib);

    FreeImage_FlipVertical(fib);

    QImage image(data, width, height, QImage::Format_RGB888);

    this->pixmap = QPixmap::fromImage (image.rgbSwapped());

    this->scene->setSceneRect(this->pixmap.rect());
    this->scene->addPixmap(this->pixmap);
    this->graphicsView->setScene(this->scene);
}

void FiaViewer::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty()) {

	QByteArray ba = fileName.toLatin1();
	const char *path = ba.data(); 

	this->setImage(FIA_LoadFIBFromFile(path));
    }
}

/* 
void FiaViewer::about() 
{
    QMessageBox::about(this,"About FiaViewer",
                "This app was coded for educational purposes.\n"
                "Number 1 is: " + QString::number(spinBox1->value()) + "\n\n"
                "Bye.\n");
}
*/

void FiaViewer::mousePressEvent( QMouseEvent * e)
{
   

    if ( e->button() == Qt::LeftButton ) {

	if(this->zoomFactor * ZOOM_INC > ZOOM_MAX)
            return;

//	std::cout << e->x() << std::endl;
	this->graphicsView->translate(e->x(), e->y());

        //this->zoom(this->zoomFactor * ZOOM_INC);
	this->graphicsView->scale(ZOOM_INC,ZOOM_INC);


	//buttonDown = TRUE;
	//moveOffset = mapToParent( e->pos() );
	//emit doActivate();
    } else if ( e->button() == Qt::RightButton ) {
	//emit doActivate();
	//emit popupOperationMenu( e->globalPos() );
	
	//if(this->zoomFactor / ZOOM_DEC < ZOOM_MIN)
        //    return;

        this->graphicsView->translate(e->x(), e->y());

	this->graphicsView->scale(ZOOM_DEC,ZOOM_DEC);
    }

   e->accept();
}

