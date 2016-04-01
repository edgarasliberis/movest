function [mv_x, mv_y, type] = loadmvs(input_file, varargin)
% LOADMVS Imports motion vectors extracted by Movest Decoder
%    Specify file name as the first argument and the number of frames to
%    to process (Inf if unsure). Function can interleave x and y values
%    (although not very efficiently) if 'Interleave' vararg is set to true.

p = inputParser;
addRequired(p, 'input_file', @ischar);
addOptional(p, 'Frames', 50, @isnumeric);
addParameter(p, 'Interleave', false, @islogical);
parse(p, input_file, varargin{:});

maxframes = p.Results.Frames;
interleave = p.Results.Interleave;

infile = fopen(input_file, 'r');

line = fgets(infile);
dims = textscan(line, '%d %d');
[data, count] = fscanf(infile, '%d, %d, %d;', [3, dims{1} * dims{2} * maxframes]);

framesRead = count / 3 / (dims{1} * dims{2});

mv_x = reshape(data(1, :), [dims{1}, dims{2}, framesRead]);
mv_y = reshape(data(2, :), [dims{1}, dims{2}, framesRead]);
type = reshape(data(3, :), [dims{1}, dims{2}, framesRead]);

if interleave
    mv_xi = zeros(dims{1}, 2 * dims{2}, framesRead);
    mv_xi(:, 1:2:end) = mv_x(:, :);
    mv_xi(:, 2:2:end) = mv_y(:, :);
    
    mv_x = mv_xi;
    mv_y = [];
    
    types_i = zeros(dims{1}, 2 * dims{2}, framesRead);
    types_i(:, 1:2:end) = type(:, :);
    types_i(:, 2:2:end) = type(:, :);
    
    type = types_i;
end
