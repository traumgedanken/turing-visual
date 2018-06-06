#include "request.h"
#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include <QTextStream>
#include <iostream>

Request::Request(FunctionName _functionName, int _id, QString _code, TuriProgram *_program) {
    functionName = _functionName;
    code = _code;
    program = _program;
    id = _id;
}


QString Request::serialize() {
    QJsonDocument doc;
    QJsonObject requestObj;

    requestObj.insert("functionName", functionName);
    requestObj.insert("code", code);
    requestObj.insert("id", id);
    if (program != nullptr) {
        QJsonArray programArr;
        for (int i = 0; i < program->count(); i++) {
            TuriCommand * currCommand = program->getCommand(i);
            QJsonObject commandObj;
            commandObj.insert("currentState", currCommand->getCurrentState());
            commandObj.insert("currentSymbol", QString(currCommand->getCurrentSymbol()));
            commandObj.insert("nextState", currCommand->getNextState());
            commandObj.insert("nextSymbol", QString(currCommand->getNextSymbol()));
            commandObj.insert("direction", currCommand->getDirection());
            programArr.append(commandObj);
        }
        requestObj.insert("program", programArr);
    } else requestObj.insert("program", QJsonValue());
    doc.setObject(requestObj);
    return QString(doc.toJson());
}

Request Request::deserialize(QString source) {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(
    QByteArray::fromStdString(source.toStdString()), &err);
    QJsonObject requestObj = doc.object();
    QString _code = requestObj.value("code").toString();
    FunctionName _functionName = (FunctionName)requestObj.value("functionName").toInt();
    int _id = requestObj.value("id").toInt();
    TuriProgram * _program = nullptr;
    if (!requestObj.value("program").isNull()) {
        QJsonArray programArr = requestObj.value("program").toArray();
        _program = new TuriProgram();
        for (auto value : programArr) {
            QJsonObject commandObj = value.toObject();
            _program->addCommand(new TuriCommand(
                                    commandObj.value("currentState").toString(),
                                    commandObj.value("nextState").toString(),
                                    commandObj.value("currentSymbol").toString().at(0),
                                    commandObj.value("nextSymbol").toString().at(0),
                                   (DIRECTION)commandObj.value("direction").toInt()));
        }
    }
    return Request(_functionName, _id, _code, _program);
}

