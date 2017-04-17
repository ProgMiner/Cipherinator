#pragma once

#include "mainwindow.h"
#include "editdialog.h"
#include "settingsdialog.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QJSValueIterator>
#include <QJSEngine>
#include <QtDebug>
#include <QDir>

#define null nullptr
#define APPNAME "Cipherinator"
#define APPVERSION "v1.0"
#define SETTINGSFILE (QDir::homePath() + "/Cipherinator.xml")
#define DEFAULTCODECNAME "UTF-8"
#define DEFAULTCODEC (*QTextCodec::codecForName(DEFAULTCODECNAME))
#define DEFAULTDECODER QTextDecoder(&DEFAULTCODEC)

namespace Main {

struct Plugin{

    QString code;
    bool enabled;

    bool operator == (const Plugin &B) const {
       return this->code == B.code && this->enabled == B.enabled;
    }

};

struct Settings{
    //General settings

    //Alphabet settings
    QMap<QString, QString> alphabet;
    //Plugins settings
    QMap<QString, Plugin> plugin;

    void operator = (const Settings &B) {
       this->alphabet = B.alphabet;
       this->plugin = B.plugin;
    }

    bool operator != (const Settings &B) const {
       return !(this->alphabet == B.alphabet && this->plugin == B.plugin);
    }

    bool operator == (const Settings &B) const {
       return this->alphabet == B.alphabet && this->plugin == B.plugin;
    }

};

extern Settings* settings;
extern MainWindow* mainWindow;
extern SettingsDialog* settingsDialog;
extern QMap<QString, QJSValue> extraActions, ciphers;
extern QMap<QString, QString> cipherDescriptions;
extern void applySettings();
extern void writeSetting(Settings settings);
extern void applyPlugin(QString code);

}
