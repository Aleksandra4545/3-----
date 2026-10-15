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

Time& operator=(const Time& other) { // оператор присваив.
    if (this != &other) {
        totalSeconds = other.totalSeconds;
    }
    return *this;
}

// Создание времени из часов, минут и секундstatic Time fromHMS(int hours, int minutes, int seconds) {
    if (hours < 0 || minutes < 0 || minutes > 59 ||
        seconds < 0 || seconds > 59) {
        throw invalid_argument("Некорректное значение времени");
    }
    return Time(static_cast<long long>(hours) * 3600LL + minutes * 60LL + seconds);

}


