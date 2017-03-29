#include "displaylistitemmodel.h"

displayListItemModel::displayListItemModel()
{
}

displayListItemModel::displayListItemModel(const QString &monitorName, const unsigned int brightness,
                       const unsigned int minBrightness, const unsigned int maxBrightness,
                       const unsigned int stepBrightness, const QStringList whitepointValues)
    : m_monitorName(monitorName), m_brightness(brightness),
      m_minBrightness(minBrightness),m_maxBrightness(maxBrightness),
      m_stepBrightness(stepBrightness), m_whitepointValues(whitepointValues),
      m_whitepointSelected("")
{
}

QString displayListItemModel::monitorName() const
{
    return m_monitorName;
}

void displayListItemModel::setMonitorName(const QString &newName)
{
    m_monitorName=newName;
    emit monitorNameChanged();
}

unsigned int displayListItemModel::brightness() const
{
    return m_brightness;
}



void displayListItemModel::setBrightness(const unsigned int newBrightness)
{
    m_brightness=newBrightness;
    qDebug()<<"[displayListModel]: brightness changed to "<<m_brightness;
    emit brightnessChanged((long)m_brightness);
}

unsigned int displayListItemModel::minBrightness() const
{
    return m_minBrightness;
}

void displayListItemModel::setMinBrightness(const unsigned int newMinBrightness)
{
    m_minBrightness=newMinBrightness;
    emit minBrightnessChanged();
}

unsigned int displayListItemModel::maxBrightness() const
{
    return m_maxBrightness;
}

void displayListItemModel::setMaxBrightness(const unsigned int newMaxBrightness)
{
    m_maxBrightness=newMaxBrightness;
    emit maxBrightnessChanged();
}

unsigned int displayListItemModel::stepBrightness() const
{
    return m_stepBrightness;
}

void displayListItemModel::setStepBrightness(const unsigned int newStepBrightness)
{
    m_stepBrightness=newStepBrightness;
    emit stepBrightnessChanged();
}

QStringList displayListItemModel::whitepointValues() const
{
    qDebug()<<"whitepoints values:"<<m_whitepointValues;
    return m_whitepointValues;
}

void displayListItemModel::setWhitepointValues(const QStringList newWhitepointValues)
{
    m_whitepointValues = newWhitepointValues;
    emit whitepointValuesChanged();
}

QString displayListItemModel::whitepointSelected() const
{
    return m_whitepointSelected;
}

void displayListItemModel::setWhitepointSelected(const QString &newWhitepointSelected)
{
    m_whitepointSelected=newWhitepointSelected;
    qDebug()<<"new whitepoint selection:"<<m_whitepointSelected;
    emit whitepointSelectedChanged(m_whitepointSelected);
}


