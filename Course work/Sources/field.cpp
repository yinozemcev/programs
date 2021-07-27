#include "field.h"

Field::Field(QWidget *parent)
{
    //Устанавливаем поле, инициализируем поля state, position, flowLastCellPosition, flowLastCellState
    setupField(parent);
    this->position = 0;
    this->setState(Cell::Empty);
    flowLastCellPosition = CELL_START_POSITION;
    flowLastCellState = Cell::Empty;
}

void Field::setupField(QWidget* parent)
{
    //Создаем объекты класса CellWidget и размещаем их на нужной позиции, соединяем сигналы каждой ячейки со слотами MainWindow и Field
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            CellWidget* new_cell = new CellWidget(parent);
            //Ставим ячейку в правильные координаты (ячейки распологаются слева направо и сверху вниз)
            //ставим ей правильное поле position
            new_cell->setGeometry(CELL_X_OFFSET + j*CELL_SIZE, CELL_Y_OFFSET + i*CELL_SIZE, CELL_SIZE, CELL_SIZE);
            new_cell->setState(Cell::Empty);
            new_cell->setPosition(static_cast<unsigned int>(10*i + j));
            //Соединяем слоты
            connect(new_cell, SIGNAL(clickedToCell(unsigned int)), this, SLOT(selectCell(unsigned int)));
            connect(new_cell, SIGNAL(clickedToCell(unsigned int)), parent, SLOT(tryJoin()));
            cellArray.push_back(new_cell);
        }
    }
}

void Field::selectCell(unsigned int position)
{
    //Выделение ячейки
    if ((position < CELL_COUNT) && (cellArray[position]->getState() == Cell::Empty))
    {
        //Проверяем что выделенная ячейка существует
        if (cellArray[getPosition()] != nullptr)
        {
            //Проверяем, что выделенная ячейка действительно выделена, если это так делаем ее пустой
            if (cellArray[getPosition()]->getState() == Cell::Selected)
            {
                cellArray[getPosition()]->setState(Cell::Empty);
                cellArray[getPosition()]->repaint();
            }
        }

        //Делаем нажатую ячейку выделенной
        cellArray[position]->setState(Cell::Selected);
        cellArray[position]->repaint();

        //Обновляем положение и состояние выделенной ячейки
        this->setState(cellArray[position]->getState());
        this->setPosition(position);
    }
}

void Field::prolongFlow()
{
    /*
     * Функция продления потока воды
     *
     * Изначально flowLastCellPosition == CELL_START_POSITION. В этой ситуации следующей ячейкой в потоке будет
     * являтся ячейка с положением 0.Чтобы вода в нее попала, у нее должно быть состояние горизонтальная труба,
     * либо угловая труба №2, иначе соединения не будет.
     * В общем случае вода в трубу может попасть из 2 ячеек, значения которых заносятся в prevState и nextState
     * Далее, если труба с состоянием prevState полная, а с состояние nextState совместимо с состоянием текущей трубы,
     * труба с состоянием nextState заполняется, путем прибавления FILL_CELL. И наоборот, если nextState - полная,
     * а prevState совместимо с состоянием текущей трубы, трубы с prevState заполняется. Также изменяется flowLastCellPosition
     * в соотвествии с тем, куда был продолежн поток. В итоге перерисовывается виджет в положении flowLastCellPosition.
     */
    unsigned int prevState = 0, nextState = 0;

    if ((flowLastCellPosition == 99) && ((flowLastCellState == Cell::HorizontalFull) || (flowLastCellState == Cell::Corner4Full)))
    {
        emit finishGame(true);
        return;
    }

    if (flowLastCellPosition == CELL_START_POSITION)
    {
        if ((cellArray[0]->getState() == Cell::Horizontal) || (cellArray[0]->getState() == Cell::Corner2))
        {
            cellArray[0]->setState(cellArray[0]->getState() + FILL_CELL);
            flowLastCellState = Cell::State(cellArray[0]->getState());
            flowLastCellPosition = 0;
        }
        else
        {
            emit finishGame(false);
            return;
        }
    }
    else
    {
        switch (flowLastCellState)
        {
        case (VerticalFull):
        {
            if ((flowLastCellPosition / 10 > 0) && (flowLastCellPosition / 10 < 9))
            {
                prevState = Cell::State(cellArray[flowLastCellPosition + TO_UP]->getState());
                nextState = Cell::State(cellArray[flowLastCellPosition + TO_DOWN]->getState());

                if (prevState >= Cell::VerticalFull)
                {
                    if ((nextState == Cell::Vertical) || (nextState == Cell::Corner3) || (nextState == Cell::Corner4))
                    {
                        cellArray[flowLastCellPosition + TO_DOWN]->setState(nextState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_DOWN]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_DOWN;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
                else if (nextState >= Cell::VerticalFull)
                {
                    if ((prevState == Cell::Horizontal) || (prevState == Cell::Corner1) || (prevState == Cell::Corner2))
                    {
                        cellArray[flowLastCellPosition + TO_UP]->setState(prevState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_UP]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_UP;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
            }
            else
            {
                emit finishGame(false);
                return;
            }
            break;
        }
        case (HorizontalFull):
        {
            if (flowLastCellPosition == 0)
            {
                if ((cellArray[TO_RIGHT]->getState() == Cell::Horizontal) || (cellArray[TO_RIGHT]->getState() == Cell::Corner2))
                {
                    cellArray[TO_RIGHT]->setState(cellArray[TO_RIGHT]->getState() + FILL_CELL);
                    flowLastCellState = Cell::State(cellArray[TO_RIGHT]->getState());
                    flowLastCellPosition = TO_RIGHT;
                }
                else
                {
                    emit finishGame(false);
                    return;
                }
            }
            else if ((flowLastCellPosition % 10 > 0) && (flowLastCellPosition % 10 < 9))
            {
                prevState = Cell::State(cellArray[flowLastCellPosition + TO_LEFT]->getState());
                nextState = Cell::State(cellArray[flowLastCellPosition + TO_RIGHT]->getState());

                if (prevState >= Cell::VerticalFull)
                {
                    if ((nextState == Cell::Horizontal) || (nextState == Cell::Corner2) || (nextState == Cell::Corner3))
                    {
                        cellArray[flowLastCellPosition + TO_RIGHT]->setState(nextState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_RIGHT]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_RIGHT;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
                else if (nextState >= Cell::VerticalFull)
                {
                    if ((prevState == Cell::Horizontal) || (prevState == Cell::Corner1) || (prevState == Cell::Corner4))
                    {
                        cellArray[flowLastCellPosition + TO_LEFT]->setState(prevState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_LEFT]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_LEFT;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
            }
            else
            {
                emit finishGame(false);
                return;
            }
            break;
        }
        case (Corner1Full):
        {
            if ((flowLastCellPosition / 10 < 9) && (flowLastCellPosition % 10 < 9))
            {
                prevState = Cell::State(cellArray[flowLastCellPosition + TO_RIGHT]->getState());
                nextState = Cell::State(cellArray[flowLastCellPosition + TO_DOWN]->getState());

                if (prevState >= Cell::VerticalFull)
                {
                    if ((nextState == Cell::Vertical) || (nextState == Cell::Corner3) || (nextState == Cell::Corner4))
                    {
                        cellArray[flowLastCellPosition + TO_DOWN]->setState(nextState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_DOWN]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_DOWN;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
                else if (nextState >= Cell::VerticalFull)
                {
                    if ((prevState == Cell::Horizontal) || (prevState == Cell::Corner2) || (prevState == Cell::Corner3))
                    {
                        cellArray[flowLastCellPosition + TO_RIGHT]->setState(prevState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_RIGHT]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_RIGHT;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
            }
            else
            {
                emit finishGame(false);
                return;
            }
            break;
        }
        case (Corner2Full):
        {
            if (flowLastCellPosition == 0)
            {
                if ((cellArray[TO_DOWN]->getState() == Cell::Vertical) || (cellArray[TO_DOWN]->getState() == Cell::Corner4))
                {
                    cellArray[TO_DOWN]->setState(Cell::State(cellArray[TO_DOWN]->getState() + FILL_CELL));
                    flowLastCellState = Cell::State(cellArray[TO_DOWN]->getState());
                    flowLastCellPosition = TO_DOWN;
                }
                else
                {
                    emit finishGame(false);
                    return;
                }
            }
            else if ((flowLastCellPosition % 10 > 0) && (flowLastCellPosition / 10 < 9))
            {
                prevState = Cell::State(cellArray[flowLastCellPosition + TO_LEFT]->getState());
                nextState = Cell::State(cellArray[flowLastCellPosition + TO_DOWN]->getState());

                if (prevState >= Cell::VerticalFull)
                {
                    if ((nextState == Cell::Vertical) || (nextState == Cell::Corner3) || (nextState == Cell::Corner4))
                    {
                        cellArray[flowLastCellPosition + TO_DOWN]->setState(nextState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_DOWN]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_DOWN;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
                else if (nextState >= Cell::VerticalFull)
                {
                    if ((prevState == Cell::Horizontal) || (prevState == Cell::Corner1) || (prevState == Cell::Corner4))
                    {
                        cellArray[flowLastCellPosition + TO_LEFT]->setState(prevState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_LEFT]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_LEFT;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
            }
            else
            {
                emit finishGame(false);
                return;
            }
            break;
        }
        case (Corner3Full):
        {
            if ((flowLastCellPosition / 10 > 0) && (flowLastCellPosition % 10 > 0))
            {
                prevState = Cell::State(cellArray[flowLastCellPosition + TO_LEFT]->getState());
                nextState = Cell::State(cellArray[flowLastCellPosition + TO_UP]->getState());

                if (prevState >= Cell::VerticalFull)
                {
                    if ((nextState == Cell::Vertical) || (nextState == Cell::Corner1) || (nextState == Cell::Corner2))
                    {
                        cellArray[flowLastCellPosition + TO_UP]->setState(nextState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_UP]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_UP;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
                else if (nextState >= Cell::VerticalFull)
                {
                    if ((prevState == Cell::Horizontal) || (prevState == Cell::Corner1) || (prevState == Cell::Corner4))
                    {
                        cellArray[flowLastCellPosition + TO_LEFT]->setState(prevState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_LEFT]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_LEFT;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
            }
            else
            {
                emit finishGame(false);
                return;
            }
            break;
        }
        case (Corner4Full):
        {
            if ((flowLastCellPosition / 10 > 0) && (flowLastCellPosition % 10 < 9))
            {
                prevState = Cell::State(cellArray[flowLastCellPosition + TO_UP]->getState());
                nextState = Cell::State(cellArray[flowLastCellPosition + TO_RIGHT]->getState());

                if (prevState >= Cell::VerticalFull)
                {
                    if ((nextState == Cell::Horizontal) || (nextState == Cell::Corner2) || (nextState == Cell::Corner3))
                    {
                        cellArray[flowLastCellPosition + TO_RIGHT]->setState(nextState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_RIGHT]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_RIGHT;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
                else if (nextState >= Cell::VerticalFull)
                {
                    if ((prevState == Cell::Vertical) || (prevState == Cell::Corner1) || (prevState == Cell::Corner2))
                    {
                        cellArray[flowLastCellPosition + TO_UP]->setState(prevState + FILL_CELL);
                        flowLastCellState = Cell::State(cellArray[flowLastCellPosition + TO_UP]->getState());
                        flowLastCellPosition = flowLastCellPosition + TO_UP;
                    }
                    else
                    {
                        emit finishGame(false);
                        return;
                    }
                }
            }
            else
            {
                emit finishGame(false);
                return;
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }
    cellArray[flowLastCellPosition]->repaint();
}

std::vector<CellWidget*> Field::getCells()
{
    //Возрат вектора cellArray
    return (this->cellArray);
}

unsigned int Field::getPosition()
{
    //Возврат поля position
    return (this->position);
}

bool Field::setPosition(unsigned int position)
{
    //Установка поля position, если переданное в функцию значение не превышает количества ячеек
    if (position < CELL_COUNT)
    {
        this->position = position;
        return (true);
    }
    return (false);
}

void Field::setVisible(bool isVisible)
{
    //В цикле все ячейки скрываются или показываются
    for (unsigned int i = 0; i < CELL_COUNT; i++)
    {
        cellArray[i]->setVisible(isVisible);
    }
}

void Field::changeState(unsigned int position, Cell::State state)
{
    //Изменение состояние ячейки в положении position на то, что передано аргументом state
    cellArray[position]->setState(state);
    cellArray[position]->repaint();
}
