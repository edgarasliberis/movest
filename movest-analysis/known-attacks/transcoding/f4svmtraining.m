c = computeSadFreqs('f4-vectors/clean/received', ...
    'hidenseek-vectors/clean/transcoded');
d = computeSadFreqs('f4-vectors/dirty/received', ...
    'hidenseek-vectors/dirty/transcoded');

truth_c = zeros(size(c, 1), 1);
truth_d = ones(size(d, 1), 1);

model_f4 = fitcsvm([c; d], [truth_c; truth_d]);

cvruns = zeros(100, 1);

for i = 1:length(cvruns)
    CVSVMModel = crossval(model_f4, 'KFold', 8);
    cvruns(i) = kfoldLoss(CVSVMModel);
end

[mean, stddev, mean_ci, stddev_ci] = normfit(cvruns)
