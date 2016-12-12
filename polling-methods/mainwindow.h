#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString attributes[20];

    QString table[20][20] = {
       {"x", "y", "z", "x", "z", "w"},
       {"y", "z", "y", "t", "t", "z"},
       {"z", "x", "t", "u", "v", "u"},
       {"v", "t", "u", "v", "u", "v"},
       {"w", "u", "x", "w", "x", "t"},
       {"u", "w", "v", "y", "y", "y"},
       {"t", "v", "w", "z", "w", "x"}
    };

    int n = 0, // table i length (when filled)
        m = 0; // table j length (when filled)

    void attachText(QString text);
    void do_process();
    bool preprocess_data();
    void test_data();

    void do_vare();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
