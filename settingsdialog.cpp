#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QSettings>  // Добавляем для работы с настройками

SettingsDialog::SettingsDialog(QSoundEffect* soundEffect, QWidget *parent)
    : QDialog(parent), m_soundEffect(soundEffect),
    initialVolume(soundEffect->volume()),
    currentVolume(soundEffect->volume())
{
    setWindowTitle("Настройки");
    setFixedSize(350, 200);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Заголовок
    QLabel* titleLabel = new QLabel("Громкость музыки", this);
    titleLabel->setStyleSheet("font-family: 'Saturn'; font-size: 20px; color: #2d5a3f;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Слайдер громкости
    QSlider* volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(currentVolume * 100);
    connect(volumeSlider, &QSlider::valueChanged, this, &SettingsDialog::updateVolume);

    // Горизонтальный лейаут для кнопок
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    // Кнопка "Сохранить"
    QPushButton* saveButton = new QPushButton("Сохранить", this);
    saveButton->setStyleSheet(
        "QPushButton {"
        "   font-family: 'Saturn';"
        "   font-size: 18px;"
        "   color: black;"
        "   background-color: rgba(187, 224, 190, 150);"
        "   border: 2px solid #2d5a3f;"
        "   padding: 5px 20px;"
        "}"
        );
    connect(saveButton, &QPushButton::clicked, this, &SettingsDialog::saveSettings);

    // Кнопка "Закрыть"
    QPushButton* closeButton = new QPushButton("Закрыть", this);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   font-family: 'Saturn';"
        "   font-size: 18px;"
        "   color: black;"
        "   background-color: rgba(224, 187, 187, 150);"
        "   border: 2px solid #5a2d2d;"
        "   padding: 5px 20px;"
        "}"
        );
    connect(closeButton, &QPushButton::clicked, this, &SettingsDialog::discardChanges);

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(closeButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(volumeSlider);
    mainLayout->addLayout(buttonLayout);

    setStyleSheet(
        "SettingsDialog {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "}"
        );
}

void SettingsDialog::updateVolume(int volume) {
    currentVolume = volume / 100.0f;
    m_soundEffect->setVolume(currentVolume); // Временное изменение громкости
}

void SettingsDialog::saveSettings() {
    QSettings settings;
    settings.setValue("audio/volume", currentVolume);
    this->accept();
}

void SettingsDialog::discardChanges() {
    // Восстанавливаем исходную громкость
    m_soundEffect->setVolume(initialVolume);
    this->reject();
}
