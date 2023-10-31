#include "VoltEigen.h"

void VoltEigen::setMatrix(size_t rowNodeId, double resistance) {
    _G[rowNodeId][rowNodeId] += (1/resistance);
}
void VoltEigen::setMatrix(size_t rowNodeId, size_t colNodeId, double resistance) {
    _G[rowNodeId][rowNodeId] += (1/resistance);
    _G[rowNodeId][colNodeId] -= (1/resistance);
}
void VoltEigen::setInputVector(size_t rowNodeId, double inputVolt, double resistance) {
    _G[rowNodeId][rowNodeId] += (1/resistance);
    _I[rowNodeId] += (inputVolt/resistance);
}
void VoltEigen::solve() {
    cerr << "G = " << endl;
    for (size_t rowId = 0; rowId < _numNodes; ++ rowId) {
        GRBLinExpr current;
        for (size_t colId = 0; colId < _numNodes; ++ colId) {
            current += _G[rowId][colId] * _vVoltage[colId];
            cerr << _G[rowId][colId];
            if (colId < _numNodes - 1) {
                cerr << ", ";
            }
        }
        cerr << ";" << endl;
        _model.addConstr(current == _I[rowId], "I_constr_n" + to_string(rowId));
    }
    cerr << "I = " << endl;
    for (size_t rowId = 0; rowId < _numNodes; ++ rowId) {
        cerr << _I[rowId] << ";" << endl;
    }
    _model.optimize();
    for (size_t rowId = 0; rowId < _numNodes; ++ rowId) {
        _V[rowId] = _vVoltage[rowId].get(GRB_DoubleAttr_X);
        cerr << setprecision(15) << _V[rowId] << " ";
    }
}