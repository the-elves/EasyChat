#include "chatclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatClient w1("main");
    w1.show();

    return a.exec();
}
