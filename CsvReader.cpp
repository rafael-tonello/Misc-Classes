#include "CsvReader.h"


CsvReader::CsvReader(string filename)
{
    file = ifstream(filename);
    auto r = file.is_open();
    parseFirstLine();
}

vector<string> CsvReader::getColumnNames()
{
    return header;
}

bool CsvReader::isEof()
{
    accessLock.lock();
    bool ret = file.peek() == EOF;
    accessLock.unlock();
    return ret;

}

map<string, string> CsvReader::getNext()
{
    map<string, string> result = {};
    if (!isEof())
    {
        accessLock.lock();
        uint extraColumnCount = 0;
        string line;
        std::getline(this->file, line);
        auto values = strUtils.split(line, ";");
        for (size_t c = 0; c < values.size(); c++)
        {
            string columnName = c < header.size() ? header[c] : ("extra_column" + extraColumnCount++);
            result[columnName] = values[c];
        }

        accessLock.unlock();
    }

    return result;
}

void CsvReader::parseFirstLine(){
    if (!isEof())
    {
        string line;
        std::getline(this->file, line);
        header = strUtils.split(line, ";");
    }
}