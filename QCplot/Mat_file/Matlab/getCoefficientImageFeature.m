%~ 与c++进行交互，实现在线过程中滤波和特征提取
%% ~ 其中，特征提取部分需要和上位机软件的设置一致
function imageFeature=getCoefficientImageFeature(rawData)
currentParametersSettings='mean';
sample_dots = 1000; %~ 每个通道采样数是1000个点
chnum = 8;%~ 8个通道
file_num = 1;
fr_num = NaN;

%% ~ progress data
% p=true;
win_length = 20;%~ 窗长
imageFeature = ExFeature(rawData, win_length, currentParametersSettings);%~ 提取每一帧的特征（一帧特征存储在Feature中为一行），并按行存储到到Feature的2000*384数组中，则Feature数组为整个.bin文件“训练数据集”的所有特征参数集合；
% if p
% for i=1:8
%     figure;
%     plot(rawData(i,:));
% end
% p=false;
% end
end
%% ~ functions
function [ vFeature ] = ExFeature( data, win_length ,currentParametersSettings)
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
%     line = envelopeDetection(line);
    line = abs(line);
    line = smooth(line, 10)'; % smooth
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