#ifndef ENCRYPTCBC_H
#define ENCRYPTCBC_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "aes.h"

class encryptCBC : public QWidget
{
    Q_OBJECT
public:
    explicit encryptCBC(QWidget *parent = nullptr);

private slots:
    void encrypt();

private:
    QPlainTextEdit *m_plainTextEdit;
    QLineEdit *m_keyEdit;
    QLabel *m_cipherLabel;
    QLabel *m_ivLabel;
    AES m_aes;

    QString decToBin(int num, int width);
    int binToDec(const QString& binStr);
    QString addRoundYihuo(const QString& state, const QString& key);
};

#endif // ENCRYPTCBC_H
