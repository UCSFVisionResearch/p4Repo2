function [P, imdata, X, Y] = OverlayPlot(imdata, cutOff, threshold)
%OverlayPlot Plot cones detected by AOim2xy overlayed with image

% Default values depending on the number of input arguments
if nargin < 3
    threshold = 0;
    if nargin < 2
        cutOff = 0.61;
    end
end

if ischar(imdata)
    imdata = imread(imdata);
end
Id = im2double(imdata);

% Low pass filter
H_low = AOfirKernel(cutOff);
If = imfilter(Id, H_low);

% High pass filter
H_high = AOfirKernel(0.05);
If = If - imfilter(If, H_high);

[mRows,nCols] = size(If);
[X,Y] = AOimf2xy(If, threshold);

L = AOxy2plot(X, Y, mRows, nCols);
P = zeros(mRows, nCols, 3);
P(:,:,2) = L;
P(:,:,1) = (L * 0.5);
P = 
imwrite(P, 'plot.tif');

end

