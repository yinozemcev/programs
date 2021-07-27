#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include "cell.h"

class CellWidget : public QWidget, public Cell
{
    /*
     * Класс CellWidget - класс, являющийся графической оболочкой ячейки.
     *
     * Поля класса
     * position - положение ячейки на игровом поле, может быть установлено функцией setPosition(position)
     * и получено функцией getPosition().
     *
     * Методы класса
     * CellWidget(parent) - конструктор класса, который наследуется от QWidget.
     *
     * mouseReleaseEvent(event) - слот, в котором обрабатывается событие отпускания кнопки мыши (выделение ячейки).
     *
     * paintEvent(event) - слот, в котором CellWidget перерисовывается в соответствии с его состоянием.
     *
     * Сигналы
     *
     * clickedToCell(position) - сигнал, который посылается в Field и MainWindow и сообщает о нажатии на ячейку в
     * положении position.
     *
     */
    Q_OBJECT
public:
    explicit CellWidget(QWidget *parent = nullptr);
    bool setPosition(unsigned int position);
    unsigned int getPosition();
private:
    unsigned int position;
private slots:
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
signals:
    void clickedToCell(unsigned int position);
};

#endif // CELLWIDGET_H
