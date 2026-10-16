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

// создание времени из часов, минут и секунд 
static Time fromHMS(int hours, int minutes, int seconds) {
    if (hours < 0 || minutes < 0 || minutes > 59 ||
        seconds < 0 || seconds > 59) {
        throw invalid_argument("Некорректное значение времени");
    }
    return Time(static_cast<long long>(hours) * 3600LL + minutes * 60LL + seconds);

}

// нестандартный деструктор
~Time() {
    --objectCount;
}

long long getSeconds() const {
    ++accessCount;
    return totalSeconds;
}

int getAccessCount() const {
    return accessCount;
}

static int getObjectCount() {
    return objectCount;
}

// формат HH:MM:SS
string toString() const {
    ++accessCount;

    const bool negative = totalSeconds < 0;
    const unsigned long long absoluteValue = negative
        ? static_cast<unsigned long long>(-(totalSeconds + 1)) + 1ULL
        : static_cast<unsigned long long>(totalSeconds);

    const unsigned long long hours = absoluteValue / 3600ULL;
    const unsigned int minutes = static_cast<unsigned int>((absoluteValue % 3600ULL) / 60ULL);
    const unsigned int seconds = static_cast<unsigned int>(absoluteValue % 60ULL);

    ostringstream out;
    if (negative) {
        out << '-';
    }
    out << setfill('0') << setw(2) << hours << ':'
        << setw(2) << minutes << ':'
        << setw(2) << seconds;
    return out.str();
}

// Префиксный инкремент
Time& operator++() {
    ++totalSeconds;
    return *this;
}

// Постфиксный инкремент
Time operator++(int) {
    Time oldValue(*this);
    ++(*this);
    return oldValue;
}

// Префиксный декремент
Time& operator--() {
    --totalSeconds;
    return *this;
}

// Постфиксный декремент
Time operator--(int) {
    Time oldValue(*this);
    --(*this);
    return oldValue;
}

}



