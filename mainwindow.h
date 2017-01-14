#pragma once

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void applySettings();
    void calculate();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_clicked();

    void on_lineEdit_textChanged();

    void on_comboBox_activated(int i);

    void on_action_2_triggered();

    void on_action_5_triggered();

    void on_lineEdit_2_textChanged();

    void actionGroup_triggered(QAction *action);

private:
    Ui::MainWindow *ui;
};
