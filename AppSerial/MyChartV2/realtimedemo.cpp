#include <QApplication>
#include <QButtonGroup>
#include <QIcon>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include "realtimedemo.h"
#include "chartdir.h"
#include "data_transfer.h"
#include <math.h>
#include <stdio.h>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "qserialport.h"
#include <QVariant>
#include <QPlainTextEdit>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet("* {font-family:arial;font-size:11px}");
    RealtimeDemo demo;
    demo.show();
    return app.exec();
}

QSerialPort *serial;
static const int DataInterval = 50;
static const int ChartUpdateInterval = 5;

RealtimeDemo::RealtimeDemo(QWidget *parent) :
    QDialog(parent)
{
    serial = new QSerialPort(this);
    //
    // Set up the GUI
    //
    setFixedSize(800, 285);
    setWindowTitle("Simple Realtime Chart");

    // The frame on the left side
    QFrame *frame = new QFrame(this);
    frame->setGeometry(4, 4, 180, 277);
    frame->setFrameShape(QFrame::StyledPanel);


    // Label COM selected
    (new QLabel("Select COMSerial",frame))->setGeometry(6,10,95,14);
    // Select COM combobox
    cbx_PortCom = new QComboBox(frame);
    cbx_PortCom->setGeometry(100,6,72,21);
    connect(cbx_PortCom, SIGNAL(currentIndexChanged(int)),SLOT(updatePortInfo()));
    // Label BAUD selected
    (new QLabel("Select BAUDRATE",frame))->setGeometry(6,34,108,14);
    // Select BAUD combobox
    cbx_Baud = new QComboBox(frame);
    cbx_Baud->setGeometry(100,30,72,21);
    cbx_Baud->addItem(QStringLiteral("9600"),QSerialPort::Baud9600);
    cbx_Baud->addItem(QStringLiteral("19200"),QSerialPort::Baud19200);
    cbx_Baud->addItem(QStringLiteral("38400"),QSerialPort::Baud38400);
    cbx_Baud->addItem(QStringLiteral("115200"),QSerialPort::Baud115200);
    connect(cbx_Baud, SIGNAL(currentIndexChanged(int)),SLOT(updatePortInfo()));

    // Connect button
    btn_Connect = new QPushButton("CONNECT",frame);
    btn_Connect->setGeometry(4,54,168,20);
    connect(btn_Connect,SIGNAL(clicked()),SLOT(setupConnection()));
    fillPortInfo();

    // Select device
    (new QLabel("SELECT DEVICE:", frame))->setGeometry(6,84,95,14);
    cbx_DeviceID = new QComboBox(frame);
    cbx_DeviceID->setGeometry(100,80,72,21);
    cbx_DeviceID->addItem("Device_2",DEVICE2);
    cbx_DeviceID->addItem("Device_3",DEVICE3);


    // Init value Kp Ki Kd
    send_Kp = 0.001;
    send_Ki = 0.001;
    send_Kd = 0.001;

    // Label SendVal1
    (new QLabel("K_p",frame))->setGeometry(6,107,27,14);
    KP_ValTxt = new QLineEdit(frame);
    KP_ValTxt->setGeometry(50,106,66,18);
    KP_ValTxt->setText(QString::number(send_Kp, 'f', 4));

    // Label SendVal2
    (new QLabel("K_i",frame))->setGeometry(6,126,27,14);
    KI_ValTxt = new QLineEdit(frame);
    KI_ValTxt->setGeometry(50,125,66,18);
    KI_ValTxt->setText(QString::number(send_Ki, 'f', 4));

    // Label SendVal3
    (new QLabel("K_d",frame))->setGeometry(6,145,27,14);
    KD_ValTxt = new QLineEdit(frame);
    KD_ValTxt->setGeometry(50, 144, 66, 18);
    KD_ValTxt->setText(QString::number(send_Kd, 'f', 4));

    QPushButton *btn_KCommend = new QPushButton("K-CMD",frame);
    btn_KCommend->setGeometry(120, 109, 52, 24);
    connect(btn_KCommend,SIGNAL(clicked()),SLOT(sendKCommand()));

    QPushButton *btn_KRequest = new QPushButton("K_RQT",frame);
    btn_KRequest->setGeometry(120, 134, 52, 24);
    connect(btn_KRequest,SIGNAL(clicked()),SLOT(sendKRequest()));

    send_Setpoint = 1800;
    send_Period = sizeof (float);
    send_Duty = 50;

    (new QLabel("SetPoint",frame))->setGeometry(6,170,40,14);
    SetPoint_ValTxt = new QLineEdit(frame);
    SetPoint_ValTxt->setGeometry(50, 168, 66, 18);
    SetPoint_ValTxt->setText(QString::number((quint32)send_Setpoint, 'f', 0));

    (new QLabel("Period",frame))->setGeometry(6,189,40,14);
    Period_ValTxt = new QLineEdit(frame);
    Period_ValTxt->setGeometry(50, 187, 66, 18);
    Period_ValTxt->setText(QString::number(send_Period, 'f', 0));

    (new QLabel("Duty",frame))->setGeometry(6,208,40,14);
    Duty_ValTxt = new QLineEdit(frame);
    Duty_ValTxt->setGeometry(50,206, 66, 18);
    Duty_ValTxt->setText(QString::number(send_Duty, 'f', 0));

    QPushButton *btn_ParamCommend = new QPushButton("Pr-CMD",frame);
    btn_ParamCommend->setGeometry(120, 172, 52, 24);
    connect(btn_ParamCommend,SIGNAL(clicked()),SLOT(sendParamCommand()));

    QPushButton *btn_ParamRequest = new QPushButton("Pr_RQT",frame);
    btn_ParamRequest->setGeometry(120, 197, 52, 24);
    connect(btn_ParamRequest,SIGNAL(clicked()),SLOT(sendParamRequest()));


    // Send data button
    QPushButton *btn_Start = new QPushButton("START",frame);
    btn_Start->setGeometry(4,231,83,20);
    connect(btn_Start,SIGNAL(clicked()),SLOT(sendStartControl()));
    // Send data button
    QPushButton *btn_Stop = new QPushButton("STOP",frame);
    btn_Stop->setGeometry(89,231,83,20);
    connect(btn_Stop,SIGNAL(clicked()),SLOT(sendStopControl()));

    // Refresh connection button
    QPushButton *btn_Refresh = new QPushButton("REFRESH",frame);
    btn_Refresh->setGeometry(4,252,168,20);
    connect(btn_Refresh,SIGNAL(clicked()),SLOT(fillPortInfo()));




    // Chart Viewer
    m_ChartViewer = new QChartViewer(this);
    m_ChartViewer->setGeometry(192, 8, 600, 270);
    m_ChartViewer->setUpdateInterval(1);
    connect(m_ChartViewer, SIGNAL(viewPortChanged()), SLOT(drawChart()));

    // Clear data arrays to Chart::NoValue
    for (int i = 0; i < sampleSize; ++i) {
        m_timeStamps[i] = m_dataSeriesA[i] = Chart::NoValue;
    }
    // Set m_nextDataTime to the current time. It is used by the real time random number
    // generator so it knows what timestamp should be used for the next data point.
    m_nextDataTime = QDateTime::currentDateTime();

    // Set up the data acquisition mechanism. In this demo, we just use a timer to get a
    // sample every 50ms.
    QTimer *dataRateTimer = new QTimer(this);
    dataRateTimer->start(DataInterval);
    connect(dataRateTimer, SIGNAL(timeout()), SLOT(getData()));

    // Set up the chart update timer
    m_ChartUpdateTimer = new QTimer(this);
    connect(m_ChartUpdateTimer, SIGNAL(timeout()), SLOT(updateChart()));

    // Data received Value display
    (new QLabel("Data rcv", m_ChartViewer))->setGeometry(256, 252, 48, 21);
    m_Value = new QLabel(m_ChartViewer);
    m_Value->setGeometry(298, 254, 59, 18);
    m_Value->setFrameShape(QFrame::StyledPanel);


    // Create Data transfer
    dataTransfer = new Data_transfer(this);
    // Can start now
    m_ChartUpdateTimer->start(ChartUpdateInterval);
    updateLog();


}



//
// Update log file
//
void RealtimeDemo::updateLog(){
    QString s_Kp = KP_ValTxt->text();
    send_Kp = s_Kp.toDouble();
    send_Ki = KI_ValTxt->text().toDouble();
    send_Kd = KD_ValTxt->text().toDouble();

    send_Setpoint = SetPoint_ValTxt->text().toDouble();
    send_Duty = Duty_ValTxt->text().toDouble();
    send_Period = Period_ValTxt->text().toDouble();


}

//
// Send data
//
void RealtimeDemo::sendStartControl(){

    quint16 len;
    quint16 currID;

    DATA_TYPE currType = TYPE_CONTROL;
    currID = cbx_DeviceID->currentData().toInt();
    len = dataTransfer->processData(currType,sendBuff, currID,MESSAGE_CONTROL_START,0,0);
    if (!serial->isOpen()) {
        QMessageBox::warning(this,tr("ERROR"), tr("COM is not opened!"));
    } else {
        serial->write((char*)sendBuff,len);
    }
}

void RealtimeDemo::sendStopControl(){
    quint16 len;
    quint16 currID;

    DATA_TYPE currType = TYPE_CONTROL;
    currID = cbx_DeviceID->currentData().toInt();
    len = dataTransfer->processData(currType,sendBuff, currID,MESSAGE_CONTROL_STOP,0,0);
    if (!serial->isOpen()) {
        QMessageBox::warning(this,tr("ERROR"), tr("COM is not opened!"));
    } else {
        serial->write((char*)sendBuff,len);
    }
}

void RealtimeDemo::sendKCommand(){
    quint16 len;
    quint16 currID;

    quint32 kp;
    quint32 ki;
    quint32 kd;

    updateLog();

    kp = (quint32)(send_Kp*1000);
    ki = (quint32)(send_Ki*1000);
    kd = (quint32)(send_Kd*1000);

    DATA_TYPE currType = TYPE_COMMAND_K;
    currID = cbx_DeviceID->currentData().toInt();
    len = dataTransfer->processData(currType,sendBuff, currID,kp,ki,kd);
    if (!serial->isOpen()) {
        QMessageBox::warning(this,tr("ERROR"), tr("COM is not opened!"));
    } else {
        serial->write((char*)sendBuff,len);
    }
}

void RealtimeDemo::sendKRequest(){

}

void RealtimeDemo::sendParamCommand(){
    quint16 len;
    quint16 currID;

    quint32 setPoint;
    quint32 period;
    quint32 duty;

    updateLog();

    setPoint = (quint32)(send_Setpoint*1000);
    period = (quint32)(send_Period*1000);
    duty = (quint32)(send_Duty*1000);

    DATA_TYPE currType = TYPE_COMMAND_PARAM;
    currID = cbx_DeviceID->currentData().toInt();
    len = dataTransfer->processData(currType,sendBuff, currID,setPoint,period,duty);
    if (!serial->isOpen()) {
        QMessageBox::warning(this,tr("ERROR"), tr("COM is not opened!"));
    } else {
        serial->write((char*)sendBuff,len);
    }
}

void RealtimeDemo::sendParamRequest(){

}

//
// A utility to shift a new data value into a data array
//
static void shiftData(double *data, int len, double newValue)
{
    memmove(data, data + 1, sizeof(*data) * (len - 1));
    data[len - 1] = newValue;
}


void RealtimeDemo::setupConnection(){
    if (serial->isOpen()){
        serial->close();
        serial->setPortName("");
        btn_Connect->setText("CONNECT");
    } else {
        // Serial info
        serial->setPortName(com);
        serial->setBaudRate(baud);
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setFlowControl(QSerialPort::NoFlowControl);

        if (serial->open(QIODevice::ReadWrite)) {
            btn_Connect->setText("DISCONNECT");
        } else {
            /* Connection fail => Do not thing */
        }

    }
}

void RealtimeDemo::fillPortInfo(){
    cbx_PortCom->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        cbx_PortCom->addItem(info.portName());
    }


    serial->close();
    serial->setPortName("");
    btn_Connect->setText("CONNECT");
}

void RealtimeDemo::updatePortInfo(){
    com = cbx_PortCom->currentText();
    baud = cbx_Baud->currentText().toInt();
}
//
// Shift new data values into the real time data series
//
void RealtimeDemo::getData()
{
    // The current time
    QDateTime now = QDateTime::currentDateTime();

    // This is our formula for the random number generator
    do
    {
        // We need the currentTime in millisecond resolution
        double currentTime = Chart::chartTime2(m_nextDataTime.toTime_t())
                             + m_nextDataTime.time().msec() / 1000.0;

        // Get a data sample
        double p = currentTime * 4;
        double dataA = 20 + cos(p * 129241) * 10 + 1 / (cos(p) * cos(p) + 0.01);

        // Shift the values into the arrays
        shiftData(m_dataSeriesA, sampleSize, dataA);
        shiftData(m_timeStamps, sampleSize, currentTime);

        m_nextDataTime = m_nextDataTime.addMSecs(DataInterval);
    }
    while (m_nextDataTime < now);

    //
    // We provide some visual feedback to the latest numbers generated, so you can see the
    // data being generated.
    //
    m_Value->setText(QString::number(m_dataSeriesA[sampleSize - 1], 'f', 2));
}

//
// Update the chart. Instead of drawing the chart directly, we call updateViewPort, which
// will trigger a ViewPortChanged signal. We update the chart in the signal handler
// "drawChart". This can take advantage of the built-in rate control in QChartViewer to
// ensure a smooth user interface, even for extremely high update rate. (See the
// documentation on QChartViewer.setUpdateInterval).
//
void RealtimeDemo::updateChart()
{
    m_ChartViewer->updateViewPort(true, false);
}

//
// Draw chart
//
void RealtimeDemo::drawChart()
{
    // Create an XYChart object 600 x 270 pixels in size, with light grey (f4f4f4)
    // background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
    XYChart *c = new XYChart(600, 270, 0xf4f4f4, 0x000000, 1);
    QColor bgColor = palette().color(backgroundRole()).rgb();
    c->setRoundedFrame((bgColor.red() << 16) + (bgColor.green() << 8) + bgColor.blue());

    // Set the plotarea at (55, 62) and of size 520 x 175 pixels. Use white (ffffff)
    // background. Enable both horizontal and vertical grids by setting their colors to
    // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
    c->setPlotArea(55, 62, 520, 175, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
    c->setClipping();

    // Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
    // grey (dddddd) background, black (000000) border, and a glass like raised effect.
    c->addTitle("Embedded big Exercise::CAN_TEAM", "timesbd.ttf", 15
        )->setBackground(0xdddddd, 0x000000, Chart::glassEffect());

    // Add a legend box at the top of the plot area with 9pts Arial Bold font. We set the
    // legend box to the same width as the plot area and use grid layout (as opposed to
    // flow or top/down layout). This distributes the 3 legend icons evenly on top of the
    // plot area.

    // Configure the y-axis with a 10pts Arial Bold axis title
    c->yAxis()->setTitle("Pulse response", "", 10);

    // Configure the x-axis to auto-scale with at least 75 pixels between major tick and
    // 5  pixels between minor ticks. This shows more minor grid lines on the chart.
    c->xAxis()->setTickDensity(75, 5);


    // Set the axes width to 2 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);

    // Now we add the data to the chart.
    double lastTime = m_timeStamps[sampleSize - 1];
    if (lastTime != Chart::NoValue)
    {
        // Set up the x-axis to show the time range in the data buffer
        c->xAxis()->setDateScale(lastTime - DataInterval * sampleSize / 1000, lastTime);

        // Set the x-axis label format
        c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

        // Create a line layer to plot the lines
        LineLayer *layer = c->addLineLayer();

        // The x-coordinates are the timeStamps.
        layer->setXData(DoubleArray(m_timeStamps, sampleSize));

        // The 3 data series are used to draw 3 lines. Here we put the latest data values
        // as part of the data set name, so you can see them updated in the legend box.
        char buffer[1024];

        layer->addDataSet(DoubleArray(m_dataSeriesA, sampleSize), 0xcc0000, buffer);

    }

    // Set the chart image to the WinChartViewer
    m_ChartViewer->setChart(c);
    // Release ram
    delete c;
}
