#ifndef PHONATIONRTML_H
#define PHONATIONRTML_H

#include <vector>
#include <fstream>
#include <essentia/algorithmfactory.h>

using namespace std;
using namespace essentia;
using namespace essentia::standard;

class PhonationRTML
{

public:
    PhonationRTML(int samplingRate, int bufferSize);
    ~PhonationRTML();

    vector<double> computeFeatures(vector<Real> frame);
    vector<double> getProbs(vector<double> features);
    vector<double> normalizeAttributes(vector<double> features);
    Real getPitchConfidence(vector<Real> inputBuffer);

protected:

private:
    int iX;
    int circularBufferSize;
    int samplingRate;
    int bufferSize;
    vector<vector<double> > matrix;
    vector<vector<double> > weights1;
    vector<vector<double> > weights2;
    vector<double> attRanges;
    vector<double> attBases;

    Algorithm* yin;
    Real pitch;
    Real pitchConf;

    Algorithm* windowing;
    Algorithm* spectrum;
    Algorithm* mfcc;
    Algorithm* envelope;
    Algorithm* tempFlat;
    Algorithm* specFlat;
    Algorithm* centroid;

    vector<Real> frame;
    vector<Real> windowedFrame;
    vector<Real> envSignal;
    vector<Real> spec;
    vector<Real> mfccCoeffs;
    vector<Real> mfccBands;
    Real tempFlatCoef;
    Real specFlatCoef;
    Real specCentCoef;

    vector<double> features;
    vector<double> normFeatures;

    vector<double> probabilities;

    vector<double> tempFeatures;

};

#endif // PHONATIONRTML_H
