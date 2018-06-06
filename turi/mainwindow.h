#pragma once

#include <QCloseEvent>
#include <QMainWindow>
#include <graphwidget.h>
#include <turiparsererror.h>
#include <turiprogram.h>
#include "client.h"

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
    bool firstRun = true;
    QString originCodeText;
    Client * client = nullptr;
    int carriageIndex = -1;

  public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();

  private slots:
    void on_codeEdit_textChanged();

    void on_actionOpen_triggered();

    void on_setupBtn_clicked();

    void on_inputEdit_textChanged(const QString & arg1);

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void closeEvent(QCloseEvent * ev);

    void on_actionExit_triggered();

    void on_actionAbout_Turi_IDE_triggered();

    void on_nextBtn_clicked();

    void on_prevBtn_clicked();

    void on_runBtn_clicked();

    void on_resetBtn_clicked();

    void on_resetAndRunBtn_clicked();

    void on_moveLeftBtn_clicked();

    void on_moveRightBtn_clicked();

  private:
    Ui::MainWindow * ui;

    void printProgramTable();

    void printErrorsList();

    void validateSetupBtn();

    int onClose();

    void printProgram();

    void markCurrentLine(int line);

    void setLineCounter();
};
