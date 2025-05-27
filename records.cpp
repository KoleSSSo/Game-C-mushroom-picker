#include "records.h"
#include <QPushButton>
#include <QFile>
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

    // Чтение рекордов из файла
    QFile file("records.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);  // Объявляем QTextStream здесь
        QList<QPair<int, QString>> records;

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(":");
            if (parts.size() == 2) {
                int score = parts[1].toInt();
                records.append(qMakePair(score, parts[0]));
            }
        }
        file.close();

        // Сортировка по убыванию
        std::sort(records.begin(), records.end(),
                  [](const QPair<int, QString>& a, const QPair<int, QString>& b) {
                      return a.first > b.first;
                  });

        // Отображение топ-10
        QVBoxLayout* recordsLayout = new QVBoxLayout();
        for (int i = 0; i < qMin(10, records.size()); ++i) {
            QString recordText = QString("%1. %2 - %3 очков")
                                     .arg(i+1)
                                     .arg(records[i].second)
                                     .arg(records[i].first);
            QLabel* recordLabel = new QLabel(recordText, this);
            recordLabel->setStyleSheet("font-family: 'Saturn'; font-size: 18px;");
            recordsLayout->addWidget(recordLabel);
        }

        layout->addLayout(recordsLayout);
    }

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
        "}");
}
