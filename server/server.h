#pragma once

#include "turicarriage.h"
#include <QTcpServer>
#include <request.h>

class Server : public QObject {
    // to save not completed requests
    QString buffer;

    QTcpServer * tcpServer;
    Q_OBJECT
    QVector<TuriCarriage *> carriages;

    // restriction
    static const int MAX_CLIENT_NUMBER = 3;

  public:
    explicit Server(QObject * parent = nullptr);

  private:
    // get rerialized response
    QString fromRequest(Request & req);

    // return number of active clients
    int numberOfClients();

    // add new client, return his id
    int newClient(TuriCarriage * carriage);

  public slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();
};
