function mvlist = typedMvs(mvs, types)
%TYPEDMVS Filters MVs, keep only non-skip ones (type 0)

mvsSeqorder = permute(mvs, [2, 1, 3]);
typesSeqorder = permute(types, [2, 1, 3]);
mvlist = mvsSeqorder(typesSeqorder == 0);

end

