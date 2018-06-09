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
        Response res(NETWORK_FALSE, "Invalid index.\n"                         \
                                    "Server was rebooted");                    \
        return res.serialize();                                                \
    }

Server::Server(QObject * parent) : QObject(parent) {
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    int PORT = 3000;
    if (!tcpServer->listen(QHostAddress::Any, PORT)) {
        cerr << "Error listen." << endl;
    } else {
        cout << "Started at: " << PORT << endl;
    }
}

void Server::onNewConnection() {
    cout << "Got new connection." << endl;
    QTcpSocket * clientSocket = tcpServer->nextPendingConnection();
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(clientSocket, SIGNAL(disconnected()), this,
            SLOT(onClientDisconnected()));
}

void Server::onReadyRead() {
    QTcpSocket * clientSocket = static_cast<QTcpSocket *>(sender());
    QByteArray data = clientSocket->readAll();
    cout << "Received:" << endl << data.toStdString() << endl;
    buffer.append(QString(data));
    int index = buffer.indexOf(NETWORK_SEPARATOR);
    // check if buffer contains full request
    if (index == -1) return;
    // take full request
    QString newRequestStr = buffer.mid(0, index);
    // clean buffer from processed request
    buffer = buffer.mid(index + 1);
    Request req =
        Request::deserialize(newRequestStr);

    QString responseStr = fromRequest(req);
    cout << "Sending:" << endl << responseStr.toStdString() << endl;
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
        if (req.id == -1 && numberOfClients() >= MAX_CLIENT_NUMBER) {
            Response res(NETWORK_FALSE, "Server is overload. Wait please");
            return res.serialize();
        }
        TuriCarriage * carriage = nullptr;
        try {
            carriage = new TuriCarriage(req.code, req.program);
        } catch (std::exception) {
            Response res(NETWORK_FALSE, "Error at setup. Invalid input");
            return res.serialize();
        }
        if (req.id != -1) {
            RETURN_INVALID_INDEX_ERROR
            delete carriages[req.id];
            carriages[req.id] = nullptr;
        }
        int newId = newClient(carriage);
        Response res(NETWORK_TRUE, carriage->getCurrentWord(),
                     carriage->getCurrentState(), newId,
                     carriage->getCurrentLine());
        return res.serialize();
    }
    case FN_CARRIAGE_PREV: {
        RETURN_INVALID_INDEX_ERROR
        TuriCarriage * carriage = carriages[req.id];
        bool result = carriage->prev();
        Response res(result ? NETWORK_TRUE : NETWORK_SPECIAL_STATUS,
                     carriage->getCurrentWord(), carriage->getCurrentState(),
                     carriage->getPosition(), carriage->getCurrentLine());
        return res.serialize();
    }
    case FN_CARRIAGE_NEXT: {
        RETURN_INVALID_INDEX_ERROR
        TuriCarriage * carriage = carriages[req.id];
        bool result = carriage->next();
        Response res(result ? NETWORK_TRUE : NETWORK_SPECIAL_STATUS,
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
    case FN_CARRIAGE_DELETE: {
        RETURN_INVALID_INDEX_ERROR
        delete carriages[req.id];
        carriages[req.id] = nullptr;
        Response res(NETWORK_TRUE);
        return res.serialize();
    }
    default: {
        Response res(NETWORK_ERROR_CODE, "Unrecognized command");
        return res.serialize();
    }
    }
}

void Server::onClientDisconnected() { cout << "Client disconnected." << endl; }

int Server::numberOfClients() {
    int number = 0;
    for (auto & carriage : carriages) {
        if (carriage != nullptr) number++;
    }
    return number;
}

int Server::newClient(TuriCarriage * carriage) {
    int oldLength = carriages.length();
    if (numberOfClients() == oldLength) {
        carriages.append(carriage);
        return oldLength;
    }
    int newId = 0;
    while (carriages[newId] != nullptr)
        newId++;
    carriages[newId] = carriage;
    return newId;
}
