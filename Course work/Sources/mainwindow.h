#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QPainter>
#include "field.h"
#include "pipeflow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    /*
     * Класс MainWindow - класс окна, в котором работает программа.
     *
     * Поля класса
     * addPipe, movePipes, prolongFlow - таймеры класса QTimer, которые осуществляют игровой цикл.
     * interval - время до следующего течения воды по трубам (зависит от уровня сложности)
     *
     * addPipe - посылает раз в 1 секунду сигнал о добавлении трубы.
     * movePipes - посылает 8 раз в секунду сигнал о движении трубы (увеличение их смещения по Y).
     * prolongFlow - посылает сигнал о продолжении потока воды в трубах. Начальный интервал 3 секунды, с каждым сигналом
     * он уменьшается на 50 мс, но не может стать меньше 1 секунды.
     *
     * field - объект класса Field, задающий игровое поле.
     *
     * pipeflow - объект класса PipeFlow, задающий поток труб.
     *
     * Методы класса
     * MainWindow(parent) - конструктор класса, в нем устанвливается интерфейс, создается игровое поле
     * и поток труб (изначально они скрыты и показываются после нажатия на кнопку "Старт").
     *
     * ~MainWindow() - деструктор класса, в котором завершается работа программы.
     *
     * tryJoin() - слот, связанный с сигналами clickedToCell(position) и clickedToPipe(number).
     * Функция соединяет выбранную трубу с выбранной ячейкой (если они существуют).
     *
     * on_pushButton_start_clicked() - слот, обрабатывающий нажатие на кнопку "Старт".
     * Показывается краткое информационное сообщение о сути игры, показывается игровое поле, поток труб и котлы с водой.
     * Запускается игровой цикл функцией startGameCycle().
     *
     * finishGame(success) - слот, который завершает игровой цикл и выводит сообщение в зависимости от результата игры (если
     * success == true - победа, иначе поражение). Также при победе показывается завершающая анимация наливания воды в котел.
     *
     * decreaseTime() - слот, который уменьшает значение поля interval и выводит его
     *
     * updateTime() - слот, устанавливающий полной время до следующего течения воды (время зависит от уровня сложности)
     *
     * checkStart() - слот, который проверяет, началось ли течение воды. Связан с сигналом empty() потока труб,
     * поэтому если течения воды не началось (трубы не были установлены), игроку засчитывается поражение
     *
     * startGameCycle() - метод, запускающий игровой цикл. Создаются таймеры addPipe, movePipes, prolongFlow и им устанавливается
     * соответствующие значения интервалов работы. Также устанавливается нужное состояние стартовому и финальному котлу.
     */
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void tryJoin();
    void on_pushButton_start_clicked();
    void finishGame(bool success);
    void decreaseTime();
    void updateTime();
    void checkStart();
private:
    void startGameCycle();
    QTimer* addPipe;
    QTimer* movePipes;
    QTimer* prolongFlow;
    unsigned int interval;
    Ui::MainWindow *ui;
    Field* field;
    PipeFlow* pipeflow;
};

#endif // MAINWINDOW_H
