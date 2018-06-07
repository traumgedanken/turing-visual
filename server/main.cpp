#include "server.h"
#include <QCoreApplication>

using namespace std;

int main(int argc, char * argv[]) {
    QCoreApplication a(argc, argv);

    Server server(&a);

    return a.exec();
}
