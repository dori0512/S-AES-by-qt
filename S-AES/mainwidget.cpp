#include "mainwidget.h"
#include "encryptionwidget.h"
#include "decryptionwidget.h"
#include "double_en_de_cryption.h"
#include "crackwidget.h"
#include "tri_2keys_widget.h"
#include "encryptcbc.h"
#include "decryptcbc.h"
#include "aes.h"
#include <QPushButton>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    // 创建 QStackedWidget
    m_stackedWidget = new QStackedWidget(this);

    // 创建 EncryptWidget 和 DecryptWidget、CrackWidget
    EncryptWidget *encryptWidget = new EncryptWidget(this);
    DecryptWidget *decryptWidget = new DecryptWidget(this);
    double_en_de_cryption *double_en_de_widget = new double_en_de_cryption(this);
    CrackWidget *crackWidget = new CrackWidget(this);
    Tri_2keys_Widget *tri_2keys_Widget = new Tri_2keys_Widget(this);
    encryptCBC *encryptcbc = new encryptCBC(this);
    decryptCBC *decryptcbc = new decryptCBC(this);

    // 将 EncryptWidget 、DecryptWidget 、double_en_de_widget 、crackWidget 添加到 QStackedWidget 中
    m_stackedWidget->addWidget(encryptWidget);
    m_stackedWidget->addWidget(decryptWidget);
    m_stackedWidget->addWidget(double_en_de_widget);
    m_stackedWidget->addWidget(crackWidget);
    m_stackedWidget->addWidget(tri_2keys_Widget);
    m_stackedWidget->addWidget(encryptcbc);
    m_stackedWidget->addWidget(decryptcbc);

    // 创建按钮
    QPushButton *encryptButton = new QPushButton(" ", this);
    QPushButton *decryptButton = new QPushButton(" ", this);
    QPushButton *en_de_cryptButton = new QPushButton("双重加密", this);
    QPushButton *crackButton = new QPushButton("中间相遇攻击", this);
    QPushButton *tri_2keys_Button = new QPushButton("32bit三重加解密", this);
    QPushButton *encryptcbcButton = new QPushButton("CBC加密", this);
    QPushButton *decryptcbcButton = new QPushButton("CBC解密", this);

    encryptButton->setIcon(QIcon(":/image/icons/lock.png"));
    encryptButton->setIconSize(QSize(36, 36));
    decryptButton->setIcon(QIcon(":/image/icons/unlock.png"));
    decryptButton->setIconSize(QSize(36, 36));
    en_de_cryptButton->setIcon(QIcon(":/image/icons/double_lock.png"));
    en_de_cryptButton->setIconSize(QSize(72, 36));
    crackButton->setIcon(QIcon(":/image/icons/double_lock.png"));
    crackButton->setIconSize(QSize(72, 36));
    encryptButton->setFixedHeight(70);
    decryptButton->setFixedHeight(70);
    en_de_cryptButton->setFixedHeight(70);
    crackButton->setFixedHeight(70);
    tri_2keys_Button->setFixedHeight(70);
    encryptcbcButton->setFixedHeight(70);
    decryptcbcButton->setFixedHeight(70);
    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_stackedWidget);
    QHBoxLayout *laybtn_1keys = new QHBoxLayout;
    laybtn_1keys->addWidget(encryptButton);
    laybtn_1keys->addWidget(decryptButton);
    layout->addItem(laybtn_1keys);
    QHBoxLayout *laybtn_2keys = new QHBoxLayout;
    laybtn_2keys->addWidget(en_de_cryptButton);
    laybtn_2keys->addWidget(crackButton);
    layout->addItem(laybtn_2keys);
    //layout->addWidget(en_de_cryptButton);
    //layout->addWidget(crackButton);
    layout->addWidget(tri_2keys_Button);
    QHBoxLayout *laybtn_cbc = new QHBoxLayout;
    laybtn_cbc->addWidget(encryptcbcButton);
    laybtn_cbc->addWidget(decryptcbcButton);
    layout->addItem(laybtn_cbc);


    // 设置布局
    setLayout(layout);

    // 连接按钮的 clicked 信号到 QStackedWidget 的 setCurrentIndex 槽
    connect(encryptButton, &QPushButton::clicked, m_stackedWidget, [this]() {
        m_stackedWidget->setCurrentIndex(0);
    });
    connect(decryptButton, &QPushButton::clicked, m_stackedWidget, [this]() {
        m_stackedWidget->setCurrentIndex(1);
    });
    connect(en_de_cryptButton, &QPushButton::clicked, m_stackedWidget, [this]() {
        m_stackedWidget->setCurrentIndex(2);
    });
    connect(crackButton, &QPushButton::clicked, m_stackedWidget, [this]() {
        m_stackedWidget->setCurrentIndex(3);
    });
    connect(tri_2keys_Button, &QPushButton::clicked, m_stackedWidget, [this]() {
        m_stackedWidget->setCurrentIndex(4);
    });
    connect(encryptcbcButton, &QPushButton::clicked, m_stackedWidget, [this]() {
        m_stackedWidget->setCurrentIndex(5);
    });
    connect(decryptcbcButton, &QPushButton::clicked, m_stackedWidget, [this]() {
        m_stackedWidget->setCurrentIndex(6);
    });

    connect(encryptButton, &QPushButton::clicked, this, [encryptButton, decryptButton, en_de_cryptButton, crackButton, tri_2keys_Button, encryptcbcButton, decryptcbcButton]() {
        // 设置当前按钮的样式表() {
        // 设置按钮的样式表，可以在这里定义按钮的颜色等外观属性
        encryptButton->setStyleSheet("background-color: #adeaea; color: #238e68;");
        // 还原其他按钮的颜色
        decryptButton->setStyleSheet("");
        en_de_cryptButton->setStyleSheet("");
        crackButton->setStyleSheet("");
        tri_2keys_Button->setStyleSheet("");
        encryptcbcButton->setStyleSheet("");
        decryptcbcButton->setStyleSheet("");
    });
    connect(decryptButton, &QPushButton::clicked, this, [encryptButton, decryptButton, en_de_cryptButton, crackButton, tri_2keys_Button, encryptcbcButton, decryptcbcButton]() {
        // 设置当前按钮的样式表() {
        // 设置按钮的样式表，可以在这里定义按钮的颜色等外观属性
        encryptButton->setStyleSheet("");
        decryptButton->setStyleSheet("background-color: #adeaea; color: #238e68;");
        en_de_cryptButton->setStyleSheet("");
        crackButton->setStyleSheet("");
        tri_2keys_Button->setStyleSheet("");
        encryptcbcButton->setStyleSheet("");
        decryptcbcButton->setStyleSheet("");
    });
    connect(en_de_cryptButton, &QPushButton::clicked, this, [encryptButton, decryptButton, en_de_cryptButton, crackButton, tri_2keys_Button, encryptcbcButton, decryptcbcButton]() {
        // 设置当前按钮的样式表() {
        // 设置按钮的样式表，可以在这里定义按钮的颜色等外观属性
        encryptButton->setStyleSheet("");
        decryptButton->setStyleSheet("");
        en_de_cryptButton->setStyleSheet("background-color: #adeaea; color: #238e68;");
        crackButton->setStyleSheet("");
        tri_2keys_Button->setStyleSheet("");
        encryptcbcButton->setStyleSheet("");
        decryptcbcButton->setStyleSheet("");
    });
    connect(crackButton, &QPushButton::clicked, this, [encryptButton, decryptButton, en_de_cryptButton, crackButton, tri_2keys_Button, encryptcbcButton, decryptcbcButton]() {
        // 设置当前按钮的样式表() {
        // 设置按钮的样式表，可以在这里定义按钮的颜色等外观属性
        encryptButton->setStyleSheet("");
        // 还原其他按钮的颜色
        decryptButton->setStyleSheet("");
        en_de_cryptButton->setStyleSheet("");
        crackButton->setStyleSheet("background-color: #adeaea; color: #238e68;");
        tri_2keys_Button->setStyleSheet("");
        encryptcbcButton->setStyleSheet("");
        decryptcbcButton->setStyleSheet("");
    });
    connect(tri_2keys_Button, &QPushButton::clicked, this, [encryptButton, decryptButton, en_de_cryptButton, crackButton, tri_2keys_Button, encryptcbcButton, decryptcbcButton]() {
        // 设置当前按钮的样式表() {
        // 设置按钮的样式表，可以在这里定义按钮的颜色等外观属性
        encryptButton->setStyleSheet("");
        // 还原其他按钮的颜色
        decryptButton->setStyleSheet("");
        en_de_cryptButton->setStyleSheet("");
        crackButton->setStyleSheet("");
        tri_2keys_Button->setStyleSheet("background-color: #adeaea; color: #238e68;");
        encryptcbcButton->setStyleSheet("");
        decryptcbcButton->setStyleSheet("");
    });
    connect(encryptcbcButton, &QPushButton::clicked, this, [encryptButton, decryptButton, en_de_cryptButton, crackButton, tri_2keys_Button, encryptcbcButton, decryptcbcButton]() {
        // 设置当前按钮的样式表() {
        // 设置按钮的样式表，可以在这里定义按钮的颜色等外观属性
        encryptButton->setStyleSheet("");
        // 还原其他按钮的颜色
        decryptButton->setStyleSheet("");
        en_de_cryptButton->setStyleSheet("");
        crackButton->setStyleSheet("");
        tri_2keys_Button->setStyleSheet("");
        encryptcbcButton->setStyleSheet("background-color: #adeaea; color: #238e68;");
        decryptcbcButton->setStyleSheet("");
    });
    connect(decryptcbcButton, &QPushButton::clicked, this, [encryptButton, decryptButton, en_de_cryptButton, crackButton, tri_2keys_Button, encryptcbcButton, decryptcbcButton]() {
        // 设置当前按钮的样式表() {
        // 设置按钮的样式表，可以在这里定义按钮的颜色等外观属性
        encryptButton->setStyleSheet("");
        // 还原其他按钮的颜色
        decryptButton->setStyleSheet("");
        en_de_cryptButton->setStyleSheet("");
        crackButton->setStyleSheet("");
        tri_2keys_Button->setStyleSheet("");
        encryptcbcButton->setStyleSheet("");
        decryptcbcButton->setStyleSheet("background-color: #adeaea; color: #238e68;");
    });
}
