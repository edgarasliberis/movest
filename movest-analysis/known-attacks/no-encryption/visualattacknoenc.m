[mvs, ~, types] = loadmvs('mvs-noenc.txt', 'Interleave', true);
plot2d(lsbplane(mvs(:, :, 1)))
