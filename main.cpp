#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //главный класс приложения
    QApplication a(argc, argv);

    qDebug() << "Current path:" << QDir::currentPath();
    //Создание и отображение главного окна
    MainWindow w;
    w.show();

    //запуск основного цикла приложения
    return a.exec();
}
