#include "timersForDebug.h"

map<string, __TimersForDebugInfo> TimersForDebug::timers;

void TimersForDebug::begin(string name)
{
    if (!timers.count(name))
    {
        __TimersForDebugInfo tmp;
        tmp.name = name;
        timers[name] = tmp;
    }

    timers[name].currStart = Utils::getCurrentTimeMicroseconds();
}

void TimersForDebug::end(string name)
{
    if (timers.count(name))
    {

        auto tmp = Utils::getCurrentTimeMicroseconds();
        if (timers[name].currStart != 0)
            timers[name].totalTime = tmp - timers[name].currStart;
            
        timers[name].steps++;
    }
}

__TimersForDebugInfo TimersForDebug::get(string name)
{
    if (timers.count(name))
        return timers[name];
    
    return  __TimersForDebugInfo();
}

vector<__TimersForDebugInfo> TimersForDebug::getAll()
{
    vector<__TimersForDebugInfo> result;
    for (auto &c: timers)
        result.push_back(c.second);

    return result;
}

void TimersForDebug::clear()
{
    timers.clear();
}