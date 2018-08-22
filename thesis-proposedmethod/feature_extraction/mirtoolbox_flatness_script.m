myDir = uigetdir; %directory of the audio files
myFiles = dir(fullfile(myDir,'*.wav'));

file_names = cell(1,1);
result = cell(1,2);

result{1,1} = 'TEMPORAL_FLATNESS';
result{1,2} = 'SPECTRAL_FLATNESS';

for k = 1:length(myFiles)
  baseFileName = myFiles(k).name;
  fullFileName = fullfile(myDir, baseFileName);
  
  file_names = [file_names; baseFileName];
  
  env = mirgetdata(mirenvelope(fullFileName));
  env2 = env(env~=0);
  tempf = geomean(env2)/mean(env2);
  
  spec = mirspectrum(miraudio(fullFileName));
  specf = mirgetdata(mirflatness(spec));
    
  tempResult = [tempf,specf];
  result = [result;num2cell(tempResult)];
end

all_result = [file_names result];

fid=fopen('mirtoolbox_flatness_results.csv','wt');
[rows,cols]=size(all_result);
for i=1:rows
      fprintf(fid,'%s,',all_result{i,1:end-1});
      fprintf(fid,'%s\n',all_result{i,end});
end
fclose(fid);
