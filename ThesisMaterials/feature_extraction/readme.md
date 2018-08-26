## Feature Extraction

Feature extraction part of our experiments is done by utilizing 4 different toolboxes. Here, we give the links for each toolbox and share the required scripts in order to extract the features from given audio files.

Detailed information about the feature space used in our experiments can be found in Section 2.3 of the thesis. As mentioned in Subsection 2.3.1, we use the entire recordings for some features and the middle 600 ms for the others. For obtaining the trimmed versions of the recordings, a Matlab script shared as [matlab_trimming_script.m](https://github.com/furkanyesiler/PhonationModes-MasterThesis/blob/master/ThesisMaterials/feature_extraction/matlab_trimming_script.m) can be used. 

### RASTAMAT

This toolbox can be found [here](https://labrosa.ee.columbia.edu/matlab/rastamat/)

The Matlab script required to extract the MFCC, RASTA-PLP and PLP features by using this toolbox is shared as [rastamat_script.m](https://github.com/furkanyesiler/PhonationModes-MasterThesis/blob/master/ThesisMaterials/feature_extraction/rastamat_script.m). In our experiments, we use this script on the entire recordings, not the trimmed versions.

### MIRToolbox

This toolbox can be found [here](https://www.jyu.fi/hytk/fi/laitokset/mutku/en/research/materials/mirtoolbox)

The Matlab scripts required to extract the Temporal Flatness, Spectral Flatness and Spectral Flux features by using this toolbox are shared as [mirtoolbox_flatness_script.m](https://github.com/furkanyesiler/PhonationModes-MasterThesis/blob/master/ThesisMaterials/feature_extraction/mirtoolbox_flatness_script.m) and [mirtoolbox_flux_script.m](https://github.com/furkanyesiler/PhonationModes-MasterThesis/blob/master/ThesisMaterials/feature_extraction/mirtoolbox_flux_script.m). In our experiments, we use the script for Flatness features on the entire recordings and the script for Flux features on the trimmed versions of the recordings.

### Praat

This toolbox can be found [here](http://www.fon.hum.uva.nl/praat/)

The Praat scripts required to extract the Formants Information (A1-I1-B1, A2-I2-B2, A3-I3-B3) and LFCC features by using this toolbox are shared as [praat_formant_script.m](https://github.com/furkanyesiler/PhonationModes-MasterThesis/blob/master/ThesisMaterials/feature_extraction/praat_formant_script) and [praat_lfcc_script.m](https://github.com/furkanyesiler/PhonationModes-MasterThesis/blob/master/ThesisMaterials/feature_extraction/praat_lfcc_script). In our experiments, we use the script for Formants features on the entire recordings and the script for LFCC features on the trimmed versions of the recordings.

### ProsodyPro

This toolbox/script can be found [here](http://www.homepages.ucl.ac.uk/~uclyyix/ProsodyPro/)

The modified version of this script for extracting various "BID" features is shared as [ProsodyPro_modified.praat](https://github.com/furkanyesiler/PhonationModes-MasterThesis/blob/master/ThesisMaterials/feature_extraction/ProsodyPro_modified.praat). In our experiments, we use the modified script on the entire recordings. We modified the labeling part of the script so that it only takes the middle 600 ms to extract the features. After the feature extraction, we exclude "Interval", "F_dispersion1_5" and "median_pitch" columns of the BID.txt file.
