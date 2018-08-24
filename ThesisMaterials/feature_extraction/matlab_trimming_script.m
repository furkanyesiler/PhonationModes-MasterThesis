myDir = uigetdir; %directory of the audio files
myFiles = dir(fullfile(myDir,'*.wav'));

newSubFolder = sprintf('%s/%s', myDir, 'trimmed');

if ~exist(newSubFolder, 'dir')
  mkdir(newSubFolder);
end

for k = 1:length(myFiles)
    baseFileName = myFiles(k).name;
    fullFileName = fullfile(myDir, baseFileName);
    name = strsplit(baseFileName,'.');
    outnameS = "trimmed/" + name{1} + "_trimmed.wav";
    outname = char(outnameS);
    [y,fs] = audioread(fullFileName);
    dur = length(y)/fs;
    start = ((dur/2)-0.3)*fs;
    finish = (start + (0.6)*fs)-1;
    audiowrite(outname,y(start:finish),fs);
end
