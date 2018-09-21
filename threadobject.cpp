#include "ThreadObject.h"
#include <QThread>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <limits>

ThreadObject::ThreadObject(QObject *parent):QObject(parent)
  ,m_runCount(10)
  ,m_runCount2(std::numeric_limits<int>::max())
  ,m_isStop(true)
{
}
ThreadObject::~ThreadObject()
{

}
void ThreadObject::setRunCount(int count)
{
    m_runCount = count;
}

void ThreadObject::runLoop()
{
//    {
//        QMutexLocker locker(&m_stopMutex);
//        m_isStop = false;
//    }
//    QElapsedTimer timer;
//    timer.start();
    Test t;
    while(1)
    {
//        {
//            QMutexLocker locker(&m_stopMutex);
//            if(m_isStop)
//                return;
//        }
        t.setup();

    }
}
void ThreadObject::stop()
{
    QMutexLocker locker(&m_stopMutex);
    m_isStop = true;
}
