#ifndef PIPEFLOW_H
#define PIPEFLOW_H

#include "pipewidget.h"

#define PIPE_SIZE 60 //Размер трубы
#define PIPE_X_OFFSET 45 //Смещение потока труб по координате Х относительно левого верхнего угла окна программы
#define PIPE_Y_OFFSET 160 //Смещение потока труб по координате Y относительно левого верхнего угла окна программы
#define PIPE_Y_MAX 590 //Максимальное смещение трубы, при превышении которого труба удаляется
#define PIPE_MAX_COUNT 50 //Максимальное количество созданнных труб
#define SELECTION 6 //Значение, переводящее невыбранную трубу в выбранную, соответствующую данной

class PipeFlow : public QObject, public Pipe
{
    /*
     * Класс PipeFlow - класс, являющийся абстракцией потока труб.
     *
     * Поля класса
     * number - порядковый номер выбранной трубы, может быть установлен методом setNumber(number)
     * и получен методом getNumber().
     *
     * pipeArray - вектор, который хранит указатели на все PipeWidget'ы игрового поля,
     * получить его значение можно с помощью метода getPipes().
     *
     * parent - виджет, предками которого будут все трубы потока.
     *
     * Методы класса
     * PipeFlow(parent) - конструктор класса, в котором устанавливаются начальные значения полям number и parent.
     *
     * setVisible(isVisible) - метод, скрывающий все трубы, если isVisible == false, иначе показывает их.
     *
     * removePipe(number) - метод, удаляющий из потока трубу с номером number.
     *
     * selectPipe(number) - слот, который выделяет трубу с номером number
     * и снимает выделение с предыдущей выделенной трубы.
     *
     * increaseOffset() - слот, который двигает все трубы (увеличивает их смещение)
     * и удаляет трубу, вышедшую за границу (если такая есть) методом removePipe(number).
     *
     * addPipe() - слот, который добавляет новую трубу со случайным состоянием в конец потока и в pipeArray,
     * если количество труб не превышает PIPE_MAX_COUNT.
     *
     * isEmpty() - метод, проверяющий поток труб на пустоту. Возращает true если поток труб пуст, иначе возвращает false.
     *
     * Сигналы
     *
     * empty() - сигнал, посылаемый, если isEmpty() == true при удалении трубы при ее выходе за границу
     *
     */
    Q_OBJECT
public:
    explicit PipeFlow(QWidget *parent = nullptr);
    std::vector<PipeWidget*> getPipes();
    unsigned int getNumber();
    bool setNumber(unsigned int number);
    void setVisible(bool isVisible);
    void removePipe(unsigned int number);
public slots:
    void selectPipe(unsigned int number);
    void increaseOffset();
    void addPipe();
signals:
    void empty();
private:
    bool isEmpty();
    unsigned int number;
    QWidget* parent;
    std::vector<PipeWidget*> pipeArray;
};

#endif // PIPEFLOW_H
