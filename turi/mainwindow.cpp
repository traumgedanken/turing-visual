#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextCodec>
#include <turiparser.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFileBtn_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("TURI files (*.turi)"));
    QStringList fileNames;
    if (dialog.exec()){
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

void MainWindow::on_codeEdit_textChanged()
{
    QString codeText = ui->codeEdit->toPlainText();
    TuriParser parser(codeText);
    parser.parseTuriProgram();
    auto errors = parser.getErrors();
    for (auto error : errors) {
        std::cout << "At line " << error->getLine() << " : " << error->getErrorText().toStdString() << std::endl;
    }
}
