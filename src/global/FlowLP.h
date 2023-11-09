#ifndef FLOW_LP_H
#define FLOW_LP_H

#include <gurobi_c++.h>
#include "../base/Include.h"
#include "RGraph.h"
#include "../base/DB.h"
using namespace std;

class FlowLP {
    public:
        // FlowLP(RGraph& rGraph, vector<double> vMediumLayerThickness, vector<double> vMetalLayerThickness, vector<double> vConductivity, double currentNorm);
        FlowLP(DB& db, RGraph& rGraph);
        ~FlowLP() {}

        void setObjective(double areaWeight, double viaWeight, double diffWeight);
        void setConserveConstraints(bool useDemandCurrent);
        // width unit = meter
        // void addSViaAreaConstraints(size_t netId, double area);
        // void addTViaAreaConstraints(size_t netId, size_t tPortId, double area);
        // void setWidthConstraints();
        void addViaAreaConstraints(size_t netId, size_t vEdgeId, double area);
        void addCapacityConstraints(OASGEdge* e1, bool right1, double ratio1, OASGEdge* e2, bool right2, double ratio2, double width);
        void addCapacityConstraints(OASGEdge* e1, bool right1, double ratio1, double width);
        void addSameNetCapacityConstraints(OASGEdge* e1, bool right1, double ratio1, OASGEdge* e2, bool right2, double ratio2, double width);
        // void relaxCapacityConstraints(GRBLinExpr& obj, OASGEdge* e1, bool right1, double ratio1, OASGEdge* e2, bool right2, double ratio2, double width);
        void relaxCapacityConstraints(vector<double> vLambda);
        void relaxCapacityConstraints(vector<double> vLambda, vector<double> vSameNetLambda);
        void solve();
        void collectResult();
        void printResult();
        void solveRelaxed();
        void collectRelaxedResult();
        void printRelaxedResult();

        int numCapConstrs() const { return _numCapConstrs; }
        double area() const { return _area; }
        double viaArea() const { return _viaArea; }
        double overlap() const { return _overlap; }
        vector<double> vOverlap() { return _vOverlap; }
        double vOverlap(size_t ovId) const { return _vOverlap[ovId]; }
        double sameNetOverlap() const { return _sameNetOverlap; }
        double vSameNetOverlap(size_t ovId) const { return _vSameNetOverlap[ovId]; }

    private:
        DB& _db;
        RGraph& _rGraph;
        GRBEnv _env;
        GRBModel _model;
        GRBModel* _modelRelaxed;
        // gurobi variables
        GRBVar*** _vPlaneLeftFlow;   // flows on the left of horizontal OASGEdges, index = [netId] [layId] [pEdgeId]
        GRBVar*** _vPlaneRightFlow;  // flows on the right of horizontal OASGEdges, index = [netId] [layId] [pEdgeId]
        GRBVar*** _vPlaneDiffFlow;   // auxiliary variables representing abs(leftFlow - rightFlow), index = [netId] [layId] [pEdgeId]
        GRBVar*** _vViaFlow;         // flows on vertical OASGEdges, index = [netId] [layPairId] [vEdgeId]
        GRBVar**  _vMaxViaCost;      // the maximum flow on an OASGEdge, index = [netId] [vEdgeId]
        int _numCapConstrs;          // number of capacity constraints
        int _numNetCapConstrs;       // number of same net capacity constraints

        // input constants
        // vector<double> _vMediumLayerThickness;
        // vector<double> _vMetalLayerThickness;
        // vector<double> _vConductivity;
        // double _currentNorm;      // the current value range (to avoid Gurobi error)
        double _area;       // the resulting area, assigned in collectRelaxedResult
        double _viaArea;
        double _overlap;    // the resulting overlapped width, assigned in collectRelaxedResult
        vector<double> _vOverlap;   // the resulting overlapped width of each capConstr, assigned in collectRelaxedResult
        double _sameNetOverlap;
        vector<double> _vSameNetOverlap;
};

#endif