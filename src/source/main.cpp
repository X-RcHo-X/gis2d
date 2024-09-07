#include "base_window.h"
#include "base_styler.h"

//Qt
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    CBaseWindow2D w;
    w.show();
    BaseStyler::setStyleSheet(":/qss");
    return a.exec();
}
