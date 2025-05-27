#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSoundEffect>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QSoundEffect* soundEffect, QWidget *parent = nullptr);
    float getCurrentVolume() const { return currentVolume; }

private slots:
    void updateVolume(int volume);
    void saveSettings();
    void discardChanges();

private:
    QSoundEffect* m_soundEffect;
    float initialVolume;
    float currentVolume;
};

#endif // SETTINGSDIALOG_H
