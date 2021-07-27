#ifndef PIPE_H
#define PIPE_H


class Pipe
{
    /*
     * Класс Pipe - базовый класс, представляющий собой абстракцию трубы.
     *
     * Поля класса
     * state - текущее состояние трубы (одно из значений перечисления Pipe::State), может быть установлено функцией
     * setState(state) и получено функцией getState().
     *
     * Методы класса
     *
     * Pipe(state) - конструктор класса, который устанавливает состоянию трубы переданное значение state.
     */
public:
    enum State
    {
        /*
         * Перечисление State содержит все возможные состояния трубы.
         *
         */
        Vertical, //Вертикальная труба
        Horizontal, //Горизонтальная труба
        Corner1, //Угловая труба №1
        Corner2, //Угловая труба №2
        Corner3, //Угловая труба №3
        Corner4, //Угловая труба №4
        VerticalSelected, //Выбранная вертикальная труба
        HorizontalSelected, //Выбранная горизонтальная труба
        Corner1Selected, //Выбранная угловая труба №1
        Corner2Selected, //Выбранная угловая труба №2
        Corner3Selected, //Выбранная угловая труба №3
        Corner4Selected, //Выбранная угловая труба №4
    };

    Pipe(Pipe::State state = Pipe::Vertical);
    bool setState(unsigned int state);
    unsigned int getState();
private:
    unsigned int state;
};

#endif // PIPE_H
