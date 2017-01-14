#pragma once

#include <QDialog>
#include <QTableWidget>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    void closeEvent(QCloseEvent *event);
    void refreshAlphabetCombobox();
    void refreshPluginTable();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_textEdit_textChanged();

    void on_tableWidget_customContextMenuRequested();

    void on_action_4_triggered();

    void on_action_5_triggered();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_action_triggered();

    void on_action_2_triggered();

private:
    Ui::SettingsDialog *ui;
};
