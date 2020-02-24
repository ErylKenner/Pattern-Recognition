#include <bits/stdc++.h> 
#include "ClassifierCase1.h"


ClassifierCase1::ClassifierCase1(const std::vector<VectorXf> &_means,
      const std::vector<MatrixXf> &_covariances,
      const std::vector<float> &_priorProb)
    : ClassifierBase(_means, _covariances, _priorProb){
    W = means[0] - means[1];
    X0 = 0.5f * (means[0] + means[1]) - (covariances[0](0, 0) * covariances[0](0, 0)) / W.dot(W.transpose()) * logf(priorProb[0] / priorProb[1])*W;
}

//Returns the index of the class the input belongs to
int ClassifierCase1::Classify(const VectorXf &input){
    if(W.dot(input - X0) >= 0.0f){
        return 0;
    } else{
        return 1;
    }
}

//Returns the parameters for the decision boundary
PlotParams ClassifierCase1::GetPlotParams(){
    PlotParams ret;
    ret.a = 0;
    ret.b = -W(0, 0) / W(1, 0);
    ret.c = X0(1, 0) - ret.b * X0(0, 0);
    return ret;
}

//Returns the maximum error of the classifier
/*
float ClassifierCase1::GetErrorBound(){
    return 0.0f; //TODO: implement this
}
*/