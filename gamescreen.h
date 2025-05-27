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
    ~GameScreen();

signals:
    void returnToMenuRequested();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void updateTimer();
    void showPauseMenu();

private:
    void setupUI();
    void movePlayer(int dx, int dy);
    void loadTextures();

    QList<Mushroom*> m_mushrooms;
    void spawnMushroom();
    void checkCollisions();

    QSoundEffect* m_backgroundMusic;
    QLabel* m_timeLabel;
    QPixmap m_background;
    QPixmap m_playerTexture;
    QTimer* m_gameTimer;
    QTimer* m_clockTimer;
    int m_timeLeft = 90;
    int m_playerX = 400;
    int m_playerY = 300;
    const int m_playerSpeed = 10;
    bool m_texturesLoaded = false;
};

#endif // GAMESCREEN_H
