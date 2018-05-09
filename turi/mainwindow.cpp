#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextCodec>
#include <iostream>
#include <turicarette.h>
#include <turiparser.h>

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_codeEdit_textChanged() {
    QString codeText = ui->codeEdit->toPlainText();
    TuriParser parser(codeText);
    if (program != nullptr) delete program;
    program = parser.parseTuriProgram();
    // TuriParserError::clearErrorsList(errors);
    errors = parser.getErrors();
    printErrorsList();
    if (errors.isEmpty()) {
        printProgramTable();
        validateRunBtn();
    }
}

void MainWindow::printErrorsList() {
    ui->errorsList->clear();
    for (auto error : errors) {
        ui->errorsList->addItem(error->toQString());
    }
}

void MainWindow::printProgramTable() {
    ui->tableWidget->setRowCount(program->count());
    for (int i = 0; i < program->count(); i++) {
        TuriCommand * currentCommand = program->getCommand(i);
        ui->tableWidget->setItem(
            i, 0, new QTableWidgetItem(currentCommand->getCurrentState()));
        ui->tableWidget->setItem(
            i, 1, new QTableWidgetItem(currentCommand->getCurrentSymbol()));
        ui->tableWidget->setItem(
            i, 2, new QTableWidgetItem(currentCommand->getNextSymbol()));
        QString direction =
            currentCommand->getDirection() == LEFT
                ? "L"
                : currentCommand->getDirection() == NONE ? "N" : "R";
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(direction));
        ui->tableWidget->setItem(
            i, 4, new QTableWidgetItem(currentCommand->getNextState()));
    }
}

void MainWindow::on_actionOpen_triggered() {
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("TURI files (*.turi)"));
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        QString fileName = fileNames.at(0);

        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray a = file.readAll();
        QString codeStr = QTextCodec::codecForMib(106)->toUnicode(a);
        file.close();

        ui->codeEdit->setText(codeStr);
    }
}

void MainWindow::on_actionLight_triggered() {}

void MainWindow::on_runBtn_clicked() {
    TuriCarette carette(ui->inputEdit->text());
    if (carette.exec(program) == 0) {
        ui->outputResult->setText(carette.getResult());
    } else
        ui->outputResult->setText("Matching state not found");
}

void MainWindow::on_inputEdit_textChanged(const QString & arg1) {
    validateRunBtn();
}

void MainWindow::validateRunBtn() {
    ui->runBtn->setEnabled(!ui->inputEdit->text().isEmpty() &&
                           errors.isEmpty());
}
