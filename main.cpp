#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>

#include <qqmlcontext.h>


#include "ddcutilController.h"
#include "displaylistitemmodel.h"

static QList<QObject*> displayList;
static DDCutilController* brightnessController;
static QQmlApplicationEngine* engine;

class QDetectThread: public QThread {
private slots:
    void onFinishedInternal() {
        if (displayList.count() > 0) {
            engine->rootContext()->setContextProperty("displayListItemModel", QVariant::fromValue(displayList));
            engine->rootContext()->setContextProperty("tabViewIndex", 1);
        } else {
            engine->rootContext()->setContextProperty("tabViewIndex", 2);
        }
    }

private:
    virtual void run() {
        // This is a hack to migrate to the main thread context
        QObject::connect(this, &QThread::finished, this, &QDetectThread::onFinishedInternal);

        brightnessController->detect();

        for (unsigned int iDisp=0; iDisp<brightnessController->monitorNumber(); iDisp++)
        {
            long bmax = brightnessController->brightnessMax(iDisp);
            displayListItemModel* displayInstance= new displayListItemModel(
                        brightnessController->nameForDisplay(iDisp),
                        brightnessController->brightness(iDisp),
                        0, bmax, bmax * 0.05,
                        QStringList(brightnessController->whitePointList(iDisp))
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
            displayInstance->moveToThread(engine->thread());
            displayList.append(displayInstance);
        }
    }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QDetectThread detectThread;

    engine = new QQmlApplicationEngine();

    brightnessController = new DDCutilController();

    engine->rootContext()->setContextProperty("displayListItemModel", QVariant::fromValue(displayList));
    engine->rootContext()->setContextProperty("tabViewIndex", 0);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

    detectThread.start();

    qDebug() << "main thread: " << QObject().thread()->currentThreadId();
    return app.exec();
}
