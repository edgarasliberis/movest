function mvhist(mvs, granularity)
%MVHIST Displays histogram of the motion vectors
%   Handy shortcut for a log-scale histogram. 'granularity' parameter
%   allows to specify number of buckets used.
if ~exist('granularity', 'var')
    granularity = 200;
end

figure;
histogram(reshape(mvs, [], 1), granularity);
xlim([-512 512])
set(gca,'yscale','log');
% TODO: make y-axis log-scaled, tweak other bits
end

