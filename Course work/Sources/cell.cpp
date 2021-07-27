#include "cell.h"

Cell::Cell(Cell::State state)
{
    //Устанавливаем значение state
    this->state = state;
}

bool Cell::setState(unsigned int state)
{
    //Проверяем переданное в аргументы значение на корректность и в случае успеха заносим в поле state
    if (state <= Cell::Corner4Full)
    {
        this->state = state;
        return true;
    }
    return false;
}

unsigned int Cell::getState()
{
    //Возврат значеняи поля state
    return (this->state);
}
