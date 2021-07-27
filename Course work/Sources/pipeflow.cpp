#include "pipeflow.h"

PipeFlow::PipeFlow(QWidget *parent)
{
    //Инициализируем number, устанавливаем значение parent
    this->parent = parent;
    this->number = 0;
}

void PipeFlow::addPipe()
{
    //Запускаем генератор случайных чисел
    srand(static_cast<unsigned int>(time(nullptr)));
    //Если количество труб меньше максимального, добавляем трубу
    if (pipeArray.size() < PIPE_MAX_COUNT)
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        //Создаем трубу, ставим ей начальное смещение и случайное состояние
        PipeWidget* new_pipe = new PipeWidget(parent);
        new_pipe->setGeometry(PIPE_X_OFFSET, PIPE_Y_OFFSET, PIPE_SIZE, PIPE_SIZE);
        new_pipe->setState(rand() % 6);
        new_pipe->setOffset(PIPE_Y_OFFSET);
        new_pipe->setNumber(pipeArray.size());

        //Соединяем сигналы каждой трубы со слотами потока труб и MainWindow
        connect(new_pipe, SIGNAL(clickedToPipe(unsigned int)), this, SLOT(selectPipe(unsigned int)));
        connect(new_pipe, SIGNAL(clickedToPipe(unsigned int)), parent, SLOT(tryJoin()));
        //Добавляем трубу в вектор pipeArray
        pipeArray.push_back(new_pipe);
    }
}

void PipeFlow::selectPipe(unsigned int number)
{
    //Проверяем переданное значение на корректность
    if (number < PIPE_MAX_COUNT)
    {
        //Проверяем существование выделенной трубы
        if (pipeArray[this->getNumber()] != nullptr)
        {
            //Проверяем, что выделенная труба действительно такая и снимаем с нее выделение
            if (pipeArray[this->getNumber()]->getState() >= Pipe::VerticalSelected)
            {
                pipeArray[this->getNumber()]->setState(Pipe::State(pipeArray[getNumber()]->getState() - SELECTION));
                pipeArray[this->getNumber()]->repaint();
            }
        }

        //Выделяем трубу c номером number
        pipeArray[number]->setState(Pipe::State(pipeArray[number]->getState() + SELECTION));
        pipeArray[number]->repaint();

        //Обновляем состояние выделенной ячейки и ее порядковый номер
        this->setState(Pipe::State(pipeArray[number]->getState()));
        this->setNumber(number);
    }
}

std::vector<PipeWidget*> PipeFlow::getPipes()
{
    //Возврат вектора pipeArray
    return (this->pipeArray);
}

void PipeFlow::increaseOffset()
{
    unsigned int newOffset = 0;
    //Для всех существующих труб увеличиваем смещение на 10 и изменяем положение трубы
    for (unsigned int i = 0; i < pipeArray.size(); i++)
    {
        if (pipeArray[i] != nullptr)
        {
            newOffset = pipeArray[i]->getOffset() + 10;
            pipeArray[i]->setOffset(newOffset);
            pipeArray[i]->setGeometry(PIPE_X_OFFSET, static_cast<int>(newOffset), PIPE_SIZE, PIPE_SIZE);
            pipeArray[i]->show();

            //Если труба вышла за границу удаляем ее
            if (newOffset > PIPE_Y_MAX)
            {
                removePipe(i);
            }

            //Если труб в потоке не осталось, испускаем сигнал empty()
            if (this->isEmpty())
            {
                emit empty();
            }
        }
    }
}

unsigned int PipeFlow::getNumber()
{
    //Возврат поля number
    return (this->number);
}

bool PipeFlow::setNumber(unsigned int number)
{
    //Установка поля number, если переданное значение не превышает максимальное
    if (number < PIPE_MAX_COUNT)
    {
        this->number = number;
        return (true);
    }
    return (false);
}

void PipeFlow::setVisible(bool isVisible)
{
    //В цикле показываем либо скрываем все трубы (если isVisible == true - показываем, иначе скрываем)
    for (unsigned int i = 0; i < pipeArray.size(); i++)
    {
        pipeArray[i]->setVisible(isVisible);
    }
}

void PipeFlow::removePipe(unsigned int number)
{
    //Удаляем трубу из вектора и делаем указатель на нее нулевым
    delete pipeArray[number];
    pipeArray[number] = nullptr;
}

bool PipeFlow::isEmpty()
{
    //Проверяем массив труб pipeArray на наличие труб, в случае если нашли ненулевой указатель, возвращаем false,
    //Если не нашли, возвращаем true
    for (unsigned int i = 0; i < pipeArray.size(); i++)
    {
        if (pipeArray[i] != nullptr)
        {
            return false;
        }
    }
    return true;
}
