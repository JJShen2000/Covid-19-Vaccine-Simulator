#ifndef SIMAONOBJECTIVE_HPP
#define SIMAONOBJECTIVE_HPP

#include <set>

//  #define DETAIL
#define SHOW
// #define CHANGES

#include "SFEIRVDModel.hpp"

class SimAonObjective : public SFEIRVDModel {
public:
    void setOutFile(const std::string& fname) {
        fout.open(fname);
        if (!fout) {
            std::cout << "fail open file\n";
        }
    }

protected:
    virtual void statisticInit() override {
        allcnt.resize(N_lc, std::vector<uint>(N_ag));
        accum_icnt.resize(N_lc, std::vector<uint>(N_ag));
        vcnt.resize(N_lc, std::vector<uint>(N_ag));
        dcnt.resize(N_lc, std::vector<uint>(N_ag));
        for (auto& v : ndp) {
            ++allcnt[v.loc][v.age];
            if (v.stateID == 'I') ++accum_icnt[v.loc][v.age];
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
                fout << 0 << ',' << i << ',' << j << ',';
                fout << 0 << ',' << 0 << ',';
                fout << dcnt[i][j] << ',' << accum_icnt[i][j] << ',' << vcnt[i][j] << ',';

                double dratio = dcnt[i][j] / (double)allcnt[i][j];
                double accum_iratio = accum_icnt[i][j] / (double)allcnt[i][j];
                double vratio = vcnt[i][j] / (double)allcnt[i][j];
                fout << dratio << ',' << accum_iratio << ',' << vratio << '\n';
            
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

    virtual void statisticUnit(const Time::TimeStep& ts, const Nodes& s2f, const Nodes& s2v, const Nodes& s2e, const Nodes& f2e, const Nodes& e2i, const Nodes& i2f, const Nodes& i2r, const Nodes& i2d) override {
        //std::cout << "statistic unit\n";
        for (const auto& v : s2v) {
            ++vcnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : s2e) {
            ++accum_icnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : f2e) {
            ++accum_icnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : i2d) {
            ++dcnt[v.getLocation()][v.getAge()];
        }

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

        std::vector<std::vector<uint>> cur_death(N_lc, std::vector<uint>(N_ag));
        std::vector<std::vector<uint>> cur_infec(N_lc, std::vector<uint>(N_ag));

        for (auto& v : i2d) {
            ++cur_death[v.getLocation()][v.getAge()];
        }
        for (auto& v : s2e) {
            ++cur_infec[v.getLocation()][v.getAge()];
        }
        for (auto& v : f2e) {
            ++cur_infec[v.getLocation()][v.getAge()];
        } 

        uint p = ts.getDay() * ts.getPeriodLength() + ts.getPeriod() + 1;

        for (uint i = 0; i < N_lc; ++i) {
            for (uint j = 0; j < N_ag; ++j) {
                fout << p << ',' << i << ',' << j << ',';
                fout << cur_death[i][j] << ',' << cur_infec[i][j] << ',';
                fout << dcnt[i][j] << ',' << accum_icnt[i][j] << ',' << vcnt[i][j] << ',';

                double dratio = dcnt[i][j] / (double)allcnt[i][j];
                double accum_iratio = accum_icnt[i][j] / (double)allcnt[i][j];
                double vratio = vcnt[i][j] / (double)allcnt[i][j];
                fout << dratio << ',' << accum_iratio << ',' << vratio << '\n';
            
            }
        }

        #ifdef SHOW
        std::cout << p << "\t\t" << scnts << "\t\t" << fcnts << "\t\t" << ecnts << "\t\t" << icnts << "\t\t" << rcnts << "\t\t" << vcnts << "\t\t" << dcnts << "\n";
        #endif

        // writeUnit(ts.getDay() * ts.getPeriodLength() + ts.getPeriod() + 1);

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

    inline void writeHeader() {
        //fout << "\"Period\",\"Town\",\"num_infection\",\"num_dead\",\"ratio_infection\",\"ratio_dead\"\n";
        fout << "\"Period\"" << ',';
        fout << "\"Town\"" << ',';
        fout << "\"Age\"" << ',';

        fout << "\"Deaths in that period\"" << ',';
        fout << "\"Infections in that period\"" << ',';

        fout << "\"Deaths\"" << ',';
        fout << "\"Infections\"" << ',';
        fout << "\"Vaccinated\"" << ',';

        fout << "\"Deaths (ratio)\"" << ',';
        fout << "\"Infections (ratio)\"" << ',';
        fout << "\"Vaccinated (ratio)\"" << '\n';


        #ifdef SHOW
        std::cout << "Period\t\tS\t\tF\t\tE\t\tI\t\tR\t\tV\t\tD\n";
        #endif
    }

    void writeUnit(uint p) {
        #ifdef SHOW
        std::cout << p << "\t\t" << scnts << "\t\t" << fcnts << "\t\t" << ecnts << "\t\t" << icnts << "\t\t" << rcnts << "\t\t" << vcnts << "\t\t" << dcnts << "\n";
        #endif
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

    #ifdef SHOW
    uint icnts, scnts, fcnts, rcnts, ecnts, vcnts, dcnts;
    #endif

    std::ofstream fout;
    std::vector<std::vector<uint>> allcnt, accum_icnt, vcnt, dcnt;
};

#endif
