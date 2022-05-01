#include "Time.hpp"
#include "Type.hpp"

Time::TimeStep::TimeStep() {}

Time::TimeStep::TimeStep(const std::pair<Day, Period>& p, Time* par) {
    _val = p;
    _par = par;
}

inline uint Time::TimeStep::toPeriods() const {
    return _val.first * _par->getPeriods() + _val.second;
}

Time::TimeStep& Time::TimeStep::operator*() { return *this; }
Time::TimeStep* Time::TimeStep::operator->() { return this; }

Time::TimeStep& Time::TimeStep::operator++() {
    if (getPeriod() == _par->getPeriods() - 1) {
        this->_val = {getDay() + 1, 0};
    }
    else {
        ++this->_val.second;
    }
    return *this;
}

Time::TimeStep Time::TimeStep::operator++(int) {
    TimeStep tmp = *this;
    ++(*this);
    return tmp;
}

        // friend TimeStep operator+(const TimeStep& a, const TimeStep& b) {
        //     TimeStep re(a);
        //     re._val.first += b.getDay();
        //     re._val.second += b.getPeriod();
        //     re._val.first += re._val.second / re._par->getPeriods();
        //     re._val.second = re._val.second % re._par->getPeriods();
        //     return re;
        // }

uint operator-(const Time::TimeStep& a, const Time::TimeStep& b) { 
    return a.toPeriods() - b.toPeriods();
}

bool operator==(const Time::TimeStep& a, const Time::TimeStep& b) { return a._val == b._val; }
bool operator!=(const Time::TimeStep& a, const Time::TimeStep& b) { return a._val != b._val; }
bool operator<(const Time::TimeStep& a, const Time::TimeStep& b) {
    return (a.getDay() == b.getDay())? a.getPeriod() < b.getPeriod() : a.getDay() < b.getDay(); 
}

Day Time::TimeStep::getDay() const { return _val.first; }
Period Time::TimeStep::getPeriod() const { return _val.second; }
Period Time::TimeStep::getPeriodLength() const { return _par->getPeriods(); }



Time::Time() = default;
Time::Time(Day N_day, Period N_period) {
    _N_day = N_day;
    _N_period = N_period;
}

void Time::setDay(Day n) {
#ifdef DEBUG
    LOG("--set days")
#endif
    _N_day = n;
}

void Time::setPeriod(Period n) {
    _N_period = n;
}

Time::TimeStep Time::begin() { return TimeStep({0, 0}, this); }
Time::TimeStep Time::end() { return TimeStep({getDays(), 0}, this); }

Day Time::getDays() const { return _N_day; }
Period Time::getPeriods() const { return _N_period; }