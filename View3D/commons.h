#ifndef COMMONS_H
#define COMMONS_H


#define SPI_SPEED 500000 	// from 500 000 Hz to 32 Mhz
#define SPI_Channel_ADC 0 	// CE0
#define SPI_Channel_DAC 1 	// CE1

//#define DAC_CHANNEL_A 0 	// Vout A
//#define DAC_CHANNEL_D 1 	// Vout B

//#define ADC_SMA_CURRENT_CHANNELA 0 	// Connect this to the Resistor bornes for port A
//#define ADC_SMA_CURRENT_CHANNELD 1 	// Connect this to the Resistor bornes for port D


#define RESISTOR_CURRENT_A 0.47; // Ohm
#define RESISTOR_CURRENT_D 0.47; // Ohm

#define CURRENT_COEF 2

#define SMA_THRESHOLD 300 	// use to detect SMA

#define NB_HEADSET 4
#define NB_SMA_PER_HEADSET 3

#define Is_Safe_Output true

#define CAMERA_LEFT_INDEX 0
#define CAMERA_RIGHT_INDEX 1

#define DISPARITY_MAX 1200

#define IS_IMSHOW_ENABLE true
#define IS_IMWRITE_ENABLE false
#define IS_VIDEOWRITE_ENABLE false

#define FPS_VIDEO 3


typedef enum  {
    SMA_CHANNEL_A = 0,
    SMA_CHANNEL_D = 3
}SMA_Channel;

typedef enum  {
    DAC_CHANNEL_A = 0,
    DAC_CHANNEL_B = 1
}DAC_Channel;

typedef enum  {
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1 = 1,
    ADC_CHANNEL_2 = 2,
    ADC_CHANNEL_3 = 3,
    ADC_CHANNEL_4 = 4,
    ADC_CHANNEL_5 = 5,
    ADC_CHANNEL_6 = 6,
    ADC_CHANNEL_7 = 7,
}ADC_Channel;

typedef struct {
    unsigned int Frequency  ; // From 10 to 500 Hz
    unsigned int DutyCycle  ; // From 0 to 50 %
    unsigned int DuringTime ; // in ms
}Signal_t;

#endif // COMMONS_H
