#ifndef FIA_VIEWER_H
#define FIA_VIEWER_H

#include "FreeImage.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QRubberBand>
 
class FiaViewer : public QGraphicsView
{
    Q_OBJECT

private:
    float zoomFactor;
    QPixmap pixmap;
    QGraphicsScene *scene;
    QRubberBand *rubberBand;
    QPoint mouseDownPos;
    QPoint lastMouseViewPos;

protected:
    void mousePressEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );
    void mouseMoveEvent( QMouseEvent * );
 
public:
    FiaViewer();

    void fitImageToViewer(bool fit);
    void setImage(QPixmap pix);
    void setImage(FIBITMAP *fib);

    void zoom(float zoom);


 public slots:
     void zoomIn() { this->scale(1.2, 1.2); }
     void zoomOut() { this->scale(1 / 1.2, 1 / 1.2); }
};
 
 
#endif
