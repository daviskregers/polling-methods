#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTableWidgetItem* ptwi;

    for( int i = 0; i < 20; i++) {
        for( int j = 0; j < 20; j++) {
            ptwi = new QTableWidgetItem(table[i][j]);
            ui->tableWidget->setItem(i,j,ptwi);
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::attachText(QString text) {
    ui->textEdit->setText( text + "\n" + ui->textEdit->toPlainText() );
}

bool MainWindow::preprocess_data() {
    attachText("Processing input data");

    this->n = 0;
    this->m = 0;

    int maxCol = 19;
    int maxRow = 19;

    // reading

    for( int i = 0; i < maxRow; i++ ) {

        if(ui->tableWidget->item(i, 0)->text() != "") {
            this->n++;
            this->m = 0;
        }
        else {
            maxRow = i + 1;
            break;
        }

        bool found_attr = false;

        for(int j = 0; j < maxCol; j++) {

            if(ui->tableWidget->item(i, j)->text() != "") {
                this->m++;
                table[i][j] = ui->tableWidget->item(i, j)->text();
            }
            else {
                maxCol = j + 1;
                break;
            }

        }

    }

    // validation

    bool hasDuplicate = false;
    int j = 0, i = 0;
    for(int j = 0; j <= this->n && !hasDuplicate; j++) { // each alternative

        QString valid[20] = {};

        for(int i = 0; i <= this->m && !hasDuplicate; i++) { // each expert

            for(int k = 0; k < i && !hasDuplicate; k++) { // duplicate search

                if(valid[k] == table[i][j] && table[i][j] != "") {
                    attachText( "duplicate found on ("+ QString::number(i+1) +";e" + QString::number(j+1) +"); char: "+ ui->tableWidget->item(i,j)->text() );
                    hasDuplicate = true;
                }
                else {
                    valid[i] = table[i][j];
                }

            }


        }
    }


    attachText("Read "+ QString::number(this->n) +" alternatives");
    attachText("Read "+ QString::number(this->m) +" experts");

    // attributes
    if(!hasDuplicate) {

        for( int j = 0; j < 20; j++ ) {
            this->attributes[j] = "";
        }

        for( int j = 0; j < 20; j++ ) {

            if(ui->tableWidget->item(j, 0)->text() != "") {
                this->attributes[j] = ui->tableWidget->item(j, 0)->text();
            }

        }

        QString attribs = "";
        for( int j = 0; j < 20; j++ ) {
            if(attributes[j] != "") {
                attribs = attribs + attributes[j] + ", ";
            }
        }

        attachText("Alternatives: " + attribs);

    }

    return !hasDuplicate;

}

void MainWindow::do_vare() {
    attachText("Using vare method");

    QString occurences[20] = {};
    QString discarded = "";
    bool finished = false;
    int iters = 0;

        while(!finished) {
            iters++;

            if( iters > 50 ) {
                attachText("50 Max iterations exceeded - stopping");
                break;
            }

            for( int i = 0; i < 20; i++) {
                occurences[i] = "";
            }

            for( int i = 0; i < 20; i++) { // each column
                if(table[0][i] == "") break;

                for( int k = 0; k < 20; k++) {

                    if(table[k][i] == "") break;

                    // check if discarded
                    if( discarded.contains(table[k][i], Qt::CaseInsensitive) ) {
                        continue; // increase row
                    }
                    else {

                        bool found = false;
                        // not discarded, check if not already occured
                        for(int j = 0; j < 20; j++) {

                            if( occurences[j].contains(table[k][i]) ) {
                                occurences[j].remove(table[k][i], Qt::CaseInsensitive);
                                occurences[j+1] = occurences[j+1] + table[k][i];
                                found = true;
                                break;
                            }

                        }

                        if(!found) {
                            occurences[1] = occurences[1] + table[k][i];
                        }
                        break;

                    }

                }

            }

            QString votes = "";
            QString discard = "";
            int maxVal = 0;
            for( int i = 0; i < 20; i++) {
                if(occurences[i] != "") {
                    maxVal = i;
                    discard.append(occurences[i]);
                    votes = votes + " ["+ QString::number(i) +"] => " + occurences[i] + "\n";
                }
            }

            attachText("Step 1: following highest votes casted: \n" + votes);
            qDebug() << "maxVal: " << QString::number(maxVal);
            qDebug() << "maxValval: " << occurences[maxVal];
            qDebug() << "maxVallen: " << occurences[maxVal].length();

            if(occurences[maxVal].length() > 1) {

                discard.remove(occurences[maxVal], Qt::CaseInsensitive);
                discarded.append(discard);

                attachText(" discard lowest votes: " + discard);

            }
            else {
                attachText("\n found the winner! It is: " + occurences[maxVal]);
                finished = true;
            }

        }


}

void MainWindow::do_cumbs() {

    attachText("Using cumbs method");

    QString occurences[20] = {};
    QString discarded = "";
    bool finished = false;
    int iters = 0;

        while(!finished) {
            iters++;

            if( iters > 50 ) {
                attachText("50 Max iterations exceeded - stopping");
                break;
            }

            for( int i = 19; i >= 0; i--) {
                occurences[i] = "";
            }

            for( int i = 0; i < 20; i++) {
                if(!discarded.contains(attributes[i], Qt::CaseInsensitive)) {
                    occurences[0] = occurences[0] + attributes[i];
                }
            }

            for( int i = 19; i >= 0; i--) { // each column
                if(table[0][i] == "") continue;

                for( int k = 19; k >= 0; k--) {

                    if(table[k][i] == "") continue;

                    // check if discarded
                    if( discarded.contains(table[k][i], Qt::CaseInsensitive) ) {
                        continue; // increase row
                    }
                    else {

                        bool found = false;
                        // not discarded, check if not already occured
                        for(int j = 19; j >= 0; j--) {

                            if( occurences[j].contains(table[k][i]) ) {
                                occurences[j].remove(table[k][i], Qt::CaseInsensitive);
                                occurences[j+1] = occurences[j+1] + table[k][i];
                                found = true;
                                break;
                            }

                        }

                        if(!found) {
                            occurences[1] = occurences[1] + table[k][i];
                        }
                        break;

                    }

                }

            }

            QString votes = "";
            QString discard = "";
            int maxVal = 0;
            for( int i = 0; i < 20; i++) {
                if(occurences[i] != "") {
                    maxVal = i;

                }
            }

            int minVal = 19;
            for( int i = 19; i >= 0; i--) {
                if(occurences[i] != "") {
                    minVal = i;
                    discard.append(occurences[i]);
                    votes = votes + " ["+ QString::number(i) +"] => " + occurences[i] + "\n";
                }
            }

            attachText("Step 1: following highest votes casted: \n" + votes);
            qDebug() << "maxVal: " << QString::number(maxVal);
            qDebug() << "maxValval: " << occurences[maxVal];
            qDebug() << "maxVallen: " << occurences[maxVal].length();

            if(occurences[minVal].length() > 1) {

//                discard.remove(occurences[maxVal], Qt::CaseInsensitive);
                discarded.append(occurences[maxVal]);

                attachText(" discard lowest votes: " + discard);

            }
            else {
                attachText("\n found the winner! It is: " + occurences[minVal]);
                finished = true;
            }

        }
}

void MainWindow::do_process() {

    if(preprocess_data()) {

        if(ui->comboBox->currentIndex() == 0) {
            do_vare();
        }
        else if(ui->comboBox->currentIndex() == 1) {
            do_cumbs();
        }

    }



}

void MainWindow::on_pushButton_clicked() {

    attachText("\nStart processing");
    do_process();
    attachText("Processing finished\n");

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}
