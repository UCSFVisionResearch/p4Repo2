function [X, Y] = AOimf2xy(If, threshold)
% AOim2xy Takes an AO image and computes the locations of the cones
%   [X, Y] = AOim2xy(I) 
%   [X, Y] = AOim2xy(I, threshold) 
%   Returns two arrays (X and Y) that correspond to the location of all the 
%   cones found in the input image in cartesian coordinates. Note that 
%   length(X) or length(Y) equals the number of detected cones. 
%
%   I       The finite 2-D sequence (matrix) representing the AO retinal
%   image of interest. This is typically an 8-bit grayscale image (uint8).
%   However, if a color image is given then it will be converted into
%   grayscale prior to any further processing. Image must have already been
%   filtered.
%
%   threshold (optional) Minimum threshold to identify a pixel as a local
%   local minimum. The algorithm identifies cones by identifying local minima.
%   Certain PSFs that may correspond to cones can be quite dim. If they are in 
%   fact cones, then those cones may be dead or not functioning. Setting this 
%   threshold value (0 to 255) is an optional method to ignore these dimmer 
%   PSFs.
%
%   Disclaimer (Kaccie Y. Li): Please do not use this algorithm to determine 
%   whether the observed signal is or is not a cone. Cones are effectively 
%   point sources so the "cones" observed in the image are only an array of 
%   PSFs. 
%
%   Requires Matlab's image processing toolbox.
%
%   Coded by Kaccie Y. Li (2006); modified 2014 by James V. Botelho
%   Copyright Roordalab

% Verify input validity
Dim = size(If);
if length(Dim) == 3;
    warning('Converting color image to to grayscale')
    If = rgb2gray(If);
    %%% Previously coded as the following:
    %%%     I(:,:,1);
end

% Default values depending on the number of input arguments
if nargin < 2
	threshold = 0;
end

% Begin algorithm
If = im2double(If);
%%% Previously coded as the following, seemingly a mistake, but possibly
%%% deliberate and definitely *not* numerically equivalent
%%%     I = double(I);
Ic = imcomplement(If);
[mRows, nCols] = size(Ic);

% Morphological markers generation
localMins = imregionalmin(Ic, 4);
strElt = strel('disk', 1, 0);
coneMarks = imdilate(localMins, strElt);
[L, numMark] = bwlabel(coneMarks);
stats = regionprops(L, 'centroid');

G = zeros(mRows, nCols);
thresholdD = (threshold / 255.0);
for ii = 1:numMark
    loc = stats(ii).Centroid;
    loc = round(loc);
    if If(loc(2), loc(1)) > thresholdD
        G(loc(2), loc(1)) = 1;
    end
end

[Y,X] = find(G == 1);
