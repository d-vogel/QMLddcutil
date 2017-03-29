#ifndef DISPLAYLISTITEMMODEL_H
#define DISPLAYLISTITEMMODEL_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDebug>

class displayListItemModel: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString monitorName READ monitorName NOTIFY monitorNameChanged)
    Q_PROPERTY(unsigned int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY(unsigned int minBrightness READ minBrightness WRITE setMinBrightness NOTIFY minBrightnessChanged)
    Q_PROPERTY(unsigned int maxBrightness READ maxBrightness WRITE setMaxBrightness NOTIFY maxBrightnessChanged)
    Q_PROPERTY(unsigned int stepBrightness READ stepBrightness WRITE setStepBrightness NOTIFY stepBrightnessChanged)
    Q_PROPERTY(QStringList whitepointValues READ whitepointValues WRITE setWhitepointValues NOTIFY whitepointValuesChanged)
    Q_PROPERTY(QString whitepointSelected READ whitepointSelected WRITE setWhitepointSelected NOTIFY whitepointSelectedChanged)
public:
    displayListItemModel();
    displayListItemModel(const QString &monitorName, const unsigned int brightness,
                         const unsigned int minBrightness, const unsigned int maxBrightness,
                         const unsigned int stepBrightness, const QStringList whitepointValues);


    QString monitorName() const;
    void setMonitorName(const QString &newName);

    unsigned int brightness() const;
    void setBrightness(const unsigned int newBrightness);

    unsigned int minBrightness() const;
    void setMinBrightness(const unsigned int newMinBrightness);

    unsigned int maxBrightness() const;
    void setMaxBrightness(const unsigned int newMaxBrightness);

    unsigned int stepBrightness() const;
    void setStepBrightness(const unsigned int newStepBrightness);

    QStringList whitepointValues() const;
    void setWhitepointValues(const QStringList newWhitepointValues);

    QString whitepointSelected() const;
    void setWhitepointSelected(const QString &newWhitepointSelected);

signals:
    void monitorNameChanged();
    void brightnessChanged(long newBrightness);
    void minBrightnessChanged();
    void maxBrightnessChanged();
    void stepBrightnessChanged();
    void whitepointValuesChanged();
    void whitepointSelectedChanged(QString newWhitePoint);
private:
    QString m_monitorName;
    unsigned int m_brightness;
    unsigned int m_minBrightness;
    unsigned int m_maxBrightness;
    unsigned int m_stepBrightness;
    QStringList m_whitepointValues;
    QString m_whitepointSelected;
};

#endif // DISPLAYLISTITEMMODEL_H
