#include <QObject>
#include <QUrl>

class SimpleMediaPlayer : public QObject {
    Q_OBJECT
public:
    explicit SimpleMediaPlayer(QObject* parent = nullptr);
    void play(const QUrl& url);
    void setVolume(int volume);

private:
    class Impl;
    Impl* d;
};
