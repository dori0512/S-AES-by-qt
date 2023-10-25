![A5CHLV ZSJU{AJL )AYSVFU](https://github.com/dori0512/S-AES-by-qt/assets/130364519/b3f7d08e-da80-4101-8cdc-f33ba0b5460d)# S-AES-by-qt
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

**加密解密结果互相验证，证明使用密码分组链（CBC）模式完成三重加密解密成功。**

- 在CBC模式下进行加密，并尝试篡改密文分组后进行解密，比较篡改前后的解密结果

沿用上述加解密的明文、密钥、随机数和密文，将密文篡改为：111111000011001111001100000100101000111100100110 解密结果如下所示：


此时解密所得的明文为010001110001101010111111001110111010011101111111
## 使用说明
1. 安装依赖：确保系统中已安装必要的依赖库和程序运行环境。
2. 下载源代码：从项目仓库中下载源代码到本地。
3. 编译程序：使用编译器对源代码进行编译，生成可执行文件。
4. 运行程序：在qt/c++环境中运行生成的可执行文件，打开GUI界面。

## 核心代码附录：
