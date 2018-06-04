#include <QCoreApplication>
#include <server.h>

using namespace std;

int main(int argc, char * argv[]) {
    QCoreApplication a(argc, argv);

    Server server(&a);

    return a.exec();
}
