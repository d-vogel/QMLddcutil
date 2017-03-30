#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <qqmlcontext.h>


#include "ddcutilController.h"
#include "displaylistitemmodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    DDCutilController* brightnessController= new DDCutilController();
    brightnessController->detect();

    QList<QObject*> displayList;
    for (unsigned int iDisp=0; iDisp<brightnessController->monitorNumber(); iDisp++)
    {
        displayListItemModel* displayInstance= new displayListItemModel(
                    brightnessController->nameForDisplay(iDisp),
                    brightnessController->brightness(iDisp),
                    0,
                    brightnessController->brightnessMax(iDisp),
                    1, QStringList(brightnessController->whitePointList(iDisp))
                    );
        //New connect syntax, see https://wiki.qt.io/New_Signal_Slot_Syntax
        QObject::connect(displayInstance, &displayListItemModel::brightnessChanged, [=](const long newBrightness)
        {
            brightnessController->setBrightness(iDisp, newBrightness);
        });
        QObject::connect(displayInstance, &displayListItemModel::whitepointSelectedChanged, [=](const QString newWhitePoint)
        {
            brightnessController->setWhitepoint(iDisp, newWhitePoint);
        });
        displayList.append(displayInstance);

    }

    engine.rootContext()->setContextProperty("displayListItemModel", QVariant::fromValue(displayList));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
