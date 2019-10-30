#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include <QObject>
#define MAX_LENGTH 32768



#define LEN_START       1
#define LEN_CMD         1
#define LEN_DATA_LEN    1
#define LEN_DEVICE_ID   2
#define LEN_STOP        2

#define LEN_VALUE       4
#define LEN_REQUEST     1
#define LEN_COTROL      1

#define MESSAGE_K           0x01
#define MESSAGE_PARAM       0x02
#define MESSAGE_SENSOR      0x03
#define MESSAGE_CONTROL     0x04

#define MESSAGE_LEN_K_CMD       14
#define MESSAGE_LEN_K_RQT       3
#define MESSAGE_LEN_PARAM_CMD   14
#define MESSAGE_LEN_PARAM_RQT   3
#define MESSAGE_LEN_CONTROL     3

#define MESSAGE_REQUEST     0xaf


#define MESSAGE_START       0x3a
#define MESSAGE_STOP_1      0x0d
#define MESSAGE_STOP_2      0x0a

#define MESSAGE_CONTROL_START   0x01
#define MESSAGE_CONTROL_STOP    0x02

typedef enum {
    TYPE_COMMAND_K = 0,
    TYPE_COMMAND_PARAM,
    TYPE_CONTROL,
    TYPE_REQUEST_K,
    TYPE_REQUEST_PARAM,
    TYPE_SENSOR_RCV,
}DATA_TYPE;


class Data_transfer : public QObject
{
    Q_OBJECT
public:
    explicit Data_transfer(QObject *parent = 0);
    quint16 processData(DATA_TYPE type, quint8 *data, quint16 ID, quint32 value1, quint32 value2, quint32 value3);

    quint16 processStartCotrol(quint8 *data, quint16 ID);

    void copyToArray(quint8 *_data, quint8 *x, int len);

private:
    quint8 buffer[MAX_LENGTH];
    int len;

};

#endif // DATA_TRANSFER_H
