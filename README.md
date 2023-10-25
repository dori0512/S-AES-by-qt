# S-AES-by-qt
Simple AES encryption and decryption with a concise front-end interface is implemented in Qt/C++, providing a simplified yet professional solution.
## 简介
该项目是基于参考教科书《密码编码学于网络安全—原理与实践(第8版)》附录D简化AES的算法实现，部分关键代码附于本文档最后一节。

开发平台与版本：Qt 5.13.2/Qt Creator 4.10.1

**S-AES算法介绍详见附录文件**
## 实现功能
### 基本GUI界面和加解密测试
- 程序提供GUI界面支持用户交互,支持输入16位数据和16位密钥，并输出16位密文
  
  **初始界面/加密界面**
  
  ![(M )X~3WB0GE~@3DNV2$U(7](https://github.com/dori0512/S-AES-by-qt/assets/130364519/f049f4f7-977b-4d03-b313-129e16336449)
  
  **解密界面**
  
  ![T8H%BAOGK@J$5`U}%S@@)_J](https://github.com/dori0512/S-AES-by-qt/assets/130364519/a56f25cf-01df-480a-8da1-15c2d7f241b7)
  
  **加密功能示例：  
  正常输入16bit数据和16bit密文，点击加密按钮**：
   
  ![P_I{G(CCT33E@NWF2 TG8MB](https://github.com/dori0512/S-AES-by-qt/assets/130364519/6c7d7dfd-6216-4c02-ac09-c78d5af0bfc9)
  
  **解密功能示例：**  
  **使用加密的密钥和得到的密文能解密出原数据：**
  
  ![$V`__R8R}6QJ2SX7%OH67LN](https://github.com/dori0512/S-AES-by-qt/assets/130364519/7e60d7f7-514e-4353-834b-51d1d8d51b2d)
  
  **预防错误输入的提示示例：**

  ![2~{1)VYG)S 1%BMP7HD`6DP](https://github.com/dori0512/S-AES-by-qt/assets/130364519/85ec1438-ba04-4f79-8d10-5400e5afd5ce)
  
  更多预防错误输入的提示信息详见以下部分相关代码：
 ```Qt
  void EncryptWidget::encrypt()
{
    QString plainText = m_plainTextEdit->toPlainText();
    QString key = m_keyEdit->text();

    if (plainText.isEmpty() || key.isEmpty()) { // 检查输入是否为空
        QMessageBox::information(this, "提示", "加密失败：请正确输入明文和密钥");
        return;
    }
    if (key.length() != 16) { // 检查是否为16bit的密钥
        QMessageBox::information(this, "提示", "加密失败：请输入16bit的密钥");
        return;
    }
    // 检查输入是否为ASCII编码字符串或二进制字符串
    bool isAscii = true;
    bool isBinary = true;
    for (int i = 0; i < plainText.length(); i++) {
        if (plainText[i].unicode() > 255) {
            isAscii = false;
        }
        if (plainText[i] != '0' && plainText[i] != '1') {
            isBinary = false;
        }
    }
    for (int i = 0; i < key.length(); i++) {
        if (key[i] != '0' && key[i] != '1' && key.length() != 16) {
            QMessageBox::information(this, "提示", "加密失败：请输入16bit的二进制密钥");
            return;
        }
    }

    if (!isAscii && !isBinary) {
        QMessageBox::information(this, "提示", "加密失败：明文需要是二进制或能转化为ASCII的字符串");
        return;
    }

    if (isAscii) {
        if (plainText.length() % 2 != 0) {
            QMessageBox::information(this, "提示", "加密失败：ASCII编码字符串必须为2字节的倍数");
            return;
        }
    }

    QString encryptedText;
    if (isBinary && plainText.length() == 16) {
        QString encryptedBinaryText = m_aes.encrypt(plainText, key);
        encryptedText = encryptedBinaryText;
    }
    else if(isBinary && plainText.length() != 16){
        QMessageBox::information(this, "提示", "加密失败：明文需要是16bit二进制数或能转化为ASCII的字符串");
        return;
    }
    else {

        for (int i = 0; i < plainText.length(); i+=2) {
            // 将ASCII编码字符串转换为二进制字符串进行加密
            QString binaryChar = QString("%1").arg(plainText[i].unicode(), 8, 2, QChar('0'))+QString("%1").arg(plainText[i+1].unicode(), 8, 2, QChar('0'));
            QString encryptedBinaryText = m_aes.encrypt(binaryChar, key);

            // 将加密后的二进制字符串转换为ASCII编码字符串输出
            QChar asciiCharleft(encryptedBinaryText.mid(0,8).toInt(nullptr, 2));
            encryptedText += asciiCharleft;
            QChar asciiCharright(encryptedBinaryText.mid(8,8).toInt(nullptr, 2));
            encryptedText += asciiCharright;
        }
    }

    m_cipherLabel->setText(encryptedText);
    m_cipherLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
}
```

### 交叉测试
- 该项目能确保算法和程序在异构的系统或平台上都可以正常运行
- 设有A和B两组位同学，选择相同的密钥K，A、B组同学编写的程序对明文P进行加密后得到相同的密文C，B组同学接收到A组程序加密的密文C后，使用B组程序进行解密能得到与A相同的明文P
  
例如：已知的数据、密钥和按正确加密流程得到的密文示例如下：

数据：1111111111111111 密钥：1111111111111111 密文：0101001101000011 通过程序验证如下：

加密：

![2CCEJC )4VE0@N23SL6@`4O](https://github.com/dori0512/S-AES-by-qt/assets/130364519/d7f99487-a1b8-4cb1-953e-8f1a7930c223)

解密：

![7(B2MX6~{EKG659L5(`JR_V](https://github.com/dori0512/S-AES-by-qt/assets/130364519/5731ee55-f7ff-4c20-9f4a-39db5694b17f)

与预期结果相同，即已验证该程序能通过交叉测试。

### 扩展功能
- 该项目支持输入ASCII编码字符串作为数据输入（分组为2字节的倍数）
- 对应地，输出也可以是ASCII字符串（可能是乱码）

验证:

![FOFEO8Y)WAM)M K8R~ 1R_4](https://github.com/dori0512/S-AES-by-qt/assets/130364519/9f7c4908-396a-47be-b5df-582ced5a059a)

![RVLOB~O 4BP5QX9H}Y0R`RO](https://github.com/dori0512/S-AES-by-qt/assets/130364519/4df62b43-10c7-49f6-9a32-de9c4c2f7461)


### 多重加密
#### 1 双重加密
- 将S-AES算法通过双重加密进行扩展
- 分组长度仍为16位，而密钥长度变为32位

**输入为二进制数时：**

![KKZ(}D~M({I53~M4X{1V9KE](https://github.com/dori0512/S-AES-by-qt/assets/130364519/436d08dc-7768-4182-826a-b470e41a9b42)

验证：

![ZYM D`ED3)Z@RAQRYV3IR`H](https://github.com/dori0512/S-AES-by-qt/assets/130364519/b17bf176-fbb6-4e32-a47f-649af7205e13)

**输入为分组为2字节的倍数的字符串时：**

![M WU3H)4V`C3XNVYK$NG1BO](https://github.com/dori0512/S-AES-by-qt/assets/130364519/bb088e23-e232-4902-9118-352f8c39cb0b)

验证:

![C96_Y7FVK1BW 4Y$E ~MN%M](https://github.com/dori0512/S-AES-by-qt/assets/130364519/6d9f7384-f86d-4d06-9dc9-366ffa1e5d0e)

注:此时为简化界面，双重加密解密功能共用一个界面Widget窗口
#### 2 中间相遇攻击
- 假设找到了使用相同密钥的明文、密文对，尝试使用中间相遇攻击的方法找到正确的密钥Key (K1+K2)

假设给定明文为0110111101101011和密文为0000011100111000时，求其对应的密钥

![A5CHLV ZSJU{AJL )AYSVFU](https://github.com/dori0512/S-AES-by-qt/assets/130364519/020f2b58-0b2c-4af3-95ed-1250f40102dd)

所得结果为：Foun key:   
00000000000111000000001011010100  
00000000001010110000001110111101  
00000000001111000000001011001010  
00000000010011110000000011100000  
00000000100110110000000111010000  
00000000101010010000000100010010  
00000000111000110000000011011001  
00000000111100000000000110101001  
00000001000000000000000011011001  
00000001000100100000001110011000  
00000001110110110000000111101010  
00000010000100000000000101111111  
00000010101111110000001001100100  
00000011001011110000000111010010  
00000011010011000000001101101100  
00000011011000110000000010000101  
00000011101100110000001111010101  
00000011111000110000000011110110  

破解时长：29.259821 秒

**任取其一进行加密、解密验证**：此时假设给定密钥为00000011011000110000000010000101  

加密验证：

![W0R3_H(@_ZW5UMQG(5PF_KW](https://github.com/dori0512/S-AES-by-qt/assets/130364519/ead1e780-d58e-4d73-86cf-f968bd9b11c6)

解密验证：

![(3`HC Z8D%7M2ZOO_LQI8QB](https://github.com/dori0512/S-AES-by-qt/assets/130364519/f1430c2a-ab08-499e-afc9-88ce7cd7fe8c)

明文为0110111101101011和密文为0000011100111000，验证成功。
#### 3 三重加密
- 该项目还将S-AES算法通过三重加密进行扩展：能使用32位密钥Key (K1+K2) 完成三重加密解密  

检验时，选定一组明文为0110111101101011 密钥为1010011100111011 1010011101111111 密文为0000001110001100

**加密：**

扩展加密结果：
![U@{ 85PGLR}0Z 0%W0(P0Q](https://github.com/dori0512/S-AES-by-qt/assets/130364519/be8ca137-dbcf-4e3f-b28f-5b1f845a1690)

此时按原理分步加密进行结果验证：

第一步：使用前半部分16bit key1进行加密:

![@FHP9X$BSJMD40OG Y@822C](https://github.com/dori0512/S-AES-by-qt/assets/130364519/0cc134fa-3597-41e1-9091-a29ede18cae2)

第二步：以上述加密结果使用后半部分16bit key2进行解密：

![U`VRR 7)T~1ZLT5V%BM3%FP](https://github.com/dori0512/S-AES-by-qt/assets/130364519/e7f80cdd-d8c0-4dca-b847-897537dee1eb)

第三步：以上述解密结果使用前半部分16bit key1进行加密得到最终密文：

![DB9UJ M~6I1`2B{~$R6ZLBW](https://github.com/dori0512/S-AES-by-qt/assets/130364519/040c7d7c-0396-40e2-a799-e53589676a8d)

**解密：**

扩展解密结果：

![DNAI 6JF}~ 5 D OH7SKGBG](https://github.com/dori0512/S-AES-by-qt/assets/130364519/606ab9bb-dc8e-4846-ba79-c8206d668bb5)

此时按原理分步解密进行结果验证：

第一步：使用前半部分16bit key1进行解密:

![Q64Y`T1}7JI9)MNQY(4)GUH](https://github.com/dori0512/S-AES-by-qt/assets/130364519/403bf028-1413-49a8-9ff2-b24ced5e3b7d)

第二步：以上述解密结果使用后半部分16bit key2进行加密：

![I{{{2)PN4($J)P)@%}P(Z 7](https://github.com/dori0512/S-AES-by-qt/assets/130364519/d300d3cc-7ede-4471-8e78-94138e5c5d33)

第三步：以上述加密结果使用前半部分16bit key1进行解密得到最终明文：

![%YXLCN{M(E2P(APB%G_4Z(R](https://github.com/dori0512/S-AES-by-qt/assets/130364519/35983614-7c77-4670-ad8a-5dedacd5e2e3)

**加密解密结果互相验证，证明使用32位密钥Key (K1+K2) 完成三重加密解密成功。**

### 工作模式
- 基于S-AES算法，在较长的明文消息上使用密码分组链（CBC）模式进行加密，利用随机数算法生成初始向量（16位）

检验时，随机选定一组示例：明文为0110111101101011 1010011100111011 1010011101111111密钥为0110111101101011 

进入CBC模式，进行加密：  

![DHJ71N~ZY@8N0HG9OJI%3KN](https://github.com/dori0512/S-AES-by-qt/assets/130364519/3490ea4b-1443-45fe-bb07-101496be85e9)

得到结果密文：111001000011001111001100000100101000111100100110 后，将结果和生成的随机数输入进行解密

![IW63O3AYS_{L48PK_FTMP3I](https://github.com/dori0512/S-AES-by-qt/assets/130364519/cc0b52fa-afc9-40b2-84b9-8a9a67ff78e5)

得到和已知明文相同的解密结果 0110111101101011 1010011100111011 1010011101111111

**加密解密结果互相验证，证明使用密码分组链（CBC）模式完成三重加密解密成功。**

- 在CBC模式下进行加密，并尝试篡改密文分组后进行解密，比较篡改前后的解密结果

沿用上述加解密的明文、密钥、随机数和密文，将密文篡改为：111111000011001111001100000100101000111100100110 解密结果如下所示：

![DN}N@0)`BVBR A)VZ7PJNQU](https://github.com/dori0512/S-AES-by-qt/assets/130364519/d9b536ee-4263-4eb9-82c5-3604f47fe90b)

此时解密所得的明文为010001110001101010111111001110111010011101111111

我们逐位比较两个字符串的对应位置，判断差异：

明文:  011011110110101110100111001110111010011101111111

密文:  010001110001101010111111001110111010011101111111

区别分析：
在第3位、第5位、第8位、第11位、第15位、第20位、第22位以及第24位等位置上，两个字符串不同。

## 使用说明
1. 安装依赖：确保系统中已安装必要的依赖库和程序运行环境。
2. 下载源代码：从项目仓库中下载源代码到本地。
3. 编译程序：使用编译器对源代码进行编译，生成可执行文件。
4. 运行程序：在qt/c++环境中运行生成的可执行文件，打开GUI界面。

## 核心后端代码附录
**普通AES加解密实现**：aes.cpp中
```qt
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

```

**双重加密实现**：double_en_de_cryption.cpp中
```Qt
void double_en_de_cryption::double_encrypt()
{
    QString plainText = m_plainTextEdit->toPlainText();
    QString key = m_keyEdit->text();

    if (plainText.isEmpty() || key.isEmpty()) { // 检查输入是否为空
        QMessageBox::information(this, "提示", "加密失败：请输入明文和密钥");
        return;
    }
    if (key.length() != 32) { // 检查是否为32bit的密钥
        QMessageBox::information(this, "提示", "加密失败：请输入32bit的密钥");
        return;
    }
    // 检查输入是否为ASCII编码字符串或二进制字符串
    bool isAscii = true;
    bool isBinary = true;
    for (int i = 0; i < plainText.length(); i++) {
        if (plainText[i].unicode() > 255) {
            isAscii = false;
        }
        if (plainText[i] != '0' && plainText[i] != '1') {
            isBinary = false;
        }
    }
    for (int i = 0; i < key.length(); i++) {
        if (key[i] != '0' && key[i] != '1' && key.length() != 32) {
            QMessageBox::information(this, "提示", "加密失败：请输入32bit的二进制密钥");
            return;
        }
    }

    if (!isAscii && !isBinary) {
        QMessageBox::information(this, "提示", "加密失败：明文需要是二进制或能转化为ASCII的字符串");
        return;
    }

    if (isAscii) {
        if (plainText.length() % 2 != 0) {
            QMessageBox::information(this, "提示", "加密失败：ASCII编码字符串必须为2字节的倍数");
            return;
        }
    }

    QString encryptedText;
    if (isBinary && plainText.length() == 16) {
        QString encryptedBinaryText_first = m_aes.encrypt(plainText, key.mid(0,16));
        QString encryptedBinaryText_second = m_aes.decrypt(encryptedBinaryText_first, key.mid(16,16));
        encryptedText = encryptedBinaryText_second;
    }
    else if(isBinary && plainText.length() != 16){
        QMessageBox::information(this, "提示", "加密失败：明文需要是16bit二进制数或能转化为ASCII的字符串");
        return;
    }
    else {

        for (int i = 0; i < plainText.length(); i+=2) {
            // 将ASCII编码字符串转换为二进制字符串进行加密
            QString binaryChar = QString("%1").arg(plainText[i].unicode(), 8, 2, QChar('0'))+QString("%1").arg(plainText[i+1].unicode(), 8, 2, QChar('0'));
            QString encryptedBinaryText_first = m_aes.encrypt(binaryChar, key.mid(0,16));
            QString encryptedBinaryText_second = m_aes.decrypt(encryptedBinaryText_first, key.mid(16,16));


            // 将加密后的二进制字符串转换为ASCII编码字符串输出
            QChar asciiCharleft(encryptedBinaryText_second.mid(0,8).toInt(nullptr, 2));
            encryptedText += asciiCharleft;
            QChar asciiCharright(encryptedBinaryText_second.mid(8,8).toInt(nullptr, 2));
            encryptedText += asciiCharright;
        }
    }

    m_keyEdit->setStyleSheet("font: 25 10pt '微软雅黑';" //字体
                             "color: rgb(31,31,31);"		//字体颜色
                             "padding-left:20px;"       //内边距-字体缩进
                             "background-color: rgb(255, 255, 255);" //背景颜色
                             "border:2px solid rgb(20,196,188);border-radius:15px;height: 40px;");//边框粗细-颜色-圆角设置
    m_cipherTextEdit->clear();
    m_cipherTextEdit->insertPlainText("双重加密所得密文为：" );
    m_cipherTextEdit->appendPlainText(encryptedText);
    //m_cipherLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

void double_en_de_cryption::double_decrypt()
{
    QString cipherText = m_cipherTextEdit->toPlainText();
    QString key = m_keyEdit->text();

    if (cipherText.isEmpty() || key.isEmpty()) { // 检查输入是否为空
        QMessageBox::information(this, "提示", "解密失败：请输入密文和密钥");
        return;
    }
    if (key.length() != 32) { // 检查是否为16bit的密钥
        QMessageBox::information(this, "提示", "解密失败：请输入32bit的密钥");
        return;
    }

    // 检查输入是否为ASCII编码字符串或二进制字符串
    bool isAscii = true;
    bool isBinary = true;
    for (int i = 0; i < cipherText.length(); i++) {
        if (cipherText[i].unicode() > 255) {
            isAscii = false;
        }
        if (cipherText[i] != '0' && cipherText[i] != '1') {
            isBinary = false;
        }
    }
    for (int i = 0; i < key.length(); i++) {
        if (key[i] != '0' && key[i] != '1' && key.length() != 32) {
            QMessageBox::information(this, "提示", "加密失败：请输入32bit的二进制密钥");
            return;
        }
    }
    if (!isAscii && !isBinary) {
        QMessageBox::information(this, "提示", "解密失败：密文需要是二进制或能转化为ASCII的字符串");
        return;
    }

    if (isAscii) {
        if (cipherText.length() % 2 != 0) {
            QMessageBox::information(this, "提示", "加密失败：ASCII编码字符串必须为2字节的倍数");
            return;
        }
    }

    QString decryptedText;
    if (isBinary && cipherText.length() == 16) {
        QString decryptedBinaryText_first = m_aes.encrypt(cipherText, key.mid(16,16));
        QString decryptedBinaryText_second = m_aes.decrypt(decryptedBinaryText_first, key.mid(0,16));
        decryptedText = decryptedBinaryText_second;
    }
    else if(isBinary && cipherText.length() != 16){
        QMessageBox::information(this, "提示", "解密失败：密文需要是16bit二进制数或能转化为ASCII的字符串");
        return;
    }
    else {
        for (int i = 0; i < cipherText.length(); i+=2) {
            // 将ASCII编码字符串转换为二进制字符串进行解密
            QString binaryChar = QString("%1").arg(cipherText[i].unicode(), 8, 2, QChar('0'))+QString("%1").arg(cipherText[i+1].unicode(), 8, 2, QChar('0'));
            QString decryptedBinaryText_first = m_aes.encrypt(binaryChar, key.mid(16,16));
            QString decryptedBinaryText_second = m_aes.decrypt(decryptedBinaryText_first, key.mid(0,16));

            // 将解密后的二进制字符串转换为ASCII编码字符串输出
            QChar asciiCharleft(decryptedBinaryText_second.mid(0,8).toInt(nullptr, 2));
            decryptedText += asciiCharleft;
            QChar asciiCharright(decryptedBinaryText_second.mid(8,8).toInt(nullptr, 2));
            decryptedText += asciiCharright;
        }
    }

    m_keyEdit->setStyleSheet("QLineEdit { font: 25 10pt '微软雅黑';" // 字体
                              "color: rgb(31, 31, 31);" // 字体颜色
                              "padding-left: 20px;" // 内边距-字体缩进
                              "background-color: rgb(255, 255, 255);" // 背景颜色
                              "border: 2px solid rgb(244, 67, 54);" // 边框样式和颜色
                              "border-radius: 15px;" // 圆角半径
                              "height: 40px; }"); // 控件高度
    m_plainTextEdit->clear();
    m_plainTextEdit->insertPlainText("双重解密所得明文为：" );
    m_plainTextEdit->appendPlainText(decryptedText);
    //m_decryptedLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
}
```

**中间相遇攻击实现**：crackwidget.cpp中
```Qt
void CrackWidget::crack()
{
    QString knownPlaintext = m_plainTextEdit->toPlainText();
    QString knownCiphertext = m_cipherTextEdit->toPlainText();

    if (knownPlaintext.isEmpty() || knownCiphertext.isEmpty()) { // 检查输入是否为空
        QMessageBox::information(this, "提示", "破解失败：请输入明文和密文");
        return;
    }

    if (knownPlaintext.size() != knownCiphertext.size()) { // 检查明文和密文数量是否一致
        QMessageBox::information(this, "提示", "破解失败：明文和密文数量不一致");
        return;
    }

    bool isAscii_p = true;
    bool isBinary_p = true;
    for (int i = 0; i < knownPlaintext.length(); i++) {
        if (knownPlaintext[i].unicode() > 255) {
            isAscii_p = false;
        }
        if (knownPlaintext[i] != '0' && knownPlaintext[i] != '1') {
            isBinary_p = false;
        }
    }
    bool isAscii_c = true;
    bool isBinary_c = true;
    for (int i = 0; i < knownCiphertext.length(); i++) {
        if (knownCiphertext[i].unicode() > 255) {
            isAscii_c = false;
        }
        if (knownCiphertext[i] != '0' && knownCiphertext[i] != '1') {
            isBinary_c = false;
        }
    }

    if (!isAscii_c && !isBinary_c && !isAscii_p && !isBinary_p) {
        QMessageBox::information(this, "提示", "破解失败：明密文需要是二进制或能转化为ASCII的字符串");
        return;
    }

    if (isBinary_c && isBinary_p && knownCiphertext.length() == 16 && knownPlaintext.length() == 16) {

    }
    else if(isBinary_c && isBinary_p && knownCiphertext.length() != 16 && knownPlaintext.length() != 16){
        QMessageBox::information(this, "提示", "加密失败：明文需要是16bit二进制数或能转化为ASCII的字符串");
        return;
    }
    else {

        for (int i = 0; i < knownPlaintext.length(); i++) {
            // 将ASCII编码字符串转换为二进制字符串进行加密
            QString binaryChar_p = QString("%1").arg(knownPlaintext[i].unicode(), 8, 2, QChar('0'));
            knownPlaintext =binaryChar_p;
            QString binaryChar_c = QString("%1").arg(knownCiphertext[i].unicode(), 8, 2, QChar('0'));
            knownCiphertext =binaryChar_c;
        }
    }

    QElapsedTimer timer;
    timer.start();

    int key_1;
    int key_2;
    int count = 0;
    QString binaryKey_1;
    QString binaryKey_2;
    QString allkey = "Foun key: \n";

            for (key_1 = 0; key_1 <= 1023; ++key_1) {
                binaryKey_1 = QString::number(key_1, 2).rightJustified(16, '0');
                QString midText_left = m_saes.encrypt(knownPlaintext, binaryKey_1);
                for (key_2 = 0; key_2 <= 1023; ++key_2){
                    binaryKey_2 = QString::number(key_2, 2).rightJustified(16, '0');
                    QString midText_right = m_saes.encrypt(knownCiphertext, binaryKey_2);

                    if (midText_left == midText_right) {
                        qDebug() << "Found key: " << binaryKey_1 + binaryKey_2;
                        allkey = allkey + binaryKey_1 + binaryKey_2 + "\n";
                        count++;
                        //break;

                    }
                }
            }

    if (count) {
            m_keyEdit->setText(allkey);
    } else {
            qDebug() << "Key not found.";
            QMessageBox::information(this, "提示", "破解失败：未找到正确的密钥");
                    }

    // 获取结束时间戳
    qint64 elapsedTimeUs = timer.nsecsElapsed() / 1000;
    double elapsedTimeSec = static_cast<double>(elapsedTimeUs) / 1000000.0;

    // 计算破解时长并显示
    m_timeLabel->setText(QString("破解时长：%1 秒").arg(QString::number(elapsedTimeSec, 'f', 6)));
}
```

三重加密算法实现：tri_2keys_widget.cpp中：
```Qt
void Tri_2keys_Widget::tri_2keys_encrypt()
{
    QString plainText = m_plainTextEdit->toPlainText();
    QString key = m_keyEdit->text();

    if (plainText.isEmpty() || key.isEmpty()) { // 检查输入是否为空
        QMessageBox::information(this, "提示", "加密失败：请输入明文和密钥");
        return;
    }
    if (key.length() != 32) { // 检查是否为32bit的密钥
        QMessageBox::information(this, "提示", "加密失败：请输入32bit的密钥");
        return;
    }
    // 检查输入是否为ASCII编码字符串或二进制字符串
    bool isAscii = true;
    bool isBinary = true;
    for (int i = 0; i < plainText.length(); i++) {
        if (plainText[i].unicode() > 255) {
            isAscii = false;
        }
        if (plainText[i] != '0' && plainText[i] != '1') {
            isBinary = false;
        }
    }
    for (int i = 0; i < key.length(); i++) {
        if (key[i] != '0' && key[i] != '1' && key.length() != 32) {
            QMessageBox::information(this, "提示", "加密失败：请输入32bit的二进制密钥");
            return;
        }
    }

    if (!isAscii && !isBinary) {
        QMessageBox::information(this, "提示", "加密失败：明文需要是二进制或能转化为ASCII的字符串");
        return;
    }

    if (isAscii) {
        if (plainText.length() % 2 != 0) {
            QMessageBox::information(this, "提示", "加密失败：ASCII编码字符串必须为2字节的倍数");
            return;
        }
    }

    QString encryptedText;
    if (isBinary && plainText.length() == 16) {
        //key1加密，key2解密，key1再加密
        QString encryptedBinaryText_first = m_aes.encrypt(plainText, key.mid(0,16));
        QString encryptedBinaryText_second = m_aes.decrypt(encryptedBinaryText_first, key.mid(16,16));
        QString encryptedBinaryText_third = m_aes.encrypt(encryptedBinaryText_second, key.mid(0,16));
        encryptedText = encryptedBinaryText_third;
    }
    else if(isBinary && plainText.length() != 16){
        QMessageBox::information(this, "提示", "加密失败：明文需要是16bit二进制数或能转化为ASCII的字符串");
        return;
    }
    else {

        for (int i = 0; i < plainText.length(); i+=2) {
            // 将ASCII编码字符串转换为二进制字符串进行加密
            QString binaryChar = QString("%1").arg(plainText[i].unicode(), 8, 2, QChar('0'))+QString("%1").arg(plainText[i+1].unicode(), 8, 2, QChar('0'));
            QString encryptedBinaryText_first = m_aes.encrypt(binaryChar, key.mid(0,16));
            QString encryptedBinaryText_second = m_aes.decrypt(encryptedBinaryText_first, key.mid(16,16));
            QString encryptedBinaryText_third = m_aes.encrypt(encryptedBinaryText_second, key.mid(0,16));


            // 将加密后的二进制字符串转换为ASCII编码字符串输出
            QChar asciiCharleft(encryptedBinaryText_third.mid(0,8).toInt(nullptr, 2));
            encryptedText += asciiCharleft;
            QChar asciiCharright(encryptedBinaryText_third.mid(8,8).toInt(nullptr, 2));
            encryptedText += asciiCharright;
        }
    }

    m_keyEdit->setStyleSheet("font: 25 10pt '微软雅黑';" //字体
                             "color: rgb(31,31,31);"		//字体颜色
                             "padding-left:20px;"       //内边距-字体缩进
                             "background-color: rgb(255, 255, 255);" //背景颜色
                             "border:2px solid rgb(20,196,188);border-radius:15px;height: 40px;");//边框粗细-颜色-圆角设置
    m_cipherTextEdit->clear();
    m_cipherTextEdit->insertPlainText("三重加密所得密文为：（key1先加密，key2再解密，最后key1加密）" );
    m_cipherTextEdit->appendPlainText(encryptedText);
    //m_cipherLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
}

void Tri_2keys_Widget::tri_2keys_decrypt()
{
    QString cipherText = m_cipherTextEdit->toPlainText();
    QString key = m_keyEdit->text();

    if (cipherText.isEmpty() || key.isEmpty()) { // 检查输入是否为空
        QMessageBox::information(this, "提示", "解密失败：请输入密文和密钥");
        return;
    }
    if (key.length() != 32) { // 检查是否为16bit的密钥
        QMessageBox::information(this, "提示", "解密失败：请输入32bit的密钥");
        return;
    }

    // 检查输入是否为ASCII编码字符串或二进制字符串
    bool isAscii = true;
    bool isBinary = true;
    for (int i = 0; i < cipherText.length(); i++) {
        if (cipherText[i].unicode() > 255) {
            isAscii = false;
        }
        if (cipherText[i] != '0' && cipherText[i] != '1') {
            isBinary = false;
        }
    }
    for (int i = 0; i < key.length(); i++) {
        if (key[i] != '0' && key[i] != '1' && key.length() != 32) {
            QMessageBox::information(this, "提示", "加密失败：请输入32bit的二进制密钥");
            return;
        }
    }
    if (!isAscii && !isBinary) {
        QMessageBox::information(this, "提示", "解密失败：密文需要是二进制或能转化为ASCII的字符串");
        return;
    }

    if (isAscii) {
        if (cipherText.length() % 2 != 0) {
            QMessageBox::information(this, "提示", "加密失败：ASCII编码字符串必须为2字节的倍数");
            return;
        }
    }

    QString decryptedText;
    if (isBinary && cipherText.length() == 16) {
        QString decryptedBinaryText_first = m_aes.decrypt(cipherText, key.mid(0,16));
        QString decryptedBinaryText_second = m_aes.encrypt(decryptedBinaryText_first, key.mid(16,16));
        QString decryptedBinaryText_third = m_aes.decrypt(decryptedBinaryText_second, key.mid(0,16));
        decryptedText = decryptedBinaryText_third;
    }
    else if(isBinary && cipherText.length() != 16){
        QMessageBox::information(this, "提示", "解密失败：密文需要是16bit二进制数或能转化为ASCII的字符串");
        return;
    }
    else {
        for (int i = 0; i < cipherText.length(); i+=2) {
            // 将ASCII编码字符串转换为二进制字符串进行解密
            QString binaryChar = QString("%1").arg(cipherText[i].unicode(), 8, 2, QChar('0'))+QString("%1").arg(cipherText[i+1].unicode(), 8, 2, QChar('0'));
            QString decryptedBinaryText_first = m_aes.decrypt(binaryChar, key.mid(0,16));
            QString decryptedBinaryText_second = m_aes.encrypt(decryptedBinaryText_first, key.mid(16,16));
            QString decryptedBinaryText_third = m_aes.decrypt(decryptedBinaryText_second, key.mid(0,16));

            // 将解密后的二进制字符串转换为ASCII编码字符串输出
            QChar asciiCharleft(decryptedBinaryText_third.mid(0,8).toInt(nullptr, 2));
            decryptedText += asciiCharleft;
            QChar asciiCharright(decryptedBinaryText_third.mid(8,8).toInt(nullptr, 2));
            decryptedText += asciiCharright;
        }
    }

    m_keyEdit->setStyleSheet("QLineEdit { font: 25 10pt '微软雅黑';" // 字体
                              "color: rgb(31, 31, 31);" // 字体颜色
                              "padding-left: 20px;" // 内边距-字体缩进
                              "background-color: rgb(255, 255, 255);" // 背景颜色
                              "border: 2px solid rgb(244, 67, 54);" // 边框样式和颜色
                              "border-radius: 15px;" // 圆角半径
                              "height: 40px; }"); // 控件高度
    m_plainTextEdit->clear();
    m_plainTextEdit->insertPlainText("三重解密所得明文为：（key1先解密，key2再加密，最后key1解密）" );
    m_plainTextEdit->appendPlainText(decryptedText);
    //m_decryptedLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
}
```

**工作模式CBC加解密实现**：encryptcbc.cpp中：
```Qt
//将num转化为宽度为width的二进制（宽度不够用0填充）
QString encryptCBC::decToBin(int num, int width) {
    QString binStr = QString("%1").arg(num, width, 2, QChar('0'));//第一个参数是要填充的数字，第二个参数为最小宽度，第三个参数为进制，第四个参数为当原始数字长度不足最小宽度时用于填充的字符
    return binStr.right(width);
}

//将二进制转化为十进制
int encryptCBC::binToDec(const QString& binStr) {
    return binStr.toInt(nullptr, 2);
}

QString encryptCBC::addRoundYihuo(const QString& state, const QString& key) {
    QString newState;

    for (int i = 0; i < state.length(); i += 8) {
        int num1 = binToDec(state.mid(i, 8));
        int num2 = binToDec(key.mid(i, 8));
        int result = num1 ^ num2;
        newState += decToBin(result, 8);
    }
    return newState;
}


void encryptCBC::encrypt()
{
    QString plainText = m_plainTextEdit->toPlainText();
    QString key = m_keyEdit->text();

    if (plainText.isEmpty() || key.isEmpty()) { // 检查输入是否为空
        QMessageBox::information(this, "提示", "加密失败：请输入明文和密钥");
        return;
    }
    if (key.length() != 16) { // 检查是否为16bit的密钥
        QMessageBox::information(this, "提示", "加密失败：请输入16bit的密钥");
        return;
    }
    // 检查输入是否为ASCII编码字符串或二进制字符串
    bool isAscii = true;
    bool isBinary = true;
    for (int i = 0; i < plainText.length(); i++) {
        if (plainText[i].unicode() > 255) {
            isAscii = false;
        }
        if (plainText[i] != '0' && plainText[i] != '1') {
            isBinary = false;
        }
    }
    for (int i = 0; i < key.length(); i++) {
        if (key[i] != '0' && key[i] != '1' && key.length() != 16) {
            QMessageBox::information(this, "提示", "加密失败：请输入16bit的二进制密钥");
            return;
        }
    }

    if (!isAscii && !isBinary) {
        QMessageBox::information(this, "提示", "加密失败：明文需要是二进制或能转化为ASCII的字符串");
        return;
    }

    if (isAscii) {
        if (plainText.length() % 2 != 0) {
            QMessageBox::information(this, "提示", "加密失败：ASCII编码字符串必须为2字节的倍数");
            return;
        }
    }

    QString encryptedText = "";
    QString IV_binaryString;
    QString encryptedTextBolck;
    QString midtext;
    //生成IV 16bits二进制随机数
    for (int i = 0; i < 16; ++i) {
        int bit = QRandomGenerator::global()->bounded(2); // 生成0或1
        IV_binaryString.append(QString::number(bit));
    }

    if (isBinary && plainText.length() % 16 == 0) {
        for (int i = 0; i * 16 < plainText.length(); i++){
            if(i==0){
                midtext = addRoundYihuo(IV_binaryString,plainText.mid(0,16));
                encryptedTextBolck = m_aes.encrypt(midtext, key);
            }
            else{
                midtext = addRoundYihuo(encryptedTextBolck,plainText.mid(i*16,16));
                encryptedTextBolck = m_aes.encrypt(midtext, key);
            }
            encryptedText += encryptedTextBolck;
        }
    }
    else if(isBinary && plainText.length() % 16 != 0){
        QMessageBox::information(this, "提示", "加密失败：明文需要是分组为16bit二进制数或能转化为ASCII的字符串");
        return;
    }
    else {

        for (int i = 0; i < plainText.length(); i+=2) {
            // 将ASCII编码字符串转换为二进制字符串进行加密
            QString binaryChar = QString("%1").arg(plainText[i].unicode(), 8, 2, QChar('0'))+QString("%1").arg(plainText[i+1].unicode(), 8, 2, QChar('0'));

            if(i==0){
                midtext = addRoundYihuo(IV_binaryString,binaryChar);
                encryptedTextBolck = m_aes.encrypt(midtext, key);
            }
            else{
                midtext = addRoundYihuo(encryptedTextBolck,binaryChar);
                encryptedTextBolck = m_aes.encrypt(midtext, key);
            }

            // 将加密后的二进制字符串转换为ASCII编码字符串输出
            QChar asciiCharleft(encryptedTextBolck.mid(0,8).toInt(nullptr, 2));
            encryptedText += asciiCharleft;
            QChar asciiCharright(encryptedTextBolck.mid(8,8).toInt(nullptr, 2));
            encryptedText += asciiCharright;
        }
    }

    m_cipherLabel->setText(encryptedText);
    m_cipherLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_ivLabel->setText(IV_binaryString);
    m_ivLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
}
```

在decryptcbc.cpp中：
```Qt
//将num转化为宽度为width的二进制（宽度不够用0填充）
QString decryptCBC::decToBin(int num, int width) {
    QString binStr = QString("%1").arg(num, width, 2, QChar('0'));//第一个参数是要填充的数字，第二个参数为最小宽度，第三个参数为进制，第四个参数为当原始数字长度不足最小宽度时用于填充的字符
    return binStr.right(width);
}

//将二进制转化为十进制
int decryptCBC::binToDec(const QString& binStr) {
    return binStr.toInt(nullptr, 2);
}

QString decryptCBC::addRoundYihuo(const QString& state, const QString& key) {
    QString newState;

    for (int i = 0; i < state.length(); i += 8) {
        int num1 = binToDec(state.mid(i, 8));
        int num2 = binToDec(key.mid(i, 8));
        int result = num1 ^ num2;
        newState += decToBin(result, 8);
    }
    return newState;
}

void decryptCBC::decrypt()
{
    QString cipherText = m_cipherTextEdit->toPlainText();
    QString key = m_keyEdit->text();

    if (cipherText.isEmpty() || key.isEmpty()) { // 检查输入是否为空
        QMessageBox::information(this, "提示", "解密失败：请输入密文和密钥");
        return;
    }
    if (key.length() != 16) { // 检查是否为16bit的密钥
        QMessageBox::information(this, "提示", "解密失败：请输入16bit的密钥");
        return;
    }

    // 检查输入是否为ASCII编码字符串或二进制字符串
    bool isAscii = true;
    bool isBinary = true;
    for (int i = 0; i < cipherText.length(); i++) {
        if (cipherText[i].unicode() > 255) {
            isAscii = false;
        }
        if (cipherText[i] != '0' && cipherText[i] != '1') {
            isBinary = false;
        }
    }
    for (int i = 0; i < key.length(); i++) {
        if (key[i] != '0' && key[i] != '1' && key.length() != 16) {
            QMessageBox::information(this, "提示", "加密失败：请输入16bit的二进制密钥");
            return;
        }
    }
    if (!isAscii && !isBinary) {
        QMessageBox::information(this, "提示", "解密失败：密文需要是二进制或能转化为ASCII的字符串");
        return;
    }

    if (isAscii) {
        if (cipherText.length() % 2 != 0) {
            QMessageBox::information(this, "提示", "加密失败：ASCII编码字符串必须为2字节的倍数");
            return;
        }
    }

    QString decryptedText;
    QString IV_binaryString = m_ivEdit->text();
    QString midtext;
    QString decryptedTextBolck;
    if (isBinary && cipherText.length() % 16 == 0) {
        for (int i = 0; i * 16 < cipherText.length(); i++){
            if(i==0){
                midtext = m_aes.decrypt(cipherText.mid(0,16), key);
                decryptedTextBolck = addRoundYihuo(IV_binaryString,midtext);
            }
            else{
                midtext = m_aes.decrypt(cipherText.mid(i*16,16), key);
                decryptedTextBolck = addRoundYihuo(cipherText.mid(i*16-16,16),midtext);
            }
            decryptedText += decryptedTextBolck;
        }

    }
    else if(isBinary && cipherText.length() % 16 != 0){
        QMessageBox::information(this, "提示", "解密失败：密文需要是16bit二进制数或能转化为ASCII的字符串");
        return;
    }
    else {
        for (int i = 0; i < cipherText.length(); i+=2) {
            // 将ASCII编码字符串转换为二进制字符串进行解密
            QString binaryChar = QString("%1").arg(cipherText[i].unicode(), 8, 2, QChar('0'))+QString("%1").arg(cipherText[i+1].unicode(), 8, 2, QChar('0'));
            if(i==0){
                midtext = m_aes.decrypt(binaryChar, key);
                decryptedTextBolck = addRoundYihuo(IV_binaryString,midtext);
            }
            else{
                QString before_binaryChar = QString("%1").arg(cipherText[i-2].unicode(), 8, 2, QChar('0'))+QString("%1").arg(cipherText[i-1].unicode(), 8, 2, QChar('0'));
                midtext = m_aes.decrypt(binaryChar, key);
                decryptedTextBolck = addRoundYihuo(before_binaryChar,midtext);
            }

            // 将解密后的二进制字符串转换为ASCII编码字符串输出
            QChar asciiCharleft(decryptedTextBolck.mid(0,8).toInt(nullptr, 2));
            decryptedText += asciiCharleft;
            QChar asciiCharright(decryptedTextBolck.mid(8,8).toInt(nullptr, 2));
            decryptedText += asciiCharright;
        }
    }

    m_decryptedLabel->setText(decryptedText);
    m_decryptedLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
}
```

