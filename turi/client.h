#pragma once

#include <QObject>
#include <request.h>
#include <response.h>

class Client : public QObject {
    Q_OBJECT
    Request * request = nullptr;
    QString answer;

  public:
    explicit Client(Request & req, QObject * parent,
                    QString hostAdress = "127.0.0.1", int port = 3000);

    Response getResponse();

  public slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
};
