# S-AES-by-qt
Simple AES encryption and decryption with a concise front-end interface is implemented in Qt/C++, providing a simplified yet professional solution.
## 简介
该项目是基于参考教科书《密码编码学于网络安全—原理与实践(第8版)》附录D简化AES的算法实现。
具体S-AES加密解密原理详见附录
开发平台与版本：Qt 5.13.2/Qt Creator 4.10.1

**具体S-AES加密解密原理详见附录**
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
  **错误输入示例：**

  ![2~{1)VYG)S 1%BMP7HD`6DP](https://github.com/dori0512/S-AES-by-qt/assets/130364519/85ec1438-ba04-4f79-8d10-5400e5afd5ce)
  
  更多错误输入的提示信息详见以下部分相关代码：
  
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


### 交叉测试
- 该项目能确保算法和程序在异构的系统或平台上都可以正常运行
- 设有A和B两组位同学，选择相同的密钥K，A、B组同学编写的程序对明文P进行加密后得到相同的密文C，B组同学接收到A组程序加密的密文C后，使用B组程序进行解密能得到与A相同的明文P

### 扩展功能
- 支持输入ASCII编码字符串作为数据输入（分组为2字节）
- 对应地，输出也可以是ASCII字符串（可能是乱码）

### 多重加密
#### 1 双重加密
- 将S-AES算法通过双重加密进行扩展
- 分组长度仍为16位，而密钥长度变为32位

#### 2 中间相遇攻击
- 假设找到了使用相同密钥的明文、密文对
- 尝试使用中间相遇攻击的方法找到正确的密钥Key (K1+K2)

#### 3 三重加密
- 将S-AES算法通过三重加密进行扩展
- 提供两种模式选择：
  - 使用32位密钥Key (K1+K2) 完成三重加密解密
  - 使用48位密钥Key (K1+K2+K3) 完成三重加解密

### 工作模式
- 基于S-AES算法，在较长的明文消息上使用密码分组链（CBC）模式进行加密
- 注意生成初始向量（16位），加解密双方需要共享该向量
- 在CBC模式下进行加密，并尝试篡改密文分组后进行解密，比较篡改前后的解密结果

## 使用说明
1. 安装依赖：确保系统中已安装必要的依赖库和程序运行环境。
2. 下载源代码：从项目仓库中下载源代码到本地。
3. 编译程序：使用编译器对源代码进行编译，生成可执行文件。
4. 运行程序：运行生成的可执行文件，打开GUI界面。
5. 根据需要选择相应的功能关卡进行测试。
