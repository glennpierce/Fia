#ifndef FIA_VIEWER_H
#define FIA_VIEWER_H
 
#include "ui_FiaViewer.h"

#include "FreeImage.h"
 
class FiaViewer : public QMainWindow, private Ui::FiaViewerWindow
{
    Q_OBJECT

private:
    float zoomFactor;
    QPixmap pixmap;
    QGraphicsScene *scene;

protected:
    void mousePressEvent( QMouseEvent * );
    //void mouseReleaseEvent( QMouseEvent * );
    //void mouseMoveEvent( QMouseEvent * );
 
public:
    FiaViewer();

    void fitImageToViewer(bool fit);
    void setImage(QPixmap pix);
    void setImage(FIBITMAP *fib);

    void zoom(float zoom);


 public slots:
     void zoomIn() { this->graphicsView->scale(1.2, 1.2); }
     void zoomOut() { this->graphicsView->scale(1 / 1.2, 1 / 1.2); }

     void fileOpen();
//    void about();
};
 
 
#endif
