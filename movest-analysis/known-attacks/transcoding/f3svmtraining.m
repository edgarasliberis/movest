c = computeSadFreqs('clean/received', 'clean/transcoded');
d = computeSadFreqs('f3-vectors/received', 'f3-vectors/transcoded');

truth_c = zeros(size(c, 1), 1);
truth_d = ones(size(d, 1), 1);

model_f3 = fitcsvm([c; d], [truth_c; truth_d]);

cvruns = zeros(100, 1);

for i = 1:length(cvruns)
    CVSVMModel = crossval(model_f3, 'KFold', 8);
    cvruns(i) = kfoldLoss(CVSVMModel);
end

[mean, stddev, mean_ci, stddev_ci] = normfit(cvruns)
