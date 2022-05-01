#ifndef BASEMODEL_HPP
#define BASEMODEL_HPP

//#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>

#include "Time.hpp"
#include "ContactMatrix.hpp"

#include "Node.hpp"
#include "ContactGroup.hpp"

class BaseModel {
public:

    void simulate();

protected:
    virtual void simulate_unit(const Time::TimeStep& ts) = 0;

    void loadGraph(std::istream& in);

    std::unordered_map<std::string, std::vector<double>> loadParam(std::istream& in) const;

    uint N_nd; // # of nodes
    uint N_pr; // # of periods per day
    uint N_gp; // # of contact groups
    uint N_ag; // # of age groups
    uint N_cm; // # of contact matrices
    uint N_lc; // # of locations

    ContactMatrixPool cmp;
    ContactGroupPool cgpp;
    NodePool ndp;
    Time tm;
};

#endif