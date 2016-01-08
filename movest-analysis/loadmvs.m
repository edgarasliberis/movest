function [mv_x, mv_y, type] = loadmvs(input_file, varargin)

p = inputParser;
addRequired(p, 'input_file', @ischar);
addOptional(p, 'Frames', 50, @isnumeric);
addParameter(p, 'MBDims', [45 80], @(x) size(x) == [1 2]);
addParameter(p, 'Interleave', false, @islogical);
parse(p, input_file, varargin{:});

maxframes = p.Results.Frames;
mbdims = p.Results.MBDims;
interleave = p.Results.Interleave;

infile = fopen(input_file, 'r');

if interleave
    mv_x = zeros(mbdims(1), mbdims(2) * 2, maxframes); 
    mv_y = [];
    type = zeros(mbdims(1), mbdims(2) * 2, maxframes);
else
    mv_x = zeros(mbdims(1), mbdims(2), maxframes); 
    mv_y = zeros(mbdims(1), mbdims(2), maxframes);
    type = zeros(mbdims(1), mbdims(2), maxframes);
end

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
            xy = textscan(pairs{k}, '%d, %d, %d');
            if interleave 
                mv_x(row, 2*k-1, frame) = xy{1};
                mv_x(row, 2*k, frame) = xy{2};
                type(row, 2*k-1, frame) = xy{3};
                type(row, 2*k, frame) = xy{3};
            else
                mv_x(row, k, frame) = xy{1};
                mv_y(row, k, frame) = xy{2};
                type(row, k, frame) = xy{3};
            end
        end
        row = row + 1;
    end
end