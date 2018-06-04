#ifndef SERVER_H
#define SERVER_H

#include "request.h"
#include <QObject>
#include <QTcpServer>

class Server : public QObject {
    QTcpServer * tcpServer;
    Q_OBJECT

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

#endif // SERVER_H
