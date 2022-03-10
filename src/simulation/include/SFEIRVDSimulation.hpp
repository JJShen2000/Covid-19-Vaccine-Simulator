#ifndef SFEIRVDSIMULATION_HPP
#define SFEIRVDSIMULATION_HPP

#include <set>

//#define DETAIL
//#define SHOW
// #define CHANGES

#include "SFEIRVDModel.hpp"

class SFEIRVDSimulation : public SFEIRVDModel {
public:
    void setStateOutFile(const std::string& fname) {
        fstate.open(fname);
        if (!fstate) {
            std::cout << "fail open file\n";
        }
    }

    void setAnalOutFile(const std::string& fname) {
        fanal.open(fname);
        if (!fanal) {
            std::cout << "fail open file\n";
        }
    }

protected:
    virtual void statisticInit() override {
        allcnt.resize(N_lc, std::vector<uint>(N_ag));
        scnt.resize(N_lc, std::vector<uint>(N_ag));
        fcnt.resize(N_lc, std::vector<uint>(N_ag));
        ecnt.resize(N_lc, std::vector<uint>(N_ag));
        icnt.resize(N_lc, std::vector<uint>(N_ag));
        rcnt.resize(N_lc, std::vector<uint>(N_ag));
        vcnt.resize(N_lc, std::vector<uint>(N_ag));
        dcnt.resize(N_lc, std::vector<uint>(N_ag));

        accum_icnt.resize(N_lc, std::vector<uint>(N_ag));
        accum_vcnt.resize(N_lc, std::vector<uint>(N_ag));
        have_infected.resize(N_nd);

        for (auto& v : ndp) {
            ++allcnt[v.loc][v.age];
            if (v.stateID == 'I') {
                ++icnt[v.loc][v.age];
                ++accum_icnt[v.loc][v.age];
                have_infected[v.id] = 1;
            }
            else ++scnt[v.loc][v.age];
        }

        #ifdef SHOW
        icnts = scnts = fcnts = rcnts = ecnts = vcnts = dcnts = 0;
        for (auto& v : ndp) {
            if (v.stateID == 'I') {
                ++icnts;
            }
            else {
                ++scnts;
            }
        }
        #endif

        writeHeader();

        for (uint i = 0; i < N_lc; ++i) {
            for (uint j = 0; j < N_ag; ++j) {
                double sratio = scnt[i][j] / (double)allcnt[i][j];
                double fratio = fcnt[i][j] / (double)allcnt[i][j];
                double eratio = ecnt[i][j] / (double)allcnt[i][j];
                double iratio = icnt[i][j] / (double)allcnt[i][j];
                double rratio = rcnt[i][j] / (double)allcnt[i][j];
                double vratio = vcnt[i][j] / (double)allcnt[i][j];
                double dratio = dcnt[i][j] / (double)allcnt[i][j];
                fstate << 0 << ',' << i << ',' << j << ',';
                fstate << scnt[i][j] << ',' << fcnt[i][j] << ',' << ecnt[i][j] << ',' << icnt[i][j] << ',' << rcnt[i][j] << ',' << vcnt[i][j] << ',' << dcnt[i][j] << ',';
                fstate << sratio  << ',' << fratio  << ',' << eratio  << ',' << iratio  << ',' << rratio  << ',' << vratio << ','  << dratio  << '\n';
            
            }
        }

        for (uint i = 0; i < N_lc; ++i) {
            for (uint j = 0; j < N_ag; ++j) {
                fanal << 0 << ',' << i << ',' << j << ',';
                fanal << 0 << ',' << 0 << ',';
                fanal << dcnt[i][j] << ',' << accum_icnt[i][j] << ',' << accum_vcnt[i][j] << ',';

                double dratio = dcnt[i][j] / (double)allcnt[i][j];
                double accum_iratio = accum_icnt[i][j] / (double)allcnt[i][j];
                double vratio = accum_vcnt[i][j] / (double)allcnt[i][j];
                fanal << dratio << ',' << accum_iratio << ',' << vratio << '\n';
            
            }
        }

        #ifdef SHOW
        std::cout << 0 << "\t\t" << scnts << "\t\t" << fcnts << "\t\t" << ecnts << "\t\t" << icnts << "\t\t" << rcnts << "\t\t" << vcnts << "\t\t" << dcnts << "\n";
        #endif

        #ifdef DETAIL
        for (auto& v : ndp) {
            if (v.stateID == 'I') {
                I.insert(v.id);
            }
            else {
                S.insert(v.id);
            }
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

    virtual void statisticUnit(const Time::TimeStep& ts, const std::vector<uint>& s2f, const std::vector<uint>& s2v, const std::vector<uint>& s2e, const std::vector<uint>& f2e, const std::vector<uint>& e2i, const std::vector<uint>& i2f, const std::vector<uint>& i2r, const std::vector<uint>& i2d) override {
        //std::cout << "statistic unit\n";
        for (const auto& v : s2f) {
            --scnt[ndp[v].loc][ndp[v].age];
            ++fcnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : s2v) {
            --scnt[ndp[v].loc][ndp[v].age];
            ++vcnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : s2e) {
            --scnt[ndp[v].loc][ndp[v].age];
            ++ecnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : f2e) {
            --fcnt[ndp[v].loc][ndp[v].age];
            ++ecnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : e2i) {
            --ecnt[ndp[v].loc][ndp[v].age];
            ++icnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : i2f) {
            --icnt[ndp[v].loc][ndp[v].age];
            ++fcnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : i2r) {
            --icnt[ndp[v].loc][ndp[v].age];
            ++rcnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : i2d) {
            --icnt[ndp[v].loc][ndp[v].age];
            ++dcnt[ndp[v].loc][ndp[v].age];
        }

        for (const auto& v : s2v) {
            ++accum_vcnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : s2f) {
            ++accum_vcnt[ndp[v].loc][ndp[v].age];
        }
        for (const auto& v : s2e) {
            if (!have_infected[v]) {
                ++accum_icnt[ndp[v].loc][ndp[v].age];
                have_infected[v] = 1;
            }
        }
        for (const auto& v : f2e) {
            if (!have_infected[v]) {
                ++accum_icnt[ndp[v].loc][ndp[v].age];
                have_infected[v] = 1;
            }
        }

        // for (auto v : s2e) if (ndp[v].stateID != 'E') std::cout << "GG\n";
        // for (auto v : f2e) if (ndp[v].stateID != 'E') std::cout << "GG\n";
        // for (auto v : s2f) if (ndp[v].stateID != 'F') std::cout << "GG\n";
        // for (auto v : s2v) if (ndp[v].stateID != 'V') std::cout << "GG\n";
        // for (auto v : e2i) if (ndp[v].stateID != 'I') std::cout << "GG\n";
        // for (auto v : i2f) if (ndp[v].stateID != 'F') std::cout << "GG\n";
        // for (auto v : i2r) if (ndp[v].stateID != 'R') std::cout << "GG\n";
        // for (auto v : i2d) if (ndp[v].stateID != 'D') std::cout << "GG\n";

        // for (int i = 0; i < I.size(); ++i) if (ndp[I[i]].stateID != 'I') std::cout << "GG\n";
        // for (int i = 0; i < E.size(); ++i) if (ndp[E[i]].stateID != 'E') std::cout << "GG\n";

        #ifdef CHANGES
        if (s2f.size()) {
            std::cout << "s2f" << ' ';
            for (const auto& v : s2f) {
                std::cout << v.getID() << ' ';
            }    
            std::cout << '\n';
        }
        if (s2v.size()) {
            std::cout << "s2v" << ' ';
            for (const auto& v : s2v) {
                std::cout << v.getID() << ' ';
            }    
            std::cout << '\n';
        }
        if (s2e.size()) {
            std::cout << "s2e" << ' ';
            for (const auto& v : s2e) {
                std::cout << v.getID() << ' ';
            }
            std::cout << '\n';    
        }
        if (f2e.size()) {
            std::cout << "f2e" << ' ';
            for (const auto& v : f2e) {
                std::cout << v.getID() << ' ';
            }
            std::cout << '\n';    
        }
        if (e2i.size()) {
            std::cout << "e2i" << ' ';
            for (const auto& v : e2i) {
                std::cout << v.getID() << ' ';
            }
            std::cout << '\n';    
        }
        if (i2f.size()) {
            std::cout << "i2f" << ' ';
            for (const auto& v : i2f) {
                std::cout << v.getID() << ' ';
            }
            std::cout << '\n';    
        }
        if (i2r.size()) {
            std::cout << "i2r" << ' ';
            for (const auto& v : i2r) {
                std::cout << v.getID() << ' ';
            }
            std::cout << '\n';    
        }
        if (i2d.size()) {
            std::cout << "i2d" << ' ';
            for (const auto& v : i2d) {
                std::cout << v.getID() << ' ';
            }
            std::cout << '\n';    
        }
        #endif

        #ifdef SHOW
        icnts += e2i.size() - i2f.size() - i2r.size() - i2d.size();
        scnts -= s2f.size() + s2v.size() + s2e.size();
        fcnts += s2f.size() - f2e.size() + i2f.size();
        rcnts += i2r.size();
        ecnts += s2e.size() + f2e.size() - e2i.size();
        vcnts += s2v.size();
        dcnts += i2d.size();
        #endif

        uint p = ts.getDay() * ts.getPeriodLength() + ts.getPeriod() + 1;
        for (uint i = 0; i < N_lc; ++i) {
            for (uint j = 0; j < N_ag; ++j) {
                double sratio = scnt[i][j] / (double)allcnt[i][j];
                double fratio = fcnt[i][j] / (double)allcnt[i][j];
                double eratio = ecnt[i][j] / (double)allcnt[i][j];
                double iratio = icnt[i][j] / (double)allcnt[i][j];
                double rratio = rcnt[i][j] / (double)allcnt[i][j];
                double vratio = vcnt[i][j] / (double)allcnt[i][j];
                double dratio = dcnt[i][j] / (double)allcnt[i][j];
                fstate << p << ',' << i << ',' << j << ',';
                fstate << scnt[i][j] << ',' << fcnt[i][j] << ',' << ecnt[i][j] << ',' << icnt[i][j] << ',' << rcnt[i][j] << ',' << vcnt[i][j] << ',' << dcnt[i][j] << ',';
                fstate << sratio  << ',' << fratio  << ',' << eratio  << ',' << iratio  << ',' << rratio  << ',' << vratio << ','  << dratio  << '\n';
            
            }
        }

        std::vector<std::vector<uint>> cur_death(N_lc, std::vector<uint>(N_ag));
        std::vector<std::vector<uint>> cur_infec(N_lc, std::vector<uint>(N_ag));

        for (auto& v : i2d) {
            ++cur_death[ndp[v].loc][ndp[v].age];
        }
        for (auto& v : s2e) {
            ++cur_infec[ndp[v].loc][ndp[v].age];
        }
        for (auto& v : f2e) {
            ++cur_infec[ndp[v].loc][ndp[v].age];
        }
        uint temp = 0;

        for (uint i = 0; i < N_lc; ++i) {
            for (uint j = 0; j < N_ag; ++j) {
                fanal << p << ',' << i << ',' << j << ',';
                fanal << cur_death[i][j] << ',' << cur_infec[i][j] << ',';
                fanal << dcnt[i][j] << ',' << accum_icnt[i][j] << ',' << accum_vcnt[i][j] << ',';

                double dratio = dcnt[i][j] / (double)allcnt[i][j];
                double accum_iratio = accum_icnt[i][j] / (double)allcnt[i][j];
                double vratio = accum_vcnt[i][j] / (double)allcnt[i][j];
                fanal << dratio << ',' << accum_iratio << ',' << vratio << '\n';
                temp += cur_infec[i][j];
            }
        }
        std::cout << p << '\t' << temp << '\n';
#ifdef SHOW
        std::cout << p << "\t\t" << scnts << "\t\t" << fcnts << "\t\t" << ecnts << "\t\t" << icnts << "\t\t" << rcnts << "\t\t" << vcnts << "\t\t" << dcnts << "\n";
        #endif

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
        fstate.close();
        fanal.close();
    }

    inline void writeHeader() {
        //fout << "\"Period\",\"Town\",\"num_infection\",\"num_dead\",\"ratio_infection\",\"ratio_dead\"\n";
        fstate << "\"Period\"" << ',';
        fstate << "\"Town\"" << ',';
        fstate << "\"Age\"" << ',';

        fstate << "\"num_susceptible\"" << ',';
        fstate << "\"num_failed\"" << ',';
        fstate << "\"num_exposed\"" << ',';
        fstate << "\"num_infected\"" << ',';
        fstate << "\"num_recovered\"" << ',';
        fstate << "\"num_vaccined\"" << ',';
        fstate << "\"num_dead\"" << ',';
        
        fstate << "\"ratio_susceptible\"" << ',';
        fstate << "\"ratio_failed\"" << ',';
        fstate << "\"ratio_exposed\"" << ',';
        fstate << "\"ratio_infected\"" << ',';
        fstate << "\"ratio_recovered\"" << ',';
        fstate << "\"ratio_vaccined\"" << ',';
        fstate << "\"ratio_dead\"" << '\n';

        fanal << "\"Period\"" << ',';
        fanal << "\"Town\"" << ',';
        fanal << "\"Age\"" << ',';

        fanal << "\"Deaths in that period\"" << ',';
        fanal << "\"Infections in that period\"" << ',';

        fanal << "\"Deaths\"" << ',';
        fanal << "\"Infections\"" << ',';
        fanal << "\"Vaccinated\"" << ',';

        fanal << "\"Deaths (ratio)\"" << ',';
        fanal << "\"Infections (ratio)\"" << ',';
        fanal << "\"Vaccinated (ratio)\"" << '\n';

        #ifdef SHOW
        std::cout << "Period\t\tS\t\tF\t\tE\t\tI\t\tR\t\tV\t\tD\n";
        #endif
    }

    // void writeUnit(uint p) {
    //     for (uint i = 0; i < N_lc; ++i) {
    //         for (uint j = 0; j < N_ag; ++j) {
    //             double sratio = scnt[i][j] / (double)allcnt[i][j];
    //             double fratio = fcnt[i][j] / (double)allcnt[i][j];
    //             double eratio = ecnt[i][j] / (double)allcnt[i][j];
    //             double iratio = icnt[i][j] / (double)allcnt[i][j];
    //             double rratio = rcnt[i][j] / (double)allcnt[i][j];
    //             double vratio = vcnt[i][j] / (double)allcnt[i][j];
    //             double dratio = dcnt[i][j] / (double)allcnt[i][j];
    //             fstate << p << ',' << i << ',' << j << ',';
    //             fstate << scnt[i][j] << ',' << fcnt[i][j] << ',' << ecnt[i][j] << ',' << icnt[i][j] << ',' << rcnt[i][j] << ',' << vcnt[i][j] << ',' << dcnt[i][j] << ',';
    //             fstate << sratio  << ',' << fratio  << ',' << eratio  << ',' << iratio  << ',' << rratio  << ',' << vratio << ','  << dratio  << '\n';
            
    //         }
    //     }

    //     #ifdef SHOW
    //     std::cout << p << "\t\t" << scnts << "\t\t" << fcnts << "\t\t" << ecnts << "\t\t" << icnts << "\t\t" << rcnts << "\t\t" << vcnts << "\t\t" << dcnts << "\n";
    //     #endif
    // }

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

    #ifdef SHOW
    uint icnts, scnts, fcnts, rcnts, ecnts, vcnts, dcnts;
    #endif

    std::ofstream fstate, fanal;
    std::vector<std::vector<uint>> allcnt, icnt, scnt, fcnt, rcnt, ecnt, vcnt, dcnt;
    std::vector<std::vector<uint>> accum_icnt, accum_vcnt;

    std::vector<bool> have_infected;
};

#endif
