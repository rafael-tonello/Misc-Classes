#ifndef __OBSERVABLELIST_H__
#define __OBSERVABLELIST_H__
#include <vector>
#include <functional>

using namespace std;

template <typename T>
class EventStream{
private:
    vector<function<void(T)>> list;
public:

    void listen(function<void(T)> observer)
    {
        list.push_back(observer);
    }

    void stream(T item)
    {
        for(auto &c: list)
            c(item);
    }
};

template <typename T>
class EventStream_Stateful{
private:
    vector<function<void(T)>> list;

    T lastData;
public:

    EventStream_Stateful(){}
    EventStream_Stateful(T initialState)
    {
        lastData = initialState;
    }

    void listen(function<void(T)> observer, bool callWithLastStreamedData = true)
    {
        list.push_back(observer);
        observer(lastData);
    }

    void stream(T item)
    {
        for(auto &c: list)
            c(item);
        lastData = item;
    }

    T getLastStreamedData()
    {
        return lastData;
    }
};

#endif
