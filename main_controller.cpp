#include "main_controller.h"
#include <QCoreApplication>

CountsModel MainController::s_model;

MainController::MainController(QObject *parent)
    : QObject{parent}
    , m_thread(&MainController::modelUpdate, this)
{
    connect(this, &MainController::updated, this, &MainController::process);
}

MainController::~MainController()
{
    isWork = false;
    cv.notify_one();

    if(m_thread.joinable()) {
        m_thread.join();
    }
}

CountsModel &MainController::model()
{
    return s_model;
}

void MainController::process()
{
    std::unique_lock lock(s_model.mutex());
    isUpdated = false;
    s_model.reset();
    cv.notify_one();
}

int MainController::rate() const
{
    return s_model.rate();
}

void MainController::modelUpdate()
{
    while(true) {
        std::unique_lock lock(s_model.mutex());
        cv.wait(lock, [this](){return !isUpdated || !isWork;});
        s_model.updateData();
        isUpdated = true;

        if(!isWork)
            break;

        static int st_rate = 0;
        if(st_rate != s_model.rate()) {
            st_rate = s_model.rate();
            emit rateChanged();
        }

        emit updated();
    }
}
