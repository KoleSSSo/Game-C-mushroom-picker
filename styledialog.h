#ifndef STYLEDIALOG_H
#define STYLEDIALOG_H

#include <QDialog>

class StyleDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StyleDialog(const QString& title, const QString& message, QWidget *parent = nullptr);

private:
    void setupUI(const QString& title, const QString& message);
};

#endif // STYLEDIALOG_H
