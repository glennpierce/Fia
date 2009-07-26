#include <QApplication>
 
#include "FiaViewer.h"
#include "FreeImage.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FiaViewer *viewer = new FiaViewer;

    //QPixmap pix("/home/glenn/Devel/Fia/Tests/Data/spider-eating-a-fly.jpg");
    //viewer->setImage(pix); 
   
    FIBITMAP *fib = FreeImage_Load(FIF_JPEG,
		"/home/glenn/Devel/Fia/Tests/Data/water-strider.jpg", JPEG_DEFAULT);
 
    viewer->setImage(fib);

    //viewer->fitImageToViewer(false);
    //viewer->zoom(50.0);

    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    viewer->show();

    return app.exec();
}
