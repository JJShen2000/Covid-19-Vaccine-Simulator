#include "StatisticsAgedGeoStatePopulation.hpp"
#include <iostream>


void StatisticsAgedGeoStatePopulation::setOutStream(std::ostream* out) {
    this->out = out;
}

void StatisticsAgedGeoStatePopulation::setOutputSetting(std::string& setting) {
    
    switch (setting[0]) {
        case 'E':
            time_opt = TimeOpt::End;
            break;
        case 'D':
            time_opt = TimeOpt::Daily;
            break;
        case 'P':
            time_opt = TimeOpt::Periodic;
            break;
    }
    switch (setting[1]) {
        case 'S':
            age_opt = AgeOpt::Sum;
            break;
        case 'E':
            age_opt = AgeOpt::Each;
            break;
    }
    switch (setting[2]) {
        case 'S':
            town_opt = TownOpt::Sum;
            break;
        case 'E':
            town_opt = TownOpt::Each;
            break;
    }

    if (setting.length() <= 3) return;


    auto stt = setting.substr(3, setting.length() - 3);
    // cout << "stt " << stt << '\n';
    auto in = [](std::string& s, char c) {
        return s.find(c) != std::string::npos;
    };

    state_cnt_opt = 0;
    if (in(stt, 'A')) {
        state_cnt_opt |= (1 << 0);
    }
    if (in(stt, 'S')) {
        state_cnt_opt |= (1 << 1);
    }
    if (in(stt, 'V')) {
        state_cnt_opt |= (1 << 2);
    }
    // if (in(stt, 'W')) {
    //     state_cnt_opt |= (1 << 3);
    // }
    if (in(stt, 'E')) {
        state_cnt_opt |= (1 << 4);
    }
    if (in(stt, 'I')) {
        state_cnt_opt |= (1 << 5);
    }
    if (in(stt, 'J')) {
        state_cnt_opt |= (1 << 6);
    }
    if (in(stt, 'K')) {
        state_cnt_opt |= (1 << 7);
    }
    if (in(stt, 'F')) {
        state_cnt_opt |= (1 << 8);
    }
    if (in(stt, 'R')) {
        state_cnt_opt |= (1 << 9);
    }
    if (in(stt, 'D')) {
        state_cnt_opt |= (1 << 10);
    }
    // cout << state_cnt_opt << '\n';
    // for (uint i = 0; i < (uint)StateCntOpt::NState; ++i) {
    //     if (state_cnt_opt & (1 << i)) {
    //         cout << column_name[i] << '\n';
    //     }
    // }
}

void StatisticsAgedGeoStatePopulation::statisticInit() {
    // cout << "statistic init\n";
    allcnt.resize(N_lc, std::vector<uint>(N_ag));
    scnt.resize(N_lc, std::vector<uint>(N_ag));
    vcnt.resize(N_lc, std::vector<uint>(N_ag));
    // wcnt.resize(N_lc, std::vector<uint>(N_ag));
    ecnt.resize(N_lc, std::vector<uint>(N_ag));
    icnt.resize(N_lc, std::vector<uint>(N_ag));
    jcnt.resize(N_lc, std::vector<uint>(N_ag));
    kcnt.resize(N_lc, std::vector<uint>(N_ag));
    fcnt.resize(N_lc, std::vector<uint>(N_ag));
    rcnt.resize(N_lc, std::vector<uint>(N_ag));
    dcnt.resize(N_lc, std::vector<uint>(N_ag));

    allnew.resize(N_lc, std::vector<uint>(N_ag));
    snew.resize(N_lc, std::vector<uint>(N_ag));
    vnew.resize(N_lc, std::vector<uint>(N_ag));
    // wnew.resize(N_lc, std::vector<uint>(N_ag));
    enew.resize(N_lc, std::vector<uint>(N_ag));
    inew.resize(N_lc, std::vector<uint>(N_ag));
    jnew.resize(N_lc, std::vector<uint>(N_ag));
    knew.resize(N_lc, std::vector<uint>(N_ag));
    fnew.resize(N_lc, std::vector<uint>(N_ag));
    rnew.resize(N_lc, std::vector<uint>(N_ag));
    dnew.resize(N_lc, std::vector<uint>(N_ag));

    for (auto& v : ndp) {
        // ++cnts[0].cnt[v.loc][v.age];
        // if (v.stateID == 'I') {
        //     ++cnts[5].cnt[v.loc][v.age];
        // }
        // else {
        //     ++cnts[2].cnt[v.loc][v.age];
        // }
        ++allcnt[v.loc][v.age];
        if (v.stateID == 'I') {
            ++icnt[v.loc][v.age];
        }
        else {
            ++scnt[v.loc][v.age];
        }
    }
    writeHeader();
}

void StatisticsAgedGeoStatePopulation::statisticUnit(const Time::TimeStep& ts, const Transition& trans) {
    ////cout << "statistic\n";
    update(trans);
    if (time_opt == TimeOpt::Periodic) {
        writeLine(ts);
    }
    else if (time_opt == TimeOpt::Daily && ts.getPeriod() == 0) {
        writeLine(ts);
    }
    using std::cout;
    // if (trans.e2i.size()) { cout << "e2i: "; for (auto v : trans.e2i) cout << v << ' '; cout << '\n'; }
    // if (trans.f2e.size()) { cout << "f2e: "; for (auto v : trans.f2e) cout << v << ' '; cout << '\n'; }
    // if (trans.i2j.size()) { cout << "i2j: "; for (auto v : trans.i2j) cout << v << ' '; cout << '\n'; }
    // if (trans.i2k.size()) { cout << "i2k: "; for (auto v : trans.i2k) cout << v << ' '; cout << '\n'; }
    // if (trans.j2f.size()) { cout << "j2f: "; for (auto v : trans.j2f) cout << v << ' '; cout << '\n'; }
    // if (trans.j2r.size()) { cout << "j2r: "; for (auto v : trans.j2r) cout << v << ' '; cout << '\n'; }
    // if (trans.k2d.size()) { cout << "k2d: "; for (auto v : trans.k2d) cout << v << ' '; cout << '\n'; }
    // if (trans.k2f.size()) { cout << "k2f: "; for (auto v : trans.k2f) cout << v << ' '; cout << '\n'; }
    // if (trans.k2r.size()) { cout << "k2r: "; for (auto v : trans.k2r) cout << v << ' '; cout << '\n'; }
    // if (trans.s2e.size()) { cout << "s2e: "; for (auto v : trans.s2e) cout << v << ' '; cout << '\n'; }
    // if (trans.s2v.size()) { cout << "s2v: "; for (auto v : trans.s2v) cout << v << ' '; cout << '\n'; }
    // if (trans.v2e.size()) { cout << "v2e: "; for (auto v : trans.v2e) cout << v << ' '; cout << '\n'; }

    // cout << ts.getDay() << '\n';
    // if (trans.e2i.size()) { cout << "e2i: " << trans.e2i.size() << '\n'; }
    // if (trans.f2e.size()) { cout << "f2e: " << trans.f2e.size() << '\n'; }
    // if (trans.i2j.size()) { cout << "i2j: " << trans.i2j.size() << '\n'; }
    // if (trans.i2k.size()) { cout << "i2k: " << trans.i2k.size() << '\n'; }
    // if (trans.j2f.size()) { cout << "j2f: " << trans.j2f.size() << '\n'; }
    // if (trans.j2r.size()) { cout << "j2r: " << trans.j2r.size() << '\n'; }
    // if (trans.k2d.size()) { cout << "k2d: " << trans.k2d.size() << '\n'; }
    // if (trans.k2f.size()) { cout << "k2f: " << trans.k2f.size() << '\n'; }
    // if (trans.k2r.size()) { cout << "k2r: " << trans.k2r.size() << '\n'; }
    // if (trans.s2e.size()) { cout << "s2e: " << trans.s2e.size() << '\n'; }
    // if (trans.s2v.size()) { cout << "s2v: " << trans.s2v.size() << '\n'; }
    // if (trans.v2e.size()) { cout << "v2e: " << trans.v2e.size() << '\n'; }
    // if (trans.v2w.size()) { cout << "v2w: "; for (auto v : trans.v2w) cout << v << ' '; cout << '\n'; }
    // if (trans.w2e.size()) { cout << "w2e: "; for (auto v : trans.w2e) cout << v << ' '; cout << '\n'; }
}

void StatisticsAgedGeoStatePopulation::statisticEnd() {
    // cout << "statistic end\n";
    if (time_opt == TimeOpt::End) {
        writeLine(Time::TimeStep({0, 0}, nullptr));
    }
}

void StatisticsAgedGeoStatePopulation::update(const Transition& trans) {  
    transition(trans.s2v, scnt, vcnt, vnew);
    // transition(trans.v2w, vcnt, wcnt, wnew);
    transition(trans.s2e, scnt, ecnt, enew);
    transition(trans.v2e, vcnt, ecnt, enew);
    // transition(trans.w2e, wcnt, ecnt, enew);
    transition(trans.f2e, fcnt, ecnt, enew);
    transition(trans.e2i, ecnt, icnt, inew);
    transition(trans.i2j, icnt, jcnt, jnew);
    transition(trans.i2k, icnt, kcnt, knew);
    transition(trans.j2f, jcnt, fcnt, fnew);
    transition(trans.j2r, jcnt, rcnt, rnew);
    transition(trans.k2r, kcnt, rcnt, rnew);
    transition(trans.k2d, kcnt, dcnt, dnew);
    transition(trans.k2f, kcnt, fcnt, fnew);
}

void StatisticsAgedGeoStatePopulation::transition(const Nodes& nodes, std::vector<std::vector<uint>>& src, std::vector<std::vector<uint>>& des, std::vector<std::vector<uint>>& desnew) {
    for (auto v : nodes) {
        --src[ndp[v].loc][ndp[v].age];
        ++des[ndp[v].loc][ndp[v].age];
        ++desnew[ndp[v].loc][ndp[v].age];
    }
}

void StatisticsAgedGeoStatePopulation::writeHeader() {
    if (time_opt == TimeOpt::Daily) {
        (*out) << "Day,";
    }
    else if (time_opt == TimeOpt::Periodic) {
        (*out) << "Period,";
    }

    if (town_opt == TownOpt::Each) {
        (*out) << "Town,";
    }

    if (age_opt == AgeOpt::Each) {
        (*out) << "Age,";
    }

    bool adddot = 0;
    for (uint i = 0; i < (uint)StateCntOpt::NState; ++i) {
        if (state_cnt_opt & (1 << i)) {
            if (adddot) {
                (*out) << ",";
            }
            adddot = 1;
            (*out) << column_name[i] << " Number";
            if (adddot) {
                (*out) << ",";
            }
            adddot = 1;
            (*out) << " New Added " << column_name[i];
        }
    }
        
    (*out) << "\n";
}

void StatisticsAgedGeoStatePopulation::writeLine(const Time::TimeStep& ts) {
    // cout << "write line\n";
    for (uint town = 0; town < (town_opt == TownOpt::Each? N_lc : 1); ++town) {
        for (uint age = 0; age < (age_opt == AgeOpt::Each? N_ag : 1); ++age) {
            if (time_opt == TimeOpt::Daily) {
                (*out) << ts.getDay() << ",";
            }
            else if (time_opt == TimeOpt::Periodic) {
                (*out) << ts.getPeriod() + ts.getPeriodLength() * ts.getDay() << ",";
            }

            if (town_opt == TownOpt::Each) {
                (*out) << town << ",";
            }

            if (age_opt == AgeOpt::Each) {
                (*out) << age << ",";
            }

            bool adddot = 0;
            if (state_cnt_opt & (1 << (uint)StateCntOpt::All)) writeTerm(town, age, adddot, allcnt, allnew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::S)) writeTerm(town, age, adddot, scnt, snew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::V)) writeTerm(town, age, adddot, vcnt, vnew);
            // if (state_cnt_opt & (1 << (uint)StateCntOpt::W)) writeTerm(town, age, adddot, wcnt, wnew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::E)) writeTerm(town, age, adddot, ecnt, enew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::I)) writeTerm(town, age, adddot, icnt, inew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::J)) writeTerm(town, age, adddot, jcnt, jnew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::K)) writeTerm(town, age, adddot, kcnt, knew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::F)) writeTerm(town, age, adddot, fcnt, fnew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::R)) writeTerm(town, age, adddot, rcnt, rnew);
            if (state_cnt_opt & (1 << (uint)StateCntOpt::D)) writeTerm(town, age, adddot, dcnt, dnew);
            (*out) << '\n';
        }
    }
    // (*out) << '\n';
}

void StatisticsAgedGeoStatePopulation::writeTerm(uint town, uint age, bool& adddot, std::vector<std::vector<uint>>& cnt, std::vector<std::vector<uint>>& nnew) {
    uint smcnt = 0, smnew = 0;
    if (town_opt == TownOpt::Sum && age_opt == AgeOpt::Sum) {
        for (uint town = 0; town < N_lc; ++town) {
            for (uint age = 0; age < N_ag; ++age) {
                smcnt += cnt[town][age];
                smnew += nnew[town][age];
                nnew[town][age] = 0;
            }
        }
    }
    else if (town_opt == TownOpt::Each && age_opt == AgeOpt::Sum) {
        for (uint age = 0; age < N_ag; ++age) {
            smcnt += cnt[town][age];
            smnew += nnew[town][age];
            nnew[town][age] = 0;
        }
    }
    else if (age_opt == AgeOpt::Each) {
        for (uint town = 0; town < N_lc; ++town) {
            smcnt += cnt[town][age];
            smnew += nnew[town][age];
            nnew[town][age] = 0;
        }
    }
    else {
        smcnt = cnt[town][age];
        smnew = nnew[town][age];
        nnew[town][age] = 0;
    }

    if (adddot) {
        (*out) << ",";
    }
    adddot = 1;

    (*out) << smcnt;

    if (adddot) {
        (*out) << ",";
    }
    adddot = 1;

    (*out) << smnew;


}