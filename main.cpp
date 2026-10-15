#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdexcept>
#include <type_traits>

using namespace std;

class Time {
private:                      // конструк. по умолчан.
    long long totalSeconds;
    mutable int accessCount;
    static int objectCount;
public:
    Time() : totalSeconds(0), accessCount(0)
{
    ++objectCount;
    }
      // явн. конструкт. из кол-ва секунд
    explicit Time(long long seconds) : totalSeconds(seconds), accessCount(0) {
    ++objectCount;
}
                    
Time(const Time& other)     // конструк. копир.
    : totalSeconds(other.totalSeconds), accessCount(0) {
    ++objectCount;
}



}
