#ifndef MUSHROOM_H
#define MUSHROOM_H

#include <QObject>
#include <QPixmap>
#include <QRect>

class Mushroom : public QObject
{
    Q_OBJECT
public:
    enum Type {
        White,      // +10
        Boletus,    // +8 (подосиновик)
        Birch,      // +8 (подберезовик)
        Chanterelle,// +5 (лисичка)
        Russula,    // +2 (сыроежка)
        Toadstool,  // -10 (поганка)
        Amanita     // -10 (мухомор)
    };

    explicit Mushroom(Type type, QObject *parent = nullptr);

    QPixmap texture() const { return m_texture; }
    int value() const { return m_value; }
    QRect rect() const { return m_rect; }
    void setPosition(int x, int y) { m_rect.moveTo(x, y); }
    Type type() const { return m_type; }

private:
    void loadTexture();

    Type m_type;
    QPixmap m_texture;
    int m_value;
    QRect m_rect;
};

#endif // MUSHROOM_H
