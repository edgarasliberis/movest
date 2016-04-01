function lsbs = lsbplane(plane)
%LSBPLANE Extracts the LSB of every element in an input matrix.
%   Extracts the LSB of every element in an input matrix, treating
%   elements as 16-bit signed integers.
lsbs = bitget(plane, 1, 'int16');
end
