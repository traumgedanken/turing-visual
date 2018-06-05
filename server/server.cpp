#include "server.h"
#include "response.h"
#include "turiprogram.h"
#include <QDir>
#include <QTcpSocket>
#include <iostream>
#include "turiparser.h"

using namespace std;

Server::Server(QObject * parent) : QObject(parent) {
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    int PORT = 3000;
    if (!tcpServer->listen(QHostAddress::Any, PORT)) {
        cerr << "error listen " << endl;
    } else {
        cout << "started at " << PORT << endl;
    }
}

void Server::onNewConnection() {
    cout << "got new connection" << endl;
    QTcpSocket * clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(disconnected()), this,
            SLOT(onClientDisconnected()));
}

void Server::onReadyRead() {
    QTcpSocket * clientSocket = static_cast<QTcpSocket *>(sender());
    cout << "receiving data" << endl;
    QByteArray data = clientSocket->readAll();
    cout << "Received:" << endl << data.toStdString() << endl;
    Request req =
        Request::deserialize(QString::fromStdString(data.toStdString()));

    QString responseStr = fromRequest(req);
    cout << "Sending: " << endl << responseStr.toStdString() << endl;
    clientSocket->write(responseStr.toUtf8());
    clientSocket->flush();
}

QString Server::fromRequest(Request & req) {
    switch (req.functionName) {
    case FN_PARSE_PROGRAM: {
        TuriParser parser(req.code);
        TuriProgram * program = parser.parseTuriProgram();
        Response res(0, program);
        QString result = res.serialize();
        res.clean();
        return result;
    }
    default: { return ""; }
    }
}

void Server::onClientDisconnected() { cout << "Client disconnected " << endl; }
