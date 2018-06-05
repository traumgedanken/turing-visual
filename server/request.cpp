#include "request.h"
#include <QByteArray>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>
#include <QTextStream>

Request::Request(FunctionName _functionName, QString _code, TuriProgram *_program) {
    functionName = _functionName;
    code = _code;
    program = _program;
}


QString Request::serialize() {
    QJsonDocument doc;
    QJsonObject requestObj;

    requestObj.insert("functionName", functionName);
    requestObj.insert("code", code);
    if (program != nullptr) {
        // @TODO
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
    TuriProgram * _program = nullptr;
    if (!requestObj.value("program").isNull()) {
        // @TODO
    }
    return Request(_functionName, _code, _program);
}

void Request::clean() {
    if (program != nullptr) delete program;
}
