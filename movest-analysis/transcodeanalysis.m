frames = 30;
nummvs = 212000;
[mvse, ~, typese] = loadmvs('decoded.txt', frames, 'Interleave', true);
[mvst, ~, typest] = loadmvs('decoded_trans2.txt', frames, 'Interleave', true);
[mvso, ~, typeso] = loadmvs('decoded_orig2.txt', frames, 'Interleave', true);

mvliste = typedMvs(mvse, typese);
mvlistt = typedMvs(mvst, typest);
mvlisto = typedMvs(mvso, typeso);

mvliste = mvliste(1:nummvs);
mvlistt = mvlistt(1:nummvs);
mvlisto = mvlisto(1:nummvs);

sad_te = abs(mvliste - mvlistt);
sad_to = abs(mvliste - mvlisto);
sad_te(sad_te > 16) = 0;
sad_to(sad_to > 16) = 0;

te = sad_te(1:20000);
to = sad_to(1:20000);

histogram(te, 17);
hold on;
histogram(to, 17);

%te_data = sum(reshape(sad_te, nummvs / 80, []));
%to_data = sum(reshape(sad_to, nummvs / 80, []));

%plot(te_data, 'b.-'); hold on; plot(to_data, 'r.-');
%plot(te_data - to_data, 'b.-');
