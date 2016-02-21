[mvs, ~, types] = loadmvs('mvs-msteg.txt', 'Interleave', true);
mvhist(typedMvs(mvs, types), 1000)
