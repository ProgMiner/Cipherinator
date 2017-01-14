#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

namespace Ui {
    class EditDialog;
}

class EditDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditDialog(QString* textPointer, QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~EditDialog();
private slots:
     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

private:
    QString* textPointer;
    Ui::EditDialog *ui;
};

#endif // EDITDIALOG_H
