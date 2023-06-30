
#include "stream.h"

template <typename T>
ID Common::Stream<T>::listen(function<void(T data)> observer)
{
    ID ret = currId++;
    listenerMutex.lock();
    listeners[ret] = observer;
    listenerMutex.unlock();
    return ret;
}

template <typename T>
void Common::Stream<T>::add(T data)
{
    last = data;
    listenerMutex.lock();
    for (auto &curr : this->listeners)
        curr.second(data);

    listenerMutex.unlock();

}

template <typename T>
void Common::Stream<T>::stopListen(ID id)
{
    listenerMutex.lock();
    listeners.erase(id);
    listenerMutex.unlock();;
}

template <typename T>
T Common::Stream<T>::get()
{
    return last;
}

template <typename T>
future<T> Common::Stream<T>::getNext()
{
    Common_ptr<promise<T>> prom;
        
    ID id;
    id = this->listen([&](T data){
        this->stopListen(id);
        prom->set_value(data);
    });

    return prom->get_future();
}