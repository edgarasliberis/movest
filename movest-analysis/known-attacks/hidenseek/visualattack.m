[mvx_o, mvy_o, type_o] = loadmvs('original-mvs.txt');
plot2d(lsbplane(mvx_o(:, :, 1)))
[mvx_e, mvy_e, type_e] = loadmvs('encoded-mvs.txt');
plot2d(lsbplane(mvx_e(:, :, 1)))
