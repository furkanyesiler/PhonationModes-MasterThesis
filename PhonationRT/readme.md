## PhonationRT - A Visual Feedback Prototype for Phonation Modes in Singing

The information regarding the system architecture of PhonationRT can be found in Section 4.1 of the [thesis](https://github.com/furkanyesiler/PhonationModes-MasterThesis/blob/master/MasterThesis-FurkanYesiler.pdf). The shared source code is written in C++ with Qt Creator, and compiled in Linux only.

PhonationRT uses RtAudio, Essentia and Qt for audio input, feature extraction and GUI, respectively. The required libraries to compile the source code are as follows: 

**RtAudio**
> lasound lpthread

These libraries are required to use LINUX ALSA driver for compiling the project. In order to compile the project in Windows, Mac etc. you can check the required libraries from the [RtAudio website](https://www.music.mcgill.ca/~gary/rtaudio/).

**Essentia**
> lessentia lfftw3 lyaml lavcodec lavformat lavutil lavresample lsamplerate ltag lfftw3f lchromaprint

The libraries above except "lessentia" are required to compile Essentia. Fewer libraries can be used while compiling Essentia and you can find more information in the [Essentia website](http://essentia.upf.edu/documentation/installing.html)

**Qt**
> charts widgets core gui

These libraries are installed when you install Qt to your system.

For more information about the required libraries, you can check [PhonationRT.pro].

### Multi-Layer Perceptron model
The MLP model used in this code is created and trained with Weka by using DS-C as the training set and following non-default hyperparameters: **Learning Rate:** 0.01 **Momentum Coefficient:** 0.5 **Epoch:** 2000 **Hidden Layer Size:** 17 

The source code shared is a work in progress and will be regularly updated in the following months.
