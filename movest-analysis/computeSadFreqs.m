function sads = computeSadFreqs(stegoDir, transDir)
    maxframes = Inf;
    filelist = dir(strcat(stegoDir,'/*.txt'));
    sads = zeros(length(filelist), 32);
    for i = 1:length(filelist)
        recv_filepath = strcat(stegoDir, '/', filelist(i).name);
        trans_filepath = strcat(transDir, '/', filelist(i).name);
        
        [mvxr, mvyr, typer] = loadmvs(recv_filepath, maxframes);
        [mvxt, mvyt, typet] = loadmvs(trans_filepath, maxframes);
        
        type = typer + typet; % TypeR AND TypeT
        x = typedMvs(abs(mvxr - mvxt), type);
        y = typedMvs(abs(mvyr - mvyt), type);
        
        tbl = tabulate([x; y]);
        rows = min(32, size(tbl, 1));
        sads(i, 1:rows) = tbl(1:rows, 3)'; 
    end
end

