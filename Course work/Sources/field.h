#ifndef FIELD_H
#define FIELD_H

#include "cellwidget.h"

#define CELL_SIZE 60 //Размер ячейки
#define CELL_X_OFFSET 150 //Смещение игрового поля по координате Х относительно левого верхнего угла окна программы
#define CELL_Y_OFFSET 90 //Смещение игрового поля по координате Y относительно левого верхнего угла окна программы
#define CELL_START_POSITION 777 //Начальное значение потока воды, используется для старта потока в ячейке с положением 0
#define CELL_COUNT 100 //Количество ячеек в поле
#define TO_UP static_cast<unsigned int>(-10) //Значение, прибавляемое для получение положения ячейки, находящейся сверху от нужной
#define TO_DOWN static_cast<unsigned int>(10) //Значение, прибавляемое для получение положения ячейки, находящейся снизу от нужной
#define TO_RIGHT static_cast<unsigned int>(1) //Значение, прибавляемое для получение положения ячейки, находящейся справа от нужной
#define TO_LEFT static_cast<unsigned int>(-1) //Значение, прибавляемое для получение положения ячейки, находящейся слева от нужной
#define FILL_CELL 7 //Значение, переводящее ячейку из незаполненного состояния в заполненное, соответствующее данному

class Field : public QObject, public Cell
{
    /*
     * Класс Field - класс, являющийся абстракцией игрового поля.
     *
     * Поля класса
     * flowLastCellState - состояние ячейки, которая на данный момент является последней в потоке воды.
     *
     * flowLastCellPosition - положение ячейки, которая на данный момент является последней в потоке воды.
     *
     * position - положение выбранной ячейки, установить это значение можно с помощью setPosition(position),
     * получить - с помощью метода getPosition().
     *
     * cellArray - вектор, который хранит указатели на все CellWidget'ы игрового поля,
     * получить его значение можно с помощью метода getCells().
     *
     * Методы класса
     * Field(parent) - конструктор класса, в котором устанавливаются начальные значения полям, а также устанавливается
     * игровое поле методом setupField(parent).
     *
     * setVisible(isVisible) - метод, скрывающий все ячейки игрового поля, если isVisible == false, иначе показывает их.
     *
     * changeState(position, state) - метод, устанавливающий ячейке в положении position состояние state.
     *
     * selectCell(position) - слот, который выделяет ячейку в положении position (если она пустая)
     * и снимает выделение с предыдущей выделенной ячейки.
     *
     * prolongFlow() - слот, который продолжает поток воды в трубопроводе. Если это невозможно испускается сигнал finishGame(false)
     * Если вода попала в нижний правый угол, испускается сигнал finishGame(true).
     *
     * Сигналы
     * finishGame(success) - сигнал, сообщающий об окончании игры. Если игра окончилась победой, то success == true, если
     * поражением - success == false.
     */
    Q_OBJECT
public:
    explicit Field(QWidget *parent = nullptr);
    std::vector<CellWidget*> getCells();
    unsigned int getPosition();
    bool setPosition(unsigned int position);
    void setVisible(bool isVisible);
    void changeState(unsigned int position, Cell::State state);
public slots:
    void selectCell(unsigned int position);
    void prolongFlow();
signals:
    void finishGame(bool success);
private:
    void setupField(QWidget* parent);
    Cell::State flowLastCellState;
    unsigned int flowLastCellPosition;
    unsigned int position;
    std::vector<CellWidget*> cellArray;
};

#endif // FIELD_H
