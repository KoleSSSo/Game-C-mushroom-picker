#include "records.h"
#include <QPushButton>
#include "mainwindow.h"

Records::Records(QWidget *parent) : QDialog(parent)
{
    setupUI();
}

void Records::setupUI()
{
    setWindowTitle("Таблица рекордов");
    setFixedSize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Лучшие игроки", this);
    title->setStyleSheet("font-family: 'Saturn'; font-size: 24px; color: #2d5a3f;");
    title->setAlignment(Qt::AlignCenter);

    // Здесь будет отображение рекордов

    QPushButton *closeButton = new QPushButton("Закрыть", this);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   font-family: 'Saturn';"
        "   font-size: 18px;"
        "   color: black;"
        "   background-color: rgba(187, 224, 190, 150);"
        "   border: 2px solid #2d5a3f;"
        "   padding: 5px 20px;"
        "}"
        );

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    layout->addWidget(title);
    layout->addStretch();
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    setStyleSheet(
        "Records {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "   border color: black"
        "   color: black"
        "}"
        );
}
