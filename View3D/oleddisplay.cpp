#include "oleddisplay.h"

OLEDdisplay::OLEDdisplay(QObject *parent) :
    QObject(parent)
{
    if(!display.init(OLED_I2C_RESET,OLED_ADAFRUIT_I2C_128x64) ){
        qDebug() << "INIT I2C DISPLAY ERROR";
        return;
    }

    display.begin();

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();   		// display it (clear display)

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    line = 0;
    line_blue = 0;
    line_yellow = 0;

    isEnable = true;
    //display.setHorizontalScrollProperties(1,0,128,0,64,1);

/*
    display.print("Hello, world!\n");
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.printf("%f\n", 3.141592);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.printf("0x%8X\n", 0xDEADBEEF);
    display.display();
*/
}

void OLEDdisplay::Write(QString _data)
{
    if(!isEnable)
        return;
    _data.append("\n");

    QByteArray array = _data.toLocal8Bit();

    //line += ((int)(array.size()*6)/display.width()) +1;

    line += CountLine(array,array.size());

    if(line>8)
    {
        display.clearDisplay();
        display.setCursor(0,0);
        line = CountLine(array,array.size());
    }
    qDebug() << "line = " <<line <<", array.size()="<<array.size()*6<<"display.width()"<<display.width();

    display.print(array.data());
    display.display();

}
void OLEDdisplay::Write_blue(QString _data)
{
    if(!isEnable)
        return;
    //display.setCursor(0,CARACTERE_H*line_blue+16);// Go at the end of the previous text

    data_blue += _data.toLocal8Bit();
    line_blue += CountLine(_data.toLocal8Bit(),_data.size());

    //qDebug() << _data;
    //qDebug() << "data_blue" << data_blue;
    //qDebug() << "line_blue = " << line_blue;

    if(line_blue>BLUE_H)
    {
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(data_yellow.data());// keep the yellow part
        display.setCursor(0,16);

        data_blue.clear();
        data_blue = _data.toLocal8Bit();
        line_blue = CountLine(data_blue,data_blue.size());
    }


    display.setCursor(0,16);
    display.print(data_blue.data());
    display.display();

    //qDebug() << "\n\n";

}
void OLEDdisplay::Write_yellow(QString _data)
{
    if(!isEnable)
        return;
    //display.setCursor(0,CARACTERE_H*line_yellow+16);// Go at the end of the previous text

    data_yellow += _data.toLocal8Bit();
    line_yellow += CountLine(_data.toLocal8Bit(),_data.size());

    //qDebug() << _data;
    //qDebug() << "data_yellow" << data_yellow;
    //qDebug() << "line_yellow = " << line_yellow;

    if(line_yellow>YELLOW_H)
    {
        display.clearDisplay();
        display.setCursor(0,16);
        display.print(data_blue.data());// keep the yellow part
        display.setCursor(0,0);

        data_yellow.clear();
        data_yellow = _data.toLocal8Bit();
        line_yellow = CountLine(data_yellow,data_yellow.size());
    }


    display.setCursor(0,0);
    display.print(data_yellow.data());
    display.display();

    //qDebug() << "\n\n";

}

void OLEDdisplay::clear()
{
    clear_yellow();
    clear_blue();
}

void OLEDdisplay::clear_blue()
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.print(data_yellow.data());// keep the yellow part
    display.setCursor(0,16);

    data_blue.clear();
    line_blue = 0;

}

void OLEDdisplay::clear_yellow()
{
    display.clearDisplay();
    display.setCursor(0,16);
    display.print(data_blue.data());// keep the blue part
    display.setCursor(0,0);

    data_yellow.clear();
    line_yellow = 0;
}

void OLEDdisplay::kill()
{

    isEnable = false;

    display.clearDisplay();   // clears the screen  buffer
    display.display();   		// display it (clear display)
    display.fillRect(0,0,display.width(),display.height(),WHITE);
    display.setCursor(0,0);
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.setTextSize(4);
    display.printf(" END  *_*   ");
    display.startscrollright(0x00, 0x0F);
    display.display();
}

void OLEDdisplay::Headset(int _Headset, int _SMA[])
{

    QString Tosend = "H"+QString::number(_Headset)+" ";
    for(int i=0;i<3;i++){

        Tosend.append("S" +QString::number(i)+":");
        if(_SMA[i]){
            Tosend.append("X ");
        }else{
            Tosend.append("O ");
        }
    }
    Tosend.append("\n");
    Write_blue(Tosend);
}
unsigned int OLEDdisplay::CountLine(QByteArray buff,int sz)
{
    int newlines = 0;
    int onthisline = 0;
    int linesize = 21;
    const char * p = buff.data();

    for(int i = 0 ; i < sz ;i++){
        if(p[i] == '\n'){
            newlines++;
            onthisline = 0;
        }else{
            onthisline++;
            if(onthisline > linesize){
                newlines ++;
                onthisline = 0;
            }
        }
    }

    //qDebug() << "newlines" << newlines;

    return newlines;
}
