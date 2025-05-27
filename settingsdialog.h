#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include <QAudioOutput>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QAudioOutput* audioOutput, QWidget *parent = nullptr);

private slots:
    void updateVolume(int volume);

private:
    QAudioOutput* m_audioOutput;  // Изменили на QAudioOutput
};

#endif // SETTINGSDIALOG_H
