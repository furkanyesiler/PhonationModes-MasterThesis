#ifndef PHONATIONRTPROCESS_H
#define PHONATIONRTPROCESS_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <stdlib.h>
#include <fstream>
#include "RtAudio.h"
#include <essentia/types.h>
#include <essentia/algorithmfactory.h>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

using namespace std;
using namespace essentia;
using namespace essentia::standard;

//! [0]
class PhonationRTProcess : public QThread
{
    Q_OBJECT

public:
    PhonationRTProcess(QObject *parent = 0);
    ~PhonationRTProcess();

    void render();
    void StartAudio();
    void initAudio();
    void DoInputAudio(double*);
    vector<double> getProb(vector<double>, vector<double>, vector<double>);
    //int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
    //           double /*streamTime*/, RtAudioStreamStatus status, void *data );

signals:
    void renderedImage(float upt_a, float upt_b, float upt_c, float upt_d);

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;
    RtAudio* adac;
    float a,b,c,d;

    Real pitchConfidence;
    vector<double> features;
    vector<double> probs;
    bool restart;
    bool abort;

    vector<Real> frame;
};

#endif // PHONATIONRTPROCESS_H
