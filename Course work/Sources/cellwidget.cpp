#include "cellwidget.h"

CellWidget::CellWidget(QWidget *parent) : QWidget(parent)
{

}

bool CellWidget::setPosition(unsigned int position)
{
    //Проверяем переданное в функцию значение на корректность и в случае успеха заносим его в поле position
    if (position < 100)
    {
        this->position = position;
        return (true);
    }
    return (false);
}

unsigned int CellWidget::getPosition()
{
    //Возврат значения переменной position
    return (this->position);
}

void CellWidget::mouseReleaseEvent(QMouseEvent* event)
{
    //Обработка отпускания кнопки мыши, испускается сигнал clickedToCell()
    Q_UNUSED(event);

    emit clickedToCell(this->position);
}

void CellWidget::paintEvent(QPaintEvent* event)
{
    //Перерисовка виджета в соответствии с его текущим состоянием state
    Q_UNUSED(event);

    QImage* image;
    QPainter painter(this);

    switch (this->getState())
    {
    case (Cell::Empty):
    {
        image = new QImage(":/textures/cell/empty.png");
        break;
    }
    case (Cell::Vertical):
    {
        image = new QImage(":/textures/cell/vertical.png");
        break;
    }
    case (Cell::Horizontal):
    {
        image = new QImage(":/textures/cell/horizontal.png");
        break;
    }
    case (Cell::Corner1):
    {
        image = new QImage(":/textures/cell/corner1.png");
        break;
    }
    case (Cell::Corner2):
    {
        image = new QImage(":/textures/cell/corner2.png");
        break;
    }
    case (Cell::Corner3):
    {
        image = new QImage(":/textures/cell/corner3.png");
        break;
    }
    case (Cell::Corner4):
    {
        image = new QImage(":/textures/cell/corner4.png");
        break;
    }
    case (Cell::Selected):
    {
        image = new QImage(":/textures/cell/selected.png");
        break;
    }
    case (Cell::VerticalFull):
    {
        image = new QImage(":/textures/cell/verticalFull.png");
        break;
    }
    case (Cell::HorizontalFull):
    {
        image = new QImage(":/textures/cell/horizontalFull.png");
        break;
    }
    case (Cell::Corner1Full):
    {
        image = new QImage(":/textures/cell/corner1Full.png");
        break;
    }
    case (Cell::Corner2Full):
    {
        image = new QImage(":/textures/cell/corner2Full.png");
        break;
    }
    case (Cell::Corner3Full):
    {
        image = new QImage(":/textures/cell/corner3Full.png");
        break;
    }
    case (Cell::Corner4Full):
    {
        image = new QImage(":/textures/cell/corner4Full.png");
        break;
    }
    }

    if (image != nullptr)
    {
        painter.drawImage(0, 0, *image);
        delete image;
    }
}
