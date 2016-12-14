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
    attachText("Apstrādā ieejas datus");

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


    attachText("Atrastas "+ QString::number(this->n) +" alternatīvas");
    attachText("Atrasti "+ QString::number(this->m) +" eksperti");

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

        attachText("Alternatīvas: " + attribs);

    }

    return !hasDuplicate;

}

void MainWindow::do_vare() {
    attachText("Izmanto Varē metodi");

    QString occurences[20] = {};
    QString discarded = "";
    bool finished = false;
    int iters = 0;

        while(!finished) {
            iters++;

            if( iters > 50 ) {
                attachText("50 iterācijas bez rezultāta, apstādina");
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

            attachText("Augstākās balsis: \n" + votes);
            qDebug() << "maxVal: " << QString::number(maxVal);
            qDebug() << "maxValval: " << occurences[maxVal];
            qDebug() << "maxVallen: " << occurences[maxVal].length();

            if(occurences[maxVal].length() > 1) {

                discard.remove(occurences[maxVal], Qt::CaseInsensitive);
                discarded.append(discard);

                attachText("Izslēdz: " + discard);

            }
            else {
                attachText("Uzvarētājs: " + occurences[maxVal]);
                finished = true;
            }

        }


}

void MainWindow::do_cumbs() {

    attachText("Uzmanto Kumbsa metodi");

    QString occurences[20] = {};
    QString discarded = "";
    bool finished = false;
    int iters = 0;

        while(!finished) {
            iters++;

            if( iters > 50 ) {
                attachText("50 iterācijas bez rezultāta, apstādina");
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

            attachText("Augstākās balsis: \n" + votes);
            qDebug() << "maxVal: " << QString::number(maxVal);
            qDebug() << "maxValval: " << occurences[maxVal];
            qDebug() << "maxVallen: " << occurences[maxVal].length();

            if(occurences[minVal].length() > 1) {

//                discard.remove(occurences[maxVal], Qt::CaseInsensitive);
                discarded.append(occurences[maxVal]);

                attachText("Izslēdz mazāk balsotos: " + discard);

            }
            else {
                attachText("Uzvarētājs: " + occurences[minVal]);
                finished = true;
            }

        }
}

void MainWindow::cpyTable() {
   for( int i = 0; i < 20; i++) {
       for( int j = 0; j < 20; j++) {
           table2[i][j] = table[i][j];
       }
   }
}

void MainWindow::cpyTable2() {
    for( int i = 0; i < 20; i++) {
        for( int j = 0; j < 20; j++) {
            table[i][j] = table2[i][j];
        }
    }
}

void MainWindow::do_nansons() {

    QString discarded = "";
    bool finished = false;
    float variance = 0;
    int iters = 0;

    cpyTable();

        while(!finished) {

            cpyTable2();
            iters++;
            variance = 0;

            if( iters > 50 ) {
                attachText("50 iterācijas bez rezultāta, apstādina");
                break;
            }

            int sums[20];
            for( int i = 0; i < 20; i++) { // row
                sums[i] = -1;
            }

            // calculate sum
            for( int i = 0; i < 20; i++) { // row
                if(table[i][0] == "") break;
                for( int j = 0; j < 20; j++) { // column
                    if(table[i][j] == "") break;

                    for( int k = i; k < 20; k++) {
//                        if(table[k][j] == "") break;

                        // check if discarded
                        if( discarded.contains(table[k][j], Qt::CaseInsensitive) || table[k][j] == "" ) {
                            continue; // increase row
                        }
                        else {

                            int idx = 0;
                            bool found = false;
                            for( int l = 0; l < 20; l++) {
                                if(table[k][j] == attributes[l] && !discarded.contains(table[k][j], Qt::CaseInsensitive)) {
                                    idx = l;
                                    found = true;
                                    break;
                                }
                            }

                            if(found){
                                if(sums[idx] == -1) sums[idx] = 0;
                                qDebug() << "adding " << QString::number(i+1) << " to " << attributes[idx] << " " << discarded;
                                sums[idx] += (i + 1);
                                table[k][j] = "-";
                                break;
                            }

                        }

                    }

                }
            }

            int num = 0;
            for( int i = 0; i < 20; i++) { // row
                if(sums[i] != -1 && !discarded.contains(attributes[i], Qt::CaseInsensitive)) {
                    attachText(attributes[i] + " sum is: " + QString::number(sums[i]));
                    num++;
                }
            }

            // calculate variance

            for( int i = 0; i < 20; i++) { // row
                if(sums[i] != -1) {
                    variance += sums[i];
                }
            }

            qDebug() << variance << " " << num;
            variance /= num;

            attachText("Atribūtu skaits " + QString::number(num));
            attachText("Vid.aritm. ir " + QString::number(variance));

            // discard elements with sum > variance
            // and find

            for( int i = 0; i < 20; i++) {
                if(sums[i] > variance && sums[i] != -1) {
                    discarded.append(attributes[i]);
                    attachText("Izslēdz "+ attributes[i]+" ar "+ QString::number(sums[i])+" > " + QString::number(variance));

                }
            }

            int num_attr = 0;
            QString attribs;
            for( int i = 0; i < 20; i++) {
                if(attributes[i] != "") {
                    num_attr++;
                    attribs.append(attributes[i]);
                }

            }

            if(num == 1) {
                attachText("Uzvarētājs: " + attributes[num-1]);
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
        else if(ui->comboBox->currentIndex() == 2) {
            do_nansons();
        }

    }



}

void MainWindow::on_pushButton_clicked() {

    ui->textEdit->clear();

    attachText("\nSāk skaitļošanu");
    do_process();
    attachText("Skaitļošana pabeigta\n");

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}
