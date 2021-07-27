#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Собери трубопровод");

    //Создаем игровое поле и поток труб, скрываем их
    interval = 0;
    field = new Field(this);
    pipeflow = new PipeFlow(this);
    ui->frame_pipeflow->hide();
    field->setVisible(false);
    pipeflow->setVisible(false);

    //Соединяем сигнал игрового поля о завершении игры со слотом MainWindow
    connect(field, SIGNAL(finishGame(bool)), this, SLOT(finishGame(bool)));
    connect(pipeflow, SIGNAL(empty()), this, SLOT(checkStart()));
    //Скрываем стартовый и финишный котлы, выбираем простой уровень сложности, скрываем лейблы с информацией о времени до
    //следующего течения воды
    ui->widget_cauldron_start->hide();
    ui->widget_cauldron_finish->hide();
    ui->label_time_txt->hide();
    ui->label_time->hide();
    ui->radioButton_easy->click();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tryJoin()
{
    /*
     * Функция размещения трубы в ячейку
     *
     * Если существует выделенная ячейка поля и выделенная труба в потоке труб, то изменяем состояние выделенной
     * ячейки и удаляем трубу из потока.
     */

    std::vector<CellWidget*> cellArray = field->getCells();
    std::vector<PipeWidget*> pipeArray = pipeflow->getPipes();

    if ((cellArray[field->getPosition()] != nullptr) && (pipeArray[pipeflow->getNumber()] != nullptr))
    {
        //Проверка налаичия выделенной трубы и ячейки
        if ((cellArray[field->getPosition()]->getState() == Cell::Selected) &&
                (pipeArray[pipeflow->getNumber()]->getState() >= Pipe::VerticalSelected))
        {
            if (!prolongFlow->isActive())
            {
                //Запускаем течение воды если оно еще не запущено в зависимости от выбранного уровня сложности
                ui->widget_cauldron_start->changeState();
                if (ui->radioButton_easy->isChecked())
                {
                    //Для простого уровня сложности интервал течения воды - 6 секунд
                    prolongFlow->start(6000);
                }

                if (ui->radioButton_normal->isChecked())
                {
                    //Для нормального уровня сложности интервал течения воды - 4 секунды
                    prolongFlow->start(4000);
                }

                if (ui->radioButton_hard->isChecked())
                {
                    //Для сложного уровня сложности интервал течения воды - 2 секунды
                    prolongFlow->start(2000);
                }
                //Изначально обновляем время
                updateTime();
                //Соединяем сигнал течения воды со слотом обновления времени
                connect(prolongFlow, SIGNAL(timeout()), this, SLOT(updateTime()));
                //Показываем лейблы с информацией о времени до следующего течения воды
                ui->label_time_txt->show();
                ui->label_time->show();
            }

            //Смена состояния ячейки в зависимости от состояния трубы
            switch (pipeArray[pipeflow->getNumber()]->getState())
            {
            case (Pipe::VerticalSelected):
            {
                field->changeState(field->getPosition(), Cell::Vertical);
                break;
            }
            case (Pipe::HorizontalSelected):
            {
                field->changeState(field->getPosition(), Cell::Horizontal);
                break;
            }
            case (Pipe::Corner1Selected):
            {
                field->changeState(field->getPosition(), Cell::Corner1);
                break;
            }
            case (Pipe::Corner2Selected):
            {
                field->changeState(field->getPosition(), Cell::Corner2);
                break;
            }
            case (Pipe::Corner3Selected):
            {
                field->changeState(field->getPosition(), Cell::Corner3);
                break;
            }
            case (Pipe::Corner4Selected):
            {
                field->changeState(field->getPosition(), Cell::Corner4);
                break;
            }
            }
            //Удаление трубы
            pipeflow->removePipe(pipeflow->getNumber());
        }
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    //Стартовое сообщение
    QMessageBox* startMessage = new QMessageBox(QMessageBox::Information,
                                                "Начало игры",
                                                "Игра \"Собери трубопровод\"\nВ игре нужно провести трубопровод "
                                                "от верхнего левого угла в правый нижний. В это время по трубе течет вода,"
                                                " если не успеть продолжить трубопровод, игра будет закончена.");
    startMessage->exec();
    //Старт игрового цикла
    startGameCycle();
    //Показ игрового поля, потока труб, скрытие кнопки "Старт"
    field->setVisible(true);
    pipeflow->setVisible(true);
    ui->frame_pipeflow->show();
    ui->pushButton_start->hide();
    ui->groupBox_level->hide();
}

void MainWindow::startGameCycle()
{
    //Создание таймеров
    addPipe = new QTimer();
    movePipes = new QTimer();
    prolongFlow = new QTimer();

    //Установка начальных состояний котлам, их показ
    ui->widget_cauldron_start->setState(0);
    ui->widget_cauldron_finish->setState(2);
    ui->widget_cauldron_start->show();
    ui->widget_cauldron_finish->show();

    //Соединение сигналов таймеров со слотами игрового поля, поткоа труб, котла и самого MainWindow
    connect(addPipe, SIGNAL(timeout()), pipeflow, SLOT(addPipe()));
    connect(addPipe, SIGNAL(timeout()), this, SLOT(decreaseTime()));
    connect(movePipes, SIGNAL(timeout()), pipeflow, SLOT(increaseOffset()));
    connect(prolongFlow, SIGNAL(timeout()), field, SLOT(prolongFlow()));
    connect(prolongFlow, SIGNAL(timeout()), ui->widget_cauldron_start, SLOT(changeState()));

    //Запускаем таймеры и сразу же добавляем трубу
    pipeflow->addPipe();
    addPipe->start(1000);
    movePipes->start(125);
}

void MainWindow::finishGame(bool success)
{
    //Завершение игрового цикла, разъединение сигнала об окончании игры, остановка таймеров
    disconnect(field, SIGNAL(finishGame(bool)), this, SLOT(finishGame(bool)));
    addPipe->stop();
    movePipes->stop();
    prolongFlow->stop();

    if (success)
    {
        //В случае победы запускаем анимацию заливания воды в финальный котел
        connect(prolongFlow, SIGNAL(timeout()), ui->widget_cauldron_finish, SLOT(changeState()));
        prolongFlow->start(500);
        QMessageBox::information(this, "Победа!", "Вы победили! Вода дошла до места назначения");
    }
    else
    {
        QMessageBox::information(this, "Поражение :(", "Вы проиграли :( Вода не дошла до места назначения");
    }

    if (prolongFlow->isActive())
    {
        //Останавливаем таймер, если он был запущен снова (в случае победы)
        prolongFlow->stop();
    }
    //Закрываем окно
    this->close();
}

void MainWindow::updateTime()
{
    if (ui->radioButton_easy->isChecked())
    {
        //Для простого уровня сложности интервал течения воды - 6 секунд
        interval = 6;
    }

    if (ui->radioButton_normal->isChecked())
    {
        //Для нормального уровня сложности интервал течения воды - 4 секунды
        interval = 4;
    }

    if (ui->radioButton_hard->isChecked())
    {
        //Для сложного уровня сложности интервал течения воды - 2 секунды
        interval = 2;
    }
    ui->label_time->setText(QString::number(interval));
}

void MainWindow::decreaseTime()
{
    if (interval > 0)
    {
        //Если интервал положительный, уменьшаем его и записываем в лейбл
        interval--;
        ui->label_time->setText(QString::number(interval));
    }
}

void MainWindow::checkStart()
{
    if (!prolongFlow->isActive() && addPipe->isActive() && movePipes->isActive())
    {
        //Если игровой цикл запущен, но трубы не были установлены (таймер prolongFlow не запущен),
        //завершаем игру поражением, т.к. трубы в потоке закончились.
        finishGame(false);
    }
}
