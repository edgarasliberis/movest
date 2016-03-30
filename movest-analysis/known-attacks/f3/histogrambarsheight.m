[mvs, ~, types] = loadmvs('mvs-f3.txt', 'Interleave', true);
% Even values have higher bars, look at -4 ... -10
mvhist(typedMvs(mvs, types))
