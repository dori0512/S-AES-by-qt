#include "encryptionwidget.h"
#include "aes.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QIcon>
#include <QPlainTextEdit>

EncryptWidget::EncryptWidget(QWidget *parent) : QWidget(parent)
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
    m_cipherLabel->setFixedHeight(70);
    m_cipherLabel->setStyleSheet("QLabel { font: 11pt '微软雅黑 Light';" // 字体
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
    this->setStyleSheet("font: 20 12pt '微软雅黑';font-weight: bold;}");
    setLayout(layout);

    setFixedSize(800, 400); // 设置固定大小

    connect(encryptButton, &QPushButton::clicked, this, &EncryptWidget::encrypt);
}

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
