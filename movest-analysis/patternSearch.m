function out = patternSearch(str, pattern)
%PATTERNSEARCH Confidence of @pattern occurring at a position in @str.
%   Returns "confidence" of a substring closely matching @pattern for 
%   every position in @str, r. Values are inverse sum of square differences
%   between @pattern and @str's substring.

out = zeros(1, length(str)-length(pattern));
for i = 1:(length(str)-length(pattern))
   substr = str(i:i+length(pattern)-1);
   diff = substr - pattern;
   out(i) = sum(diff.^2);
end

out = 1./(out + 1);

end

