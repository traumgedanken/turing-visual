#ifndef RESPONSE_H
#define RESPONSE_H

#include "turiprogram.h"
#include <QVector>

class Response {
  public:
    int status = -1;
    TuriProgram * program = nullptr;
    QString word = "";
    QString state = "";
    int id = -1;
    int line = -1;

    Response() {}
    Response(int _status, QString _word, QString _state, int _id, int _line, TuriProgram * _program = nullptr);
    QString serialize();
    static Response deserialize(QString source);
};

#endif // RESPONSE_H
