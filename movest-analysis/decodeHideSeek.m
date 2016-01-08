function bytes = decodeHideSeek(mvs, types)
%DECODEHIDESEEK Groups LSBs of the input matrix into bytes, filtering
%according to the 'types' parameter.

mvsSeqorder = permute(lsbplane(mvs), [2, 1, 3]);
typesSeqorder = permute(types, [2, 1, 3]);
bitlist = mvsSeqorder(typesSeqorder == 0);

bitsToTake = size(bitlist, 1) - mod(size(bitlist, 1), 8);
bitsMat = reshape(bitlist(1:bitsToTake), 8, []);

function r = processRow(bits)
    r = 0;
    for k = 0:7
        r = bitor(r, bitshift(bits(k+1), k), 'uint8');
    end
end
    
bytes = zeros(size(bitsMat, 2), 1);
for i = 1:size(bitsMat, 2)
    bytes(i) = processRow(bitsMat(:, i));
end

end
