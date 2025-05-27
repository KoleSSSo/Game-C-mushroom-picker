#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSettings>  // Добавляем для работы с настройками
#include <QPixmap>
#include <QPalette>
#include <QSoundEffect>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_soundEffect(new QSoundEffect(this))  // Инициализация эффекта
{
    ui->setupUi(this);                  //загружает интерфейс из .ui-файла и размещает его в окне.

    // Принудительная установка фона
    QPixmap bg("C:/programming/Mushroomer/textures/back-menu.jpg");
    bg = bg.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bg);
    this->setPalette(palette);


    // Настройка музыки
    QString audioPath = "C:/programming/Mushroomer/Game/sounds/sandbox-serenade-sky-toes-main-version-28029-02-39.wav";
    m_soundEffect->setSource(QUrl::fromLocalFile(audioPath));
    m_soundEffect->setLoopCount(QSoundEffect::Infinite);

    // Установка громкости 100% по умолчанию
    QSettings settings;
    float volume = settings.value("audio/volume", 1.0f).toFloat();  // 1.0 = 100%
    m_soundEffect->setVolume(volume);
    m_soundEffect->play();

    // Фиксированный размер (должен соответствовать размеру в Designer)
    this->setFixedSize(1080, 720);

    // Принудительное применение размеров
    ui->centralwidget->setFixedSize(this->size());

    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::onExitClicked);
    connect(ui->recordButton, &QPushButton::clicked, this, &MainWindow::onRecordClicked);
}

MainWindow::~MainWindow()               //деструктор
{
    delete ui;
}

void MainWindow::onStartClicked() {
    QString playerName = ui->playerNameEdit->text().trimmed();
    if (playerName.isEmpty()) {
        playerName = "Игрок";           // Значение по умолчанию
        return;
    }
    StyleDialog info("Старт", QString("Добро пожаловать, %1!").arg(playerName), this);
    info.exec();
}


void MainWindow::onSettingsClicked() {
    SettingsDialog settings(m_soundEffect, this);
    if (settings.exec() == QDialog::Accepted) {
        // Настройки были сохранены
        qDebug() << "Настройки сохранены, текущая громкость:" << m_soundEffect->volume();
    } else {
        // Настройки не сохранялись
        qDebug() << "Изменения отменены";
    }
}
void MainWindow::onExitClicked() {
    StyleDialog confirm("Выход", "Вы действительно хотите выйти?", this);
    if (confirm.exec() == QDialog::Accepted) {
        QApplication::quit();
    }
}

void MainWindow::onRecordClicked() {
    Records records(this);
    records.exec();
}
