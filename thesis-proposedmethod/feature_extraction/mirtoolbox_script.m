myDir = uigetdir; %directory of the audio files
myFiles = dir(fullfile(myDir,'*.wav'));

file_names = cell(1,1);
result = cell(1,4);

result{1,1} = 'TEMPORAL_FLATNESS';
result{1,2} = 'SPECTRAL_FLATNESS';
result{1,3} = 'SPECTRAL_FLUX_MEAN';
result{1,4} = 'SPECTRAL_FLUX_STD';

for k = 1:length(myFiles)
  baseFileName = myFiles(k).name;
  fullFileName = fullfile(myDir, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  
  file_names = [file_names; baseFileName];
  
  env = mirgetdata(mirenvelope(fullFileName));
  env2 = env(env~=0);
  tempf = geomean(env2)/mean(env2);
%     env = mirenvelope(fileName)
  spec = mirspectrum(fullFileName);
  flux = mirgetdata(mirflux(fullFileName));
  fluxm = mean(flux,2);
  fluxstd = std(flux,0,2);
%     
%     tempf = mirgetdata(mirflatness(env));
  specf = mirgetdata(mirflatness(spec));
    
  tempResult = [tempf,specf,fluxm,fluxstd];
  result = [result;num2cell(tempResult)];
end

all_result = [file_names result];

fid=fopen('mirtoolbox_results.csv','wt');
[rows,cols]=size(all_result);
for i=1:rows
      fprintf(fid,'%s,',all_result{i,1:end-1});
      fprintf(fid,'%s\n',all_result{i,end});
end
fclose(fid);