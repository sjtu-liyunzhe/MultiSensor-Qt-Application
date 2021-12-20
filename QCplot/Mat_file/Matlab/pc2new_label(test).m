%~ generate pc2new_label.txt
%~ ע�⣺pca��άʱ����Ҫ��֤��������������ά�ȣ���MATLAB�����ʹ��2��������mean and std���Ļ�����Ҫ��c++��Ŀ�вɼ�֡������768������ά�ȣ�
%% ~ read original data of ultrasound
function [pc1new,pc2new]=pc2new_label()
currentParametersSettings='mean';
sample_dots = 1000; %~ ÿ��ͨ����������1000����
chnum = 8;%~ 8��ͨ��
file_num = 1;
raw_data = cell(file_num, chnum); % videonum x chnum %~ ����1*8�Ŀվ������ݽṹ��cell��
fr_num = NaN;

s=pwd;%~ ��ȡ��ǰ.m�ļ�����·��

us_dir  = ('E:\Code\����ͬ��ʶ�����\QT_2threads_36(WiFi)\QCplot\Mat_file');%~ �����ź��ļ�·��������λ������еĻ����Ƿ�����һ��Ŀ¼��
cd (us_dir)
% dataglove=load('dataglove.txt');%~ ��ȡ�������׵�ֵ

file_list = dir('*1.bin');%~ dir�������г��ļ��е�����
pos_ = find(file_list.name == '_');%~ �ҵ��»�������λ��
pos_ = pos_(2);%~ �ڶ����»��ߵ�λ��
tic %~ ��ʼ��ʱ
for file_no = 1 : file_num
    fr_num = NaN;  % some files may have different frame number
    for ch_no = 1:chnum
        fp = fopen([file_list(file_no).name(1:pos_) num2str(ch_no) '.bin'], 'r');%~ ����������Ϊ�˸����ļ���������ʽ������ѭ����ȡ����
        foobar_raw = fread(fp);%~ fread �Ƕ�ȡ�������ļ�Ϊһ�У��˴�foobar_raw Ϊ2000000*1������
        if isnan(fr_num)                                     % first run in first loop
            fr_num = floor(length(foobar_raw)/sample_dots);
        else                                                        % not first loop
            if fr_num ~= floor(length(foobar_raw)/sample_dots) 
                error 'line numbers not compatible.';
            end
        end
        display(['line number is ' num2str(fr_num)]);
        foobar_raw= foobar_raw(1:fr_num*sample_dots);
        result = reshape(foobar_raw, sample_dots, fr_num)';
        fclose(fp);
        raw_data{file_no , ch_no} = result;%~ ����õ���raw_data�еľ�����2000֡���ɼ����ݵ�ʱ����200s��
    end
end
toc
cd(s)
warning 'Remeber to check the length of each matrix!'
disp 'loading finishes '

%~ combine raw data
raw_data_combine1 = cell(fr_num ,1);
disp 'start combining '
temp = zeros(chnum, sample_dots);
for i = 1: fr_num
   for j = 1: chnum
       temp(j,:) = raw_data{1, j}(i, :);
   end
   raw_data_combine1{i, 1} = temp; %~ �������ɵ�data_combineԪ������Ϊ2000*1��ÿ��cellΪ8����ͨ����*1000�ľ���
end
disp 'combine ending '

%% ~ progress data
win_length = 20;%~ ����
m = 1;
switch_len = 20; %~ �޳�ÿһ��ѵ�����ݵ���β�����ݣ����������л����̵�Ӱ�졣��������Ϊ20�����޳�ÿһ��ѵ�����ݵ�ǰ���2s

for i = 1: length(raw_data_combine1)%~ �ܹ���֡����100ms/֡
    temp = raw_data_combine1{i};%~ tempҲ��cell��ʽ���˴�����һ֡һ֡��ȡraw_data_combine1�е�����
    Feature(m, :) = ExFeature(temp, win_length ,currentParametersSettings);%~ ��ȡÿһ֡��������һ֡�����洢��Feature��Ϊһ�У��������д洢����Feature��2000*384�����У���Feature����Ϊ����.bin�ļ���ѵ�����ݼ��������������������ϣ�
    m = m+1;%~ Feature���м���
end
Feature_len = size(Feature,1);

part_len = Feature_len/3;%%%%%%%%%%%%%%%%%%%%%%%% ���׶�ѵ����ÿ�׶ζ�Ӧ���ݳ���
% part_len = Feature_len/4;%%%%%%%%%%%%%%%%%%%%%%%% �Ľ׶�ѵ��

Feature_tra = [];%~ ����ѵ����A����������
% per_len = 500;%~ 500֡���ݣ���Ӧÿ���׶ε�50sʱ�䡣ȫ����per length����ÿ���׶εĳ���
Feature_tra = Feature;

%~ ������ĸ��׶ε�ѵ�����ݣ�����ȥ�����ĸ��׶Σ�trial4�����Ƚ϶����ӣ������ݣ�ֻ����ǰ�����׶Σ�
%~ ����������׶ε�ѵ�����ݣ�Ҳ����Ҫ���иĶ���
Feature_tra = Feature_tra(1:part_len*3, :);

%%~ filter ����λǰ���������˲���
fc = 1;
fs = 10;
[b, a] = butter(5, fc/(fs/2));%~ ���ؾ��й�һ����ֹƵ�ʵ����׵�ͨ���ְ�����˹�˲����Ĵ��ݺ���ϵ��
%~ �˲��������Ƿ�ֹ��ʱ���ڵ�������ֵ�ĳ��֣������ţ����Ӷ����º���ȡ���岨��������
Feature_tra = filtfilt(b, a, Feature_tra);%~ filtfilt����ͨ��x������ͷ������������ϴ�������������ִ������λ�����˲� ������ǰ�������ݺ�filtfilt��ת���˺�����У�Ȼ�����˻ع�������

%%~ ������ѵ����������������pca��ά
%Training with Auto Labeling 
[coef, score, latent, ~, ~, mul] = pca(Feature_tra); %~ score��X���󣨴˴��� Feature_tra �������ɷֿռ��еı�ʾ����ԭ�������¹��ɵ����ɷֿռ��ӳ�䡣ÿ�ж�Ӧ�����۲�ֵ��ÿ�ж�Ӧһ�����ɷ�(����)����������ԭ����X��ͬ
pc1 = score(:, 1);
pc2 = score(:, 2);
%nor
pc1 = mapminmax(pc1', 0, 1)';
pc2 = mapminmax(pc2', 0, 1)';%~ ͨ����ÿ�е���Сֵ�����ֵ��һ��Ϊ[0,1]���������

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%?????��һС�δ��ɣ�����ͨ���ж�pc1��һ�׶ξ�ֵ��pc2�ڶ��׶εľ�ֵ���Ƿ���ӽ���0/1�����ж��Ƿ���з�ת���򵥴ֱ���ʵ��Ч�������������Ե�
pc1inverse = 1-pc1;
pc2inverse = 1-pc2; 
% %~
% R2ֵ�����ϵ����Խ�ӽ���1���Ǻϳ̶�Խ�ߡ�����Ƚ�R2ֵ���ж��Ƿ���Ҫ����һ��������ݷ�ת���ʣ���һ������ô��c++��ʵ�ֵģ��������û��dataglove��ô��
% if(calculateR2(pc2,Glove_tra)<calculateR2(pc2inverse,Glove_tra))
%     pc2 = pc2inverse;
% end
%~ ���ﳢ�����������Glove_tra�ıȽϣ�pc1��һ�׶εľ�ֵ��0�ӽ���pc2�ڶ��׶εľ�ֵ��0�ӽ�
if((mean(pc1(1+switch_len:part_len-switch_len))-0.5)>0)
    pc1 = pc1inverse;
end
if((mean(pc2(part_len+1+switch_len:part_len*2-switch_len))-0.5)>0)
    pc2 = pc2inverse;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%~ pc1��ͼ
figure; 
plot(pc1); 
title('PC#1 vs. Wrist Rotation - Original'); xlabel('time'); ylabel('Angle');legend('PC#1')
%%~ pc1��ͼ
figure; 
plot(pc2); 
title('PC#2 vs. Hand Closing - Original'); xlabel('time'); ylabel('Angle');legend('PC#2')

%%~ pc1��һ�׶���������
pc1(1+switch_len:part_len-switch_len) = 0;
%%~ pc1�ڶ��׶���������
temp = pc1(part_len+1+switch_len:part_len*2-switch_len);
temp = mapminmax(temp', 0, 1)';
pc1(part_len+1+switch_len:part_len*2-switch_len) = temp;
%~ pc1�׶����ݽ�ȡ
pc1hc = pc1(1+switch_len: part_len-switch_len, :);
pc1wr = pc1(part_len+1+switch_len: part_len*2-switch_len, :);
pc1com = pc1(part_len*2+1+switch_len: part_len*3-switch_len, :);
pc1new=[pc1hc; pc1wr; pc1com];

%%~ train_feature���ݽ�ȡ
% train_feature_hc = train_feature(1+switch_len: part_len-switch_len, :);
% train_feature_wr = train_feature(part_len+1+switch_len: part_len*2-switch_len, :);
% train_feature_com = train_feature(part_len*2+1+switch_len: part_len*3-switch_len, :);
% train_feature_new=[train_feature_hc; train_feature_wr; train_feature_com];

%%~ pc1��ͼ
figure; 
plot(pc1new); 
title('PC#1 vs. Wrist Rotation - prior'); xlabel('time'); ylabel('Angle');legend('PC#1')

%%~ pc2�ڶ������׶������������������׶���Ҫ������ι�һ�����پ������������Ч����
pc2(part_len+1+switch_len:part_len*2-switch_len) = 0;
%re normalization %~ �����׶����ݵ����ٴι�һ��
temp = pc2(part_len*2+1+switch_len:part_len*3-switch_len);
temp = mapminmax(temp', 0, 1)';
pc2(part_len*2+1+switch_len:part_len*3-switch_len) = temp;

%%~ ��ǳ���һ�׶� PC2���ݵķ�ֵ�͹�ֵ
% fre prior
figure;
pc2hc = pc2(1+switch_len: part_len-switch_len, :);%~ ��һ�׶Σ�hand closing��PC2����
[pk,lkp] = findpeaks(pc2hc, 'MinPeakHeight',0.4);%~ findpeaks���Ҿֲ����ֵ������pkΪ��ֵ��lkp�Ƿ�ֵ������
plot(1+switch_len:part_len-switch_len,pc2hc,lkp+switch_len,pk,'o')%~ �����ֵ��ǳ���'o'
hold on;
[pk,lkr] = findpeaks(-pc2hc);%~ lkr�ǹ�ֵ������
plot(lkr+switch_len,-pk,'*') %~ ����Сֵ��ǳ���'*'
hold off;
title('PC#2 ��һ�׶�'); xlabel('time'); ylabel('Angle');

unionset = union(lkr, lkp);%~ �����ֵ�͹�ֵ�Ĳ�������С������������

%%~ ����һ�׶Σ����������õ��Ĳ���Ͳ��ȣ������ڵĲ���Ͳ���֮������ݣ����¹�һ��
for i = 2: length(unionset)-1 
    backward = unionset(i-1):unionset(i);%~ ���ڲ���Ͳ���֮������е�index
    forward = unionset(i):unionset(i+1);
    pc2hc(backward) = mapminmax(pc2hc(backward)',0, 1)';
    pc2hc(forward) = mapminmax(pc2hc(forward)',0, 1)';
end

%%~ ��ǳ������׶� PC2���ݵķ�ֵ�͹�ֵ
pc2wr = pc2(part_len+1+switch_len: part_len*2-switch_len);%~ �ڶ��׶�wristrotationΪ0
pc2com = pc2(part_len*2+1+switch_len: part_len*3-switch_len);%~ �����׶�ͬ���˶�����
figure;
[pk,lkp] = findpeaks(pc2com, 'MinPeakDistance', 6, 'MinPeakHeight',0.4);
plot(1+switch_len:part_len-switch_len,pc2com,lkp+switch_len,pk,'o')
hold on;
[pk,lkr] = findpeaks(-pc2com, 'MinPeakDistance', 6);
plot(lkr+switch_len,-pk,'*')
hold off;
title('PC#2 �����׶�'); xlabel('time'); ylabel('Angle');

%%~ �������׶Σ����������õ��Ĳ���Ͳ��ȣ������ڵĲ���Ͳ���֮������ݣ����¹�һ��
unionset = union(lkr, lkp);
for i = 2: length(unionset)-1
    backward = unionset(i-1):unionset(i);
    forward = unionset(i):unionset(i+1);
    pc2com(backward) = mapminmax(pc2com(backward)',0, 1)';
    pc2com(forward) = mapminmax(pc2com(forward)',0, 1)';
end

%%~ pc2new�ǵڶ������׶δ�����pc2��ԭʼ�ڶ����ɷ���������������ͼ
pc2new = [pc2hc; pc2wr; pc2com];
figure; 
plot(pc2new); 
% hold on;
% plot(dataglove);
title('PC#2 vs. Hand Closing - With Fre Prior'); xlabel('time'); ylabel('Angle');

%%~ ��pc2newд�뵽txt�ļ��У�������λ�������c++����ȡ
fid1=fopen('pc2new_label.txt','wt');
%dataglove_label=single(dataglove);%~ �����Ƿ���Ҫ��ʽ��pc2new��double����ת����float����
fprintf(fid1,'%.8f\n',pc2new);%~ ����dataglove��������ͨ���ٻ���pc2new������
fclose(fid1);

%%~
%%�����������Իع�LR��beta���󣬴洢��txt�ļ��У�����λ�������ȡ������ͨ��MATLAB���洫�䵽c++�����������Ҫע��ά����������⣻�˴�ѡ��250��ά�ȣ�Ҫ����λ�������pca��ά���ά����ͬ
score1 = score(1+switch_len:part_len-switch_len,1:250);
score2 = score(part_len+1+switch_len: part_len*2-switch_len,1:250);
score3 = score(part_len*2+1+switch_len: part_len*3-switch_len,1:250);
projectedTrain = [score1; score2; score3];
featurerow = size(projectedTrain,1);%~ ��ȡprojectedTrain������
X=[projectedTrain ones(featurerow,1)];%~ ˮƽƴ�Ӿ���
Y_pc1=pc1new;
Y_pc2=pc2new;
beta_pc1=inv((X')*X)*(X')*Y_pc1;
beta_pc2=inv((X')*X)*(X')*Y_pc2;

% fid2=fopen('beta.txt','wt');
% fprintf(fid2,'%f\n',beta);
% fclose(fid2);

%%~ ���������ݺ󣬽��ļ��е������ƶ��������ݡ��ļ��У�������һ������д��ʹ���
cd (us_dir) %~ ��ת����һ���ļ���
file_move = dir('*.bin');%~ dir�������г��ļ��е�����
for i=1:size(file_move,1)
    movefile(file_move(i).name,'./����');
end

file_move = dir('*.txt');
for i=1:size(file_move,1)
    movefile(file_move(i).name,'./����');
end

cd(s)%~ ��ת��.m����������ļ���

opts = struct('WindowStyle','modal',... 
              'Interpreter','tex');
warndlg('\fontsize{15}\color{red} �������ɳɹ�',...
             '��ʾ', opts);
% return
end
         
         
%% ~ functions
function [ vFeature ] = ExFeature( data, win_length  ,currentParametersSettings)
% UNTITLED3 Summary of this function goes here
% Output is a line vector whose every 3 componenets stand for an ROI.
% Input is voltage amplitude in a same frame, one channel each line .
% based on \B test\2_April\feature_j.m

% parameters definition
chnum = size(data, 1);
sample_length = 1000; % num of dots sampled
sample_depth = 38.5e-3;
sample_depth = repmat(sample_depth, 1, chnum);
sampling_freq = 20e6;
sampling_freq = repmat(sampling_freq, 1, chnum);
zero_delay = 0e-6; % lingwei yanchi in seconds
sound_speed = 1540; % m/s

range = win_length;
burst_tone_freq = 5e6; % !!IMPORTANT!! used in tgc coefficient calc and Gaussian filter mid-freq
%ǰ���е���20��ûʲô����
start = 20; % start of x
endo = 20; % end of x, avoid gaussian filter jitter
tgc_alpha = 0.05; % [dB/(MHz cm)]   
sampling_balance = 128; % median of sampling range  %ѡ��128����
filter_bandwidth = 110; % in percentage 
log_comp_ratio = 0.4;

% Declaration and error handling
mDepth = zeros(chnum, sample_length);
for i = 1 : chnum % channel_no
    mDepth(i, :) = (1 : sample_length) / sample_length * sample_depth(i); % mDepth contains depth value for each dot, each line for a ch
    mDepth(i, :) = sound_speed * zero_delay + mDepth(i, :);
    tgc(i, :) = exp(tgc_alpha * burst_tone_freq / 1e6 * mDepth(i, :) * 100);
%     foobar_t(i) = 1 : sample_length(i);
end

% d = fdesign.bandpass('N,F3dB1,F3dB2', 6, 2e6, 6e6, sampling_freq); 
% Hd = design(d,'butter');

vFeature = [];
fit_foobar = 1 : range; %for fit only

% if start < range 
%     error('x_start must be greater than radius');
% end
if sample_length ~= size(data, 2)
    error('sample length mismatched.')
end

%  process start
for i = 1 : chnum % channel_no
    line = bsxfun(@times, tgc(i, :), data(i, :) - sampling_balance); % wow, how should you know
    line = gaussianFilter(line, sampling_freq(i), burst_tone_freq, filter_bandwidth); 
    line = envelopeDetection(line);
%     line = abs(line);
%     line = smooth(line, 10)'; % smooth
    line = logCompression(line, log_comp_ratio);
    %line = line(1: 200);
%     line = line;
% { 
%normal code part
for tail = start + range - 1 : range : (length(line) - endo)
    xcor = (tail - range + 1) : tail;% x coordinates in a typical segment
    %������������
    %foobar = polyfit(fit_foobar, line(xcor), 1);
    %foobar = polyfit(fit_foobar, line(xcor), 3); %Ч����Щ��
    %ARģ�������
    %m = ar(line(xcor),4,'ls');
    %m = ar(line(xcor),6);
    %foobar = m.A;
    %vFeature = [vFeature foobar]; % IMPORTANT!!!! 
    if strcmp(currentParametersSettings, 'mean and variance')
        vFeature = [vFeature mean(line(xcor)') std(line(xcor)')];
    else if strcmp(currentParametersSettings, 'mean')
        vFeature = [vFeature mean(line(xcor)') ];
        end
    end
    %max min diff
    %vFeature = [vFeature max(line(xcor)') min(line(xcor)') sum(abs(diff(line(xcor))))];
    %vFeature = [vFeature skewness(line(xcor)') kurtosis(line(xcor)')];
    %vFeature = [vFeature ; [mean(line(xcor)') std(line(xcor)') max(line(xcor)') min(line(xcor)') sum(abs(diff(line(xcor)))) skewness(line(xcor)') kurtosis(line(xcor)')]];
end
end
end

% vFeature = vFeature(:)';

function signal = gaussianFilter(signal, Fs, freq, bandwidth, plot_filter)
%GAUSSIANFILTER   Filter signals using a frequency domain Gaussian filter.
%
% DESCRIPTION:
%       gaussianFilter applies a frequency domain Gaussian filter with the
%       specified center frequency and percentage bandwidth to the input
%       signal. If the input signal is given as a matrix, the filter is
%       applied to each matrix row. 
%
% USAGE:
%       signal = gaussianFilter(signal, Fs, freq, bandwidth)
%       signal = gaussianFilter(signal, Fs, freq, bandwidth, plot_filter)
%
% INPUTS:
%       signal      - signal/s to filter
%       Fs          - sampling frequency [Hz]
%       freq        - filter center frequency [Hz]
%       bandwidth   - filter bandwidth [%]
%
% OPTIONAL INPUTS:
%       plot_filter - Boolean controlling whether the filtering process is
%                     plotted
%
% OUTPUTS:
%       signal      - filtered signal/s
%
% ABOUT:
%       author      - Bradley Treeby
%       date        - 10th December 2011
%       last update - 2nd September 2012
%
% This function is part of the k-Wave Toolbox (http://www.k-wave.org)
% Copyright (C) 2009-2014 Bradley Treeby and Ben Cox
%
% See also applyFilter, filterTimeSeries, gaussian

% This file is part of k-Wave. k-Wave is free software: you can
% redistribute it and/or modify it under the terms of the GNU Lesser
% General Public License as published by the Free Software Foundation,
% either version 3 of the License, or (at your option) any later version.
% 
% k-Wave is distributed in the hope that it will be useful, but WITHOUT ANY
% WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
% FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
% more details. 
% 
% You should have received a copy of the GNU Lesser General Public License
% along with k-Wave. If not, see <http://www.gnu.org/licenses/>. 

% suppress m-lint warnings for unused assigned values
%#ok<*ASGLU>

% compute the double-sided frequency axis
N = length(signal(1, :));
if mod(N, 2)==0
    % N is even
    f = (-N/2:N/2-1)*Fs/N;
else
    % N is odd
    f = (-(N-1)/2:(N-1)/2)*Fs/N;
end

% compute gaussian filter coefficients
mean = freq;
variance = (bandwidth/100*freq / (2*sqrt(2*log(2))))^2;
magnitude = 1;

% create the double-sided Gaussian filter
gauss_filter = max(gaussian(f, magnitude, mean, variance), gaussian(f, magnitude, -mean, variance));

% store a copy of the central signal element if required for plotting
if nargin == 5 && plot_filter
    example_signal = signal(ceil(end/2), :);
end

% apply filter
signal = real(ifft(ifftshift(...
            bsxfun(@times, gauss_filter, ...
                fftshift(fft(signal, [], 2), 2) ...
            )...
        , 2), [], 2));

% plot filter if required
if nargin == 5 && plot_filter
    
    % compute amplitude spectrum of central signal element
    as = fftshift(abs(fft(example_signal))/N);
    as_filtered = fftshift(abs(fft(signal(ceil(end/2), :)))/N);
    
    % get axis scaling factors
    [f_sc, f_scale, f_prefix] = scaleSI(f);
    
    % produce plot
    figure;
    plot(f*f_scale, as./max(as(:)), 'k-');
    hold on;
    plot(f*f_scale, gauss_filter, 'b-');
    plot(f*f_scale, as_filtered./max(as(:)), 'r-');
    xlabel(['Frequency [' f_prefix 'Hz]']);
    ylabel('Normalised Amplitude');
    legend('Original Signal', 'Gaussian Filter', 'Filtered Signal');
    axis tight;
    set(gca, 'XLim', [0 f(end)*f_scale]);
    
end
end
function gauss_distr = gaussian(x, magnitude, mean, variance)
%GAUSSIAN   Create a Gaussian distribution.
% 
% DESCRIPTION:
%       gaussian returns a Gaussian distribution f(x) with the specified
%       magnitude, mean, and variance. If these values are not specified,
%       the magnitude is normalised and values of variance = 1 and mean = 0
%       are used. For example running
%           x = -3:0.05:3;
%           plot(x, gaussian(x));
%       will plot a normalised Gaussian distribution.
%
%       Note, the full width at half maximum of the resulting distribution
%       can be calculated by FWHM = 2 * sqrt(2 * log(2) * variance).
%
% USAGE:
%       gauss_distr = gaussian(x)
%       gauss_distr = gaussian(x, magnitude)
%       gauss_distr = gaussian(x, magnitude, mean)
%       gauss_distr = gaussian(x, magnitude, mean, variance)
%
% INPUTS:
%       x           - x-axis variable
%
% OPTIONAL INPUTS:
%       magnitude   - bell height (default = normalised)
%       mean        - mean or expected value (default = 0)
%       variance    - variance ~ bell width (default = 1)
%
% OUTPUTS:
%       gauss_distr - Gaussian distribution
%
% ABOUT:
%       author      - Bradley Treeby
%       date        - 4th December 2009
%       last update - 25th August 2014
%       
% This function is part of the k-Wave Toolbox (http://www.k-wave.org)
% Copyright (C) 2009-2014 Bradley Treeby and Ben Cox

% This file is part of k-Wave. k-Wave is free software: you can
% redistribute it and/or modify it under the terms of the GNU Lesser
% General Public License as published by the Free Software Foundation,
% either version 3 of the License, or (at your option) any later version.
% 
% k-Wave is distributed in the hope that it will be useful, but WITHOUT ANY
% WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
% FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
% more details. 
% 
% You should have received a copy of the GNU Lesser General Public License
% along with k-Wave. If not, see <http://www.gnu.org/licenses/>. 

% if the variance is not given, define as unity
if nargin < 4
    variance = 1;
end

% if the mean is not given, center the distribution
if nargin < 3
    mean = 0;
end

% if the magnitude is not given, normalise
if nargin < 2
    magnitude = (2*pi*variance)^-0.5;
end

% compute the gaussian
gauss_distr = magnitude*exp(-(x - mean).^2/(2*variance));
end

function env = envelopeDetection(x)
%ENVELOPEDETECTION  Extract signal envelope using the Hilbert Transform.
%
% DESCRIPTION:
%       envelopeDetection applies the Hilbert transform to extract the
%       envelope from an input vector x. If x is a matrix, the envelope
%       along each row is returned.
%
%       Example:
%           x = toneBurst(10e6, 0.5e6, 10);
%           plot(0:length(x)-1, x, 'k-', 0:length(x)-1, envelopeDetection(x), 'r-');
%           legend('Input Signal', 'Envelope');
%
% USAGE:
%       env = envelopeDetection(x)
%
% INPUTS:
%       x           - input function
%
% OUTPUTS:
%       env         - envelope of input function
%
% ABOUT:
%       author      - Bradley Treeby
%       date        - 20th December 2010
%       last update - 14th November 2011
%       
% This function is part of the k-Wave Toolbox (http://www.k-wave.org)
% Copyright (C) 2009-2014 Bradley Treeby and Ben Cox

% This file is part of k-Wave. k-Wave is free software: you can
% redistribute it and/or modify it under the terms of the GNU Lesser
% General Public License as published by the Free Software Foundation,
% either version 3 of the License, or (at your option) any later version.
% 
% k-Wave is distributed in the hope that it will be useful, but WITHOUT ANY
% WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
% FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
% more details. 
% 
% You should have received a copy of the GNU Lesser General Public License
% along with k-Wave. If not, see <http://www.gnu.org/licenses/>.

% if the input is one-dimensional, force to be 1 x Nx
if numDim(x) == 1
    x = reshape(x, 1, []);
end

% compute the FFT of the input function (use zero padding to prevent
% effects of wrapping at the beginning of the envelope), if x is a matrix
% the fft's are computed across each row
X = fft(x, length(x(1, :))*2, 2);

% multiply the fft by -1i
X = -1i*X;

% set the DC frequency to zero
X(1) = 0;

% calculate where the negative frequencies start in the FFT
neg_f_index = ceil(length(X(1, :))/2) + 1;

% multiply the negative frequency components by -1
X(:, neg_f_index:end) = -1 * X(:, neg_f_index:end);

% compute the Hilbert transform using the inverse fft
z = ifft(X, [], 2);

% extract the envelope
env = abs(x + 1i*z(:, 1:length(x)));
end

function dim = numDim(x)
%NUMDIM   Return the number of matrix dimensions.
%
% DESCRIPTION:
%       numDim returns the number of dimensions of a matrix x. Unlike
%       the inbuilt ndims, singleton dimensions are not counted, so numDim
%       returns 1 for 1D vectors, 2 for 2D arrays, etc. 
%
% USAGE:
%       dim = numDim(x)
%
% INPUTS:
%       x           - matrix with unknown number of dimensions
%
% OUTPUTS:
%       dim         - number of dimensions
%
% ABOUT:
%       author      - Bradley Treeby
%       date        - 29th April 2009
%       last update - 29th February 2012
%       
% This function is part of the k-Wave Toolbox (http://www.k-wave.org)
% Copyright (C) 2009-2014 Bradley Treeby and Ben Cox
%
% See also ndims

% This file is part of k-Wave. k-Wave is free software: you can
% redistribute it and/or modify it under the terms of the GNU Lesser
% General Public License as published by the Free Software Foundation,
% either version 3 of the License, or (at your option) any later version.
% 
% k-Wave is distributed in the hope that it will be useful, but WITHOUT ANY
% WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
% FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
% more details. 
% 
% You should have received a copy of the GNU Lesser General Public License
% along with k-Wave. If not, see <http://www.gnu.org/licenses/>.

% get the size collapsing any singleton dimensions
sz = size(squeeze(x));

if length(sz) > 2
    dim = length(sz);
elseif sz(1) == 1 || sz(2) == 1
    dim = 1;
else
    dim = 2;
end

% % alternate check for 1D vector in any dimension
% if max(sz) == prod(sz)
%     dim = 1;
% end
end

function signal = logCompression(signal, a, normalise)
%LOGCOMPRESSION   Log compress an input signal.
%
% DESCRIPTION:
%       logCompression compresses the input signal using the expression
%       signal = log10(1 + a*signal)./log10(1 + a) 
%
% USAGE:
%       signal = logCompression(signal, a)
%       signal = logCompression(signal, a, normalise)
%
% INPUTS:
%       signal      - input signal
%       a           - compression factor
%
% OPTIONAL INPUTS
%       normalise   - Boolean controlling whether the maximum of the input
%                     signal is normalised to unity before compression
%                     (default = false). If set to true, the original
%                     magnitude is restored after compression.
%
% OUTPUTS:
%       signal      - log compressed signal
%
% ABOUT:
%       author      - Bradley Treeby
%       date        - 24th February 2011
%       last update - 24th February 2011
%
% This function is part of the k-Wave Toolbox (http://www.k-wave.org)
% Copyright (C) 2009-2014 Bradley Treeby and Ben Cox

% This file is part of k-Wave. k-Wave is free software: you can
% redistribute it and/or modify it under the terms of the GNU Lesser
% General Public License as published by the Free Software Foundation,
% either version 3 of the License, or (at your option) any later version.
% 
% k-Wave is distributed in the hope that it will be useful, but WITHOUT ANY
% WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
% FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
% more details. 
% 
% You should have received a copy of the GNU Lesser General Public License
% along with k-Wave. If not, see <http://www.gnu.org/licenses/>. 

% check for optional normalise input
if nargin == 2
    normalise = false;
end

% compress signal
if normalise
    mx = max(signal(:));
    signal = mx*(log10(1 + a*signal./mx)./log10(1 + a));
else 
    signal = log10(1 + a*signal)./log10(1 + a);
end
end