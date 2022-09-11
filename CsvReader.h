#ifndef __CSVREADER__
#define __CSVREADER__
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <StringUtils.h>
#include <functional>
#include <mutex>

using namespace std;

class CsvReader{
private:
    ifstream file;
    vector<string> header;
    void parseFirstLine();
    Shared::StringUtils strUtils;
    mutex accessLock;
public:
    CsvReader(string filename);
    vector<string> getColumnNames();

    bool isEof();
    map<string, string> getNext();
    
    template <class T>
    T getNext(function<T(map<string, string> record)> modify)
    {
        return modify(this->getNext());
    }
};
#endif
