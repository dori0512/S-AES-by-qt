#ifndef DOUBLE_EN_DE_CRYPTION_H
#define DOUBLE_EN_DE_CRYPTION_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLabel>
#include "aes.h"

class double_en_de_cryption : public QWidget
{
    Q_OBJECT

public:
    explicit double_en_de_cryption(QWidget *parent = nullptr);

private slots:
    void double_decrypt();
    void double_encrypt();

private:
    QPlainTextEdit *m_cipherTextEdit;
    QLineEdit *m_keyEdit;
    QPlainTextEdit *m_plainTextEdit;
    //QLabel *m_decryptedLabel;
    AES m_aes;


};

#endif // DOUBLE_EN_DE_CRYPTION_H
