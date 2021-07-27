#ifndef CELL_H
#define CELL_H

class Cell
{
    /*
     * Класс Cell - базовый класс, представляющий собой абстракцию ячейки.
     *
     * Поля класса
     * state - текущее состояние ячейки (одно из значений перечисления Cell::State), может быть установлено функцией
     * setState(state) и получено функцией getState().
     *
     * Методы класса
     *
     * Cell(state) - конструктор класса, который устанавливает состоянию ячейки переданное значение state.
     */
public:
    enum State
    {
        /*
         * Перечисление State содержит все возможные состояния ячейки
         *
         */
        Empty, //Пустая
        Vertical, //Вертикальная труба
        Horizontal, //Горизонтальная труба
        Corner1, //Угловая труба №1
        Corner2, //Угловая труба №2
        Corner3, //Угловая труба №3
        Corner4, //Угловая труба №4
        Selected, //Выделенная
        VerticalFull, //Заполненная водой вертикальная труба
        HorizontalFull, //Заполненная водой горизонтальная труба
        Corner1Full, //Заполненная водой угловая труба №1
        Corner2Full, //Заполненная водой угловая труба №2
        Corner3Full, //Заполненная водой угловая труба №3
        Corner4Full, //Заполненная водой угловая труба №4
    };


    Cell(Cell::State state = Cell::Empty);
    bool setState(unsigned int state);
    unsigned int getState();
private:
    unsigned int state;
};

#endif // CELL_H
