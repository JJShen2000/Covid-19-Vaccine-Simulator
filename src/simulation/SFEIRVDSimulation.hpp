#ifndef SFEIRVDSIMULATION_HPP
#define SFEIRVDSIMULATION_HPP

#include <set>

//#define DETAIL
#define SHOW
// #define CHANGES

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
        allcnt.resize(N_lc, std::vector<uint>(N_ag));
        scnt.resize(N_lc, std::vector<uint>(N_ag));
        fcnt.resize(N_lc, std::vector<uint>(N_ag));
        ecnt.resize(N_lc, std::vector<uint>(N_ag));
        icnt.resize(N_lc, std::vector<uint>(N_ag));
        rcnt.resize(N_lc, std::vector<uint>(N_ag));
        vcnt.resize(N_lc, std::vector<uint>(N_ag));
        dcnt.resize(N_lc, std::vector<uint>(N_ag));
        for (auto& v : ndp) {
            ++allcnt[v.loc][v.age];
            if (v.stateID == 'I') ++icnt[v.loc][v.age];
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
        writeUnit(0);

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
        for (const auto& v : s2f) {
            --scnt[v.getLocation()][v.getAge()];
            ++fcnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : s2v) {
            --scnt[v.getLocation()][v.getAge()];
            ++vcnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : s2e) {
            --scnt[v.getLocation()][v.getAge()];
            ++ecnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : f2e) {
            --fcnt[v.getLocation()][v.getAge()];
            ++ecnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : e2i) {
            --ecnt[v.getLocation()][v.getAge()];
            ++icnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : i2f) {
            --icnt[v.getLocation()][v.getAge()];
            ++fcnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : i2r) {
            --icnt[v.getLocation()][v.getAge()];
            ++rcnt[v.getLocation()][v.getAge()];
        }
        for (const auto& v : i2d) {
            --icnt[v.getLocation()][v.getAge()];
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

        writeUnit(ts.getDay() * ts.getPeriodLength() + ts.getPeriod() + 1);

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

        fout << "\"num_susceptible\"" << ',';
        fout << "\"num_failed\"" << ',';
        fout << "\"num_exposed\"" << ',';
        fout << "\"num_infected\"" << ',';
        fout << "\"num_recovered\"" << ',';
        fout << "\"num_vaccined\"" << ',';
        fout << "\"num_dead\"" << ',';
        
        fout << "\"ratio_susceptible\"" << ',';
        fout << "\"ratio_failed\"" << ',';
        fout << "\"ratio_exposed\"" << ',';
        fout << "\"ratio_infected\"" << ',';
        fout << "\"ratio_recovered\"" << ',';
        fout << "\"ratio_vaccined\"" << ',';
        fout << "\"ratio_dead\"" << '\n';

        #ifdef SHOW
        std::cout << "Period\t\tS\t\tF\t\tE\t\tI\t\tR\t\tV\t\tD\n";
        #endif
    }

    void writeUnit(uint p) {
        for (uint i = 0; i < N_lc; ++i) {
            for (uint j = 0; j < N_ag; ++j) {
                double sratio = scnt[i][j] / (double)allcnt[i][j];
                double fratio = fcnt[i][j] / (double)allcnt[i][j];
                double eratio = ecnt[i][j] / (double)allcnt[i][j];
                double iratio = icnt[i][j] / (double)allcnt[i][j];
                double rratio = rcnt[i][j] / (double)allcnt[i][j];
                double vratio = vcnt[i][j] / (double)allcnt[i][j];
                double dratio = dcnt[i][j] / (double)allcnt[i][j];
                fout << p << ',' << i << ',' << j << ',';
                fout << scnt[i][j] << ',' << fcnt[i][j] << ',' << ecnt[i][j] << ',' << icnt[i][j] << ',' << rcnt[i][j] << ',' << vcnt[i][j] << ',' << dcnt[i][j] << ',';
                fout << sratio  << ',' << fratio  << ',' << eratio  << ',' << iratio  << ',' << rratio  << ',' << vratio << ','  << dratio  << '\n';
            
            }
        }

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
    std::vector<std::vector<uint>> allcnt, icnt, scnt, fcnt, rcnt, ecnt, vcnt, dcnt;
};

#endif
