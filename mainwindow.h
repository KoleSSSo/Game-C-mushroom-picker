#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "records.h"
#include "styledialog.h"
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();
    void onSettingsClicked();
    void onExitClicked();
    void onRecordClicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* m_musicPlayer;  // Добавляем как члены класса
    QAudioOutput* m_audioOutput;
};

#endif // MAINWINDOW_H
