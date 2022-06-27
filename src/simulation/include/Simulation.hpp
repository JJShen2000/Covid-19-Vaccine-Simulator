#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <unordered_map>
#include <ios>

#include "ExpiringState.hpp"
#include "BaseModel.hpp"
#include "Heap.hpp"

#include <omp.h>

class BaseVaccStrat;

class Simulation : public BaseModel {
public:

    using Nodes = std::vector<uint>;
    using Dictionary = std::unordered_map<std::string, std::vector<double>>;

    class BaseVaccStrat {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp);
        virtual void vaccinate(const Simulation& sim, const Time::TimeStep& ts, Nodes& s2v, Nodes& v2w) = 0;
    protected:
        uint vacc_rollout;
        uint vacc_start_day;
        uint vacc_sec_start_day;
    };

    class VaccStratInfectnessBase : public BaseVaccStrat {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp) override;
        virtual void vaccinate(const Simulation& sim, const Time::TimeStep& ts, Nodes& s2v, Nodes& v2w) override;
    protected:
        virtual void updateScore(const Simulation& sim, const Time::TimeStep& ts) = 0;
        void increaseScore(uint u, double tau, const Simulation& sim, const Time::TimeStep& ts);
        virtual double infect_prob(uint u, uint v, const ContactGroup& cgp, double tau, const Time::TimeStep& ts, const Simulation& sim);
        double epsilon_bar(char state, uint time, const Simulation& sim);
        
        std::vector<double> score;
    };

    class VaccStratInfectiousness : public VaccStratInfectnessBase {
    public:
        virtual void updateScore(const Simulation& sim, const Time::TimeStep& ts) override;
    };

    class VaccStratMortality : public VaccStratInfectiousness {
    public:
        virtual void updateScore(const Simulation& sim, const Time::TimeStep& ts) override;
    };

    class VaccStratYLL : public VaccStratInfectiousness {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp) override;
        virtual void updateScore(const Simulation& sim, const Time::TimeStep& ts) override;
    protected:
        std::vector<double> yll;
    };

    class VaccStratInfectiousnessSym : public VaccStratInfectnessBase {
    public:
        virtual void updateScore(const Simulation& sim, const Time::TimeStep& ts) override;
    };

    class VaccStratInfectiousnessSymBias : VaccStratInfectiousnessSym {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp) override;
        virtual double infect_prob(uint u, uint v, const ContactGroup& cgp, double tau, const Time::TimeStep& ts, const Simulation& sim) override;
    protected:
        double bias;
    };

    class VaccStratMortalitySym : public VaccStratInfectiousnessSym {
    public:
        virtual void updateScore(const Simulation& sim, const Time::TimeStep& ts) override;
    };

    class VaccStratYLLSym : public VaccStratInfectiousnessSym {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp) override;
        virtual void updateScore(const Simulation& sim, const Time::TimeStep& ts) override;
    protected:
        std::vector<double> yll;
    };

    class VaccStratOrderBase : public BaseVaccStrat {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp) override;
        virtual void vaccinate(const Simulation& sim, const Time::TimeStep& ts, Nodes& s2v, Nodes& v2w) override;
    protected:
        std::vector<uint> order;
        uint head = 0;
    };

    class VaccStratAgeFirst : public VaccStratOrderBase {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp) override;
    };

    class VaccStratLocationFirst : public VaccStratOrderBase {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp) override;
    };

    class VaccStratRandom : public VaccStratOrderBase {
    public:
        virtual void init(const Simulation& sim, Dictionary& mp) override;
    };

    class VaccStratFactory {
    public:
        static Simulation::BaseVaccStrat* read(istream& in);
    };

    void loadGraph(std::istream& in);

    void loadParam(std::istream& in);

    void loadVaccinationStrat(std::istream& in);

    void loadInitInfector(std::istream& in);

    void simulate();

    friend class VaccStratInfectnessBase;
protected:

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


    void simulate_unit(const Time::TimeStep& ts);
    double infect_prob(uint u, uint v, const ContactGroup& cgp, double tau, const Time::TimeStep& ts);

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
    std::vector<double> epsilon_V1_bar;
    std::vector<double> epsilon_V2_bar;
    // priority queue to determine who to vaccinate
    // heap<double, uint> vacc_queue;

    // transmission rate
    std::vector<double> prob_trans;

    // fatality rate of I_sym
    std::vector<double> prob_death_sym;

    // immune rate of I_asym
    std::vector<double> prob_immune_asym;
    std::vector<double> prob_immune_sym;
    std::vector<double> prob_immune_sym_cond_nd;

    BaseVaccStrat* vacc_strat;

    uint infect_start_day;
};

#endif