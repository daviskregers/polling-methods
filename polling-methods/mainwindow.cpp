#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::attachText(QString text) {

    ui->textEdit->setText( ui->textEdit->toPlainText() + "\n" + text );

}

void MainWindow::on_pushButton_clicked()
{

    attachText("\nStart processing");
    attachText("Processing finished\n");

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}
