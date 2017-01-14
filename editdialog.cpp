#include "editdialog.h"
#include "ui_editdialog.h"
#include <QCloseEvent>

EditDialog::EditDialog(QString* textPointer, QWidget *parent) : QDialog(parent), ui(new Ui::EditDialog) {
    ui->setupUi(this);
    this->textPointer = textPointer == nullptr ? new QString() : textPointer;
    setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint) | Qt::WindowMaximizeButtonHint);
    ui->plainTextEdit->setPlainText(*this->textPointer);
}

void EditDialog::closeEvent(QCloseEvent *event) {
    delete this;
    event->accept();
}

EditDialog::~EditDialog() {
    delete ui;
}

void EditDialog::on_pushButton_clicked() {
    close();
}

void EditDialog::on_pushButton_2_clicked() {
    *textPointer = ui->plainTextEdit->toPlainText();
    on_pushButton_clicked();
}
