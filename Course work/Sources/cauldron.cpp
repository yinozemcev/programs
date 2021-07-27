#include "cauldron.h"

Cauldron::Cauldron(QWidget *parent) : QWidget(parent)
{

}

void Cauldron::changeState()
{
    /*
     * Если состояние котла не финальное, то увеличиваем его и перерисовываем котел
     *
     */
    if ((cauldronState != Start2) && (cauldronState != Finish6))
    {
        cauldronState++;
        this->repaint();
    }
}

bool Cauldron::setState(unsigned int state)
{
    //Проверяем значение на правильность и в случае успеха заносим в поле cauldronState
    if (state <= Cauldron::Finish6)
    {
        this->cauldronState = state;
        return true;
    }
    return false;
}

void Cauldron::paintEvent(QPaintEvent* event)
{
    //Перерисовываем котел в зависимости от его cauldronState
    Q_UNUSED(event);
    QImage* image = nullptr;
    QPainter painter(this);

    switch (this->cauldronState)
    {
    case (Cauldron::Start1):
    {
        image = new QImage(":/textures/cauldron/cauldron1.png");
        break;
    }
    case (Cauldron::Start2):
    {
        image = new QImage(":/textures/cauldron/cauldron2.png");
        break;
    }
    case (Cauldron::Finish1):
    {
        image = new QImage(":/textures/cauldron/cauldron3.png");
        break;
    }
    case (Cauldron::Finish2):
    {
        image = new QImage(":/textures/cauldron/cauldron4.png");
        break;
    }
    case (Cauldron::Finish3):
    {
        image = new QImage(":/textures/cauldron/cauldron5.png");
        break;
    }
    case (Cauldron::Finish4):
    {
        image = new QImage(":/textures/cauldron/cauldron6.png");
        break;
    }
    case (Cauldron::Finish5):
    {
        image = new QImage(":/textures/cauldron/cauldron7.png");
        break;
    }
    case (Cauldron::Finish6):
    {
        image = new QImage(":/textures/cauldron/cauldron8.png");
        break;
    }
    }

    if (image != nullptr)
    {
        painter.drawImage(0, 0, *image);
        delete image;
    }
}
