#include "mushroom.h"
#include <QDebug>

Mushroom::Mushroom(Mushroom::Type type, QObject *parent)
    : QObject(parent), m_type(type), m_rect(0, 0, 64, 64)
{
    switch(type) {
    case White: m_value = 10; break;
    case Boletus:
    case Birch: m_value = 8; break;
    case Chanterelle: m_value = 5; break;
    case Russula: m_value = 2; break;
    case Toadstool:
    case Amanita: m_value = -10; break;
    }

    loadTexture();
}

void Mushroom::loadTexture()
{
    QString path;
    switch(m_type) {
    case White: path = "C:/programming/Mushroomer/Game/images/mush1.png"; break;
    case Boletus: path = "C:/programming/Mushroomer/Game/images/mush6.png"; break;
    case Birch: path = "C:/programming/Mushroomer/Game/images/mush3.png"; break;
    case Chanterelle: path = "C:/programming/Mushroomer/Game/images/mush2.png"; break;
    case Russula: path = "C:/programming/Mushroomer/Game/images/mush7.png"; break;
    case Toadstool: path = "C:/programming/Mushroomer/Game/images/mush5.png"; break;
    case Amanita: path = "C:/programming/Mushroomer/Game/images/mush4.png"; break;
    }

    m_texture = QPixmap(path);
    if (m_texture.isNull()) {
        qDebug() << "Failed to load texture:" << path;
        // Заполняем цветом для отладки
        m_texture = QPixmap(m_rect.size());
        m_texture.fill(Qt::red);
    } else {
        m_texture = m_texture.scaled(m_rect.size(), Qt::KeepAspectRatio);
    }
}
