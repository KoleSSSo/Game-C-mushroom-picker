#include "settingsdialog.h"
#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QLabel>

SettingsDialog::SettingsDialog(QAudioOutput* audioOutput, QWidget *parent)
    : QDialog(parent), m_audioOutput(audioOutput)
{
    setWindowTitle("Настройки");
    setFixedSize(300, 200);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Заголовок
    QLabel* titleLabel = new QLabel("Громкость музыки", this);
    titleLabel->setStyleSheet("font-family: 'Saturn'; font-size: 20px; color: #2d5a3f;");
    titleLabel->setAlignment(Qt::AlignCenter);

    // Слайдер громкости
    QSlider* volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(m_audioOutput->volume() * 100);  // Конвертируем в проценты
    connect(volumeSlider, &QSlider::valueChanged, this, &SettingsDialog::updateVolume);

    // Кнопка закрытия
    QPushButton* closeButton = new QPushButton("Закрыть", this);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   font-family: 'Saturn';"
        "   font-size: 18px;"
        "   color: black;"
        "   background-color: rgba(187, 224, 190, 150);"
        "   border: 2px solid #2d5a3f;"
        "   padding: 5px 20px;"
        "}"
        );
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    layout->addWidget(titleLabel);
    layout->addWidget(volumeSlider);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    setStyleSheet(
        "SettingsDialog {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "}"
        );
}

#include <QSettings>

void SettingsDialog::updateVolume(int volume) {
    float normalizedVolume = volume / 100.0f;
    m_audioOutput->setVolume(normalizedVolume);

    // Сохраняем настройки
    QSettings settings;
    settings.setValue("audio/volume", normalizedVolume);
}
