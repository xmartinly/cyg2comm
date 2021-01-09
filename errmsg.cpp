#include "errmsg.h"

errMsg::errMsg() {
}

QString errMsg::transMsg(int code) {
    QMap<int, QString> transMsg;
    transMsg[0] =  "N/A";
    transMsg[1] =  "1:Locks Cleared";
    transMsg[2] =  "2:Already Switching";
    transMsg[4] =  "4:Parameter Default";
    transMsg[5] =  "5:Process Variables Default";
    transMsg[6] =  "6:Manual Not Allowed";
    transMsg[8] =  "8:No Good Xtal to switch";
    transMsg[9] =  "9:No Swtich Output";
    transMsg[10] = "10:No Switch Single";
    transMsg[11] = "11:Datalog Failure";
    transMsg[12] = "12:Ethernet In Use";
    transMsg[13] = "13:No ROtate: Crystal Two";
    transMsg[14] = "14:No Rotate: Layer Runing";
    transMsg[15] = "15:Invalid Equation";
    transMsg[16] = "16:Power Not Zero";
    transMsg[17] = "17:RS232 Test Failed";
    transMsg[18] = "18:RS232 Test Passed";
    transMsg[19] = "19:XIU Test Failed";
    transMsg[20] = "20:XIU Test Passed";
    transMsg[21] = "21:No XIU Test: Swtiching";
    transMsg[22] = "22:Edit In Process";
    transMsg[23] = "23:USB Disk Error";
    transMsg[24] = "24:USB Error: Disk Full";
    transMsg[25] = "25:USB Error: Data Corrupt";
    transMsg[26] = "26:USB Error: Bad Conf. File";
    transMsg[27] = "27:USB Datalog Fast Out Error";
    transMsg[28] = "28:USB Error: Generic";
    transMsg[29] = "29:(USB: Conf. file:) Structure VersionChange";
    transMsg[30] = "30:(USB:) Parameter Value Changed";
    transMsg[31] = "31:USB Busy";
    transMsg[32] = "32:(USB:) No Such File (Available)";
    transMsg[33] = "33:(USB: Too many files) Archive Old Files";
    transMsg[53] = "53:Logic Edit In Progress";
    transMsg[54] = "54:No Paren on Empty Term";
    transMsg[55] = "55:No Left Paren on Last Term.";
    transMsg[56] = "56:Select an Event to Edit";
    transMsg[57] = "57:Select an Action to Edit";
    transMsg[58] = "58:Select an Event to Delete";
    transMsg[59] = "59:Select an Action to Delete";
    transMsg[60] = "60:Max Terms: Cannot Insert";
    transMsg[61] = "61:Cannot Negate Empty Term";
    transMsg[62] = "62:Meas. Board Comm Error";
    transMsg[63] = "63:No Switch: No XIU";
    transMsg[69] = "69:Power Loss";
    QString errCode  = transMsg[code];
    return errCode ;
}
