function [X Y] = origAOim2xy(f, CutOff, Thrshld)
%AOim2xy Takes an AO image and computes the locations of the cones
%   [X Y] = AOim2xy() returns two arrays (X and Y) that correspond to the
%   location of all the cones found in the input image in cartesian
%   coordinates so length(X) or length(Y) equals the number of detected
%   cones. The input Cutoff frequency and Threshold are optional so if left
%   blank, the default values (adjusted to the field size (pixel/deg) and
%   eccentricities typically employed in Rochester and Berkeley. Make sure 
%   to declare a variable for both X and Y when calling this function.
%   For example, if you only have:
%                   OutVariable = AOim2xy(image,...)
%   Only X coordinates will be outputted!
%
%   f       the finite 2-D sequence (matrix) representing the AO retinal
%   image of interest. This is typically an 8-bit grayscale image (uint8).
%   However, if a color image is given then it will be converted into
%   grayscale prior to any further processing.
%
%   CutOff  (optional) This parameter defines the passband of the FIR
%   filter that will be applied to f to aid the cone counting process.
%   Filtering is necessary to control noise. The default value is 0.61pi
%   radians per pixel. The actual convertion to physical quantities is a
%   bit complicated and can be found in Matlab documentation for FFT as
%   well as Gonvalez and Woods' Digital Image Processing textbook.
%
%   Thrshld (optional) This is short for threshold. Certain PSFs that may
%   still correspond to cones can be quite dim. If they are in fact cones,
%   then those cones may be dead or not functioning. Setting this threshold
%   value (0 to 255) is an optional method to ignore these dimmer PSFs
%
%Please do not use this algorithm to determine whether the observed
%signal(s) is or is not a cone. Cones are effectively point sources so the
%"cones" observed in the image is only an array of PSFs. 
%Also, this function requires Matlab's image processing toolbox.
%
%Coded by Kaccie Y. Li (2006)
%copyright Roordalab

%verify input validity
ImgDim = size(f);
if length(ImgDim) == 3;
    warning('This image is color and will be converted to grayscale')
    f = f(:,:,1);
end
%set default values depending on the number of input arguments
switch nargin
    case 2;
        warning('Thrshld = 0 by default!');
        Thrshld = 0;
    case 1;
        warning('Cutoff Frequency = 0.61pi and Thrshld = 0 by default!')
        Thrshld = 0;
        CutOff = 0.61;
end

%Begin algorithm
f = double(f);
fc = imcomplement(f);
[M N] = size(fc);

%FIR filter design
[f1 f2] = freqspace(512, 'meshgrid');
H = ones(512, 512);
fr = sqrt(f1.^2 + f2.^2);
H(fr > CutOff) = 0;
window = fspecial('disk', 7);
window = window./max(window(:));
h = fwind2(H, window);
figure, imshow(h);
fc = imfilter(fc, h, 'replicate', 'same');
%imfilter is the same as conv2 but offers different options for padding the
%original sequence before applying the convolution filter
 
%Morphological markers generation
LocalMins = imregionalmin(fc, 4);
se = strel('disk', 1, 0);
ConeMark = imdilate(LocalMins, se);
[L numMark] = bwlabel(ConeMark);
stats = regionprops(L, 'centroid');
X = zeros(numMark, 1);
Y = X;
g = zeros(M, N);
for ii = 1:numMark
    loc = stats(ii).Centroid; %(x, y)
    loc = round(loc); %integral output
    if f(loc(2), loc(1)) > Thrshld
        g(loc(2), loc(1)) = 1;
    end
end
g = im2bw(g);
[Y X] = find(g == 1);
return;