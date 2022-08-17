#ifndef STATISTICSAGEDGEOSTATEPOPULATION_HPP
#define STATISTICSAGEDGEOSTATEPOPULATION_HPP

#include <ios>
#include <string>

#include "Simulation.hpp"

class StatisticsAgedGeoStatePopulation : public Simulation {
public:
    enum class TimeOpt {
        End, Daily, Periodic
    };

    enum class AgeOpt {
        Sum, Each
    };

    enum class TownOpt {
        Sum, Each
    };

    enum class StateCntOpt {
        All, S, V, W, E, I, J, K, F, R, D, NState
    };

    void setOutStream(std::ostream* out);
    
    void setOutputSetting(std::string& setting);

protected:

    virtual void statisticInit() override;

    virtual void statisticUnit(const Time::TimeStep& ts, const Transition& trans) override;

    virtual void statisticEnd() override;

    void update(const Transition& trans);

    void transition(const Nodes& nodes, std::vector<std::vector<uint>>& src, std::vector<std::vector<uint>>& des, std::vector<std::vector<uint>>& desnew);

    void writeHeader();

    void writeLine(const Time::TimeStep& ts);

    void writeTerm(uint town, uint age, bool& adddot, std::vector<std::vector<uint>>& cnt, std::vector<std::vector<uint>>& nnew);

    std::vector<std::string> column_name  = {
        "All", 
        "Susceptible",
        "Vaccinated First",
        "Vaccinated Second",
        "Exposed",
        "Infectious Presymptomatic",
        "Infectious Asymptomatic",
        "Infectious Symptomatic",
        "Failed",
        "Recovered",
        "Dead"
    };

    TimeOpt time_opt;
    AgeOpt age_opt;
    TownOpt town_opt;
    int state_cnt_opt;

    std::ostream* out;

    std::vector<std::vector<uint>> allcnt, scnt, vcnt, ecnt, icnt, jcnt, kcnt, fcnt, rcnt, dcnt;
    std::vector<std::vector<uint>> allnew, snew, vnew, enew, inew, jnew, knew, fnew, rnew, dnew;
    // std::vector<std::vector<uint>> wcnt, wnew;
};  

#endif