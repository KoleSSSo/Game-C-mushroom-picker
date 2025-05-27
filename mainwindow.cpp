#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSettings>  // Добавляем для работы с настройками
#include <QPixmap>
#include <QPalette>


MainWindow::MainWindow(QWidget *parent) //конструктор
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)            //создаёт объект интерфейса

{
    ui->setupUi(this);                  //загружает интерфейс из .ui-файла и размещает его в окне.

    // Принудительная установка фона
    QPixmap bg(":/images/images/back-menu.jpg");
    bg = bg.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bg);
    this->setPalette(palette);

    // Инициализация музыки
    QMediaPlayer* m_musicPlayer = new QMediaPlayer(this);
    QAudioOutput* m_audioOutput = new QAudioOutput(this);  // Делаем членом класса

    m_musicPlayer->setAudioOutput(m_audioOutput);
    m_musicPlayer->setSource(QUrl("qrc:/sound/sounds/sandbox-serenade-sky-toes-main-version-28029-02-39.mp3"));
    m_musicPlayer->setLoops(QMediaPlayer::Infinite);

    // Загрузка сохранённой громкости
    QSettings settings;
    float savedVolume = settings.value("audio/volume", 0.5).toFloat();
    m_audioOutput->setVolume(savedVolume);
    m_musicPlayer->play();

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
    // Создаём новый QAudioOutput для настроек
    QAudioOutput* audioOutput = findChild<QAudioOutput*>(); // Или сохраните как член класса
    if (!audioOutput) {
        qWarning() << "Audio output not found!";
        return;
    }
    SettingsDialog settings(audioOutput, this);
    settings.exec();
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
