frames = 20; % Load all
[mvs, ~, types] = loadmvs('mvs-hs.txt', frames, 'Interleave', true);
plot(chiSquareAttack(mvs, types))