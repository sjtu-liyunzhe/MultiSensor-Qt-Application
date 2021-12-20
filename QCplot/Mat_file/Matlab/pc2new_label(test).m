%~ generate pc2new_label.txt
%~ 注意：pca降维时，需要保证样本量大于特征维度，在MATLAB中如果使用2个特征（mean and std）的话，需要在c++项目中采集帧数大于768（特征维度）
%% ~ read original data of ultrasound
function [pc1new,pc2new]=pc2new_label()
currentParametersSettings='mean';
sample_dots = 1000; %~ 每个通道采样数是1000个点
chnum = 8;%~ 8个通道
file_num = 1;
raw_data = cell(file_num, chnum); % videonum x chnum %~ 创建1*8的空矩阵数据结构（cell）
fr_num = NaN;

s=pwd;%~ 获取当前.m文件所在路径

us_dir  = ('E:\Code\腕手同步识别代码\QT_2threads_36(WiFi)\QCplot\Mat_file');%~ 超声信号文件路径，在上位机软件中的话，是放在上一级目录下
cd (us_dir)
% dataglove=load('dataglove.txt');%~ 读取数据手套的值

file_list = dir('*1.bin');%~ dir函数是列出文件夹的内容
pos_ = find(file_list.name == '_');%~ 找到下划线所在位置
pos_ = pos_(2);%~ 第二个下划线的位置
tic %~ 开始计时
for file_no = 1 : file_num
    fr_num = NaN;  % some files may have different frame number
    for ch_no = 1:chnum
        fp = fopen([file_list(file_no).name(1:pos_) num2str(ch_no) '.bin'], 'r');%~ 这样处理是为了根据文件夹命名格式，方便循环读取数据
        foobar_raw = fread(fp);%~ fread 是读取二进制文件为一列，此处foobar_raw 为2000000*1个数据
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
        raw_data{file_no , ch_no} = result;%~ 这里得到的raw_data中的矩阵是2000帧，采集数据的时间是200s。
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
   raw_data_combine1{i, 1} = temp; %~ 最终生成的data_combine元胞数组为2000*1，每个cell为8（个通道）*1000的矩阵
end
disp 'combine ending '

%% ~ progress data
win_length = 20;%~ 窗长
m = 1;
switch_len = 20; %~ 剔除每一段训练数据的首尾的数据，消除手势切换过程的影响。例如设置为20，即剔除每一段训练数据的前后各2s

for i = 1: length(raw_data_combine1)%~ 总共的帧数。100ms/帧
    temp = raw_data_combine1{i};%~ temp也是cell格式，此处用于一帧一帧读取raw_data_combine1中的数据
    Feature(m, :) = ExFeature(temp, win_length ,currentParametersSettings);%~ 提取每一帧的特征（一帧特征存储在Feature中为一行），并按行存储到到Feature的2000*384数组中，则Feature数组为整个.bin文件“训练数据集”的所有特征参数集合；
    m = m+1;%~ Feature的行计数
end
Feature_len = size(Feature,1);

part_len = Feature_len/3;%%%%%%%%%%%%%%%%%%%%%%%% 三阶段训练，每阶段对应数据长度
% part_len = Feature_len/4;%%%%%%%%%%%%%%%%%%%%%%%% 四阶段训练

Feature_tra = [];%~ 用于训练的A超特征参数
% per_len = 500;%~ 500帧数据，对应每个阶段的50s时间。全称是per length，即每个阶段的长度
Feature_tra = Feature;

%~ 如果是四个阶段的训练数据，这里去掉第四个阶段（trial4动作比较多且杂）的数据，只分析前三个阶段；
%~ 如果是三个阶段的训练数据，也不需要进行改动；
Feature_tra = Feature_tra(1:part_len*3, :);

%%~ filter 零相位前后向数字滤波器
fc = 1;
fs = 10;
[b, a] = butter(5, fc/(fs/2));%~ 返回具有归一化截止频率的三阶低通数字巴特沃斯滤波器的传递函数系数
%~ 滤波的作用是防止短时间内的两个峰值的出现（紧挨着），从而导致后面取波峰波谷有问题
Feature_tra = filtfilt(b, a, Feature_tra);%~ filtfilt函数通过x在正向和反向两个方向上处理输入数据来执行零相位数字滤波 。在向前过滤数据后，filtfilt反转过滤后的序列，然后将其运回过滤器。

%%~ 对用于训练的特征参数进行pca降维
%Training with Auto Labeling 
[coef, score, latent, ~, ~, mul] = pca(Feature_tra); %~ score：X矩阵（此处即 Feature_tra ）在主成分空间中的表示，即原矩阵在新构成的主成分空间的映射。每行对应样本观测值，每列对应一个主成分(变量)，行列数与原矩阵X相同
pc1 = score(:, 1);
pc2 = score(:, 2);
%nor
pc1 = mapminmax(pc1', 0, 1)';
pc2 = mapminmax(pc2', 0, 1)';%~ 通过将每行的最小值和最大值归一化为[0,1]来处理矩阵；

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%?????这一小段存疑，可以通过判断pc1第一阶段均值和pc2第二阶段的均值，是否更接近于0/1，来判断是否进行翻转，简单粗暴。实验效果看起来还可以的
pc1inverse = 1-pc1;
pc2inverse = 1-pc2; 
% %~
% R2值是相关系数，越接近于1，吻合程度越高。这里比较R2值，判断是否需要将归一化后的数据反转。问：这一步是怎么在c++中实现的？？？如果没有dataglove怎么办
% if(calculateR2(pc2,Glove_tra)<calculateR2(pc2inverse,Glove_tra))
%     pc2 = pc2inverse;
% end
%~ 这里尝试替代上面与Glove_tra的比较，pc1第一阶段的均值与0接近，pc2第二阶段的均值与0接近
if((mean(pc1(1+switch_len:part_len-switch_len))-0.5)>0)
    pc1 = pc1inverse;
end
if((mean(pc2(part_len+1+switch_len:part_len*2-switch_len))-0.5)>0)
    pc2 = pc2inverse;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%~ pc1绘图
figure; 
plot(pc1); 
title('PC#1 vs. Wrist Rotation - Original'); xlabel('time'); ylabel('Angle');legend('PC#1')
%%~ pc1绘图
figure; 
plot(pc2); 
title('PC#2 vs. Hand Closing - Original'); xlabel('time'); ylabel('Angle');legend('PC#2')

%%~ pc1第一阶段数据修正
pc1(1+switch_len:part_len-switch_len) = 0;
%%~ pc1第二阶段数据修正
temp = pc1(part_len+1+switch_len:part_len*2-switch_len);
temp = mapminmax(temp', 0, 1)';
pc1(part_len+1+switch_len:part_len*2-switch_len) = temp;
%~ pc1阶段数据截取
pc1hc = pc1(1+switch_len: part_len-switch_len, :);
pc1wr = pc1(part_len+1+switch_len: part_len*2-switch_len, :);
pc1com = pc1(part_len*2+1+switch_len: part_len*3-switch_len, :);
pc1new=[pc1hc; pc1wr; pc1com];

%%~ train_feature数据截取
% train_feature_hc = train_feature(1+switch_len: part_len-switch_len, :);
% train_feature_wr = train_feature(part_len+1+switch_len: part_len*2-switch_len, :);
% train_feature_com = train_feature(part_len*2+1+switch_len: part_len*3-switch_len, :);
% train_feature_new=[train_feature_hc; train_feature_wr; train_feature_com];

%%~ pc1绘图
figure; 
plot(pc1new); 
title('PC#1 vs. Wrist Rotation - prior'); xlabel('time'); ylabel('Angle');legend('PC#1')

%%~ pc2第二、三阶段数据修正。（第三阶段需要经过这次归一化后，再经过后面的拉长效果）
pc2(part_len+1+switch_len:part_len*2-switch_len) = 0;
%re normalization %~ 第三阶段数据单独再次归一化
temp = pc2(part_len*2+1+switch_len:part_len*3-switch_len);
temp = mapminmax(temp', 0, 1)';
pc2(part_len*2+1+switch_len:part_len*3-switch_len) = temp;

%%~ 标记出第一阶段 PC2数据的峰值和谷值
% fre prior
figure;
pc2hc = pc2(1+switch_len: part_len-switch_len, :);%~ 第一阶段，hand closing的PC2数据
[pk,lkp] = findpeaks(pc2hc, 'MinPeakHeight',0.4);%~ findpeaks查找局部最大值，返回pk为峰值，lkp是峰值的索引
plot(1+switch_len:part_len-switch_len,pc2hc,lkp+switch_len,pk,'o')%~ 将最大值标记出来'o'
hold on;
[pk,lkr] = findpeaks(-pc2hc);%~ lkr是谷值的索引
plot(lkr+switch_len,-pk,'*') %~ 将最小值标记出来'*'
hold off;
title('PC#2 第一阶段'); xlabel('time'); ylabel('Angle');

unionset = union(lkr, lkp);%~ 计算峰值和谷值的并集，从小到大依次排列

%%~ （第一阶段）根据上述得到的波峰和波谷，将相邻的波峰和波谷之间的数据，重新归一化
for i = 2: length(unionset)-1 
    backward = unionset(i-1):unionset(i);%~ 相邻波峰和波谷之间的所有点index
    forward = unionset(i):unionset(i+1);
    pc2hc(backward) = mapminmax(pc2hc(backward)',0, 1)';
    pc2hc(forward) = mapminmax(pc2hc(forward)',0, 1)';
end

%%~ 标记出第三阶段 PC2数据的峰值和谷值
pc2wr = pc2(part_len+1+switch_len: part_len*2-switch_len);%~ 第二阶段wristrotation为0
pc2com = pc2(part_len*2+1+switch_len: part_len*3-switch_len);%~ 第三阶段同步运动数据
figure;
[pk,lkp] = findpeaks(pc2com, 'MinPeakDistance', 6, 'MinPeakHeight',0.4);
plot(1+switch_len:part_len-switch_len,pc2com,lkp+switch_len,pk,'o')
hold on;
[pk,lkr] = findpeaks(-pc2com, 'MinPeakDistance', 6);
plot(lkr+switch_len,-pk,'*')
hold off;
title('PC#2 第三阶段'); xlabel('time'); ylabel('Angle');

%%~ （第三阶段）根据上述得到的波峰和波谷，将相邻的波峰和波谷之间的数据，重新归一化
unionset = union(lkr, lkp);
for i = 2: length(unionset)-1
    backward = unionset(i-1):unionset(i);
    forward = unionset(i):unionset(i+1);
    pc2com(backward) = mapminmax(pc2com(backward)',0, 1)';
    pc2com(forward) = mapminmax(pc2com(forward)',0, 1)';
end

%%~ pc2new是第二、三阶段处理后的pc2（原始第二主成分特征参数）；绘图
pc2new = [pc2hc; pc2wr; pc2com];
figure; 
plot(pc2new); 
% hold on;
% plot(dataglove);
title('PC#2 vs. Hand Closing - With Fre Prior'); xlabel('time'); ylabel('Angle');

%%~ 将pc2new写入到txt文件中，方便上位机软件（c++）读取
fid1=fopen('pc2new_label.txt','wt');
%dataglove_label=single(dataglove);%~ 这里是否需要显式将pc2new的double类型转换成float类型
fprintf(fid1,'%.8f\n',pc2new);%~ 先拿dataglove的数据跑通了再换成pc2new的数据
fclose(fid1);

%%~
%%计算用于线性回归LR的beta矩阵，存储到txt文件中，让上位机软件读取，或者通过MATLAB引擎传输到c++；计算过程中要注意维数对齐的问题；此处选择250个维度，要与上位机软件中pca降维后的维数相同
score1 = score(1+switch_len:part_len-switch_len,1:250);
score2 = score(part_len+1+switch_len: part_len*2-switch_len,1:250);
score3 = score(part_len*2+1+switch_len: part_len*3-switch_len,1:250);
projectedTrain = [score1; score2; score3];
featurerow = size(projectedTrain,1);%~ 获取projectedTrain的行数
X=[projectedTrain ones(featurerow,1)];%~ 水平拼接矩阵
Y_pc1=pc1new;
Y_pc2=pc2new;
beta_pc1=inv((X')*X)*(X')*Y_pc1;
beta_pc2=inv((X')*X)*(X')*Y_pc2;

% fid2=fopen('beta.txt','wt');
% fprintf(fid2,'%f\n',beta);
% fclose(fid2);

%%~ 处理完数据后，将文件夹的数据移动到“备份”文件夹，方便下一次数据写入和处理
cd (us_dir) %~ 跳转到上一级文件夹
file_move = dir('*.bin');%~ dir函数是列出文件夹的内容
for i=1:size(file_move,1)
    movefile(file_move(i).name,'./备份');
end

file_move = dir('*.txt');
for i=1:size(file_move,1)
    movefile(file_move(i).name,'./备份');
end

cd(s)%~ 跳转到.m程序的所在文件夹

opts = struct('WindowStyle','modal',... 
              'Interpreter','tex');
warndlg('\fontsize{15}\color{red} 数据生成成功',...
             '提示', opts);
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
%前后切掉的20点没什么意义
start = 20; % start of x
endo = 20; % end of x, avoid gaussian filter jitter
tgc_alpha = 0.05; % [dB/(MHz cm)]   
sampling_balance = 128; % median of sampling range  %选用128更好
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
    %线性拟合提参数
    %foobar = polyfit(fit_foobar, line(xcor), 1);
    %foobar = polyfit(fit_foobar, line(xcor), 3); %效果有些差
    %AR模型提参数
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