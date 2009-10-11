#ifndef FIA_WINDOW_H
#define FIA_WINDOW_H
 
#include "ui_FiaWindow.h"
#include "FiaViewer.h"

#include "FreeImage.h"
 
class FiaWindow : public QMainWindow, private Ui::FiaViewerWindow
{
    Q_OBJECT

private:
    FiaViewer *view;

public:
    FiaWindow();
    FiaViewer* viewer() { return this->view; }

 public slots:
     void fileOpen();
};
 
#endif
