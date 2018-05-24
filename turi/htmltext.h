#include <QString>
#include <QVector>
#include <turiparsererror.h>

class HtmlText {
    QString text;
    QString ERROR_COLOR = "#E9967A";
    QString WARNING_COLOR = "#FFE4C4";

  public:
    HtmlText(QString _text);

    QString getText();

    void markErrors(QVector<TuriParserError *> & errors);
};
