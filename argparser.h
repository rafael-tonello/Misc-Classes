#include <string>
#include <vector>


using namespace std;
class ArgParser
{
private:
    vector<string> args;

    string _lastError = "";

public:
    ArgParser(){};
    ArgParser(int c, char** p, bool usesCommandAsFirst = false)
    {
        int i = usesCommandAsFirst ? 0 : 1;
        for ( ; i < c; i++)
        {
            this->args.push_back(string(p[i]));
        }
    }

    ArgParser(vector<string> args, bool usesCommandAsFirst = false)
    {
        size_t i = usesCommandAsFirst ? 0 : 1;
        size_t c = args.size();
        for ( ; i < c; i++)
        {
            this->args.push_back(args[i]);
        }
    }

    size_t size(){
        return this->args.size();
    }

    string get(int index, string defaultValue = "")
    {
        _lastError = "";
        if (index >= 0 && index < this->args.size())
            return this->args[index];

        _lastError = "Argument not found by index";
        return defaultValue;
    }

    string get(string argPrefix, string defaultValue = "")
    {
        _lastError = "";
        for (int i = 0; i < this->args.size(); i++)
        {
            auto c = this->args[i];
            if (c.find(argPrefix) == 0) // != string::npos)
            {
                if (argPrefix == c)
                {
                    //retuirn next argument 
                    if (i+1 < this->args.size())
                        return this->args[i+1];
                    else
                    {
                        this->_lastError =   "argument value is missing";
                        return defaultValue;
                    }

                }
                else if (argPrefix.size() < c.size())
                    return c.substr(argPrefix.size());
            }
        }

        this->_lastError =   "argument not found";
        return defaultValue;
    }

    

    bool contains(string argPrefix)
    {
        for (int i = 0; i < this->args.size(); i++)
        {
            auto c = this->args[i];
            if (c.find(argPrefix) == 0 /*!= string::npos*/)
            {
                return true;
            }
        }
        return false;
    }

    string getLastError(){
        return this->_lastError;
    }

};
