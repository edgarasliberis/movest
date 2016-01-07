function plot2d(vals)
%PLOT2D Shows the colourmap of the input matrix
%   Tuned to display LSB planes, black for 0, white for 1

figure;
imagesc(vals);
colormap gray;

end

