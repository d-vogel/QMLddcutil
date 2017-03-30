
#ifndef DDCUTILCONTROLLER_H
#define DDCUTILCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <ddcutil_c_api.h>


class DDCutilController: public QObject
{
    Q_OBJECT

public:
    DDCutilController();
    void detect();
    //    bool isSupported() const;
    unsigned int monitorNumber() const;
    long brightness(const unsigned int dispIdx) const;
    long brightnessMax(const unsigned int dispIdx) const;
    QList<QString> whitePointList(const unsigned int dispIdx) const;
public slots:
    void setBrightness(const unsigned int dispIdx, long value);
    QString nameForDisplay(const unsigned int iDisplay);
    void setWhitepoint(const unsigned int dispIdx, const QString &newWhitepoint);

private:

    QList<DDCA_Display_Handle> m_displayHandleList;
    QList<DDCA_Display_Info> m_displayInfoList;

    //Per display properties
    //destription mapped to vcp values for easy retrieval
    QList<QMap<QString, int>*> m_descrToVcp_perDisp;
    QList<QMap<int, QMap<int, QString>*>*> m_vcpTovcpValueWithDescr_perDisp;

};

#endif
