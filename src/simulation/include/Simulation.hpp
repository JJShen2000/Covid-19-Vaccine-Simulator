#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <unordered_map>
#include <ios>

#include "ExpiringState.hpp"
#include "BaseModel.hpp"
#include "Heap.hpp"

class Simulation : public BaseModel {
public:
    void loadGraph(std::istream& in);

    void loadParam(std::istream& in);

    void loadVaccinationStrat(std::istream& in);

    void loadInitInfector(std::istream& in);

    void simulate();
protected:

    using Nodes = std::vector<uint>;

    struct Transition {
        Nodes s2v, v2w;
        Nodes s2e, v2e, w2e, f2e;
        Nodes e2i, i2j, i2k;
        Nodes j2f, j2r, k2r, k2d, k2f;
    };

    // vaccine ineffectiveness
    double epsilon_bar(char state, uint time);

    void infection(ExpiringState& ext, double tau, const Time::TimeStep& ts, Nodes& s2e, Nodes& v2e, Nodes& w2e, Nodes& f2e);

    void partitionGroup(const Nodes& vorg, Nodes& v1, Nodes& v2, double p1);

    void partitionGroupAge(const Nodes& vorg, Nodes& v1, Nodes& v2, const std::vector<double>& p1);

    Nodes vaccination(Nodes& s2v, Nodes& v2w);

    void simulate_unit(const Time::TimeStep& ts);

    // statistic method
    virtual void statisticInit() = 0;
    virtual void statisticUnit(const Time::TimeStep&, const Transition&) = 0;
    virtual void statisticEnd() = 0;
    
    // state node container
    ExpiringState E, I_pre, I_asym, I_sym;

    // duration of a state
    double gamma_E, gamma_I_pre, gamma_I_asym, gamma_I_sym;

    // relative infectiousness
    double tau_I_pre, tau_I_asym, tau_I_sym;

    // ratio of I_asym from I_pre
    double sigma_asym;

    // vaccine efficiency
    std::vector<double> epsilon_V1;
    std::vector<double> epsilon_V2;
    // priority queue to determine who to vaccinate
    heap<double, uint> vacc_queue;
    // vaccination rollout
    uint vacc_rollout;

    // transmission rate
    std::vector<double> prob_trans;

    // fatality rate of I_sym
    std::vector<double> prob_death_sym;

    // immune rate of I_asym
    std::vector<double> prob_immune_asym;
    std::vector<double> prob_immune_sym;
    std::vector<double> prob_immune_sym_cond_nd;
};

#endif