#include "mainwidget.h"
#include "ElaApplication.h"
#include <QApplication>
#include "ElaWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElaApplication::getInstance()->init();
    MainWidget w;

    w.show();
    return a.exec();
}
