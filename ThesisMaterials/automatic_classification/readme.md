## Automatic Classification

For feature selection and automatic classification steps of our experiments, we utilize [Weka](https://www.cs.waikato.ac.nz/ml/weka/) toolbox. Below, we describe the steps in order to perform the aforementioned machine learning steps of our experiments.

### 1-Obtaining Training and Test Subsets

After the feature extraction step, you can organize the obtained data in the following way:

* Each row represents an instance (audio file).
* Each column represents a feature.
* Column with the file names should be discarded.
* The last column for "Class" should be added.
* Document must be saved as ".csv". 

An example of the data organized in the described way can be found as XX.

After obtaining the data in a ".csv" file, we use python_file.py to create 10 pairs of training and test subsets in a stratified way regarding classes.

### 2-Creating ".arff" files

We can use the ARFF Toolbox of Weka in order to transform our ".csv" file into an ".arff" file.

### 3-Feature Selection

In our experiments, we use Correlation-based Feature Selection (CFS) algorithm that can be found in Weka. For more information of the feature selection step, you can check Section 2.3.2 of the thesis.

### 4-Automatic Classification

Automatic classification step of our experiments utilize a Multi-Layer Perceptron (MLP) model that can be found in Weka. For more information regarding hyperparameter values, you can check Section 2.3.3 of the thesis.

