function bytes = seqbytes(mvs)
%SEQBYTES Groups LSBs of input matrix into bytes, sequentially

types = zeros(size(mvs, 1), size(mvs, 2), size(mvs, 3));
bytes = decodeHideSeek(mvs, types);
end
