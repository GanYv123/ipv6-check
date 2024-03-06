#include "ipv6qt.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    QApplication a(argc, argv);
    ipv6Qt w;
    w.show();
    return a.exec();
}
