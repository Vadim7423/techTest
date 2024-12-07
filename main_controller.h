#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "counts_model.h"
#include <thread>
#include <condition_variable>

class MainController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rate READ rate NOTIFY rateChanged FINAL)
public:
    explicit MainController(QObject *parent = nullptr);
    ~MainController();

    /** @brief Модель, хранящая список счетчиков, наследуется от @ref QAbstractListModel,
        отвечает за передачу данных счетчиков а Qml. Статичность метода позволяет
        вызывать ее не зависимо от текущего объекта - в частности в @ref main.cpp
        наглядный пример того, почему это сделано так. */
    static CountsModel& model();

    /** @brief Описана логика после обновления счетчиков. */
    void process();

signals:
    /** @brief Сигнал об обновлении частоты обхода счетчиков. */
    void rateChanged();
    /** @brief Сигнал об обновлении состояний счетчиков. */
    void updated();

private:
    int rate() const;
    /** @brief Метод, отвечающий за обновление счетчиков. Выполняется в отдельном потоке. */
    void modelUpdate();
    std::thread m_thread;
    bool isUpdated = false; // Переменная для сообщения потоку m_thread, что необходимо обновить счетчики.
    std::atomic_bool isWork = true; // Переменная для сообщения потоку m_thread, что необходимо остановить цикл.
    std::condition_variable cv;
    static CountsModel s_model;

};

#endif // MAINCONTROLLER_H
