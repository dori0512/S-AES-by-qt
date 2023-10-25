#include "encryptcbc.h"
#include "aes.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QIcon>
#include <QPlainTextEdit>
#include <QRandomGenerator>

encryptCBC::encryptCBC(QWidget *parent)
    : QWidget{parent}
{
    m_plainTextEdit = new QPlainTextEdit(this);
    m_plainTextEdit->setPlaceholderText("明文");
    m_plainTextEdit->setFixedHeight(100);
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
    QPushButton *encryptButton = new QPushButton(this);

    encryptButton->setText("点击进行加密");
    encryptButton->setStyleSheet("QPushButton{font: 25 12pt '楷体';font-weight: bold; color: rgb(255,255,255);background-color: rgb(20,196,188);"
                                 "border: none;border-radius:15px;height: 70px;}"
                                 "QPushButton:hover{background-color: rgb(22,218,208);}"//hover
                                 "QPushButton:pressed{background-color: rgb(17,171,164);}");

    m_cipherLabel = new QLabel(this);
    m_cipherLabel->setText("……");
    m_cipherLabel->setFixedHeight(50);
    m_cipherLabel->setStyleSheet("QLabel { font: 11pt '微软雅黑 Light';" // 字体
                                  "color: rgb(31, 31, 31);" // 字体颜色
                                  "padding-left: 20px;" // 内边距-字体缩进
                                  "background-color: rgb(255, 255, 255);" // 背景颜色
                                  "border: 2px solid rgb(20, 196, 188);"
                                  "border-radius: 15px; }"); // 边框样式和圆角半径

    m_ivLabel = new QLabel(this);
    m_ivLabel->setText("本轮随机数");
    m_ivLabel->setFixedHeight(50);
    m_ivLabel->setStyleSheet("QLabel { font: 11pt '微软雅黑 Light';" // 字体
                                  "color: rgb(31, 31, 31);" // 字体颜色
                                  "padding-left: 20px;" // 内边距-字体缩进
                                  "background-color: rgb(255, 255, 255);" // 背景颜色
                                  "border: 2px solid rgb(20, 196, 188);"
                                  "border-radius: 15px; }"); // 边框样式和圆角半径

    QFormLayout *layout = new QFormLayout(this);
    layout->addRow("明文 :", m_plainTextEdit);
    layout->addRow("密钥 :", m_keyEdit);
    layout->addWidget(encryptButton);
    layout->addRow("加密结果:", m_cipherLabel);
    layout->addRow("本轮随机数:", m_ivLabel);
    this->setStyleSheet("font: 20 12pt '微软雅黑';font-weight: bold;}");
    setLayout(layout);

    setFixedSize(800, 400); // 设置固定大小

    connect(encryptButton, &QPushButton::clicked, this, &encryptCBC::encrypt);

}

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
