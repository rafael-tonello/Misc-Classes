
#include "stream.h"

template <typename T>
ID Shared::Stream<T>::listen(function<void(T data)> observer)
{
    ID ret = currId++;
    listenerMutex.lock();
    listeners[ret] = observer;
    listenerMutex.unlock();
    return ret;
}

template <typename T>
void Shared::Stream<T>::add(T data)
{
    last = data;
    listenerMutex.lock();
    for (auto &curr : this->listeners)
        curr.second(data);

    listenerMutex.unlock();

}

template <typename T>
void Shared::Stream<T>::stopListen(ID id)
{
    listenerMutex.lock();
    listeners.erase(id);
    listenerMutex.unlock();;
}

template <typename T>
T Shared::Stream<T>::get()
{
    return last;
}