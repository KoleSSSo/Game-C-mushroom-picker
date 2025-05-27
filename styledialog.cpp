#include "styledialog.h"
#include <QApplication>

StyleDialog::StyleDialog(const QString& title, const QString& message, QWidget* parent)
    : QDialog(parent)
{
    setupUI(title, message);
}

void StyleDialog::setupUI(const QString& title, const QString& message)
{
    setWindowTitle(title);
    setFixedSize(400, 200);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Стилизованный заголовок
    QLabel* titleLabel = new QLabel(title, this);
    titleLabel->setStyleSheet("font-family: 'Saturn'; font-size: 24px; color: #2d5a3f;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Стилизованное сообщение
    QLabel* msgLabel = new QLabel(message, this);
    msgLabel->setStyleSheet("font-family: 'Saturn'; font-size: 18px; color: black;");
    msgLabel->setAlignment(Qt::AlignCenter);

    // Стилизованная кнопка
    QPushButton* okButton = new QPushButton("OK", this);
    okButton->setStyleSheet(
        "QPushButton {"
        "   font-family: 'Saturn';"
        "   font-size: 18px;"
        "   color: black;"
        "   background-color: rgba(187, 224, 190, 150);"
        "   border: 2px solid #2d5a3f;"
        "   padding: 5px 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(167, 204, 170, 200);"
        "}"
        );

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    layout->addWidget(titleLabel);
    layout->addWidget(msgLabel);
    layout->addWidget(okButton, 0, Qt::AlignCenter);

    // Фон диалога
    setStyleSheet(
        "StyleDialog {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "}"
        );
}
