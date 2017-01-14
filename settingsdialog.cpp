#include "main.h"
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QFileSystemWatcher>
#include <QTemporaryFile>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>

Main::Settings tmpSettings;
bool tableRefresh = false;

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog){
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint);
    tmpSettings = *Main::settings;
    //General

    //Alphabet
    refreshAlphabetCombobox();
    //Plugins
    ui->tableWidget->setColumnWidth(0, ui->tableWidget->width() * 0.75);
    ui->tableWidget->setColumnWidth(1, ui->tableWidget->width() * 0.25);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    refreshPluginTable();
}

void SettingsDialog::refreshAlphabetCombobox(){
    ui->comboBox->clear();
    for(QString string : tmpSettings.alphabet.uniqueKeys())
        ui->comboBox->addItem(string);
    ui->comboBox->setCurrentIndex(0);
    ui->textEdit->setPlainText(tmpSettings.alphabet[ui->comboBox->currentText()]);
}

void SettingsDialog::refreshPluginTable(){
    tableRefresh = true;
    ui->tableWidget->setRowCount(0);
    for(QString string : tmpSettings.plugin.uniqueKeys()){
        bool enabled = tmpSettings.plugin[string].enabled;
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(string, 0));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(0));
        ui->tableWidget->item(row, 1)->setCheckState((Qt::CheckState)(enabled ? 2 : 0));
    }
    tableRefresh = false;
}

void SettingsDialog::closeEvent(QCloseEvent *event) {
    delete Main::settingsDialog;
    Main::settingsDialog = nullptr;
    event->accept();
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}


void SettingsDialog::on_pushButton_2_clicked() {
    close();
}

void SettingsDialog::on_pushButton_clicked() {
    if(*Main::settings != tmpSettings) {
        *Main::settings = tmpSettings;
        Main::writeSetting(*Main::settings);
        Main::applySettings();
    }
    on_pushButton_2_clicked();
}

void SettingsDialog::on_pushButton_4_clicked() {
    if(QMessageBox::warning(this, "Подтверждение", "Удалить алфавит \"" + ui->comboBox->currentText() + "\"?", "Да", "Нет") != 0) return;
    tmpSettings.alphabet.remove(ui->comboBox->currentText());
    refreshAlphabetCombobox();
}

void SettingsDialog::on_pushButton_3_clicked() {
    bool ok = false;
    QString str = QInputDialog::getText(this, "", "Введите название алфавита:", QLineEdit::Normal, "", &ok,
                                        QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint);
    if(!ok) return;
    if(str.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Нужно ввести название алфавита!");
        return;
    }
    tmpSettings.alphabet[str] = QString();
    refreshAlphabetCombobox();
    ui->comboBox->setCurrentText(str);
}

void SettingsDialog::on_comboBox_currentTextChanged(const QString &arg1) {
    if(tmpSettings.alphabet.contains(arg1))
        ui->textEdit->setPlainText(tmpSettings.alphabet[arg1]);
}

void SettingsDialog::on_pushButton_5_clicked() {
    if(QMessageBox::warning(this, "Подтверждение", "Очистить поле алфавита?", "Да", "Нет") != 0) return;
    ui->textEdit->clear();
}

void SettingsDialog::on_textEdit_textChanged(){
    if(tmpSettings.alphabet.contains(ui->comboBox->currentText()))
        tmpSettings.alphabet[ui->comboBox->currentText()] = ui->textEdit->toPlainText();
}

void SettingsDialog::on_tableWidget_customContextMenuRequested() {
    QMenu* menu = new QMenu(this);
    QMenu* addingMenu = new QMenu("Добавить..", this);
    addingMenu->addAction(ui->action_4);
    addingMenu->addAction(ui->action_5);
    menu->addMenu(addingMenu);
    if(ui->tableWidget->currentRow() > -1){
        menu->addSeparator();
        menu->addAction(ui->action);
        menu->addAction(ui->action_3);
        menu->addAction(ui->action_2);
    }
    menu->exec(QCursor::pos());
}

void SettingsDialog::on_action_4_triggered() {
    bool ok = false;
    QString str = QInputDialog::getText(this, "", "Введите название дополнения:", QLineEdit::Normal, "", &ok,
                                        QInputDialog().windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowCloseButtonHint);
    if(!ok) return;
    if(str.isEmpty()){
        QMessageBox::warning(this, "Ошибка", "Нужно ввести название дополнения!");
        return;
    }
    if(tmpSettings.plugin.contains(str)){
        if(QMessageBox::warning(this, "Ошибка", "Дополнение с именем \"" + str + "\" уже существует! Заменить?", "Да", "Нет") == 0)
            tmpSettings.plugin.remove(str);
        else return;
    }
    tmpSettings.plugin[str] = Main::Plugin();
    tmpSettings.plugin[str].code = "function init(){\n//Your script\n}";
    tmpSettings.plugin[str].enabled = false;
    refreshPluginTable();
}

void SettingsDialog::on_action_5_triggered() {
    QString str = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::currentPath(), "JS Дополнения (*.js)");
    if(str.isEmpty()) return;
    QString fileName = str;
    str = QFileInfo(fileName).baseName();
    if(tmpSettings.plugin.contains(str)){
        if(QMessageBox::warning(this, "Ошибка", "Дополнение с именем \"" + str + "\" уже существует! Заменить?", "Да", "Нет") == 0)
            tmpSettings.plugin.remove(str);
        else return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить дополнение");
        return;
    }
    tmpSettings.plugin[str] = Main::Plugin();
    tmpSettings.plugin[str].code = QString(file.readAll());
    tmpSettings.plugin[str].enabled = true;
    refreshPluginTable();
}

void SettingsDialog::on_tableWidget_itemChanged(QTableWidgetItem *item) {
    if(item->column() != 1) return;
    if(tableRefresh) return;
    QString str = ui->tableWidget->item(item->row(), 0)->text();
    if(!tmpSettings.plugin.contains(str)) return;
    tmpSettings.plugin[str].enabled = (bool) item->checkState();
}

void SettingsDialog::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item) {
    if(item->column() == 1)
        item->setCheckState(((bool) item->checkState()) ? Qt::Unchecked : Qt::Checked);
    else if(item->column() == 0){
        (new EditDialog(&tmpSettings.plugin[ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text()].code, this))->show();
    }else qDebug() << "HOW!?";
}

void SettingsDialog::on_action_triggered() {
    QString str = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
    if(!tmpSettings.plugin.contains(str)) return;
    (new EditDialog(&tmpSettings.plugin[str].code, this))->show();
}

void SettingsDialog::on_action_2_triggered() {
    if(QMessageBox::warning(this, "Подтверждение", "Удалить дополнение \"" + ui->tableWidget->currentItem()->text() + "\"?", "Да", "Нет") != 0) return;
    tmpSettings.plugin.remove(ui->tableWidget->currentItem()->text());
    refreshPluginTable();
}
