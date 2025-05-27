#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //главный класс приложения
    QApplication a(argc, argv);


    //Создание и отображение главного окна
    MainWindow w;
    w.show();

    //запуск основного цикла приложения
    return a.exec();
}
