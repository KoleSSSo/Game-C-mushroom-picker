#ifndef RECORDS_H
#define RECORDS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

class Records : public QDialog {
    Q_OBJECT
public:
    explicit Records(QWidget *parent = nullptr);

private:
    void setupUI();
};

#endif // RECORDS_H
