#ifndef TIME_HPP
#define TIME_HPP

#include <utility>

#include "Type.hpp"

class Time {
public:

    class TimeStep {
    public:

        TimeStep();

        TimeStep(const std::pair<Day, Period>& p, Time* par);

        inline uint toPeriods() const;

        TimeStep& operator*();
        TimeStep* operator->();

        TimeStep& operator++();

        TimeStep operator++(int);

        // friend TimeStep operator+(const TimeStep& a, const TimeStep& b) {
        //     TimeStep re(a);
        //     re._val.first += b.getDay();
        //     re._val.second += b.getPeriod();
        //     re._val.first += re._val.second / re._par->getPeriods();
        //     re._val.second = re._val.second % re._par->getPeriods();
        //     return re;
        // }

        friend uint operator-(const TimeStep& a, const TimeStep& b);

        friend bool operator==(const TimeStep& a, const TimeStep& b);
        friend bool operator!=(const TimeStep& a, const TimeStep& b);
        friend bool operator<(const TimeStep& a, const TimeStep& b);

        Day getDay() const;
        Period getPeriod() const;
        Period getPeriodLength() const;

    protected:
        Time* _par;
        std::pair<Day, Period> _val;
    };

    Time();
    Time(Day N_day, Period N_period);

    void setDay(Day n);

    void setPeriod(Period n);

    TimeStep begin();
    TimeStep end();

    Day getDays() const;
    Period getPeriods() const;
protected:
    Day _N_day;
    Period _N_period;
};

#endif