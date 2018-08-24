myDir = uigetdir; %directory of the audio files
myFiles = dir(fullfile(myDir,'*.wav'));

file_names = cell(1,1);
mfcc_result = cell(1, 52);
rasta_result = cell(1, 52);
plp_result = cell(1,52);

for k = 1:13
    mfcc_result{1,k} = strcat('MFCC_', int2str(k-1), '_MEAN');
    mfcc_result{1,k+13} = strcat('MFCC_DEL_', int2str(k), '_MEAN');
    mfcc_result{1,k+26} = strcat('MFCC_', int2str(k-1), '_STD');
    mfcc_result{1,k+39} = strcat('MFCC_DEL_', int2str(k), '_STD');
    
    rasta_result{1,k} = strcat('RASTAPLP_', int2str(k-1), '_MEAN');
    rasta_result{1,k+13} = strcat('RASTAPLP_DEL_', int2str(k), '_MEAN');
    rasta_result{1,k+26} = strcat('RASTAPLP_', int2str(k-1), '_STD');
    rasta_result{1,k+39} = strcat('RASTAPLP_DEL_', int2str(k), '_STD');
    
    plp_result{1,k} = strcat('PLP_', int2str(k-1), '_MEAN');
    plp_result{1,k+13} = strcat('PLP_DEL_', int2str(k), '_MEAN');
    plp_result{1,k+26} = strcat('PLP_', int2str(k-1), '_STD');
    plp_result{1,k+39} = strcat('PLP_DEL_', int2str(k), '_STD');
end

for k = 1:length(myFiles)
  baseFileName = myFiles(k).name;
  fullFileName = fullfile(myDir, baseFileName);
  fprintf(1, 'Now reading %s\n', fullFileName);
  [x, fs] = audioread(fullFileName);
  
  file_names = [file_names;baseFileName];
  
  [ceps, a, p] = melfcc(x,fs,'lifterexp', 0, ...
                        'sumpower', 0, 'preemph', 0, ...
                        'maxfreq', 8000, 'nbands', 80, 'useenergy', 1);
  del = deltas(ceps);
  mat = [ceps; del];
  means = mean(mat,2);
  stdev = std(mat,0,2);
  tempMfccResult = transpose([means;stdev]);
  mfcc_result = [mfcc_result;num2cell(tempMfccResult)];
  
  [ceps, specs] = rastaplp(x,fs,1,12);
  del = deltas(ceps);
  mat = [ceps; del];
  means = mean(mat,2);
  stdev = std(mat,0,2);
  tempRastaResult = transpose([means;stdev]);
  rasta_result = [rasta_result;num2cell(tempRastaResult)];
  
  [ceps, specs] = rastaplp(x,fs,0,12);
  del = deltas(ceps);
  mat = [ceps; del];
  means = mean(mat,2);
  stdev = std(mat,0,2);
  tempPlpResult = transpose([means;stdev]);
  plp_result = [plp_result;num2cell(tempPlpResult)];
  
end

all_result = [file_names mfcc_result rasta_result plp_result];

fid=fopen('rastamat_results.csv','wt');
[rows,cols]=size(all_result);
for i=1:rows
      fprintf(fid,'%s,',all_result{i,1:end-1});
      fprintf(fid,'%s\n',all_result{i,end});
end
fclose(fid);
