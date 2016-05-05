function mvhist(mvs)
%MVHIST Displays histogram of the motion vectors.
%   Handy shortcut for a log-scale bin-width-of-1 histogram.

figure;
histogram(reshape(mvs, [], 1), 'BinWidth', 1);
xlim([-512 512])
set(gca,'yscale','log');
% TODO: make y-axis log-scaled, tweak other bits
end

