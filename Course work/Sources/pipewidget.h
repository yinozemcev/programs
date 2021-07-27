#ifndef PIPEWIDGET_H
#define PIPEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include "pipe.h"

class PipeWidget : public QWidget, public Pipe
{
    /*
     * Класс PipeWidget - класс, являющийся графической оболочкой трубы
     *
     * Поля класса
     * number - порядковый номер трубы в потоке труб, может быть установлено функцией setNumber(number)
     * и получено функцией getNumber().
     *
     * offset - смещение трубы по оси Y в потоке труб, может быть установлено функцией setOffset(offset)
     * и получено функцией getOffset().
     *
     * Методы класса
     * PipeWidget(parent) - конструктор класса, который наследуется от QWidget.
     *
     * mouseReleaseEvent(event) - слот, в котором обрабатывается событие отпускания кнопки мыши (выделение трубы).
     *
     * paintEvent(event) - слот, в котором PipeWidget перерисовывается в соответствии с его состоянием.
     *
     * Сигналы
     *
     * clickedToPipe(position) - сигнал, который посылается в PipeFlow и MainWindow и сообщает о нажатии на трубу в
     * положении position.
     *
     */
    Q_OBJECT
public:
    explicit PipeWidget(QWidget *parent = nullptr);
    void setOffset(unsigned int offset);
    unsigned int getOffset();
    bool setNumber(unsigned int position);
    unsigned int getNumber();
private:
    unsigned int number;
    unsigned int offset;
private slots:
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
signals:
    void clickedToPipe(unsigned int number);
};

#endif // PIPEWIDGET_H
