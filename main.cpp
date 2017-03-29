#include <QGuiApplication>
#include <QQmlApplicationEngine>

//#include <qqmlengine.h>
#include <qqmlcontext.h>
//#include <qqml.h>
//#include <QtQuick/qquickitem.h>
//#include <QtQuick/qquickview.h>

#include "ddcutilController.h"
#include "displaylistitemmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    //    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    DDCutilController* brightnessController= new DDCutilController();
    brightnessController->detect();
    brightnessController->brightness();

    QList<QObject*> displayList;
    qDebug()<<brightnessController->whitePointList();
    displayListItemModel* firstDisplay= new displayListItemModel(
                brightnessController->nameForDisplay(0),
                brightnessController->brightness(),
                0,
                brightnessController->brightnessMax(),
                1, QStringList(brightnessController->whitePointList()));

    displayList.append(firstDisplay);

    QObject::connect(firstDisplay, SIGNAL(brightnessChanged(long)),
                     brightnessController, SLOT(setBrightness(long)));
    QObject::connect(firstDisplay, SIGNAL(whitepointSelectedChanged(QString)),
                     brightnessController, SLOT(setWhitepoint(QString)));
    engine.rootContext()->setContextProperty("displayListItemModel", QVariant::fromValue(displayList));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
