#ifndef TRI_2KEYS_WIDGET_H
#define TRI_2KEYS_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QLabel>
#include "aes.h"

class Tri_2keys_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Tri_2keys_Widget(QWidget *parent = nullptr);

private slots:
    void tri_2keys_decrypt();
    void tri_2keys_encrypt();

private:
    QPlainTextEdit *m_cipherTextEdit;
    QLineEdit *m_keyEdit;
    QPlainTextEdit *m_plainTextEdit;
    //QLabel *m_decryptedLabel;
    AES m_aes;

};

#endif // TRI_2KEYS_WIDGET_H
