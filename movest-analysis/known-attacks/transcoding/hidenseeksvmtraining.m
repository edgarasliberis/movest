c = computeSadFreqs('hidenseek-vectors/clean/received', ...
    'hidenseek-vectors/clean/transcoded');
d = computeSadFreqs('hidenseek-vectors/dirty/received', ...
    'hidenseek-vectors/dirty/transcoded');

truth_c = zeros(size(c, 1), 1);
truth_d = ones(size(d, 1), 1);

model_hs = fitcsvm([c; d], [truth_c; truth_d]);

cvruns = zeros(100, 1);

for i = 1:length(cvruns)
    CVSVMModel = crossval(model_hs, 'KFold', 8);
    cvruns(i) = kfoldLoss(CVSVMModel);
end

[mean, stddev, mean_ci, stddev_ci] = normfit(cvruns)