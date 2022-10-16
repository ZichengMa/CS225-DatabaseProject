#include "dataui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dataUI w;
    w.show();
    return a.exec();
}
