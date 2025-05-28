#include "gamescreen.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QRandomGenerator>
#include <QDir>

GameScreen::GameScreen(QSoundEffect* backgroundMusic, QWidget *parent)
    : QWidget(parent), m_backgroundMusic(backgroundMusic)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(1080, 720); // Фиксируем размер окна
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    setFocusPolicy(Qt::StrongFocus);

    loadTextures();
    setupUI();

    // Главный игровой цикл (60 FPS)
    m_gameLoopTimer = new QTimer(this);
    connect(m_gameLoopTimer, &QTimer::timeout, this, &GameScreen::updateGame);
    m_gameLoopTimer->start(16);

    // Таймер игрового времени
    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, &QTimer::timeout, this, &GameScreen::updateTimer);
    m_clockTimer->start(1000);

    // Таймер спавна грибов
    m_spawnTimer = new QTimer(this);
    connect(m_spawnTimer, &QTimer::timeout, this, &GameScreen::spawnMushroom);
    m_spawnTimer->start(1500);

    // Таймер общего времени игры
    m_gameTimer = new QTimer(this);
    connect(m_gameTimer, &QTimer::timeout, this, [this]() {
        if(m_gameActive) {
            m_gameActive = false;
            m_clockTimer->stop();
            m_spawnTimer->stop();
            updateTimer();
        }
    });
    m_gameTimer->start(90000);

    setFocusPolicy(Qt::StrongFocus);
    setFocus(); // Убедимся, что фокус установлен

}


GameScreen::~GameScreen()
{
    m_gameTimer->stop();
    m_clockTimer->stop();
}

void GameScreen::updateGame()
{
    if (!m_gameActive) return;

    // Обработка движения
    int dx = 0, dy = 0;
    if (m_moveUp) dy -= m_playerSpeed;
    if (m_moveDown) dy += m_playerSpeed;
    if (m_moveLeft) dx -= m_playerSpeed;
    if (m_moveRight) dx += m_playerSpeed;

    // Нормализация диагонального движения
    if (dx != 0 && dy != 0) {
        dx *= 0.7071f; // 1/sqrt(2)
        dy *= 0.7071f;
    }

    if (dx != 0 || dy != 0) {
        movePlayer(dx, dy);
    }
}

void GameScreen::showGameOverDialog(const QString& message)
{
    m_gameActive = false;
    saveRecord();

    // Создаем диалоговое окно
    QDialog endDialog(this);
    endDialog.setWindowTitle("Игра окончена");
    endDialog.setFixedSize(400, 200);
    endDialog.setStyleSheet(
        "QDialog {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "}");

    QVBoxLayout *layout = new QVBoxLayout(&endDialog);

    // Сообщение
    QLabel *messageLabel = new QLabel(message, &endDialog);
    messageLabel->setStyleSheet("font-family: 'Saturn'; font-size: 18px;");
    messageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(messageLabel);

    // Кнопка
    QPushButton *menuButton = new QPushButton("Выйти в главное меню", &endDialog);
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
    layout->addWidget(menuButton, 0, Qt::AlignCenter);

    // Обработка нажатия кнопки
    connect(menuButton, &QPushButton::clicked, [&]() {
        endDialog.accept();
        this->close();
        emit returnToMenuRequested();
    });

    endDialog.exec();
}

void GameScreen::resizeEvent(QResizeEvent *event)
{
    // QWidget::resizeEvent(event);
    // // Обновляем позиции UI элементов при изменении размера окна
    // m_timeLabel->move(20, 20);
    // m_scoreLabel->move(width() - 220, 20);
    // m_badMushroomsLabel->move(width() / 2 - 100, 20);

    // // Масштабируем фон
    // if (!m_background.isNull()) {
    //     m_background = m_background.scaled(size(), Qt::KeepAspectRatioByExpanding);
    // }
}

void GameScreen::clearMushrooms()
{
    qDeleteAll(m_mushrooms);
    m_mushrooms.clear();
}

void GameScreen::loadTextures()
{
    // Фон
    QPixmap bg(":/images/images/back.png");
    if (!bg.isNull()) {
        m_background = bg.scaled(this->size(), Qt::IgnoreAspectRatio);
    } else {
        m_background = QPixmap(this->size());
        m_background.fill(QColor("#5a8f5a"));
    }

    // Игрок (оставляем размер 128x128)
    QPixmap player(":/images/images/hat.png");
    if (!player.isNull()) {
        m_playerTexture = player.scaled(128, 128, Qt::KeepAspectRatio);
    } else {
        m_playerTexture = QPixmap(128, 128);
        m_playerTexture.fill(Qt::blue);
    }

    // Начальная позиция игрока по центру
    m_playerX = (width() - m_playerTexture.width()) / 2;
    m_playerY = (height() - m_playerTexture.height()) / 2;
}

void GameScreen::setupUI()
{
    // Устанавливаем размеры элементов в соответствии с новым разрешением
    m_timeLabel = new QLabel(this);
    m_timeLabel->setGeometry(20, 20, 200, 40);

    m_scoreLabel = new QLabel(this);
    m_scoreLabel->setGeometry(920, height() - 60, 200, 40); // правый нижний угол


    m_badMushroomsLabel = new QLabel(this);
    m_badMushroomsLabel->setGeometry(920, 20, 200, 40);

    // Обновляем стили
    QString labelStyle = "font-family: 'Saturn'; font-size: 24px; color: white;";
    m_timeLabel->setStyleSheet(labelStyle);
    m_scoreLabel->setStyleSheet(labelStyle);
    m_badMushroomsLabel->setStyleSheet(labelStyle);

    updateTimer();
    updateScoreDisplay();
}

void GameScreen::updateScoreDisplay()
{
    m_scoreLabel->setText(QString("Очки: %1").arg(m_score));
    m_badMushroomsLabel->setText(QString("Жизни: %1/3").arg(3 - m_badMushroomsCollected));
    m_badMushroomsLabel->setStyleSheet(
        QString("font-family: 'Saturn'; font-size: 24px; color: %1;")
            .arg((3 - m_badMushroomsCollected) == 1 ? "red" : "white"));
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

    // УБИРАЕМ дублированный HUD из paintEvent, так как у нас уже есть QLabel'ы
}

void GameScreen::updateTimer()
{
    if (!m_gameActive) return;

    if (m_timeLeft > 0) {
        m_timeLeft--;
        m_timeLabel->setText(QString("Время: %1:%2")
                                 .arg(m_timeLeft / 60, 2, 10, QLatin1Char('0'))
                                 .arg(m_timeLeft % 60, 2, 10, QLatin1Char('0')));
    }

    if (m_timeLeft <= 0 && m_gameActive) {
        m_gameActive = false;
        gameOver("Время закончилось!\nВаш результат: " + QString::number(m_score));
    }
}

void GameScreen::spawnMushroom()
{
    Mushroom::Type type = static_cast<Mushroom::Type>(QRandomGenerator::global()->bounded(7));
    Mushroom* mushroom = new Mushroom(type, this);

    // Учитываем новые границы экрана
    int x = QRandomGenerator::global()->bounded(width() - mushroom->rect().width());
    int y = QRandomGenerator::global()->bounded(height() - mushroom->rect().height());
    mushroom->setPosition(x, y);

    m_mushrooms.append(mushroom);

    QTimer::singleShot(5000, [this, mushroom]() {
        if (m_mushrooms.removeOne(mushroom)) {
            mushroom->deleteLater();
            update();
        }
    });

    update();
}

void GameScreen::checkCollisions()
{
    QRect playerRect(m_playerX, m_playerY, m_playerTexture.width(), m_playerTexture.height());

    for (int i = 0; i < m_mushrooms.size(); ++i) {
        if (playerRect.intersects(m_mushrooms[i]->rect())) {
            Mushroom* collected = m_mushrooms.takeAt(i);

            // Обновляем счет
            m_score += collected->value();

            // Проверяем на несъедобные грибы
            if (collected->value() < 0) {
                m_badMushroomsCollected++;
                if (m_badMushroomsCollected >= 3) {
                    gameOver("Время закончилось!\nВаш результат: " + QString::number(m_score));
                    return;
                }
            }

            collected->deleteLater();
            i--;
            updateScoreDisplay();
            update();
        }
    }
}
void GameScreen::keyPressEvent(QKeyEvent *event)
{
    if (!m_gameActive && event->key() != Qt::Key_Escape) return;

    switch (event->key()) {
    case Qt::Key_W:
        m_moveUp = true;
        break;
    case Qt::Key_S:
        m_moveDown = true;
        break;
    case Qt::Key_A:
        m_moveLeft = true;
        break;
    case Qt::Key_D:
        m_moveRight = true;
        break;
    case Qt::Key_Up:
        m_moveUp = true;
        break;
    case Qt::Key_Down:
        m_moveDown = true;
        break;
    case Qt::Key_Left:
        m_moveLeft = true;
        break;
    case Qt::Key_Right:
        m_moveRight = true;
        break;
    case Qt::Key_Escape:
        showPauseMenu();
        break;
    case Qt::Key_P: // Добавляем паузу по клавише P
        showPauseMenu();
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void GameScreen::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W: m_moveUp = false; break;
    case Qt::Key_S: m_moveDown = false; break;
    case Qt::Key_A: m_moveLeft = false; break;
    case Qt::Key_D: m_moveRight = false; break;
    case Qt::Key_Up: m_moveUp = false; break;
    case Qt::Key_Down: m_moveDown = false; break;
    case Qt::Key_Left: m_moveLeft = false; break;
    case Qt::Key_Right: m_moveRight = false; break;
    default: QWidget::keyReleaseEvent(event);
    }
}

void GameScreen::movePlayer(int dx, int dy)
{
    if (!m_gameActive) return; // Добавляем проверку

    int newX = qBound(0, m_playerX + dx, width() - m_playerTexture.width());
    int newY = qBound(0, m_playerY + dy, height() - m_playerTexture.height());

    if (newX != m_playerX || newY != m_playerY) {
        m_playerX = newX;
        m_playerY = newY;
        checkCollisions();
        update();
    }
}

void GameScreen::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setFocus();
}

void GameScreen::saveRecord()
{
    if (m_score > 0) {
        m_recordsManager.saveRecord(m_playerName.isEmpty() ? "Игрок" : m_playerName, m_score);
    }
}


void GameScreen::showPauseMenu()
{
    bool wasActive = m_gameActive;
    m_gameActive = false;

    // Останавливаем все таймеры
    m_clockTimer->stop();
    m_spawnTimer->stop();
    m_gameLoopTimer->stop();

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

    connect(resumeButton, &QPushButton::clicked, [&, wasActive]() {
        pauseMenu.close();
        m_gameActive = wasActive;
        if(m_gameActive) {
            m_clockTimer->start();
            m_spawnTimer->start();
            m_gameLoopTimer->start();
        }
        setFocus();
    });

    connect(settingsButton, &QPushButton::clicked, [&]() {
        SettingsDialog settings(m_backgroundMusic, &pauseMenu);
        settings.exec();
    });

    connect(exitButton, &QPushButton::clicked, [this, &pauseMenu]() {
        pauseMenu.accept(); // Сначала закрываем диалог
        this->close(); // Затем закрываем игровой экран
        // Не удаляем объекты вручную - WA_DeleteOnClose позаботится об этом
        emit returnToMenuRequested();
    });

    if (pauseMenu.exec() == QDialog::Rejected && wasActive) {
        // Если просто закрыли диалог (не нажали кнопки)
        m_gameActive = wasActive;
        m_clockTimer->start();
        m_spawnTimer->start();
        m_gameLoopTimer->start();
    }
}

void GameScreen::gameOver(const QString& message)
{
    m_gameActive = false;
    m_clockTimer->stop();
    m_spawnTimer->stop();
    m_gameLoopTimer->stop();

    saveRecord();

    QDialog endDialog(this);
    endDialog.setWindowTitle("Игра окончена");
    endDialog.setFixedSize(400, 200);
    endDialog.setStyleSheet(
        "QDialog {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 3px solid #2d5a3f;"
        "}");

    QVBoxLayout *layout = new QVBoxLayout(&endDialog);

    // Сообщение
    QLabel *messageLabel = new QLabel(message, &endDialog);
    messageLabel->setStyleSheet("font-family: 'Saturn'; font-size: 18px;");
    messageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(messageLabel);

    // Кнопка
    QPushButton *menuButton = new QPushButton("Выйти в главное меню", &endDialog);
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
    layout->addWidget(menuButton, 0, Qt::AlignCenter);

    connect(menuButton, &QPushButton::clicked, [&]() {
        endDialog.accept();
        this->close();
        emit returnToMenuRequested();
    });

    endDialog.exec();
}
