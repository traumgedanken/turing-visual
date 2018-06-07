#include "server.h"
#include <QDir>
#include <QTcpSocket>
#include <defines.h>
#include <iostream>
#include <response.h>
#include <turiparser.h>
#include <turiprogram.h>

using namespace std;

#define RETURN_INVALID_INDEX_ERROR                                             \
    if (req.id < 0 || req.id >= carriages.length()) {                          \
        Response res(NETWORK_ERROR_CODE, "Invalid index");                     \
        return res.serialize();                                                \
    }

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
    case FN_NONE: {
        Response res(NETWORK_TRUE);
        return res.serialize();
    }
    case FN_PARSE_PROGRAM: {
        TuriParser parser(req.code);
        TuriProgram * program = parser.parseTuriProgram();
        Response res(NETWORK_TRUE, "", "", -1, -1, program);
        QString result = res.serialize();
        delete res.program;
        return result;
    }
    case FN_CARRIAGE_CREATE: {
        if (carriages.length() > MAX_CAPACITY) {
            Response res(NETWORK_ERROR_CODE, "Server needs to be rebooted");
            return res.serialize();
        }
        TuriCarriage * carriage = nullptr;
        try {
            carriage = new TuriCarriage(req.code, req.program);
        } catch (std::exception) {
            Response res(NETWORK_FALSE, "Error at setup. Invalid input");
            return res.serialize();
        }
        carriages.append(carriage);
        Response res(NETWORK_TRUE, carriage->getCurrentWord(),
                     carriage->getCurrentState(), carriages.length() - 1,
                     carriage->getCurrentLine());
        return res.serialize();
    }
    case FN_CARRIAGE_PREV: {
        RETURN_INVALID_INDEX_ERROR
        TuriCarriage * carriage = carriages[req.id];
        bool result = carriage->prev();
        Response res(result ? NETWORK_TRUE : NETWORK_FALSE,
                     carriage->getCurrentWord(), carriage->getCurrentState(),
                     carriage->getPosition(), carriage->getCurrentLine());
        return res.serialize();
    }
    case FN_CARRIAGE_NEXT: {
        RETURN_INVALID_INDEX_ERROR
        TuriCarriage * carriage = carriages[req.id];
        bool result = carriage->next();
        Response res(result ? NETWORK_TRUE : NETWORK_FALSE,
                     carriage->getCurrentWord(), carriage->getCurrentState(),
                     carriage->getPosition(), carriage->getCurrentLine());
        return res.serialize();
    }
    case FN_CARRIAGE_LEFT: {
        RETURN_INVALID_INDEX_ERROR
        TuriCarriage * carriage = carriages[req.id];
        carriage->moveView(LEFT);
        Response res(NETWORK_TRUE, carriage->getCurrentWord(),
                     carriage->getCurrentState(), carriage->getPosition(), -1);
        return res.serialize();
    }
    case FN_CARRIAGE_RIGHT: {
        RETURN_INVALID_INDEX_ERROR
        TuriCarriage * carriage = carriages[req.id];
        carriage->moveView(RIGHT);
        Response res(NETWORK_TRUE, carriage->getCurrentWord(),
                     carriage->getCurrentState(), carriage->getPosition(), -1);
        return res.serialize();
    }
    case FN_CARRIAGE_WORD: {
        RETURN_INVALID_INDEX_ERROR
        TuriCarriage * carriage = carriages[req.id];
        Response res(NETWORK_TRUE, carriage->getResult());
        return res.serialize();
    }
    case FN_CARRIAGE_RUN: {
        RETURN_INVALID_INDEX_ERROR
        TuriCarriage * carriage = carriages[req.id];
        while (carriage->next()) {}
        Response res(NETWORK_TRUE, carriage->getCurrentWord(),
                     carriage->getCurrentState(), carriage->getPosition(),
                     carriage->getCurrentLine());
        return res.serialize();
    }
    default: {
        Response res(NETWORK_ERROR_CODE, "Unrecognized command");
        return res.serialize();
    }
    }
}

void Server::onClientDisconnected() { cout << "Client disconnected " << endl; }
