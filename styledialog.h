#ifndef STYLEDIALOG_H
#define STYLEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class StyleDialog : public QDialog {
    Q_OBJECT
public:
    StyleDialog(const QString& title, const QString& message, QWidget* parent = nullptr);

private:
    void setupUI(const QString& title, const QString& message);
};

#endif // STYLEDIALOG_H
