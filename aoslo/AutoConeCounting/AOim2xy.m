function [X, Y] = AOim2xy(I, cutOff, threshold)
% AOim2xy Takes an AO image and computes the locations of the cones
%   [X, Y] = AOim2xy(I) 
%   [X, Y] = AOim2xy(I, cutOff) 
%   [X, Y] = AOim2xy(I, cutOff, threshold) 
%   Returns two arrays (X and Y) that correspond to the location of all the 
%   cones found in the input image in cartesian coordinates. Note that 
%   length(X) or length(Y) equals the number of detected cones. 
%
%   I       The finite 2-D sequence (matrix) representing the AO retinal
%   image of interest. This is typically an 8-bit grayscale image (uint8).
%   However, if a color image is given then it will be converted into
%   grayscale prior to any further processing.
%
%   cutOff  (optional) This parameter defines the passband of the FIR
%   filter that will be applied to I to aid the cone counting process.
%   Filtering is necessary to control noise. The default value is 0.61pi
%   radians per pixel. The actual convertion to physical quantities is a
%   bit complicated and can be found in Matlab documentation for FFT as
%   well as Gonzalez and Woods' Digital Image Processing textbook.
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
Dim = size(I);
if length(Dim) == 3;
    warning('Converting color image to to grayscale')
    I = rgb2gray(I);
    %%% Previously coded as the following:
    %%%     I(:,:,1);
end

% Default values depending on the number of input arguments
if nargin < 3
    threshold = 0;
    if nargin < 2
        cutOff = 0.61;
    end
end

% Begin algorithm
I = im2double(I);
%%% Previously coded as the following, seemingly a mistake, but possibly
%%% deliberate and definitely *not* numerically equivalent
%%%     I = double(I);
Ic = imcomplement(I);
[mRows, nCols] = size(Ic);

% Noise filter
Hw = AOfirKernel(cutOff);
Ic = imfilter(Ic, Hw, 'replicate', 'same');
 
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
    if I(loc(2), loc(1)) > thresholdD
        G(loc(2), loc(1)) = 1;
    end
end

[Y,X] = find(G == 1);
