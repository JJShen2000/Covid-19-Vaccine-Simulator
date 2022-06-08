#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "StatisticsAgedGeoStatePopulation.hpp"

using namespace std;

extern double infection_time, vaccination_time;

int main(int argc, char* argv[]) {
    // cout << argc << '\n';
   // cout << "init\n";
    stringstream ss;
    for (int i = 1; i < argc; ++i) {
        ss << argv[i];
        ss << ' ';

        // cout << argv[i] << '\n';
    }

    ifstream in_graph;
    ifstream in_param;
    ifstream in_init;
    ifstream in_vacc;

    ofstream out_statistics;

    string outfile;

    string cmd, fname, setting = "DSS";
    while (ss >> cmd) {
        // cout << "command " << cmd << '\n';
        if (cmd == "--graph" || cmd == "-g") {
            ss >> fname;
            in_graph.open(fname);
            // cout << "open graph file " << fname << '\n';
        }
        else if (cmd == "--param" || cmd == "-p") {
            ss >> fname;
            in_param.open(fname);
            // cout << "open param file " << fname << '\n';
        }
        else if (cmd == "--init" || cmd == "-i") {
            ss >> fname;
            in_init.open(fname);
            // cout << "open init file " << fname << '\n';
        }
        else if (cmd == "--vacc" || cmd == "-v") {
            ss >> fname;
            in_vacc.open(fname);
            // cout << "open vacc file " << fname << '\n';
        }
        else if (cmd == "--output-file" || cmd == "-o") {
            ss >> fname;
            out_statistics.open(fname);
            // cout << "open out file " << fname << '\n';
        }
        else if (cmd == "--output-setting" || cmd == "-s") {
            ss >> setting;
            // cout << "setting " << setting << '\n';
        }
    }

    if (!in_graph.is_open()) {
        cout << "fail to open " << fname << '\n';
        return 0;
    }

    if (!in_param.is_open()) {
        cout << "fail to open " << fname << '\n';
        return 0;
    }

    if (!in_init.is_open()) {
        cout << "fail to open " << fname << '\n';
        return 0;
    }

    if (!in_vacc.is_open()) {
        cout << "fail to open " << fname << '\n';
        return 0;
    }

    if (!out_statistics.is_open()) {
        cout << "fail to open " << fname << '\n';
        return 0;
    }

    StatisticsAgedGeoStatePopulation model;
    // cout << "graph\n";
    model.loadGraph(in_graph);
    // cout << "param\n";
    model.loadParam(in_param);
    // cout << "init\n";
    model.loadInitInfector(in_init);
    // cout << "vacc\n";
    model.loadVaccinationStrat(in_vacc);
    // cout << "outfile\n";
    model.setOutStream(&out_statistics);
    // cout << "setting\n";
    model.setOutputSetting(setting);

    //cout << "start!\n";
    //cout << "start!\n";
    model.simulate();

    printf("infection: %.5lf\n", infection_time);    
    printf("vaccination: %.5lf\n", vaccination_time);    

    return 0;
}