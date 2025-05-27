#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QSoundEffect>
#include <QAudioOutput>
#include <QString>
#include "records.h"
#include "styledialog.h"
#include "settingsdialog.h"
#include "gamescreen.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getPlayerName() const;
    QSoundEffect* getSoundEffect() const { return m_soundEffect; }  // Новый метод

private slots:
    void onStartClicked();
    void onSettingsClicked();
    void onExitClicked();
    void onRecordClicked();

private:
    Ui::MainWindow *ui;
    QSoundEffect* m_soundEffect;  // Заменяем QMediaPlayer/QAudioOutput на QSoundEffect
};

#endif // MAINWINDOW_H
