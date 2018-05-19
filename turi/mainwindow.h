#include <QCloseEvent>
#include <QMainWindow>
#include <graphwidget.h>
#include <turiparsererror.h>
#include <turiprogram.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    TuriProgram * program = nullptr;
    QVector<TuriParserError *> errors;
    QString fileName;
    bool fileIsSaved = false;
    bool codeEditedByUser = true;
    GraphWidget * graph = new GraphWidget(this, program);

  public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

  private slots:
    void on_codeEdit_textChanged();

    void on_actionOpen_triggered();

    void on_actionLight_triggered();

    void on_runBtn_clicked();

    void on_inputEdit_textChanged(const QString & arg1);

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void closeEvent(QCloseEvent * ev);

    void on_actionExit_triggered();

    void on_actionAbout_Turi_IDE_triggered();

  private:
    Ui::MainWindow * ui;

    void printProgramTable();

    void printErrorsList();

    void validateRunBtn();

    int onClose();

    void printProgram();
};
