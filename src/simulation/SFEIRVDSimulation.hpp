#ifndef SFEIRVDSIMULATION_HPP
#define SFEIRVDSIMULATION_HPP

#include <set>

//#define DETAIL

#include "SFEIRVDModel.hpp"

class SFEIRVDSimulation : public SFEIRVDModel {
public:
    void setOutFile(const std::string& fname) {
        fout.open(fname);
        if (!fout) {
            std::cout << "fail open file\n";
        }
    }

protected:
    virtual void statisticInit() override {
        //std::cout << "statistic init\n";
        allcnt.resize(N_lc);
        icnt.resize(N_lc);
        dcnt.resize(N_lc);
        for (auto& v : ndp) {
            ++allcnt[v.loc];
            if (v.stateID == 'I') ++icnt[v.loc];
        }

        #ifdef DETAIL
        for (auto& v : ndp) {
            if (v.stateID == 'I') {
                I.insert(v.id);
            }
            else {
                S.insert(v.id);
            }
        }
        #endif

        showHeader();
    }

    virtual void statisticUnit(const Time::TimeStep& ts, const Nodes& s2f, const Nodes& s2v, const Nodes& s2e, const Nodes& f2e, const Nodes& e2i, const Nodes& i2f, const Nodes& i2r, const Nodes& i2d) override {
        //std::cout << "statistic unit\n";
        for (const auto& v : i2f) {
            --icnt[v.getLocation()];
        }
        for (const auto& v : i2r) {
            --icnt[v.getLocation()];
        }
        for (const auto& v : i2d) {
            --icnt[v.getLocation()];
            ++dcnt[v.getLocation()];
        }
        for (const auto& v : e2i) {
            ++icnt[v.getLocation()];
        }
        show(ts);

        #ifdef DETAIL
        for (auto v : s2f) {
            S.erase(v.getID());
            F.insert(v.getID());
        }
        for (auto v : s2v) {
            S.erase(v.getID());
            V.insert(v.getID());
        }
        for (auto v : s2e) {
            S.erase(v.getID());
            E.insert(v.getID());
        }
        for (auto v : f2e) {
            F.erase(v.getID());
            E.insert(v.getID());
        }
        for (auto v : e2i) {
            E.erase(v.getID());
            I.insert(v.getID());
        }
        for (auto v : i2f) {
            I.erase(v.getID());
            F.insert(v.getID());
        }
        for (auto v : i2r) {
            I.erase(v.getID());
            R.insert(v.getID());
        }
        for (auto v : i2d) {
            I.erase(v.getID());
            D.insert(v.getID());
        }

        showNodes('S', S);
        showNodes('F', F);
        showNodes('E', E);
        showNodes('I', I);
        showNodes('V', V);
        showNodes('R', R);
        showNodes('D', D);
        #endif
    }

    virtual void statisticEnd() override {
        //std::cout << "statistic end\n";
        fout.close();
    }

    void showHeader() {
        fout << "\"Period\",\"Town\",\"num_infection\",\"num_dead\",\"ratio_infection\",\"ratio_dead\"\n";
    }

    void show(const Time::TimeStep& ts) {
        uint period = ts.getDay() * ts.getPeriodLength() + ts.getPeriod();
        for (uint i = 0; i < N_lc; ++i) {
            double iratio = icnt[i] / (double)allcnt[i];
            double dratio = dcnt[i] / (double)allcnt[i];
            fout << period << ',' << i << ',' << icnt[i] << ',' << dcnt[i] << ',' << iratio << ',' << dratio << '\n';
        }
    }

    #ifdef DETAIL
    void showNodes(char c, std::set<int>& st) {
        std::cout << c << ": ";
        for (auto v : st) {
            std::cout << v << ' ';
        }
        std::cout << '\n';
    }
    #endif

    #ifdef DETAIL
    std::set<int> S, F, E, I, R, V, D;
    #endif
    
    std::ofstream fout;
    std::vector<uint> allcnt, icnt, dcnt;
};

#endif
