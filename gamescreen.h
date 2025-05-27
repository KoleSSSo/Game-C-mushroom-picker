#ifndef GAMESCREEN_H
#define GAMESCREEN_H
#include "settingsdialog.h"
#include "styledialog.h"
#include "mushroom.h"
#include <QWidget>
#include <QKeyEvent>
#include <QLabel>
#include <QTimer>
#include <QSoundEffect>
#include <QList>


class GameScreen : public QWidget
{
    Q_OBJECT
public:
    explicit GameScreen(QSoundEffect* backgroundMusic, QWidget *parent = nullptr);
    void setPlayerName(const QString& name) { m_playerName = name; }

    ~GameScreen();

signals:

    void returnToMenuRequested();

protected:
           // Обработчики событий
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateGame();          // Главный игровой цикл
    void updateTimer();
    void spawnMushroom();       // Создание нового гриба
    void showPauseMenu();

private:
    // Игровые объекты
    QList<Mushroom*> m_mushrooms;
    QString m_playerName; // Добавляем эту строку

    // Таймеры
    QTimer* m_gameLoopTimer;
    QTimer* m_clockTimer;
    QTimer* m_spawnTimer;
    QTimer* m_gameTimer;

    // Графика
    QPixmap m_background;
    QPixmap m_playerTexture;
    bool m_texturesLoaded = false;

    // Аудио
    QSoundEffect* m_backgroundMusic;

    // UI элементы
    QLabel* m_timeLabel;
    QLabel* m_scoreLabel;
    QLabel* m_badMushroomsLabel;

    // Игровые переменные
    int m_timeLeft = 90;        // Время игры в секундах
    int m_score = 0;            // Текущий счет
    int m_badMushroomsCollected = 0; // Собрано ядовитых грибов
    int m_playerX = 400;        // Позиция игрока
    int m_playerY = 300;
    const int m_playerSpeed = 15; // Увеличили скорость

    // Состояние управления
    bool m_moveUp = false;
    bool m_moveDown = false;
    bool m_moveLeft = false;
    bool m_moveRight = false;
    bool m_gameActive = true;

    // Приватные методы
    void setupUI();
    void loadTextures();
    void movePlayer(int dx, int dy);
    void checkCollisions();
    void updateScoreDisplay();
    void gameOver();
    void clearMushrooms();
    void saveRecord(); // Новый метод для сохранения рекорда


};

#endif // GAMESCREEN_H
