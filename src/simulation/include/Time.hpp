#ifndef TIME_HPP
#define TIME_HPP

#include <utility>

#include "Type.hpp"

class Time {
public:

    class TimeStep {
    public:

        TimeStep() = default;

        TimeStep(const std::pair<Day, Period>& p, Time* par) {
            _val = p;
            _par = par;
        }

        TimeStep& operator*() { return *this; }
        TimeStep* operator->() { return this; }

        TimeStep& operator++() {
            if (getPeriod() == _par->getPeriods() - 1) {
                this->_val = {getDay() + 1, 0};
            }
            else {
                ++this->_val.second;
            }
            return *this;
        }

        TimeStep operator++(int) {
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

        friend bool operator==(const TimeStep& a, const TimeStep& b) { return a._val == b._val; }
        friend bool operator!=(const TimeStep& a, const TimeStep& b) { return a._val != b._val; }
        friend bool operator<(const TimeStep& a, const TimeStep& b) {
            return (a.getDay() == b.getDay())? a.getPeriod() < b.getPeriod() : a.getDay() < b.getDay(); 
        }

        Day getDay() const { return _val.first; }
        Period getPeriod() const { return _val.second; }
        Period getPeriodLength() const { return _par->getPeriods(); }

    protected:
        Time* _par;
        std::pair<Day, Period> _val;
    };

    Time() = default;
    Time(Day N_day, Period N_period) {
        _N_day = N_day;
        _N_period = N_period;
    }

    void setDay(Day n) {
#ifdef DEBUG
        LOG("--set days")
#endif
        _N_day = n;
    }

    void setPeriod(Period n) {
        _N_period = n;
    }

    TimeStep begin() { return TimeStep({0, 0}, this); }
    TimeStep end() { return TimeStep({getDays(), 0}, this); }

    Day getDays() const { return _N_day; }
    Period getPeriods() const { return _N_period; }
protected:
    Day _N_day;
    Period _N_period;
};

#endif