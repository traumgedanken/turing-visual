#ifndef CLIENT_H
#define CLIENT_H

#include <request.h>
#include <response.h>
#include <QObject>
class Client : public QObject {
    Q_OBJECT
    Request * request = nullptr;
    QString answer;

  public:
    explicit Client(Request & req, QObject * parent, QString hostAdress = "127.0.0.1",
                    int port = 3000);
    Response getResponse();
  signals:

  public slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
};

#endif // CLIENT_H
