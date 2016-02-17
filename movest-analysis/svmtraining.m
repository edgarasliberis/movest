c = computeSadFreqs('mvsteg-vectors/clean/received', 'mvsteg-vectors/clean/transcoded');
d = computeSadFreqs('mvsteg-vectors/dirty/received', 'mvsteg-vectors/dirty/transcoded');

truth_c = zeros(size(c, 1), 1);
truth_d = ones(size(d, 1), 1);

model = fitcsvm([c; d], [truth_c; truth_d]);
CVSVMModel = crossval(model);
classLoss = kfoldLoss(CVSVMModel);
