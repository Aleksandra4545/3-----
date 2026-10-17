#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdexcept>
#include <type_traits>

using namespace std;

class Time {
private:
    long long totalSeconds;
    mutable int accessCount;
    static int objectCount;

public:
    // конструктор по умолчанию
    Time() : totalSeconds(0), accessCount(0) {
        ++objectCount;
    }

    // явный конструктор из количества секунд
    explicit Time(long long seconds) : totalSeconds(seconds), accessCount(0) {
        ++objectCount;
    }

    // конструктор копирования
    Time(const Time& other)
        : totalSeconds(other.totalSeconds), accessCount(0) {
        ++objectCount;
    }

    // Оператор присваивания
    Time& operator=(const Time& other) {
        if (this != &other) {
            totalSeconds = other.totalSeconds;
        }
        return *this;
    }

    // Создание времени из часов, минут и секунд
    static Time fromHMS(int hours, int minutes, int seconds) {
        if (hours < 0 || minutes < 0 || minutes > 59 ||
            seconds < 0 || seconds > 59) {
            throw invalid_argument("Некорректное значение времени");
        }
        return Time(static_cast<long long>(hours) * 3600LL + minutes * 60LL + seconds);
    }

    // Нестандартный деструктор
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

    // Формат HH:MM:SS
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

    // Составные арифметические операторы
    Time& operator+=(const Time& other) {
        totalSeconds += other.totalSeconds;
        return *this;
    }

    Time& operator-=(const Time& other) {
        totalSeconds -= other.totalSeconds;
        return *this;
    }

    Time& operator*=(const Time& other) {
        totalSeconds *= other.totalSeconds;
        return *this;
    }

    Time& operator/=(const Time& other) {
        if (other.totalSeconds == 0) {
            throw invalid_argument("Деление на ноль");
        }
        totalSeconds /= other.totalSeconds;
        return *this;
    }

    // Бинарные операторы реализованы через составные
    friend Time operator+(Time lhs, const Time& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Time operator-(Time lhs, const Time& rhs) {
        lhs -= rhs;
        return lhs;
    }

    friend Time operator*(Time lhs, const Time& rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend Time operator/(Time lhs, const Time& rhs) {
        lhs /= rhs;
        return lhs;
    }

    // Операторы сравнения
    friend bool operator<(const Time& lhs, const Time& rhs) {
        return lhs.totalSeconds < rhs.totalSeconds;
    }

    friend bool operator>(const Time& lhs, const Time& rhs) {
        return lhs.totalSeconds > rhs.totalSeconds;
    }

    friend bool operator<=(const Time& lhs, const Time& rhs) {
        return lhs.totalSeconds <= rhs.totalSeconds;
    }

    friend bool operator>=(const Time& lhs, const Time& rhs) {
        return lhs.totalSeconds >= rhs.totalSeconds;
    }

    friend bool operator==(const Time& lhs, const Time& rhs) {
        return lhs.totalSeconds == rhs.totalSeconds;
    }

    friend bool operator!=(const Time& lhs, const Time& rhs) {
        return lhs.totalSeconds != rhs.totalSeconds;
    }
};


int Time::objectCount = 0;

class Event {
private:
    string name;
    Time start;
    Time end;
    Time plannedDuration;
    Time actualDuration;

public:
    Event(const string& eventName,
          const Time& eventStart,
          const Time& eventEnd,
          const Time& planned,
          const Time& actual)
        : name(eventName),
          start(eventStart),
          end(eventEnd),
          plannedDuration(planned),
          actualDuration(actual) {}

    void print() const {
        cout << "Событие: " << name << '\n'
             << "  Начало: " << start.toString() << '\n'
             << "  Окончание: " << end.toString() << '\n'
             << "  Планируемая длительность: " << plannedDuration.toString() << '\n'
             << "  Фактическая длительность: " << actualDuration.toString() << "\n\n";
    }
};

int main() {
    cout << boolalpha;

    cout << "=== Три конструктора ===\n";
    Time t1;
    Time t2(3600);
    Time t3(t2);
    cout << "t1 = " << t1.toString() << '\n';
    cout << "t2 = " << t2.toString() << '\n';
    cout << "t3 (копия t2) = " << t3.toString() << "\n\n";

    cout << "=== Оператор присваивания = ===\n";
    Time assigned;
    assigned = t2;
    cout << "assigned = " << assigned.toString() << "\n\n";

    cout << "=== Унарные операторы ===\n";
    Time t = Time::fromHMS(10, 0, 0);
    cout << "Исходное время: " << t.toString() << '\n';
    cout << "++t: " << (++t).toString() << '\n';
    cout << "t++ (старое значение): " << (t++).toString() << '\n';
    cout << "После t++: " << t.toString() << '\n';
    cout << "--t: " << (--t).toString() << '\n';
    cout << "t-- (старое значение): " << (t--).toString() << '\n';
    cout << "После t--: " << t.toString() << "\n\n";

    cout << "=== Бинарные операторы ===\n";
    Time a(120);
    Time b(60);
    cout << "a = " << a.toString() << ", b = " << b.toString() << '\n';
    cout << "a + b = " << (a + b).toString() << '\n';
    cout << "a - b = " << (a - b).toString() << '\n';
    cout << "a * b = " << (a * b).toString() << '\n';
    cout << "a / b = " << (a / b).toString() << "\n\n";

    cout << "=== Составные операторы ===\n";
    Time c(120);
    c += b;
    cout << "После c += b: " << c.toString() << '\n';
    c -= b;
    cout << "После c -= b: " << c.toString() << '\n';
    c *= b;
    cout << "После c *= b: " << c.toString() << '\n';
    c /= b;
    cout << "После c /= b: " << c.toString() << "\n\n";

    cout << "=== Операторы сравнения ===\n";
    cout << "a < b:  " << (a < b) << '\n';
    cout << "a > b:  " << (a > b) << '\n';
    cout << "a <= b: " << (a <= b) << '\n';
    cout << "a >= b: " << (a >= b) << '\n';
    cout << "a == b: " << (a == b) << '\n';
    cout << "a != b: " << (a != b) << "\n\n";

    cout << "=== Проверка explicit ===\n";
    static_assert(!is_convertible<int, Time>::value,
                  "Конструктор должен запрещать неявное преобразование");
    Time explicitTime(7200);
    Time explicitTime2 = static_cast<Time>(3600);
    // Time invalid = 3600; // Ошибка компиляции: запрещено explicit.
    cout << "Прямое создание: " << explicitTime.toString() << '\n';
    cout << "Явное преобразование: " << explicitTime2.toString() << "\n\n";

    cout << "=== const и mutable ===\n";
    const Time constTime = Time::fromHMS(12, 30, 0);
    cout << "Время: " << constTime.toString() << '\n';
    cout << "Обращений после toString(): " << constTime.getAccessCount() << '\n';
    (void)constTime.getSeconds();
    cout << "Обращений после getSeconds(): " << constTime.getAccessCount() << "\n\n";

    cout << "=== Расписание на день ===\n";
    Event lecture("Лекция",
                  Time::fromHMS(9, 0, 0), Time::fromHMS(10, 30, 0),
                  Time::fromHMS(1, 30, 0), Time::fromHMS(1, 25, 0));
    Event lunch("Обед",
                 Time::fromHMS(13, 0, 0), Time::fromHMS(13, 45, 0),
                 Time::fromHMS(0, 45, 0), Time::fromHMS(0, 50, 0));
    Event mlStudy("Изучение ML",
                  Time::fromHMS(17, 0, 0), Time::fromHMS(19, 0, 0),
                  Time::fromHMS(2, 0, 0), Time::fromHMS(2, 0, 0));
    lecture.print();
    lunch.print();
    mlStudy.print();

    cout << "=== Статическое поле ===\n";
    cout << "Количество существующих объектов Time: "
         << Time::getObjectCount() << "\n\n";

    cout << "=== Проверка деления на ноль ===\n";
    try {
        Time zero(0);
        Time result = a / zero;
        cout << result.toString() << '\n';
    } catch (const invalid_argument& error) {
        cout << "Ошибка: " << error.what() << '\n';
    }

    return 0;
}
