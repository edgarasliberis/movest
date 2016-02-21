function percentages = chiSquareAttack(mvs, types)
%CHISQUAREATTACK Computes probability of embedding.
%   Returns a vector of 100 elements, where ith position denotes the
%   probability of embedding in the first i% of data.

mvvals = typedMvs(mvs, types); % Retrieve only non-skip MVs
mvvals = mvvals(mvvals ~= 0);
mvvals = mvvals(mvvals ~= 1);
mvvals = mvvals - min(mvvals); % Normalise
percentages = zeros(100, 1); % First i% of data are considered

% We use maxval to compute number of categories (pairs)
maxval = max(mvvals);
if mod(maxval, 2) == 1
    maxval = maxval + 1;
end
cat = maxval / 2 + 1;

for i = 1:100
    X = zeros(cat);
    Y = zeros(cat);

    % Compute the frequencies of even (2k) and odd (2k+1) values into the
    % bin number k.
    numvals = floor(i * size(mvvals, 1) / 100);
    for j = 1:numvals
        if mod(mvvals(j), 2) == 0
            % +1 for Matlab indexing
            X(mvvals(j) / 2 + 1) = X(mvvals(j) / 2 + 1) + 1;
        else
            Y((mvvals(j)-1)/2 + 1) = Y((mvvals(j)-1)/2 + 1) + 1;
        end
    end
    
    % Expected frequency
    Z = (X + Y) / 2;
    
    % Minimum frequency condition, filter insignificant data
    X = X(Z > 2);
    Z = Z(Z > 2);
    
    % Number of categories after filtering
    n = size(Z, 1);
    
    % Chi-squared test
    chi2 = sum(((X - Z) .^ 2) ./ Z);
    percentages(i) = 1 - gammainc(chi2/2, (n-1)/2);
end
