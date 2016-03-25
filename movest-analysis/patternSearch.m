function out = patternSearch(str, pattern)
%PATTERNSEARCH Summary of this function goes here
%   Detailed explanation goes here

out = zeros(1, length(str)-length(pattern));
for i = 1:(length(str)-length(pattern))
   substr = str(i:i+length(pattern)-1);
   diff = substr - pattern;
   out(i) = sum(diff.^2);
end

out = 1./(out + 1);

end

