#include "tri_2keys_widget.h"
#include "aes.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QIcon>

Tri_2keys_Widget::Tri_2keys_Widget(QWidget *parent)
    : QWidget{parent}
{
    m_plainTextEdit = new QPlainTextEdit(this);
    m_plainTextEdit->setPlaceholderText("明文");
    m_plainTextEdit->setFixedHeight(80);
    m_plainTextEdit->setStyleSheet("font: 25 10pt '微软雅黑 Light';" //字体
                                   "color: rgb(31,31,31);"		//字体颜色
                                   "padding-left:20px;"       //内边距-字体缩进
                                   "background-color: rgb(255, 255, 255);" //背景颜色
                                   "border:2px solid rgb(20,196,188);border-radius:15px;");//边框粗细-颜色-圆角设置

    m_keyEdit = new QLineEdit(this);
    m_keyEdit->setPlaceholderText("密钥");
    m_keyEdit->setStyleSheet("font: 25 10pt '微软雅黑';" //字体
                             "color: rgb(31,31,31);"		//字体颜色
                             "padding-left:20px;"       //内边距-字体缩进
                             "background-color: rgb(255, 255, 255);" //背景颜色
                             "border:2px solid rgb(20,196,188);border-radius:15px;height: 40px;");//边框粗细-颜色-圆角设置
    m_cipherTextEdit = new QPlainTextEdit(this);
    m_cipherTextEdit->setPlaceholderText("密文");
    m_cipherTextEdit->setFixedHeight(80);
    m_cipherTextEdit->setStyleSheet("QPlainTextEdit { font: 25 10pt '微软雅黑 Light';" // 字体
                                     "color: rgb(31, 31, 31);" // 字体颜色
                                     "padding-left: 20px;" // 内边距-字体缩进
                                     "background-color: rgb(255, 255, 255);" // 背景颜色
                                     "border: 2px solid rgb(244, 67, 54);" // 边框样式和颜色
                                     "border-radius: 15px; }"); // 圆角半径

    QPushButton *encryptButton = new QPushButton(this);

    encryptButton->setText("点击进行加密");
    encryptButton->setStyleSheet("QPushButton{font: 25 12pt '楷体';font-weight: bold; color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                 "border: none;border-radius:15px;height: 70px;}"
                                 "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                 "QPushButton:pressed{background-color: rgb(17,171,164);}");

    QPushButton *decryptButton = new QPushButton(this);
    decryptButton->setText("点击进行解密");
    decryptButton->setStyleSheet("QPushButton { font: 25 12pt '楷体';" // 字体
                                 "font-weight: bold;" // 字体加粗
                                 "color: rgb(255, 255, 255);" // 字体颜色
                                 "background-color: rgb(244, 67, 54);" // 背景颜色
                                 "border: none;" // 边框样式
                                 "border-radius: 15px;" // 圆角半径
                                 "height: 70px; }" // 控件高度
                                 "QPushButton:hover { background-color: rgb(229, 57, 53); }" // 鼠标悬停时的背景颜色
                                 "QPushButton:pressed { background-color: rgb(198, 40, 34); }"); // 鼠标按下时的背景颜色


    QFormLayout *layout = new QFormLayout(this);
    layout->addRow("明文 :", m_plainTextEdit);
    layout->addRow("密钥 :", m_keyEdit);
    layout->addRow("密文:", m_cipherTextEdit);
    QHBoxLayout *laybtn = new QHBoxLayout;
    laybtn->addWidget(encryptButton);
    laybtn->addWidget(decryptButton);
    layout->addItem(laybtn);

    this->setStyleSheet("font: 20 12pt '微软雅黑';font-weight: bold;}");
    setLayout(layout);

    setFixedSize(800, 400); // 设置固定大小

    connect(encryptButton, &QPushButton::clicked, this, &Tri_2keys_Widget::tri_2keys_encrypt);
    connect(decryptButton, &QPushButton::clicked, this, &Tri_2keys_Widget::tri_2keys_decrypt);

}


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
