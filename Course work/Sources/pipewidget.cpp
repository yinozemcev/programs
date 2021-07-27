#include "pipewidget.h"

PipeWidget::PipeWidget(QWidget *parent) : QWidget(parent)
{
    //Инициализируем поле number
    this->number = 0;
}

void PipeWidget::setOffset(unsigned int offset)
{
    //Устанавливаем поле offset
    this->offset = offset;
}

unsigned int PipeWidget::getOffset()
{
    //Возрат поля offset
    return (this->offset);
}

bool PipeWidget::setNumber(unsigned int number)
{
    //Устанавливаем значение поля number, если оно не превышает максимальное количество труб
    if (number < 50)
    {
        this->number = number;
        return (true);
    }
    return (false);
}

unsigned int PipeWidget::getNumber()
{
    //Возрат поля number
    return (this->number);
}

void PipeWidget::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    //Испускаем сигнал clickedToCell при отпускании мыши
    emit clickedToPipe(this->number);
}

void PipeWidget::paintEvent(QPaintEvent* event)
{
    //Перерисовываем трубу в соответствии с ее текущим состоянием
    Q_UNUSED(event);

    QImage* image = nullptr;
    QPainter painter(this);

    switch (this->getState())
    {
    case (Pipe::Vertical):
    {
        image = new QImage(":/textures/pipe/vertical.png");
        break;
    }
    case (Pipe::Horizontal):
    {
        image = new QImage(":/textures/pipe/horizontal.png");
        break;
    }
    case (Pipe::Corner1):
    {
        image = new QImage(":/textures/pipe/corner1.png");
        break;
    }
    case (Pipe::Corner2):
    {
        image = new QImage(":/textures/pipe/corner2.png");
        break;
    }
    case (Pipe::Corner3):
    {
        image = new QImage(":/textures/pipe/corner3.png");
        break;
    }
    case (Pipe::Corner4):
    {
        image = new QImage(":/textures/pipe/corner4.png");
        break;
    }
    case (Pipe::VerticalSelected):
    {
        image = new QImage(":/textures/pipe/verticalSelected.png");
        break;
    }
    case (Pipe::HorizontalSelected):
    {
        image = new QImage(":/textures/pipe/horizontalSelected.png");
        break;
    }
    case (Pipe::Corner1Selected):
    {
        image = new QImage(":/textures/pipe/corner1Selected.png");
        break;
    }
    case (Pipe::Corner2Selected):
    {
        image = new QImage(":/textures/pipe/corner2Selected.png");
        break;
    }
    case (Pipe::Corner3Selected):
    {
        image = new QImage(":/textures/pipe/corner3Selected.png");
        break;
    }
    case (Pipe::Corner4Selected):
    {
        image = new QImage(":/textures/pipe/corner4Selected.png");
        break;
    }
    }

    if (image != nullptr)
    {
        painter.drawImage(0, 0, *image);
    }
    delete image;
}
