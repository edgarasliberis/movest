function [mv_x, mv_y] = load_mvs(input_file, maxframes, mbdims)
infile = fopen(input_file, 'r');

if ~exist('maxframes', 'var')
    maxframes = 5000;
end

if ~exist('mbdims', 'var')
    mbdims = [45 80];
end

mv_x = zeros(mbdims(1), mbdims(2), maxframes); 
mv_y = zeros(mbdims(1), mbdims(2), maxframes); 
frame = 1;
row = 1;
newline = sprintf('\n');

while ~feof(infile)
    if(frame > maxframes) 
        break;
    end
    
    line = fgets(infile);
    
    if(strcmp(line, newline))
        frame = frame + 1;
        row = 1;
    else
        pairs = strsplit(line, '; ');
        for k = 1:length(pairs)-1
            xy = textscan(pairs{k}, '%d, %d');
            mv_x(row, k, frame) = xy{1};
            mv_y(row, k, frame) = xy{2};
        end
        row = row + 1;
    end
end