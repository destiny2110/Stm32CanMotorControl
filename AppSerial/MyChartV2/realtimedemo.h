#ifndef REALTIMEDEMO_H
#define REALTIMEDEMO_H

#include <QDialog>
#include <QDateTime>
#include <QTimer>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QLineEdit>
#include "qchartviewer.h"
#include "data_transfer.h"

// The number of samples per data series used in this demo
const int sampleSize = 240;
typedef enum {
    DEVICE2 = 0x0200,
    DEVICE3 = 0X0300,
} DEVICE_ID;

class RealtimeDemo : public QDialog {
    Q_OBJECT
public:
    RealtimeDemo(QWidget *parent = 0);

private:
    double m_timeStamps[sampleSize];	// The timestamps for the data series
    double m_dataSeriesA[sampleSize];	// The values for the data series A

    QDateTime m_nextDataTime;           // Used by the random number generator to generate realtime data.

    QLabel *m_Value;                   // Label to display the realtime value

    QLabel *sld1_Value;
    QLabel *sld2_Value;
    QLabel *sld3_Value;


    QLineEdit *KP_ValTxt;
    QLineEdit *KI_ValTxt;
    QLineEdit *KD_ValTxt;

    QLineEdit *SetPoint_ValTxt;
    QLineEdit *Period_ValTxt;
    QLineEdit *Duty_ValTxt;


    QChartViewer *m_ChartViewer;        // QChartViewer control
    QTimer *m_ChartUpdateTimer;         // The chart update timer

    QPushButton *btn_Connect;           // Connect or disconect button
    QComboBox *cbx_Baud;
    QComboBox *cbx_PortCom;
    QComboBox *cbx_DeviceID;

    QString com;
    qint32 baud;

    QSlider *sld_Val1;
    QSlider *sld_Val2;
    QSlider *sld_Val3;

    float send_Kp;
    float send_Ki;
    float send_Kd;

    unsigned long send_Setpoint;
    unsigned long send_Period;
    unsigned long send_Duty;

    quint8 sendBuff[30];

    Data_transfer *dataTransfer;
    DEVICE_ID curr_DeviceID;

private slots:
    void setupConnection();             // Connect or disconect to serial COM
    void fillPortInfo();
    void updatePortInfo();
    void getData();

    void sendKCommand();
    void sendKRequest();
    void sendParamCommand();
    void sendParamRequest();
    void sendStartControl();
    void sendStopControl();




    // Get new data values
    void updateChart();                 // Update the chart.
    void drawChart();                   // Draw the chart.
    void updateLog();                   // Update log file
};

#endif // REALTIMEDEMO_H
