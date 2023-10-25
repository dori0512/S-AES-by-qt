#ifndef DECRYPTCBC_H
#define DECRYPTCBC_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "aes.h"

class decryptCBC : public QWidget
{
    Q_OBJECT
public:
    explicit decryptCBC(QWidget *parent = nullptr);

private slots:
    void decrypt();

private:
    QPlainTextEdit *m_cipherTextEdit;
    QLineEdit *m_keyEdit;
    QLineEdit *m_ivEdit;
    QLabel *m_decryptedLabel;
    AES m_aes;

    QString decToBin(int num, int width);
    int binToDec(const QString& binStr);
    QString addRoundYihuo(const QString& state, const QString& key);

};

#endif // DECRYPTCBC_H
