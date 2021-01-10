#ifndef DATA_COMPUTE_H
#define DATA_COMPUTE_H

#include <QObject>
#include <bitset>
#include <iostream>
#include <QTextStream>
#include <iomanip>
#include <sstream>

#include <math.h>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

class DataCompute {
public:
    DataCompute();

    typedef struct InfiCMDs {
        QList<int>  cmdRespLength;
        QList<int> respShow;  // -1 = show, no chart; 0 = hide; 1 = show,
        QByteArray  cmdByte;
        QStringList cmdHuman;
        QStringList cmdRespFormat;
    } InfiCMD;


    static QString hbyteStatus(QString bin_string) {
        QString str = "";
        int i_bin_string = bin_string.length();
        for (int i = 0; i < i_bin_string; i++) {
            if(bin_string.at(i) == "1") {
                str += QString::number(i + 1) + "#";
            }
        }
        return str;
    }


    static QString infCmdHuman(QString cmd, QString cmd_id) {
        //0=string, 1=int, 2=float,3=byte
        QString s_cmd_human;
        int i_cmd_id = cmd_id.toInt();
        if(cmd == "H") {
            s_cmd_human = "Hello,string,-1";
        }
        if(cmd == "SS") {
            switch (i_cmd_id) {
                case 0:
                    s_cmd_human = "Crystal Life,byte,1";
                    break;
                case 1:
                    s_cmd_human = "Crystal Remaining,byte,1";
                    break;
                case 2:
                    s_cmd_human = "Crystal Postion,byte,1";
                    break;
                case 3:
                    s_cmd_human = "Sensor Status,byte,1";
                    break;
                case 4:
                    s_cmd_human = "FFrequency,long,8";
                    break;
                case 5:
                    s_cmd_human = "FActivity,int,4";
                    break;
                case 6:
                    s_cmd_human = "Current Z-Ratio,int,4";
                    break;
                case 7:
                    s_cmd_human = "AFrequency,long,8";
                    break;
                case 8:
                    s_cmd_human = "AActivity,int,4";
                    break;
                case 12:
                    s_cmd_human = "Sensor Thickness,float,4";
                    break;
                case 13:
                    s_cmd_human = "Sensor Raw Rate,float,4";
                    break;
                default:
                    s_cmd_human = "defalut,string,0";
                    break;
            }
        }
        if(cmd == "SL") {
            switch (i_cmd_id) {
                case 1:
                    s_cmd_human = "Filtered Rate,float,4";
                    break;
                case 2:
                    s_cmd_human = "Displayed Rate,float,4";
                    break;
                case 3:
                    s_cmd_human = "Power,float,4";
                    break;
                case 4:
                    s_cmd_human = "Thickness,float,4";
                    break;
                case 5:
                    s_cmd_human = "State,byte,99";
                    break;
                case 6:
                    s_cmd_human = "State Time,int,4";
                    break;
                case 7:
                    s_cmd_human = "Active Sensor,byte,1";
                    break;
                case 8:
                    s_cmd_human = "Power Number,byte,1";
                    break;
                case 12:
                    s_cmd_human = "Raw Rate,float,4";
                    break;
                default:
                    s_cmd_human = "defalut,string,0";
                    break;
            }
        }
        if(cmd == "SG") {
            switch (i_cmd_id) {
                case 1:
                    s_cmd_human = "Active Process,int,4";
                    break;
                case 2:
                    s_cmd_human = "Active Layer,byte,1";
                    break;
                case 3:
                    s_cmd_human = "Relay Status,byte,5";
                    break;
                case 4:
                    s_cmd_human = "TTLInput Status,byte,4";
                    break;
                case 5:
                    s_cmd_human = "Logic Status,byte,13";
                    break;
                case 7:
                    s_cmd_human = "Status Message,byte,1";
                    break;
                case 8:
                    s_cmd_human = "Cause of Stop,byte,1";
                    break;
                case 9:
                    s_cmd_human = "Transient Message,byte,1";
                    break;
                default:
                    s_cmd_human = "defalut,string,0";
                    break;
            }
        }
        return s_cmd_human;
    }

    static QString formatInput(QString hexStr) {
        return hexStr.indexOf("0") == 0 ? hexStr.mid(1, 1) : hexStr;
    }

    static QByteArray INFCtrlCMD(QByteArray cmd) {
        int i_cmd_len = cmd.length(), i_checksum = DataCompute::InficonTFCCheckSum(cmd);
        cmd.append(char(i_checksum));
        cmd.prepend(char(0));
        cmd.prepend(char(i_cmd_len));
        return cmd;
    }

    static QByteArray XTC3Cmd(QString Str);

    static QString ICCmdToName(QString cmd);

    static bool IsDigital(QString str) {
        return str.contains(QRegExp("^\\d+$"));
    }

    static QByteArray intToByte(int number) {
        QByteArray abyte0;
        abyte0.resize(1);
        abyte0[0] = char(0xff & number);
        return abyte0;
    }

    static int infMsgLength(QByteArray leng_bytes) {
        QByteArray _length_bytes = ReviseData(leng_bytes);
        return HexTodec(_length_bytes.toHex().data());
    }

    static QByteArray strToByte(QString _cmd) {
        std::string strx = _cmd.toStdString();
        const char *ch = strx.c_str();
        return ch;
    }

    static double BinaryToDecimal(QString Bin) {
        //QString str;
        int i;
        double Num = 0;
        int signal = 1;
        if (Bin.at(0) == '1') {
            signal = -1;
        }
        int exp = 0;
        for (i = 1; i <= 8; i++) {
            exp = exp * 2 + Bin.at(i).toLatin1() - 48;
        }
        //if (exp == 0) return QString('0');
        if (exp == 0) {
            return 0.0;
        }
        exp -= 127;
        Num = pow(2.0, exp);
        for(i = 9; i < 32; i++) {
            if (Bin.at(i).toLatin1() - 48 != 0) {
                Num += pow(2.0, exp - (i - 9 + 1));
            }
        }
        Num *= signal;
        //        str = QString("%1").arg(Num);
        return Num;
    }

    static double HexTofloat(QString str) {
        str.replace(QRegExp("[\\s]+"), "");
        if (str.contains("0x", Qt::CaseInsensitive)) {
            str.remove("0x", Qt::CaseInsensitive);
        }
        //double f;
        int Len = str.length();
        int i;
        QString m_Binary, m_Decimal;
        for (i = 0; i < 8 - Len; i++) {
            str.insert(0, '0');
        }
        m_Binary = HexToBinary(str);
        //m_Decimal = BinaryToDecimal(m_Binary);
        //return f = m_Decimal.toDouble();
        return BinaryToDecimal(m_Binary);
    }

    static QString HexToBinary(QString Hex) {
        QString str;
        int i;
        for(i = 0; i < Hex.length(); i++) {
            char buf[5];
            memset(buf, 0, 5);
            if (Hex.at(i).toLatin1() >= '0' && Hex.at(i).toLatin1() <= '9') {
                _itoa_s(Hex.at(i).toLatin1() - 48, buf, 2);
            } else if (Hex.at(i).toLatin1() >= 'A' && Hex.at(i).toLatin1() <= 'Z') {
                _itoa_s(Hex.at(i).toLatin1() - 65 + 10, buf, 2);
            } else if (Hex.at(i).toLatin1() >= 'a' && Hex.at(i).toLatin1() <= 'z') {
                _itoa_s(Hex.at(i).toLatin1() - 97 + 10, buf, 2);
            }
            if (strlen(buf) == 0) {
                str += "0000";
            }
            if (strlen(buf) == 1) {
                str += "000";
            }
            if (strlen(buf) == 2) {
                str += "00";
            }
            if (strlen(buf) == 3) {
                str += "0";
            }
            str += buf;
        }
        return str;
    }

    static int hexStrToInt(QString hex_str) {
        int x ;
        sscanf_s(hex_str.toStdString().c_str(), "%x", &x);
        return x;
    }

    static double hexStrToFloat(QString hex_str) {
        QString s_hex_temp;
        double d_value;
        if(hex_str.length() != 8) {
            d_value = 0.0;
        }
        for (int i = 4; i > 0; i--) {
            s_hex_temp += hex_str.mid((i - 1) * 2, 2);
        }
        d_value = HexTofloat(s_hex_temp);
        return  d_value;
    }

    static int HexTodec(char *s) {
        char *p = s;
        if(*p == '\0') {
            return 0;
        }
        while (*p == '0') {
            p++;
        }
        int dec = 0;
        char c;
        while ((c = *p++)) {
            dec <<= 4;
            if(c >= '0' && c <= '9') {
                dec += c - '0';
                continue;
            }
            if(c >= 'a' && c <= 'f') {
                dec += c - 'a' + 10;
                continue;
            }
            if(c >= 'A' && c <= 'F') {
                dec += c - 'A' + 10;
                continue;
            }
            return -1;
        }
        return dec;
    }


    static double ReadFreq(QByteArray FreqBytes);

    static double ReadLong(QByteArray FreqBytes);

    static double ReadFloat(QByteArray DataBytes);

    static double ReadRate(QByteArray RateBytes);

    static double ReadThk(QByteArray ThkBytes);

    static int ReadInt(QByteArray ActBytes);

    static QByteArray ReviseData(QByteArray Bytes);

    static QString XTC3State(QByteArray);

    static QString ICCYGState(QByteArray);

    static QStringList MassPointsGen(QString mass_points_string);

    static QString gaugeStatus(int status_code);

    static int InficonTFCCheckSum(QByteArray bytes);

    static InfiCMD ICCYGCmdGen(QString cmd_string) {
        bool ok;
        cmd_string.append(";");
        QStringList ql_cmd_list = cmd_string.toUpper().split(";"), ql_cmd_human, ql_cmd_format;
        QList<int> l_resp_length;
        QByteArray ba_cmd_byte, s_cmd_temp;
        foreach (QString s_single_cmd, ql_cmd_list) {
            if(s_single_cmd.count(",") == 2) {
                s_single_cmd = s_single_cmd.replace(" ", "");
                QStringList sl_single_cmd = s_single_cmd.split(","),
                            sl_cmd_human = infCmdHuman(sl_single_cmd.at(0), sl_single_cmd.at(1)).split(",");
                ql_cmd_human.append(sl_cmd_human.at(0));
                l_resp_length.append(sl_cmd_human.at(2).toInt());
                ql_cmd_format.append(sl_cmd_human.at(1));
                s_cmd_temp += strToByte(sl_single_cmd.at(0)) + intToByte(sl_single_cmd.at(1).toInt(&ok)) + (sl_single_cmd.at(2).toInt(&ok) ? intToByte(sl_single_cmd.at(2).toInt(&ok)) : "");
            }
        }
        ba_cmd_byte = DataCompute::INFCtrlCMD(s_cmd_temp);
        InfiCMD inficon_command;
        inficon_command.cmdRespLength = l_resp_length;
        inficon_command.cmdByte = ba_cmd_byte;
        inficon_command.cmdHuman = ql_cmd_human;
        inficon_command.cmdRespFormat = ql_cmd_format;
        return inficon_command;
    }


};

#endif // DATA_COMPUTE_H
