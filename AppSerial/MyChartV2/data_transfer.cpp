#include "data_transfer.h"

Data_transfer::Data_transfer(QObject *parent) : QObject(parent)
{
    for (int i=0;i<1024;i++)
    {
        buffer[i]=0;
    }
}

void Data_transfer::copyToArray(quint8 *_data, quint8 *x, int len)
{
    _data += (len - 1);
    for (int i=0;i<len;i++)
    {
        *_data = *x;
        _data--;
        x++;
        //*_data++ = *x++;
    }
}

quint16 Data_transfer::processData(DATA_TYPE type, quint8 *data, quint16 ID, quint32 value1, quint32 value2, quint32 value3)
{
    int index = 0;
    // Prepare start byte //
    data[index] = 0x3a; // data[0]
    index+=1;

    // Prepare command + len + ID  + payload //
    switch(type) {
    case (TYPE_COMMAND_K):
        // CMD //
        data[index++] = MESSAGE_K;
        // LEN //
        data[index++] = MESSAGE_LEN_K_CMD;

        // ID //
        copyToArray(data+index, (quint8*)&ID, LEN_DEVICE_ID);
        index+= LEN_DEVICE_ID;

        // PAYLOAD //
        copyToArray(data+index,(quint8*)&value1,LEN_VALUE);
        index+= LEN_VALUE;
        copyToArray(data+index,(quint8*)&value2,LEN_VALUE);
        index+= LEN_VALUE;
        copyToArray(data+index,(quint8*)&value3,LEN_VALUE);
        index+= LEN_VALUE;

        break;
    case (TYPE_REQUEST_K):
        // CMD //
        data[index++] = MESSAGE_K;

        // LEN //
        data[index++] = MESSAGE_LEN_K_RQT;

        // ID //
        copyToArray(data+index, (quint8*)&ID, LEN_DEVICE_ID);
        index+= LEN_DEVICE_ID;

        // PAYLOAD //
        data[index++] = MESSAGE_REQUEST;

        break;
    case (TYPE_COMMAND_PARAM):
        // CMD //
        data[index] = MESSAGE_PARAM;  //data[1]
        index++; // index = 2
        // LEN //
        data[index] = MESSAGE_LEN_PARAM_CMD; // data[2]
        index++; // index = 3
        // ID //
        copyToArray(data+index, (quint8*)&ID, LEN_DEVICE_ID);  // data[4] = base
        index+= LEN_DEVICE_ID;

        // PAYLOAD //
        copyToArray(data+index,(quint8*)&value1,LEN_VALUE);
        index+= LEN_VALUE;
        copyToArray(data+index,(quint8*)&value2,LEN_VALUE);
        index+= LEN_VALUE;
        copyToArray(data+index,(quint8*)&value3,LEN_VALUE);
        index+= LEN_VALUE;
        break;
    case (TYPE_REQUEST_PARAM):
        // CMD //
        data[index++] = MESSAGE_PARAM;

        // LEN //
        data[index++] = MESSAGE_LEN_PARAM_RQT;

        // ID //
        copyToArray(data+index, (quint8*)&ID, LEN_DEVICE_ID);
        index+= LEN_DEVICE_ID;

        // PAYLOAD //
        data[index++] = MESSAGE_REQUEST;

        break;
    case (TYPE_CONTROL):
        // CMD //
        data[index] = MESSAGE_CONTROL;
        index+=1;
        // LEN //
        data[index] = MESSAGE_LEN_CONTROL;
        index+=1;

        // ID //
        copyToArray(data+index, (quint8*)&ID, 2);
        index+= 2;

        // PAYLOAD //
        copyToArray(data+index,(quint8*)&value1,1);
        index+= 1;
        break;
    default:
        break;
    }

    // Prepare Stop byte
    data[index++] = MESSAGE_STOP_1;
    data[index++] = MESSAGE_STOP_2;

    return index;
}
quint16 Data_transfer::processStartCotrol(quint8 *data, quint16 ID){
    int index = 0;
    // Prepare start byte //
    data[index++] = 0x3a;
    // CMD //
    data[index++] = 0x04;

    // LEN //
    data[index++] = 3;

    // ID //
    copyToArray(data+index, (quint8*)&ID, 2);
    index+= 2;

    // PAYLOAD //
    data[index++] = 0x01;

    // Stop byte
    data[index++] = 0x0d;
    data[index++] = 0x0a;

    return index;
}
