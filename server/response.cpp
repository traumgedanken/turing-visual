#include "response.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Response::Response(int _status, QString _word, QString _state, int _id, int _line, TuriProgram * _program) {
    status = _status;
    program = _program;
    word = _word;
    state = _state;
    id = _id;
    line = _line;
}

QString Response::serialize() {
    QJsonDocument doc;
    QJsonObject responseObj;

    responseObj.insert("status", status);
    responseObj.insert("word", word);
    responseObj.insert("state", state);
    responseObj.insert("id", id);
    responseObj.insert("line", line);
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
    QString _word = responseObj.value("word").toString();
    QString _state = responseObj.value("state").toString();
    int _id = responseObj.value("id").toInt();
    int _line = responseObj.value("line").toInt();
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

    return Response(_status, _word, _state, _id, _line, _program);
}
