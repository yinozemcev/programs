#ifndef CAULDRON_H
#define CAULDRON_H

#include <QWidget>
#include <QPainter>

class Cauldron : public QWidget
{
    /*
     * Класс Cauldron - класс виджетов, на которых будут отображаться стартовый и финишный котлы.
     *
     * Поля класса
     * cauldronState - текущее состояние котла (одно из значений перечисления Cauldron::State), может быть установлено функцией
     * setState(state), затем изменяется сигналом timeout() таймера prolongFlow, который соединен со слотом changeState().
     *
     * Методы класса
     * Cauldron(parent) - конструктор, наследованный от QWidget.
     *
     * paintEvent(event) - метод, перерисовывающий виджет в соответствии с cauldronState.
     */
    Q_OBJECT
public:
    enum State
    {
        /*
         * Перечисление State содержит все возможные значения состояния котлов
         * Состояния StartN используются для стартового котла, FinishN - для финишного.
         */
        Start1,
        Start2,
        Finish1,
        Finish2,
        Finish3,
        Finish4,
        Finish5,
        Finish6
    };
    explicit Cauldron(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    bool setState(unsigned int state);
public slots:
    void changeState();
private:
    unsigned int cauldronState;
};

#endif // CAULDRON_H
