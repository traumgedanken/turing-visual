#include "response.h"
#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include <QTextStream>
#include <iostream>

Response::Response(int _status, TuriProgram * _program) {
    status = _status;
    program = _program;
}

QString Response::serialize() {
    QJsonDocument doc;
    QJsonObject responseObj;

    responseObj.insert("status", status);

    if (program == nullptr) {
        responseObj.insert("program", QJsonValue());
        responseObj.insert("errors", QJsonValue());
    } else {
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
        responseObj.insert("program", programArr);

        QJsonArray errorsArr;
        QVector<TuriParserError *> errors = program->getErrors();
        for (auto error : errors) {
            QJsonObject errorObj;
            errorObj.insert("line", error->getLine());
            errorObj.insert("errorText", error->getErrorText());
            errorObj.insert("errorType", error->getErrorType());
            errorsArr.append(errorObj);
        }
        responseObj.insert("errors", errorsArr);
    }

    doc.setObject(responseObj);
    return QString(doc.toJson());
}

Response Response::deserialize(QString source) {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(
        QByteArray::fromStdString(source.toStdString()), &err);
    QJsonObject responseObj = doc.object();

    int _status = responseObj.value("status").toInt();
    TuriProgram * _program = nullptr;
    if (!responseObj.value("program").isNull()) {
        QJsonArray programArr = responseObj.value("program").toArray();
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
        QVector<TuriParserError *> errors;
        QJsonArray errorsArr = responseObj.value("errors").toArray();
        for (auto value : errorsArr) {
            QJsonObject errorObj = value.toObject();
            errors.append(new TuriParserError((ERROR_TYPE)errorObj.value("errorType").toInt(),
                                              errorObj.value("line").toInt(),
                                              errorObj.value("errorText").toString()));
            _program->setErrorsList(errors);
        }
    }

    return Response(_status, _program);
}

void Response::clean() {
    if (program != nullptr) delete program;
}
