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

void MainWindow::onStartClicked()
{
    QString playerName = ui->playerNameEdit->text().trimmed();
    if (playerName.isEmpty()) {
        playerName = "Игрок";
        StyleDialog warning("Ошибка", "Введите имя игрока!", this);
        warning.exec();
        return;
    }

    // Скрываем главное меню
    this->hide();

    // Создаём игровой экран
    GameScreen* gameScreen = new GameScreen(m_soundEffect);
    connect(gameScreen, &GameScreen::returnToMenuRequested, this, [this, gameScreen]() {
        gameScreen->deleteLater();
        this->show();
    });

    gameScreen->showFullScreen();
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
    QDialog exitDialog(this);
    exitDialog.setWindowTitle("Подтверждение");
    exitDialog.setFixedSize(400, 200);

    QVBoxLayout* layout = new QVBoxLayout(&exitDialog);

    QLabel* message = new QLabel("Действительно хотите выйти?", &exitDialog);
    message->setStyleSheet("font-family: 'Saturn'; font-size: 18px; color: black;");
    message->setAlignment(Qt::AlignCenter);

    QPushButton* yesButton = new QPushButton("Да", &exitDialog);
    QPushButton* noButton = new QPushButton("Нет", &exitDialog);

    QString buttonStyle =
        "QPushButton {"
        "   font-family: 'Saturn';"
        "   font-size: 18px;"
        "   color: black;"
        "   background-color: rgba(187, 224, 190, 150);"
        "   border: 2px solid #2d5a3f;"
        "   padding: 5px 20px;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(167, 204, 170, 200);"
        "}";

    yesButton->setStyleSheet(buttonStyle);
    noButton->setStyleSheet(buttonStyle);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(yesButton);
    buttonLayout->addWidget(noButton);
    buttonLayout->setAlignment(Qt::AlignCenter);

    layout->addWidget(message);
    layout->addLayout(buttonLayout);

    exitDialog.setStyleSheet(
        "QDialog {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "}");

    connect(yesButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(noButton, &QPushButton::clicked, &exitDialog, &QDialog::reject);

    exitDialog.exec();
}

void MainWindow::onRecordClicked() {
    Records records(this);
    records.exec();
}
