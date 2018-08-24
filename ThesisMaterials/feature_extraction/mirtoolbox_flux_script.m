myDir = uigetdir; %directory of the audio files
myFiles = dir(fullfile(myDir,'*.wav'));

file_names = cell(1,1);
result = cell(1,2);

result{1,1} = 'SPECTRAL_FLUX_MEAN';
result{1,2} = 'SPECTRAL_FLUX_STD';

for k = 1:length(myFiles)
  baseFileName = myFiles(k).name;
  fullFileName = fullfile(myDir, baseFileName);
  
  file_names = [file_names; baseFileName];
  
  flux = mirgetdata(mirflux(fullFileName));
  fluxm = mean(flux,2);
  fluxstd = std(flux,0,2);
    
  tempResult = [fluxm,fluxstd];
  result = [result;num2cell(tempResult)];
end

all_result = [file_names result];

fid=fopen('mirtoolbox_flux_results.csv','wt');
[rows,cols]=size(all_result);
for i=1:rows
      fprintf(fid,'%s,',all_result{i,1:end-1});
      fprintf(fid,'%s\n',all_result{i,end});
end
fclose(fid);
