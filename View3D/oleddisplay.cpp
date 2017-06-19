#include "oleddisplay.h"

OLEDdisplay::OLEDdisplay(QObject *parent) :
    QObject(parent)
{
    if(!display.init(OLED_I2C_RESET,OLED_ADAFRUIT_I2C_128x64) ){
        qDebug() << "INIT I2C DISPLAY ERROR";
    }

    display.begin();

    // init done
    display.clearDisplay();   // clears the screen  buffer
    display.display();   		// display it (clear display)

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    line = 0;
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
    display.setCursor(0,5*line_blue+16);// Go at the end of the previous text

    data_blue += _data.toLocal8Bit();
    line_blue += CountLine(data_blue,data_blue.size());


   // qDebug() << "line_blue = " << line_blue;

    if(line_blue>5)
    {
        display.clearDisplay();
        display.setCursor(0,0);
        display.print(data_yellow.data());// keep the yellow part
        display.setCursor(0,16);

        data_blue = _data.toLocal8Bit();
        line_blue = CountLine(data_blue,data_blue.size());
    }


    display.print(data_blue.data());
    display.display();

}
void OLEDdisplay::Write_yellow(QString _data)
{
    display.setCursor(0,5*line_yellow);// Go at the end of the previous text

    data_yellow += _data.toLocal8Bit();
    line_yellow += CountLine(data_yellow,data_yellow.size());


    if(line_yellow>2)
    {
        display.clearDisplay();
        display.setCursor(0,16);
        display.print(data_blue.data());// keep the blue part
        display.setCursor(0,0);

        data_yellow = _data.toLocal8Bit();
        line_yellow = CountLine(data_yellow,data_yellow.size());
    }

    display.print(data_yellow.data());
    display.display();

}

void OLEDdisplay::kill()
{
    display.clearDisplay();   // clears the screen  buffer
    display.display();   		// display it (clear display)
    display.setCursor(0,0);
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.setTextSize(4);
    display.printf(" DEAD *_*   ");
    display.startscrollright(0x00, 0x0F);
    display.display();
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

    return newlines;
}
