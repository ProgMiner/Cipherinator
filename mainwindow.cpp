#include "main.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::applySettings(){
    //Alphabet
    QActionGroup* gr = nullptr;
    if(ui->menu_3->actions().size() > 0) gr = ui->menu_3->actions()[0]->actionGroup();
    else gr = new QActionGroup(this);
    QString checkedName = gr->actions().size() > 0 ? gr->checkedAction()->text() : "";
    int checked = gr->actions().size() > 0 ? gr->actions().indexOf(gr->checkedAction()) : 0;
    ui->menu_3->clear();
    for(QString string : Main::settings->alphabet.keys())
        gr->addAction(ui->menu_3->addAction(string))->setCheckable(true);
    if(!checkedName.isEmpty())
        for(QAction* act : gr->actions())
            if(act->text() == checkedName) checked = gr->actions().indexOf(act);
    gr->actions()[checked > gr->actions().size() - 1 ? gr->actions().size() - 1 : checked]->setChecked(true);
    connect(gr, SIGNAL(triggered(QAction*)), this, SLOT(actionGroup_triggered(QAction*)));
    setWindowTitle((QString) APPNAME + (QString) ": " + gr->checkedAction()->text());
    //Plugin
    ui->comboBox->clear();
    for(QString string : Main::ciphers.uniqueKeys())
        ui->comboBox->addItem(string);
    ui->menu->clear();
    for(QString string : Main::extraActions.uniqueKeys())
        ui->menu->addAction(string);
    if(Main::extraActions.size() > 0)
        ui->menu->addSeparator();
    ui->menu->addAction(ui->action_2);
    ui->menu->addAction(ui->action_5);
}

void MainWindow::actionGroup_triggered(QAction* action){
    setWindowTitle((QString) APPNAME + (QString) ": " + action->actionGroup()->checkedAction()->text());
}

void MainWindow::on_pushButton_clicked(){
    QMessageBox::information(this, "О шифре", Main::cipherDescriptions[ui->comboBox->currentText()]);
}

void MainWindow::calculate(){
    QJSValueList args;
    args << ui->lineEdit->text() << Main::settings->alphabet[ui->menu_3->actions()[0]->actionGroup()->checkedAction()->text()]
         << ui->checkBox->isChecked() << ui->lineEdit_2->text();
    QString ret = Main::ciphers[ui->comboBox->currentText()].call(args).toString();
    if(ui) ui->plainTextEdit->setPlainText(ret);
}

void MainWindow::on_checkBox_clicked(){
    if(ui->checkBox->isChecked()) ui->lineEdit->setPlaceholderText("Зашифрованный текст");
    else ui->lineEdit->setPlaceholderText("Исходный текст");
    calculate();
}

void MainWindow::on_lineEdit_textChanged(){
    calculate();
}

void MainWindow::on_comboBox_activated(int i){
    calculate();
}

void MainWindow::on_lineEdit_2_textChanged(){
    calculate();
}

void MainWindow::on_action_2_triggered(){
    Main::settingsDialog = new SettingsDialog(Main::mainWindow);
    Main::settingsDialog->show();
}

void MainWindow::on_action_5_triggered(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("О программе");
    msgBox.setWindowIcon(windowIcon());
    msgBox.setText((QString) APPNAME + " " + (QString) APPVERSION + " by ProgMiner");
    msgBox.addButton("OK", QMessageBox::NoRole);
    msgBox.addButton("О Qt", QMessageBox::HelpRole);
    msgBox.addButton("by ProgMiner", QMessageBox::YesRole);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIconPixmap(QPixmap(":res/icon.png").scaled(64, 64));
    int i = msgBox.exec();
    if(i == 1) QMessageBox::aboutQt(this, "О Qt");
    if(i == 2) if(QMessageBox::question(this, "Подтверждение", "Открыть ссылку в браузере?", "Да", "Нет") == 0)
        QDesktopServices::openUrl(QUrl("http://byprogminer.esy.es/"));
}
