#include "aes.h"
#include <QDebug>
#include <QString>
#include <iostream>
#include <bitset>

AES::AES() {
    sbox = {
        {0x9, 0x4, 0xA, 0xB},
        {0xD, 0x1, 0x8, 0x5},
        {0x6, 0x2, 0x0, 0x3},
        {0xC, 0xE, 0xF, 0x7}
    };

    invSbox = {
        {0xA, 0x5, 0x9, 0xB},
        {0x1, 0x7, 0x8, 0xf},
        {0x6, 0x0, 0x2, 0x3},
        {0xc, 0x4, 0xd, 0xe}
    };

    rcon_1 = "10000000";
    rcon_2 = "00110000";

    roundKey = {
        {0x2, 0xd},
        {0x5, 0x5}
    };
    matrix = {
            {0x1, 0x4},
            {0x4, 0x1}
        };
    invmatrix = {
            {0x9, 0x2},
            {0x2, 0x9}
        };
}

//将num转化为宽度为width的二进制（宽度不够用0填充）
QString AES::decToBin(int num, int width) {
    QString binStr = QString("%1").arg(num, width, 2, QChar('0'));//第一个参数是要填充的数字，第二个参数为最小宽度，第三个参数为进制，第四个参数为当原始数字长度不足最小宽度时用于填充的字符
    return binStr.right(width);
}

//将二进制转化为十进制
int AES::binToDec(const QString& binStr) {
    return binStr.toInt(nullptr, 2);
}

//字节代换
QString AES::substituteBytes(const QString& state, bool inverse) {
    QString newState;

    for (int i = 0; i < state.length(); i += 4) {
        int row = binToDec(state.mid(i, 2));
        int col = binToDec(state.mid(i + 2, 2));
        int value = inverse ? invSbox[row][col] : sbox[row][col];
        newState += decToBin(value, 4);
    }

    return newState;
}
//行位移
QString AES::shiftRows(const QString& state, bool inverse) {
    QString newState = state;


        newState = newState.left(4) + newState.right(4) + newState.mid(8,4) + newState.mid(4,4);

    return newState;
}

//列混合
QString AES::mixColumns(const QString& state, bool inverse) {
    QString newState;

    for (int col = 0; col < 2; ++col) {
        QString column = state.mid(col * 8, 8);
        QString newColumn;

        for (int row = 0; row < 2; ++row) {
            int value = 0;
            int result;

            for (int i = 0; i < 2; ++i) {
                int num = binToDec(column.mid(i * 4, 4));
                int coefficient = inverse ? invmatrix[row][i] : matrix[row][i];
                result = multiply(num, coefficient);
                value = value ^ result;
            }
            newColumn += decToBin(value, 4);
        }

        newState += newColumn;
    }

    return newState;
}

//轮密钥加
QString AES::addRoundKey(const QString& state, const QString& key) {
    QString newState;

    for (int i = 0; i < state.length(); i += 8) {
        int num1 = binToDec(state.mid(i, 8));
        int num2 = binToDec(key.mid(i, 8));
        int result = num1 ^ num2;
        newState += decToBin(result, 8);
    }

    return newState;
}

QString AES::expendkey(const QString& key, int round){
    QString expendkey[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    expendkey[0] = key.left(8);
    expendkey[1] = key.mid(8,8);

    QString keyPrime1 = expendkey[1].mid(4,4) + expendkey[1].mid(0,4);
    QString keyPrime1_1 = substituteBytes(keyPrime1,false);
    expendkey[2] = addRoundKey(expendkey[0], addRoundKey(keyPrime1_1,rcon_1));
    expendkey[3] = addRoundKey(expendkey[1],expendkey[2]);

    QString keyPrime2 = expendkey[3].mid(4,4) + expendkey[3].mid(0,4);
    QString keyPrime2_1 = substituteBytes(keyPrime2,false);
    expendkey[4] = addRoundKey(expendkey[2], addRoundKey(keyPrime2_1,rcon_2));
    expendkey[5] = addRoundKey(expendkey[3],expendkey[4]);

    return expendkey[round*2] + expendkey[round*2+1];
}

QString AES::encryptBlock(const QString& plaintext, const QString& key) {
    QString state = plaintext;
    QString newkey;

    state = addRoundKey(state, key);

    for (int round = 1; round <= 2; ++round) {
        state = substituteBytes(state);
        state = shiftRows(state);
        if (round == 1){
        state = mixColumns(state);
        }
        newkey = expendkey(key,round);
        state = addRoundKey(state, newkey);
    }
    return state;
}

QString AES::decryptBlock(const QString& ciphertext, const QString& key) {
    QString state = ciphertext;
    QString newkey;

    for (int round = 2; round >= 1; --round) {
        newkey = expendkey(key,round);
        state = addRoundKey(state, newkey);
        if (round == 1){
        state = mixColumns(state, true);
        }
        state = shiftRows(state, true);
        state = substituteBytes(state, true);
    }

    state = addRoundKey(state, key);

    return state;
}

QString AES::encrypt(const QString& plaintext, const QString& key) {
//    QString ciphertext;
//    int numBlocks = plaintext.length() / 16;

//    for (int i = 0; i < numBlocks; ++i) {
//        QString block = plaintext.mid(i * 16, 16);
//        QString encryptedBlock = encryptBlock(block, key);
//        ciphertext += encryptedBlock;
//    }

//    return ciphertext;
    return encryptBlock(plaintext, key);
}

QString AES::decrypt(const QString& ciphertext, const QString& key) {
//    QString plaintext;
//    int numBlocks = ciphertext.length() / 16;

//    for (int i = 0; i < numBlocks; ++i) {
//        QString block = ciphertext.mid(i * 16, 16);
//        QString decryptedBlock = decryptBlock(block, key);
//        plaintext += decryptedBlock;
    // 直接使用 encryptBlock 函数对整个明文进行AES加密
        return decryptBlock(ciphertext, key);
}
int AES::multiply(int x, int y) {
    int gf4[16][16]={
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
        {0,2,4,6,8,10,12,14,3,1,7,5,11,9,15,13},
        {0,3,6,5,12,15,10,9,11,8,13,14,7,4,1,2},
        {0,4,8,12,3,7,11,15,6,2,14,10,5,1,13,9},
        {0,5,10,15,7,2,13,8,14,11,4,1,9,12,3,6},
        {0,6,12,10,11,13,7,1,5,3,9,15,14,8,2,4},
        {0,7,14,9,15,8,1,6,13,10,3,4,2,5,12,11},
        {0,8,3,11,6,14,5,13,12,4,15,7,10,2,9,1},
        {0,9,1,8,2,11,3,10,4,13,5,12,6,15,7,14},
        {0,10,7,13,14,4,9,3,15,5,8,2,1,11,6,12},
        {0,11,5,14,10,1,15,4,7,12,2,9,13,6,8,3},
        {0,12,11,7,5,9,14,2,10,6,1,13,15,3,4,8},
        {0,13,9,4,1,12,8,5,2,15,11,6,3,14,10,7},
        {0,14,15,1,13,3,2,12,9,7,6,8,4,10,11,5},
        {0,15,13,2,9,6,4,11,1,14,12,3,8,7,5,10}
    };
    return gf4[x][y];
}

//int AES::multiply(int x, int y) {
//    int result = 0;
//    int a[4];
//    QString astring = decToBin(x,4);
//    // 将二进制数划分为int数组
//    for (int i = 0; i < astring.length(); i++) {
//        QChar binaryChar = astring.at(i);
//        a[i] = binaryChar.digitValue();
//    }
//    int b[4];
//    QString bstring = decToBin(y,4);
//    // 将二进制数划分为int数组
//    for (int i = 0; i < bstring.length(); i++) {
//        QChar binaryChar = bstring.at(i);
//        b[i] = binaryChar.digitValue();
//    }

//    int resultbin[4] = {0,0,0,0};
//    //* 记录下x^nfx
//        int xfx[4] = {0};
//        x_de_n_fang_cheng_fx(xfx, a);
//        int x2fx[4] = {0};
//        x_de_n_fang_cheng_fx(x2fx, xfx);
//        int x3fx[4] = {0};
//        x_de_n_fang_cheng_fx(x3fx, x2fx);

//        //* 现在需要根据多项式a和b开始异或
//        if (b[0] == 1)
//            for (int i = 0; i < 4; i++)
//                resultbin[i] ^= x3fx[i];
//        if (b[1] == 1)
//            for (int i = 0; i < 4; i++)
//                resultbin[i] ^= x2fx[i];
//        if (b[2] == 1)
//            for (int i = 0; i < 4; i++)
//                resultbin[i] ^= xfx[i];
//        if (b[3] == 1)
//            for (int i = 0; i < 4; i++)
//                resultbin[i] ^= a[i];
//        QString combinedString;
//        for (int i = 0; i < 4; i++) {
//            combinedString += QString::number(resultbin[i]) ;
//        }
//        result = binToDec(combinedString);

//    return result;
//}

//// TODO 实现x^nfx的函数
//void AES::x_de_n_fang_cheng_fx(int xfx[4], int a[4]) //* xfx是结果，a是上一步的结果
//{
//    // 注意要取模
//    // 既约多项式是 x^4 + x + 1
//    //* 保存四次乘法的系数
//    if (a[0] == 0)
//    {
//        for (int i = 0; i < 3; i++)
//            xfx[i] = a[i + 1];
//    }
//    else
//    {
//        //! 如果乘数首项不为1就需要将 b1x^2+b0x 与 x+1 进行异或
//        xfx[1] = a[2];
//        xfx[2] = a[3] == 1 ? 0 : 1;
//        xfx[3] = 1;
//    }
//}
