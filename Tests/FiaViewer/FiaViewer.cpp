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
    this->zoomFactor = 1.0f; 
    this->scene = new QGraphicsScene(this);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    this->rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
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
    this->scale(zoom,zoom);
}

void FiaViewer::setImage(QPixmap pix)
{
    this->pixmap = pix.copy();
    this->scene->setSceneRect(this->pixmap.rect());
    this->scene->addPixmap(this->pixmap);
    this->setScene(this->scene);
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
    this->setScene(this->scene);
}

void FiaViewer::mousePressEvent( QMouseEvent * evt)
{
	qDebug() << "press";
	QGraphicsView::mousePressEvent(evt);

//	if(!this->scene->itemAt(this->mapToScene(evt->pos()))) {
		mouseDownPos = this->mapToScene(evt->pos()).toPoint();
		lastMouseViewPos = evt->pos();
		rubberBand->setGeometry(this->mapFromScene(mouseDownPos).x(),
			this->mapFromScene(mouseDownPos).y(), 0,0);
		rubberBand->show();
//	}


/*
    if ( e->button() == Qt::LeftButton ) {

	if(this->zoomFactor * ZOOM_INC > ZOOM_MAX)
            return;

//	std::cout << e->x() << std::endl;
//	this->graphicsView->translate(e->x(), e->y());

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
*/
}

void FiaViewer::mouseMoveEvent(QMouseEvent* evt)
{
std::cout << "here0" << std::endl;

	if(evt->buttons().testFlag(Qt::LeftButton)) {
		// handle the rubberband
std::cout << "here1" << std::endl;

		if(rubberBand->isVisible()) {

std::cout << "here2" << std::endl;

			QPoint mouseDownView = this->mapFromScene(mouseDownPos);
			lastMouseViewPos = evt->pos();
			QRect rubberRect(mouseDownView, lastMouseViewPos);
			rubberRect = rubberRect.normalized();
			rubberBand->setGeometry(rubberRect);
			QPolygonF p = this->mapToScene(rubberRect);
			QPainterPath path;
			path.addPolygon(p);
			this->scene->setSelectionArea(path, Qt::IntersectsItemShape);
		}
	}

	QGraphicsView::mouseMoveEvent(evt);
}

void FiaViewer::mouseReleaseEvent(QMouseEvent* evt)
{
	qDebug() << "release";
	rubberBand->hide();
	QGraphicsView::mouseReleaseEvent(evt);
}
