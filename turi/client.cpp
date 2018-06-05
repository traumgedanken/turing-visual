#include "client.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <iostream>

using namespace std;

Client::Client(Request & req, QObject * parent, QString hostAdress, int port)
    : QObject(parent) {
    if (parent == nullptr) return;
    request = new Request(req.functionName, req.code);
    QTcpSocket * client = new QTcpSocket(this);
    connect(client, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(client, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    QHostAddress serverAddress(hostAdress);
    int PORT = port;
    client->connectToHost(serverAddress, PORT);
    if (!client->waitForReadyRead()) throw std::exception();
}

void Client::onConnected() {
    QTcpSocket * clientSocket = static_cast<QTcpSocket *>(sender());
    QString requestStr = request->serialize();
    clientSocket->write(requestStr.toUtf8());
    clientSocket->flush();
}

void Client::onReadyRead() {
    QTcpSocket * clientSocket = static_cast<QTcpSocket *>(sender());
    QByteArray data = clientSocket->readAll();
    answer = QString::fromStdString(data.toStdString());
    clientSocket->disconnectFromHost();
}

void Client::onDisconnected() {
    if (request != nullptr) {
        delete request;
        request = nullptr;
    }
}

Response Client::getResponse() {
    Response res = Response::deserialize(answer);
    return res;
}
