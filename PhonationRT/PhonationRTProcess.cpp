#include "PhonationRTProcess.h"
#include "RtAudio.h"
#include <cmath>
#include <ctime>

#define SAMPLING_RATE 96000
#define BUFFER_SIZE 4096
#define CIRCULAR_BUFFER_SIZE 10
typedef double MY_TYPE;
#define FORMAT RTAUDIO_FLOAT64
#define WEIGHTS_1 "/home/furkan/QtProjects/PhonationRT/PhonationRT/weights1.csv"
#define WEIGHTS_2 "/home/furkan/QtProjects/PhonationRT/PhonationRT/weights2.csv"
#define ATT_RANGES "/home/furkan/QtProjects/PhonationRT/PhonationRT/attRanges.csv"
#define ATT_BASES "/home/furkan/QtProjects/PhonationRT/PhonationRT/attBases.csv"

static int iX=0;
static vector<vector<double> > matrix(15);
static vector<vector<double> > weights1;
static vector<vector<double> > weights2;
static vector<double> attRanges;
static vector<double> attBases;

double average(const vector<double>& numbers)
{
    return accumulate(numbers.begin(), numbers.end(), 0.0) / numbers.size();
}

double stand_dev(const double mean, const vector<double>& numbers)
{
    double total = 0;
    for_each (begin(numbers), end(numbers), [&](const double d) {
        total += (d - mean) * (d - mean);
    });

    return sqrt(total / (numbers.size() - 1));
}

vector<vector<double> > read2DCsv(string filename){
    ifstream file(filename);
    string line;
    vector<vector<double> > data;

    while(getline(file, line)){
        vector<double> row;
        stringstream iss(line);
        string val;

        while(getline(iss, val, ',')){
            stringstream ss2(val);
            double _val;
            ss2 >> _val;
            row.push_back(_val);
        }
        data.push_back(row);
    }


    return data;
}

vector<double> read1DCsv(string filename){
    ifstream file(filename);
    string line;
    vector<double> data;

    while(getline(file, line)){
        vector<double> row;
        stringstream iss(line);
        string val;

        while(getline(iss, val, ',')){
            stringstream ss2(val);
            double _val;
            ss2 >> _val;
            data.push_back(_val);
        }
    }

    return data;
}

vector<double> normalizeAttributes(vector<double> features){

    vector<double> tempFeatures(27);

    vector<double> attRanges = read1DCsv(ATT_RANGES);
    vector<double> attBases = read1DCsv(ATT_BASES);

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

vector<double> computeFeatures(vector<Real> frame)
{
    essentia::init();

    AlgorithmFactory& factory = AlgorithmFactory::instance();

    Algorithm* windowing = factory.create("Windowing",
                               "type", "hann",
                               "size", BUFFER_SIZE/2,
                               "zeroPadding", BUFFER_SIZE/2
                               );

    Algorithm* spectrum  = factory.create("Spectrum");

    Algorithm* mfcc  = factory.create("MFCC", "highFrequencyBound", 8000,
                                   "numberBands", 80,
                                   "inputSize", (BUFFER_SIZE/2)+1,
                                   "sampleRate", SAMPLING_RATE,
                                   "type", "magnitude"
                                   );
    Algorithm* envelope = factory.create("Envelope",
                              "sampleRate", SAMPLING_RATE);
    Algorithm* tempFlat = factory.create("Flatness");
    Algorithm* specFlat = factory.create("Flatness");
    Algorithm* centroid = factory.create("Centroid");

    vector<Real> windowedFrame;
    vector<Real> envSignal;
    vector<Real> spec;
    vector<Real> mfccCoeffs;
    vector<Real> mfccBands;
    Real tempFlatCoef;
    Real specFlatCoef;
    Real specCentCoef;

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

    vector<double> features(27);

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
    iX = (iX+1)%CIRCULAR_BUFFER_SIZE;

    vector<double> normFeatures(27);
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

vector<double> getProbs(vector<double> features)
{
    vector<double> probabilities(4);

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

PhonationRTProcess::PhonationRTProcess(QObject *parent)
    : QThread(parent)
{
    for ( int i = 0 ; i < 15 ; i++ )
       matrix[i].resize(CIRCULAR_BUFFER_SIZE);

    weights1 = read2DCsv(WEIGHTS_1);
    weights2 = read2DCsv(WEIGHTS_2);

    initAudio();
    StartAudio();
}

PhonationRTProcess::~PhonationRTProcess()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void PhonationRTProcess::render()
{
    QMutexLocker locker(&mutex);

    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = false;
        condition.wakeOne();
    }
}

void PhonationRTProcess::run()
{
    forever {
        mutex.lock();
        a = this->a;
        b = this->b;
        c = this->c;
        d = this->d;
        mutex.unlock();

        emit renderedImage(a,b,c,d);
        mutex.lock();

        if (!restart)
            condition.wait(&mutex);
        restart = false;
        mutex.unlock();
    }
}

void PhonationRTProcess::DoInputAudio(double* inputBuffer)
{
    essentia::init();

    AlgorithmFactory& factory = standard::AlgorithmFactory::instance();

    Algorithm* yin = factory.create("PitchYin", "sampleRate", SAMPLING_RATE);

    vector<Real> frame;
    Real pitch, pitchConf;
    for (int i=1; i<BUFFER_SIZE; i++) {
        frame.push_back(inputBuffer[i]);
        i++;
    }

    yin->input("signal").set(frame);
    yin->output("pitch").set(pitch);
    yin->output("pitchConfidence").set(pitchConf);

    yin->compute();

    essentia::shutdown();

    if(pitchConf > 0.7){
        features = computeFeatures(frame);
        probs = getProbs(features);

        this->a = probs[3];
        this->b = probs[2];
        this->c = probs[1];
        this->d = probs[0];
    }else{
        this->a = 0;
        this->b = 0;
        this->c = 0;
        this->d = 0;
    }
}

int record( void */*outputBuffer*/, void *inputBuffer,
                               unsigned int /*nBufferFrames*/,
                               double /*streamTime*/,
                               RtAudioStreamStatus status,
                               void *data )
{
  PhonationRTProcess *proc = (PhonationRTProcess*)data;
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;

  proc->DoInputAudio((double*)inputBuffer);

  return 0;
}

void PhonationRTProcess::initAudio()
{
    unsigned int channels, fs, bufferBytes, iDevice = 0;

    adac = new RtAudio();
    if ( adac->getDeviceCount() < 1 ) {
      std::cout << "\nNo audio devices found!\n";
      exit( 1 );
    }

    fs = SAMPLING_RATE;

    // Let RtAudio print messages to stderr.
    adac->showWarnings( true );

    // Set the same number of channels for both input and output.
    unsigned int bufferFrames = BUFFER_SIZE;
    RtAudio::StreamParameters iParams;
    iParams.deviceId = 0;
    iParams.nChannels = 1;
    iParams.firstChannel = 0;

    if ( iDevice == 0 )
      iParams.deviceId = adac->getDefaultInputDevice();

    RtAudio::StreamOptions options;
    //options.flags |= RTAUDIO_NONINTERLEAVED;

    bufferBytes = bufferFrames * channels * sizeof( MY_TYPE );

    try {

      adac->openStream( NULL, &iParams, FORMAT, fs, &bufferFrames, &record, (void *)this, &options );
    }
    catch ( RtAudioError& e ) {
      std::cout << '\n' << e.getMessage() << '\n' << std::endl;
      exit( 1 );
    }
}

void PhonationRTProcess::StartAudio(){
    try
    {
      if (adac != NULL)
      {
        if (!adac->isStreamRunning())
          adac->startStream();
      }
    }
    catch ( RtAudioError& e )
    {
      std::cout << '\n' << e.getMessage() << '\n' << std::endl;
      if ( adac->isStreamOpen() ) adac->closeStream();
    }
}
