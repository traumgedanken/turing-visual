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
        Response res(0, "", "", -1, -1, program);
        QString result = res.serialize();
        delete res.program;
        return result;
    }
    case FN_CARRIAGE_CREATE: {
        TuriCarriage * carriage = new TuriCarriage(req.code, req.program);
        carriages.append(carriage);
        Response res(0,
                     carriage->getCurrentWord(),
                     carriage->getCurrentState(),
                     carriages.length() - 1,
                     carriage->getCurrentLine());
        return res.serialize();
    }
    case FN_CARRIAGE_PREV: {
        TuriCarriage * carriage = carriages[req.id];
        bool result = carriage->prev();
        Response res(result ? 0 : 1,
                     carriage->getCurrentWord(),
                     carriage->getCurrentState(),
                     carriage->getPosition(),
                     carriage->getCurrentLine());
        return res.serialize();
    }
    case FN_CARRIAGE_NEXT: {
        TuriCarriage * carriage = carriages[req.id];
        bool result = carriage->next();
        Response res(result ? 0 : 1,
                     carriage->getCurrentWord(),
                     carriage->getCurrentState(),
                     carriage->getPosition(),
                     carriage->getCurrentLine());
        return res.serialize();
    }
    case FN_CARRIAGE_LEFT: {
        TuriCarriage * carriage = carriages[req.id];
        carriage->moveView(LEFT);
        Response res(0,
                     carriage->getCurrentWord(),
                     carriage->getCurrentState(),
                     carriage->getPosition(), -1);
        return res.serialize();
    }
    case FN_CARRIAGE_RIGHT: {
        TuriCarriage * carriage = carriages[req.id];
        carriage->moveView(RIGHT);
        Response res(0,
                     carriage->getCurrentWord(),
                     carriage->getCurrentState(),
                     carriage->getPosition(), -1);
        return res.serialize();
    }
    case FN_CARRIAGE_WORD: {
        TuriCarriage * carriage = carriages[req.id];
        Response res(0, carriage->getResult(), "", -1, -1);
        return res.serialize();
    }
    default: {
        Response res(-1, "", "", -1, -1);
        return res.serialize();
    }
    }
}

void Server::onClientDisconnected() { cout << "Client disconnected " << endl; }
