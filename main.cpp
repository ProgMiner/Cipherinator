#include "main.h"
#include <QApplication>
#include <QtXml>

namespace Main {

Settings* settings;
MainWindow* mainWindow;
SettingsDialog* settingsDialog;

QJSEngine* js;
QMap<QString, QJSValue> extraActions, ciphers;
QMap<QString, QString> cipherDescriptions;

void applySettings(){
    ciphers.clear();
    extraActions.clear();
    for(Plugin pl : settings->plugin){
        if(!pl.enabled) continue;
        applyPlugin(pl.code);
    }
    mainWindow->applySettings();
}

void writeSetting(Settings settings){
    QDomDocument doc("settings");
    QDomElement root = doc.createElement("settings");
    //Alphabet
    for(QString string : settings.alphabet.uniqueKeys()){
        QDomElement elem = doc.createElement("alphabet");
        elem.setAttribute("name", QString(string.unicode()));
        elem.appendChild(doc.createTextNode(settings.alphabet[string]));
        root.appendChild(elem);
    }
    //Plugin
    for(QString string : settings.plugin.uniqueKeys()){
        QDomElement elem = doc.createElement("plugin");
        elem.setAttribute("name", string);
        elem.appendChild(doc.createTextNode(settings.plugin[string].code));
        elem.setAttribute("enabled", settings.plugin[string].enabled ? "true" : "false");
        root.appendChild(elem);
    }
    doc.appendChild(root);
    QFile file(SETTINGSFILE);
    if(file.open(QIODevice::WriteOnly)){
        file.write(DEFAULTCODEC.fromUnicode(doc.toString()));
        file.close();
    }
}

void applyPlugin(QString code){
    QList<QString> startKeys;
    QJSValueIterator* iter = new QJSValueIterator(js->globalObject());
    while(iter->hasNext()){
        iter->next();
        startKeys.push_back(iter->name());
    }
    QJSValue ciphs = js->newObject(), exActs = js->newObject(), ciphDescs = js->newObject(), env = js->newObject();
    env.setProperty("extraActions", exActs);
    env.setProperty("ciphers", ciphs);
    env.setProperty("cipherDescriptions", ciphDescs);
    env.setProperty("addCipher", js->evaluate("function(name, func, desc){"
                                             "if(typeof(func) == 'function')"
                                             "this.ciphers[name] = func;"
                                             "if(typeof(desc) != 'undefined')"
                                             "this.cipherDescriptions[name] = desc;"
                                             "else this.cipherDescriptions[name] = 'Cipher ' + name + ' has no description.';"
                                             "}"));
    env.setProperty("addExtraAction", js->evaluate("function(name, func){"
                                             "if(typeof(func) == 'function')"
                                             "this.extraActions[name] = func;"
                                             "}"));
    js->globalObject().setProperty("Cipherinator", env);
    js->evaluate(code);
    js->evaluate("function(){if(typeof(init) == 'function') init();}").call();
    QJSValueIterator iCiphers(ciphs), iCipherDescriptions(ciphDescs), iExActs(exActs);
    while(iCiphers.hasNext()){
        iCiphers.next();
        ciphers[iCiphers.name()] = iCiphers.value();
    }
    while(iCipherDescriptions.hasNext()){
        iCipherDescriptions.next();
        cipherDescriptions[iCipherDescriptions.name()] = iCipherDescriptions.value().toString();
    }
    while(iExActs.hasNext()){
        iExActs.next();
        extraActions[iExActs.name()] = iExActs.value();
    }

    iter = new QJSValueIterator(js->globalObject());
    while(iter->hasNext()){
        iter->next();
        if(!startKeys.contains(iter->name())) js->globalObject().deleteProperty(iter->name());
    }
}

}

using namespace Main;

Settings* readSetting(){
    Settings* ret = new Settings;
    if(!QFile(SETTINGSFILE).exists()) {
        ret->alphabet["Русский"] = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
        ret->alphabet["Русский (без \"ё\")"] = "абвгдежзийклмнопрстуфхцчшщъыьэюя";
        ret->alphabet["English"] = "abcdefghijklmnopqrstuvwxyz";
        ret->plugin["Default"].enabled = true;
        ret->plugin["Default"].code = "";
        QFile defCode(":res/Default.js");
        if(defCode.open(QIODevice::ReadOnly)) ret->plugin["Default"].code = QString(defCode.readAll());
        return ret;
    }
    QFile file(SETTINGSFILE);
    if(!file.open(QIODevice::ReadOnly)) return null;
    QDomDocument doc;
    doc.setContent(DEFAULTDECODER.toUnicode(file.readAll()));
    file.close();
    QDomElement root = doc.documentElement();
    if(root.tagName() != "settings") return null;
    QDomNode node = root.firstChildElement();
    while(!node.isNull()){
        QString tagName = node.nodeName(), nameAttr = node.attributes().namedItem("name").nodeValue();
        if(tagName == "alphabet")
            ret->alphabet[nameAttr] = node.firstChild().nodeValue();
        else if(tagName == "plugin"){
            ret->plugin[nameAttr].code = node.firstChild().nodeValue();
            ret->plugin[nameAttr].enabled = node.attributes().namedItem("enabled").nodeValue() == "true";
        } else qDebug() << tagName;
        node = node.nextSiblingElement();
    }
    return ret;
}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    js = new QJSEngine();
    if((settings = readSetting()) == null) return 0;
    mainWindow = new MainWindow();
    settingsDialog = new SettingsDialog(mainWindow);

    applySettings();
    mainWindow->setWindowIcon(QIcon(":res/icon.png"));
    mainWindow->show();

    return a.exec();
}
