#include <QString>
#include <QVector>
#include <turiparsererror.h>

class HtmlText {
    QString text;

  public:
    HtmlText(QString _text);
    QString getText();

    void markErrors(QVector<TuriParserError *> & errors, QString color);
};
