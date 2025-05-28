#include "records.h"
#include <QPushButton>
#include <QFile>
#include <QScrollArea>
#include "mainwindow.h"

Records::Records(QWidget *parent) : QDialog(parent)
{
    setupUI();
}

void Records::setupUI()
{
    setWindowTitle("Таблица рекордов");
    setFixedSize(500, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this); // Главный layout

    QLabel *title = new QLabel("Лучшие игроки", this);
    title->setStyleSheet("font-family: 'Saturn'; font-size: 24px; color: #2d5a3f;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Чтение рекордов из файла
    QString filePath = QCoreApplication::applicationDirPath() + "/records.txt";
    qDebug() << "Looking for records at:" << filePath;

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QList<QPair<int, QString>> records;
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split(":");
            if (parts.size() == 2) {
                bool ok;
                int score = parts[1].toInt(&ok);
                if (ok) {
                    records.append(qMakePair(score, parts[0]));
                }
            }
        }
        file.close();

        // Сортировка по убыванию
        std::sort(records.begin(), records.end(),
                  [](const QPair<int, QString>& a, const QPair<int, QString>& b) {
                      return a.first > b.first;
                  });

        // Создаем Scroll Area для рекордов
        QScrollArea *scrollArea = new QScrollArea(this);
        QWidget *scrollContent = new QWidget();
        QVBoxLayout *recordsLayout = new QVBoxLayout(scrollContent);

        // Отображение топ-10
        for (int i = 0; i < qMin(10, records.size()); ++i) {
            QString recordText = QString("%1. %2 - %3 очков")
                                     .arg(i+1)
                                     .arg(records[i].second)
                                     .arg(records[i].first);
            QLabel* recordLabel = new QLabel(recordText, scrollContent);
            recordLabel->setStyleSheet("font-family: 'Saturn'; font-size: 18px;");
            recordsLayout->addWidget(recordLabel);
        }

        scrollArea->setWidget(scrollContent);
        scrollArea->setWidgetResizable(true);
        mainLayout->addWidget(scrollArea);
    } else {
        qDebug() << "Failed to open records file for reading:" << filePath;
        QLabel* noRecords = new QLabel("Рекорды пока отсутствуют", this);
        noRecords->setStyleSheet("font-family: 'Saturn'; font-size: 18px;");
        mainLayout->addWidget(noRecords, 0, Qt::AlignCenter);
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
    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    setStyleSheet(
        "Records {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "}");
}
