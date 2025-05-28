#include "recordsmanager.h"
#include <QCoreApplication>
#include <QDir>

RecordsManager::RecordsManager(QObject *parent) : QObject(parent)
{
    m_filePath = QCoreApplication::applicationDirPath() + "/records.txt";
}

void RecordsManager::saveRecord(const QString& name, int score)
{
    QFile file(m_filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << ":" << score << "\n";
        file.close();
        qDebug() << "Record saved:" << name << "with score" << score;
    } else {
        qDebug() << "Failed to open records file for writing:" << m_filePath;
    }
}


QMap<int, QString> RecordsManager::getRecords() const
{
    QMap<int, QString> records;
    QFile file(m_filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList parts = line.split(":");
            if (parts.size() == 2) {
                bool ok;
                int score = parts[1].toInt(&ok);
                if (ok) {
                    records.insert(score, parts[0]);
                }
            }
        }
        file.close();
    }
    return records;
}
