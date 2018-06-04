#ifndef RESPONSE_H
#define RESPONSE_H

#include "turiprogram.h"
#include <QVector>

class Response {
  public:
    int status = -1;
    TuriProgram * program = nullptr;

    Response() {}
    Response(int _status, TuriProgram * _program);
    QString serialize();
    static Response deserialize(QString source);
};

#endif // RESPONSE_H
