#include "pipe.h"

Pipe::Pipe(Pipe::State state)
{
    //Устанавливаем значение state
    this->state = state;
}

bool Pipe::setState(unsigned int state)
{
    //Проверяем переданное в аргументы значение на корректность и в случае успеха заносим в поле state
    if (state <= State::Corner4Selected)
    {
        this->state = state;
        return true;
    }
    return false;
}

unsigned int Pipe::getState()
{
    //Возврат значения поля state
    return (this->state);
}
