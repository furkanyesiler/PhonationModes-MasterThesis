#include "PhonationRTML.h"

#define WEIGHTS_1 "/home/furkan/QtProjects/rtaudio_qt/rtaudio_qt/weights1.csv"
#define WEIGHTS_2 "/home/furkan/QtProjects/rtaudio_qt/rtaudio_qt/weights2.csv"

PhonationRTML::PhonationRTML(int samplingRate, int bufferSize)
{
    iX=0;
    circularBufferSize=10;

    this->samplingRate = samplingRate;
    this->bufferSize = bufferSize;

    matrix = vector<vector<double> >(15);
    for ( int i = 0 ; i < 15 ; i++ )
       matrix[i].resize(circularBufferSize);

    weights1 = readCsv(WEIGHTS_1);
    weights2 = readCsv(WEIGHTS_2);

    attRanges = {97.2192935944, 80.580078125, 61.1225547791,
                  52.6825819016, 53.7243423462, 40.2726516724,
                  46.9290485382, 50.6482563019, 39.9500141144,
                  37.4934139252, 44.0792140961, 41.3633155823,
                  17.9154675007, 19.0229183435, 13.1100130081,
                  11.2635518312,13.0440366268, 10.1134195328,
                  7.626193881, 9.1268429756, 5.9385545254,
                  6.8323633671, 7.8975787163, 5.6380690336,
                  0.0318813678, 0.1219996009, 0.0973847469};

    attBases = {69.8644008636, -13.8837356567, 21.0981178284,
                 -22.74746418, 4.9736480713, 3.8274040222,
                 5.8905200958, 11.5960865021, 4.8332157135,
                 3.5244655609, 13.6119213104, 6.9508476257,
                 22.4922130108, 22.9877208471, 16.1800680161,
                 14.9545725584, 16.3125841618, 13.0237264633,
                 10.0290490389, 11.9054903984, 8.7095625401,
                 9.495023489, 10.5834622383, 8.1516858339,
                 0.8343323185, 0.1493279319, 0.1278168049};


}

PhonationRTML::~PhonationRTML()
{

}

Real PhonationRTML::getPitchConfidence(vector<Real> inputBuffer)
{
    init();

    AlgorithmFactory& factory = AlgorithmFactory::instance();

    yin = factory.create("PitchYin", "sampleRate", this->samplingRate);

    yin->input("signal").set(inputBuffer);
    yin->output("pitch").set(pitch);
    yin->output("pitchConfidence").set(pitchConf);

    yin->compute();

    shutdown();

    return pitchConf;
}

vector<double> PhonationRTML::computeFeatures(vector<Real> frame)
{
    init();

    AlgorithmFactory& factory = AlgorithmFactory::instance();

    windowing = factory.create("Windowing",
                               "type", "hann",
                               "size", this->bufferSize/2,
                               "zeroPadding", this->bufferSize/2
                               );

    spectrum  = factory.create("Spectrum");

    mfcc  = factory.create("MFCC", "highFrequencyBound", 8000,
                                   "numberBands", 80,
                                   "inputSize", (this->bufferSize/2)+1,
                                   "sampleRate", this->samplingRate,
                                   "type", "magnitude"
                                   );
    envelope = factory.create("Envelope",
                              "sampleRate", this->samplingRate);
    tempFlat = factory.create("Flatness");
    specFlat = factory.create("Flatness");
    centroid = factory.create("Centroid");

    windowing->input("frame").set(frame);
    windowing->output("frame").set(windowedFrame);

    envelope->input("signal").set(frame);
    envelope->output("signal").set(envSignal);

    spectrum->input("frame").set(windowedFrame);
    spectrum->output("spectrum").set(spec);

    tempFlat->input("array").set(envSignal);
    tempFlat->output("flatness").set(tempFlatCoef);

    specFlat->input("array").set(spec);
    specFlat->output("flatness").set(specFlatCoef);

    centroid->input("array").set(spec);
    centroid->output("centroid").set(specCentCoef);

    mfcc->input("spectrum").set(spec);
    mfcc->output("bands").set(mfccBands);
    mfcc->output("mfcc").set(mfccCoeffs);

    windowing->compute();
    spectrum->compute();
    envelope->compute();
    tempFlat->compute();
    specFlat->compute();
    centroid->compute();
    mfcc->compute();

    features = vector<double>(27);

    for(int i=0; i<15;i++){
        if (i<12){
            matrix[i][iX]=mfccCoeffs[i+1];
            features[i]=average(matrix[i]);
            features[i+12]=stand_dev(features[i], matrix[i]);
        } else if(i==12){
            matrix[i][iX]=tempFlatCoef;
            features[24]=average(matrix[i]);
        } else if(i==13){
            matrix[i][iX]=specFlatCoef;
            features[25]=average(matrix[i]);
        } else{
            matrix[i][iX]=specCentCoef;
            features[26]=average(matrix[i]);
        }
    }
    iX = (iX+1)%bufferSize;

    normFeatures = vector<double>(27);
    normFeatures = normalizeAttributes(features);

    delete windowing;
    delete envelope;
    delete tempFlat;
    delete specFlat;
    delete centroid;
    delete spectrum;
    delete mfcc;

    shutdown();

    return normFeatures;
}

vector<double> PhonationRTML::getProbs(vector<double> features)
{
    probabilities = vector<double>(4);

    for(int k=0; k<probabilities.size(); k++){
        for (int i=0;i<weights1.size();i++){
            double d = 0;
            double d_sig =0;
            for(int j=0; j<features.size();j++){
                d += features[j]*weights1[i][j];
            }
            d+=weights1[i][weights1[i].size()-1];
            d_sig = 1/(1+exp((-1)*d));
            probabilities[k] += d_sig*weights2[k][i];
        }
        probabilities[k]+=weights2[k][weights2[k].size()-1];
        probabilities[k] = 1/(1+exp((-1)*probabilities[k]));
    }

    double probs_sum = accumulate(probabilities.begin(), probabilities.end(), 0.0);
    for(int k=0; k<4; k++){
        probabilities[k]=probabilities[k]/probs_sum;
    }

    return probabilities;
}

vector<double> PhonationRTML::normalizeAttributes(vector<double> features){

    for(int i=0; i<features.size(); i++){
        double featureMax = attBases[i]+attRanges[i];
        double featureMin = attBases[i]-attRanges[i];

        if(features[i]>featureMax){
            featureMax = features[i];
        }else if(features[i]<featureMin){
            featureMin = features[i];
        }else{

        }

        attBases[i] = (featureMax+featureMin)/2;
        attRanges[i] = (featureMax-featureMin)/2;
        tempFeatures[i]= (features[i]-attBases[i])/attRanges[i];
    }

    return tempFeatures;

}
