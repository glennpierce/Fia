#include <QApplication>

#include "FiaWindow.h" 
#include "FiaViewer.h"
#include "FreeImage.h"

#include <iostream>
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FiaWindow *window = new FiaWindow;

    FIBITMAP *fib = FreeImage_Load(FIF_JPEG,
		"/home/glenn/Devel/Fia/Tests/FiaViewer/Test.jpg", JPEG_DEFAULT);

 
    window->viewer()->setImage(fib);

std::cout << "gg" << std::endl;

    //viewer->fitImageToViewer(false);
    //viewer->zoom(50.0);

    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    window->show();

    return app.exec();
}
