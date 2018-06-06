#pragma once

#include "request.h"
#include <QTcpServer>
#include "turicarriage.h"

class Server : public QObject {
    QTcpServer * tcpServer;
    Q_OBJECT
    QVector<TuriCarriage *> carriages;

    static const int MAX_CAPACITY = 10000;

  public:
    explicit Server(QObject * parent = nullptr);

  private:
    QString fromRequest(Request & req);
  signals:

  public slots:
    void onNewConnection();
    void onReadyRead();
    void onClientDisconnected();
};
