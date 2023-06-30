#ifndef __NRANGE_H__
#define __NRANGE_H__

/*A class for define number ranges
    Example:
        NRange<int, 1, 10> a; //raise exceptions and defualt value if same of MIN (in this case, 1)
        NRange<int, 1, 10, false> a; //do not raise exceptions and defualt value if same of MIN (in this case, 1)
        NRange<int, 1, 10, true, 5> a; //exceptions and defualt value is 5)
        NRange<int, 1, 10, false, 5> a; //do not raise exceptions and defualt value is 5)
    
    if a value out of the specified range is ifnormed, a runtime_error will be raised or a false will 
    be retuned in set function (and value of the object will no be changed). The default behaviour is
    to raise an exception
*/

template <typename T, T MIN, T MAX, bool RAISE_EXPETION = true, T Def = MIN>
class NRange{
private:
    T _value = Def;
public:
    NRange(){}
    NRange(T value)
    {
        this->set(value);
    }
    
    bool set(T value)
    {
        if (value < MIN || value > MAX)
        {
            if (RAISE_EXPETION)
                throw runtime_error("Value is out of the range");
            else
                return false;
        }
        
        _value = value;
        return true;
    }
    
    T get()
    {
        return _value;
    }
    
    operator T(){ return get(); }
    
};
#endif
