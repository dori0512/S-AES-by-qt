#ifndef CRACKWIDGET_H
#define CRACKWIDGET_H

#include <QWidget>
#include "aes.h"
#include <QPlainTextEdit>
#include <QLineEdit>

class CrackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CrackWidget(QWidget *parent = nullptr);

private slots:
    void crack();

private:
    QPlainTextEdit *m_plainTextEdit;
    QPlainTextEdit *m_cipherTextEdit;
    QLineEdit *m_keyEdit;
    QLineEdit *m_timeLabel;
    AES m_saes;

};

#endif // CRACKWIDGET_H
