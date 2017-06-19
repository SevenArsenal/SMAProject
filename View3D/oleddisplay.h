#ifndef OLEDDISPLAY_H
#define OLEDDISPLAY_H

#include <QObject>
#include <QDebug>

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

#include <getopt.h>

#define CARACTERE_H 7
#define YELLOW_H 2
#define BLUE_H 6


class OLEDdisplay : public QObject
{
    Q_OBJECT
private:
    bool isEnable;

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
    void clear();
    void clear_blue();
    void clear_yellow();
    void kill();
    void Headset(int _Headset, int _SMA[]);
};

#endif // OLEDDISPLAY_H
