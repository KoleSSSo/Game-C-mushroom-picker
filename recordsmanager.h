// recordsmanager.h
#ifndef RECORDSMANAGER_H
#define RECORDSMANAGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMap>

class RecordsManager : public QObject
{
    Q_OBJECT
public:
    explicit RecordsManager(QObject *parent = nullptr);
    void saveRecord(const QString& name, int score);
    QMap<int, QString> getRecords() const;

private:
    QString m_filePath;
};

#endif // RECORDSMANAGER_H
