#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <QObject>
#include <QDebug>

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

#include <getopt.h>


class OLEDdisplay : public QObject
{
    Q_OBJECT
private:

    ArduiPi_OLED display;
    int line;
    int line_blue;
    int line_yellow;
    QByteArray data_yellow;
    QByteArray data_blue;
    unsigned int CountLine(QByteArray buff, int sz);
public:
    explicit OLEDdisplay(QObject *parent = 0);


signals:

public slots:
    void Write(QString _data);
    void Write_blue(QString _data);
    void Write_yellow(QString _data);
    void kill();
};

#endif // OLEDDISPLAY_H
