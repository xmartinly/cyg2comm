#include "data_compute.h"

DataCompute::DataCompute() {
}


QByteArray DataCompute::XTC3Cmd(QString Str) {
    QByteArray _cmd;
    if(Str == "TOFF") {
        _cmd.resize(7);
        _cmd[0] = '\x55';
        _cmd[1] = '\x47';
        _cmd[2] = '\x05';
        _cmd[3] = '\x00';
        _cmd[4] = '\x00';
        _cmd[5] = '\x00';
        _cmd[6] = '\x00';
    }
    if(Str == "TON") {
        _cmd.resize(7);
        _cmd[0] = '\x55';
        _cmd[1] = '\x47';
        _cmd[2] = '\x05';
        _cmd[3] = '\x01';
        _cmd[4] = '\x00';
        _cmd[5] = '\x00';
        _cmd[6] = '\x00';
    }
    //QF151
    if(Str == "QF151") {
        _cmd.resize(4);
        _cmd[0] = '\x51';
        _cmd[1] = '\x46';
        _cmd[2] = '\x0f';
        _cmd[3] = '\x01';
    }
    if(Str == "ACQUIRE") {
        _cmd.resize(17);
        _cmd[0] = '\x0e';
        _cmd[1] = '\x00';
        //S1 current rate
        _cmd[2] = '\x53';
        _cmd[3] = '\x01';
        //S3 current thickness
        _cmd[4] = '\x53';
        _cmd[5] = '\x03';
        //S4 current state
        _cmd[6] = '\x53';
        _cmd[7] = '\x04';
        //S13 raw frequency
        _cmd[8] = '\x53';
        _cmd[9] = '\x0d';
        //xtal fail
        _cmd[10] = '\x53';
        _cmd[11] = '\x0e';
        //QF151 film 1 sensor number
        _cmd[12] = '\x51';
        _cmd[13] = '\x46';
        _cmd[14] = '\x0f';
        _cmd[15] = '\x01';
        //check sum
        _cmd[16] = '\x69';
    }
    //hello
    if(Str == "HELLO" || Str == "H1") {
        _cmd.resize(2);
        _cmd[0] = '\x48';
        _cmd[1] = '\x01';
    }
    //read rate
    if(Str == "RATE" || Str == "S1") {
        _cmd.resize(2);
        _cmd[0] = '\x53';
        _cmd[1] = '\x01';
    }
    //read power
    if(Str == "POWR" || Str == "S2") {
        _cmd.resize(2);
        _cmd[0] = '\x53';
        _cmd[1] = '\x02';
    }
    //read thickness
    if(Str == "THK" || Str == "S3") {
        _cmd.resize(2);
        _cmd[0] = '\x53';
        _cmd[1] = '\x03';
    }
    //S4 current state
    if(Str == "ACTS" || Str == "S4") {
        _cmd.resize(2);
        _cmd[0] = '\x53';
        _cmd[1] = '\x04';
    }
    //active sensor reset
    if(Str == "ACTS" || Str == "S8") {
        _cmd.resize(2);
        _cmd[0] = '\x53';
        _cmd[1] = '\x08';
    }
    //crystal life
    if(Str == "XLIFE" || Str == "S9") {
        _cmd.resize(2);
        _cmd[0] = '\x53';
        _cmd[1] = '\x09';
    }
    //read frequency
    if(Str == "FREQ" || Str == "S13") {
        _cmd.resize(2);
        _cmd[0] = '\x53';
        _cmd[1] = '\x0d';
    }
    //xtal fail
    if(Str == "XFAIL" || Str == "S14") {
        _cmd.resize(2);
        _cmd[0] = '\x53';
        _cmd[1] = '\x0e';
    }
    //Film1 with sensor 1#
    if(Str == "SEN1" || Str == "UF151") {
        _cmd.resize(8);
        _cmd[0] = '\x55';
        _cmd[1] = '\x46';
        _cmd[2] = '\x0f';
        _cmd[3] = '\x01';
        _cmd[4] = '\x01';
        _cmd[5] = '\x00';
        _cmd[6] = '\x00';
        _cmd[7] = '\x00';
    }
    //Film1 with sensor 2#
    if(Str == "SEN2" || Str == "UF152") {
        _cmd.resize(8);
        _cmd[0] = '\x55';
        _cmd[1] = '\x46';
        _cmd[2] = '\x0f';
        _cmd[3] = '\x01';
        _cmd[4] = '\x02';
        _cmd[5] = '\x00';
        _cmd[6] = '\x00';
        _cmd[7] = '\x00';
    }
    //remote stop
    if(Str == "STOP" || Str == "R1" ) {
        _cmd.resize(2);
        _cmd[0] = '\x52';
        _cmd[1] = '\x01';
    }
    //remote reset
    if(Str == "RST" || Str == "R2") {
        _cmd.resize(2);
        _cmd[0] = '\x52';
        _cmd[1] = '\x02';
    }
    //remote reset
    if(Str == "ZEROTHK" || Str == "R12") {
        _cmd.resize(2);
        _cmd[0] = '\x52';
        _cmd[1] = '\x0c';
    }
    //remote reset
    if(Str == "START" || Str == "R0") {
        _cmd.resize(2);
        _cmd[0] = '\x52';
        _cmd[1] = '\x00';
    }
    //final thickness
    if(Str == "FTHK" || Str == "R13") {
        _cmd.resize(2);
        _cmd[0] = '\x52';
        _cmd[1] = '\x0d';
    }
    if(Str == "END") {
        _cmd.resize(1);
        _cmd[0] = '\xff';
    }
    return _cmd;
}

QByteArray DataCompute::ReviseData(QByteArray Bytes) {
    QByteArray _Bytes;
    _Bytes.reserve(Bytes.length());
    for(int i = Bytes.length() - 1; i >= 0; --i) {
        _Bytes.append(Bytes.at(i));
    }
    return _Bytes;
}

double DataCompute::ReadFreq(QByteArray FreqBytes) {
    bool ok;
    //double c = 0.0034924596;
    double c = 0.000873114913702011;
    QByteArray _Bytes = ReviseData(FreqBytes);
    //return QString::number(_Bytes.toHex().toLongLong(&ok, 16) * c,'f',3);
    return _Bytes.toHex().toLongLong(&ok, 16) * c;
}
double DataCompute::ReadLong(QByteArray FreqBytes) {
    bool ok;
    //double c = 0.0034924596;
    QByteArray _Bytes = ReviseData(FreqBytes);
    //return QString::number(_Bytes.toHex().toLongLong(&ok, 16) * c,'f',3);
    return _Bytes.toHex().toLongLong(&ok, 16);
}

double DataCompute::ReadRate(QByteArray RateBytes) {
    QByteArray _Bytes = ReviseData(RateBytes).toHex();
    return HexTofloat(_Bytes);
}

double DataCompute::ReadThk(QByteArray ThkBytes) {
    QString _Bytes = ReviseData(ThkBytes).toHex();
    return HexTofloat(_Bytes);
}

double DataCompute::ReadFloat(QByteArray DataBytes) {
    QString _Bytes = ReviseData(DataBytes).toHex();
    return HexTofloat(_Bytes);
}


int DataCompute::ReadInt(QByteArray ActBytes) {
    bool ok;
    QByteArray _Bytes = ReviseData(ActBytes).toHex();
    return _Bytes.toInt(&ok, 16);
    //return QString::number(_Bytes.toInt(&ok, 16));
}
QString DataCompute::ICCYGState(QByteArray state_code) {
    QString state = "N/A";
    QString state_char = state_code.toHex();
    switch (HexTodec(state_char.toLatin1().data())) {
        case 0:
            state = "Ready";
            break;
        case 1:
            state = "Crucible Switch";
            break;
        case 2:
            state = "Precon Rise";
            break;
        case 3:
            state = "Precon Soak";
            break;
        case 4:
            state = "Rise 1";
            break;
        case 5:
            state = "Soak 1";
            break;
        case 6:
            state = "Rise 2";
            break;
        case 7:
            state = "Soak 2";
            break;
        case 8:
            state = "Shutter Delay";
            break;
        case 9:
            state = "Deposit";
            break;
        case 10:
            state = "Rate Ramp1";
            break;
        case 11:
            state = "Rate Ramp2";
            break;
        case 12:
            state = "Time Power";
            break;
        case 13:
            state = "Manual";
            break;
        case 14:
            state = "NonDep CNTL";
            break;
        case 15:
            state = "NonDep Hold";
            break;
        case 16:
            state = "Feed Ramp";
            break;
        case 17:
            state = "Feed";
            break;
        case 18:
            state = "Idle Ramp";
            break;
        case 19:
            state = "Idel";
            break;
        case 20:
            state = "Suspend";
            break;
    }
    return state;
}

QString DataCompute::XTC3State(QByteArray state_code) {
    QString state = "N/A";
    QString state_char = state_code.toHex();
    switch (HexTodec(state_char.toLatin1().data())) {
        case 0:
            state = "Ready";
            break;
        case 1:
            state = "Crucible Switch";
            break;
        case 2:
            state = "Rise 1";
            break;
        case 3:
            state = "Soak 1";
            break;
        case 4:
            state = "Rise 2";
            break;
        case 5:
            state = "Soak 2";
            break;
        case 6:
            state = "Shutter Delay";
            break;
        case 7:
            state = "Deposit";
            break;
        case 8:
            state = "Rate Ramp";
            break;
        case 9:
            state = "Manual";
            break;
        case 10:
            state = "Time Power";
            break;
        case 11:
            state = "Idle Ramp";
            break;
        case 12:
            state = "Idel";
            break;
    }
    return state;
}

QStringList DataCompute::MassPointsGen(QString mass_points_string) {
    QRegularExpression rx("(\\d+)");
    QList<int> list;
    QRegularExpressionMatchIterator i = rx.globalMatch(mass_points_string);
    QStringList mass_point_list;
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString word = match.captured(1);
        mass_point_list << word;
    }
    for (int i = 0; i < mass_point_list.count(); i++) {
        bool ok;
        int _mass_point = mass_point_list.at(i).toInt(&ok, 10);
        if(_mass_point > 200 || _mass_point < 0) {
            mass_point_list.removeAt(i);
        }
        if(mass_point_list.at(i) == "") {
            mass_point_list.removeAt(i);
        }
        for (int k = i + 1; k < mass_point_list.count(); k++) {
            if ( mass_point_list.at(i) == mass_point_list.at(k)) {
                mass_point_list.removeAt(k);
                k--;
            }
        }
    }
    return mass_point_list;
}

int DataCompute::InficonTFCCheckSum(QByteArray bytes) {
    QString data = bytes.toHex();
    int sum = 0, i_length = data.length() / 2;
    for (int i  = 0; i < i_length; ++i) {
        QString s_tmp = formatInput(data.mid(i * 2, 2));
        sum += HexTodec(s_tmp.toLatin1().data());
    }
    return sum % 256;
}

QString DataCompute::gaugeStatus(int status_code) {
    QString status_string;
    switch(status_code) {
        case 0:
            status_string = "Measurement data okay";
            break;
        case 1:
            status_string = "Underrange";
            break;
        case 2:
            status_string = "Overrange";
            break;
        case 3:
            status_string = "Sensor error";
            break;
        case 4:
            status_string = "Sensor off (PEG, MAG)";
            break;
        case 5:
            status_string = "No Sensor";
            break;
        case 6:
            status_string = "Identification error";
            break;
        case 7:
            status_string = "Error BPG, HPG, BCG";
            break;
    }
    return status_string;
}


