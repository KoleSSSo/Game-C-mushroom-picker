#include "gamescreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include<QRandomGenerator>


GameScreen::GameScreen(QSoundEffect* backgroundMusic, QWidget *parent)
    : QWidget(parent), m_backgroundMusic(backgroundMusic)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFocusPolicy(Qt::StrongFocus);

    loadTextures();
    setupUI();

    m_gameTimer = new QTimer(this);
    m_gameTimer->start(m_timeLeft * 1000);

    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, &QTimer::timeout, this, &GameScreen::updateTimer);
    m_clockTimer->start(1000);

    QTimer* movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, [this]() {
        int dx = 0, dy = 0;
        if (m_moveUp) dy -= m_playerSpeed;
        if (m_moveDown) dy += m_playerSpeed;
        if (m_moveLeft) dx -= m_playerSpeed;
        if (m_moveRight) dx += m_playerSpeed;

        // Нормализация диагонального движения
        if (dx != 0 && dy != 0) {
            dx *= 0.7071; // 1/sqrt(2)
            dy *= 0.7071;
        }

        if (dx != 0 || dy != 0) {
            movePlayer(dx, dy);
        }
    });
    movementTimer->start(33); // ~30 FPS
}

GameScreen::~GameScreen()
{
    m_gameTimer->stop();
    m_clockTimer->stop();
}

void GameScreen::loadTextures()
{
    // Загрузка фона
    QPixmap bg("C:/programming/Mushroomer/Game/images/back.png");
    if (!bg.isNull()) {
        m_background = bg.scaled(size(), Qt::KeepAspectRatioByExpanding);
    } else {
        m_background = QPixmap(size());
        m_background.fill(QColor("#5a8f5a"));
    }

    // Загрузка текстуры игрока
    QPixmap player("C:/programming/Mushroomer/Game/images/hat.png");
    if (!player.isNull()) {
        m_playerTexture = player.scaled(128, 128, Qt::KeepAspectRatio);
    } else {
        m_playerTexture = QPixmap(128, 128);
        m_playerTexture.fill(Qt::blue);
    }
}

void GameScreen::setupUI()
{
    m_timeLabel = new QLabel(this);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setGeometry(20, 20, 200, 40);
    m_timeLabel->setStyleSheet("font-family: 'Saturn'; font-size: 24px; color: white;");
    updateTimer();
}

void GameScreen::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Рисуем фон
    painter.drawPixmap(rect(), m_background);

    // Рисуем грибы
    foreach (Mushroom* mushroom, m_mushrooms) {
        painter.drawPixmap(mushroom->rect(), mushroom->texture());
    }

    // Рисуем игрока
    painter.drawPixmap(m_playerX, m_playerY, m_playerTexture);
}

void GameScreen::updateTimer()
{
    m_timeLeft--;
    m_timeLabel->setText(QString("Время: %1:%2")
                             .arg(m_timeLeft / 60, 2, 10, QLatin1Char('0'))
                             .arg(m_timeLeft % 60, 2, 10, QLatin1Char('0')));
    m_timeLabel->setStyleSheet("font-family: 'Saturn'; font-size: 24px; color: white;");

    if (m_timeLeft <= 0) {
        m_gameTimer->stop();
        m_clockTimer->stop();

        StyleDialog endDialog("Игра окончена", "Время закончилось!", this);
        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(endDialog.layout());

        QPushButton* menuButton = new QPushButton("Выйти в главное меню", &endDialog);
        menuButton->setStyleSheet(
            "QPushButton {"
            "   font-family: 'Saturn';"
            "   font-size: 18px;"
            "   color: black;"
            "   background-color: rgba(187, 224, 190, 150);"
            "   border: 2px solid #2d5a3f;"
            "   padding: 5px 20px;"
            "   min-width: 100px;"
            "}"
            "QPushButton:hover {"
            "   background-color: rgba(167, 204, 170, 200);"
            "}");

        layout->insertWidget(2, menuButton, 0, Qt::AlignCenter);

        connect(menuButton, &QPushButton::clicked, this, &GameScreen::returnToMenuRequested);
        endDialog.exec();
    }
}

void GameScreen::spawnMushroom()
{
    Mushroom::Type type = static_cast<Mushroom::Type>(QRandomGenerator::global()->bounded(7));
    Mushroom* mushroom = new Mushroom(type, this);

    int x = QRandomGenerator::global()->bounded(width() - mushroom->rect().width());
    int y = QRandomGenerator::global()->bounded(height() - mushroom->rect().height());
    mushroom->setPosition(x, y);

    m_mushrooms.append(mushroom);
    update();
}

void GameScreen::checkCollisions()
{
    QRect playerRect(m_playerX, m_playerY, m_playerTexture.width(), m_playerTexture.height());

    for (int i = 0; i < m_mushrooms.size(); ++i) {
        if (playerRect.intersects(m_mushrooms[i]->rect())) {
            // Обработка сбора гриба
            m_mushrooms.takeAt(i)->deleteLater();
            i--;
            // Здесь нужно добавить логику подсчета очков
        }
    }
}

void GameScreen::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:    m_moveUp = true; break;
    case Qt::Key_S:
    case Qt::Key_Down:  m_moveDown = true; break;
    case Qt::Key_A:
    case Qt::Key_Left:  m_moveLeft = true; break;
    case Qt::Key_D:
    case Qt::Key_Right: m_moveRight = true; break;
    case Qt::Key_Escape: showPauseMenu(); break;
    default: QWidget::keyPressEvent(event);
    }
}

void GameScreen::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
    case Qt::Key_Up:    m_moveUp = false; break;
    case Qt::Key_S:
    case Qt::Key_Down:  m_moveDown = false; break;
    case Qt::Key_A:
    case Qt::Key_Left:  m_moveLeft = false; break;
    case Qt::Key_D:
    case Qt::Key_Right: m_moveRight = false; break;
    default: QWidget::keyReleaseEvent(event);
    }
}

void GameScreen::movePlayer(int dx, int dy)
{
    m_playerX = qBound(0, m_playerX + dx, width() - m_playerTexture.width());
    m_playerY = qBound(0, m_playerY + dy, height() - m_playerTexture.height());
    update(); // Перерисовываем экран
}

void GameScreen::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setFocus();
}

void GameScreen::showPauseMenu()
{
    m_clockTimer->stop();

    QDialog pauseMenu(this);
    pauseMenu.setWindowTitle("Меню паузы");
    pauseMenu.setFixedSize(300, 250);
    pauseMenu.setStyleSheet(
        "QDialog {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "}");

    QVBoxLayout* layout = new QVBoxLayout(&pauseMenu);
    layout->setSpacing(10);

    QPushButton* resumeButton = new QPushButton("Продолжить", &pauseMenu);
    QPushButton* settingsButton = new QPushButton("Настройки", &pauseMenu);
    QPushButton* exitButton = new QPushButton("Выйти в меню", &pauseMenu);

    QString buttonStyle =
        "QPushButton {"
        "   font-family: 'Saturn';"
        "   font-size: 18px;"
        "   color: black;"
        "   background-color: rgba(187, 224, 190, 150);"
        "   border: 2px solid #2d5a3f;"
        "   padding: 5px 20px;"
        "   min-width: 150px;"
        "   min-height: 40px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(167, 204, 170, 200);"
        "}";

    resumeButton->setStyleSheet(buttonStyle);
    settingsButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    layout->addWidget(resumeButton);
    layout->addWidget(settingsButton);
    layout->addWidget(exitButton);

    connect(resumeButton, &QPushButton::clicked, [&]() {
        pauseMenu.close();
        m_clockTimer->start();
    });

    connect(settingsButton, &QPushButton::clicked, [&]() {
        SettingsDialog settings(m_backgroundMusic, &pauseMenu);
        if (settings.exec() == QDialog::Accepted) {
            // Настройки сохранены
        }
    });

    connect(exitButton, &QPushButton::clicked, [&]() {
        pauseMenu.close();
        emit returnToMenuRequested();
    });

    pauseMenu.exec();
}
