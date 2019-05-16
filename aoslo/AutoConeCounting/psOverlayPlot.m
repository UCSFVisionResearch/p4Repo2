function [P, I, X, Y] = psOverlayPlot(imdata, arg2, arg3)
% PSOVERLAYPLOT   Display an image and a cone plot in Photoshop.
%
%   psOverlayPlot runs the cone detector function AOim2xy, converts the result 
%   into a plot with the same dimensions as the original image, and displays
%   them both in Photoshop, with the cones overlayed on a transparent layer.
%
%   [P,I,X,Y] = psOverlayPlot(imdata)   Run the cone detector on the image 
%   given by imdata with default settings; generate a plot from the (x,y) 
%   coordinates; and display in a new document in Photoshop. Return the plot P 
%   as a logical array; the image data I; and the arrays X and Y of cone 
%   coordinates.
%
%   imdata may be a string specifying a path to a supported image file.
%
%   [P,I,X,Y] = psOverlayPlot(imdata, X, Y)   Skip running the cone detector,
%   and plot the coordinates X and Y in the format returned by a previous
%   invokation of the cone dector.
%
%   [P,I,X,Y] = psOverlayPlot(imdata, P)   Skip running the cone detector,
%   and plot the array P (treated as a logical) in the format returned by 
%   AOxy2plot.
%
%   [P,I,X,Y] = psOverlayPlot(imdata, 'Filtered')   Skip the filtering step of
%   the cone detector and treat imdata as containing already-filtered image
%   data.
%
% James V. Botelho 13 Feb 2014

% Default values depending on the number of input arguments
theX = false();
theY = false();
theP = false();
filtered = false();
    
if nargin == 3
    assert(iscolumn(arg2), 'Expecting a column vector as second argument');
    assert(iscolumn(arg3), 'Expecting a column vector as third argument');
    theX = true();
    theY = true();
    X = arg2;
    Y = arg3;
elseif nargin == 2
    if ischar(arg2) && strcmpi(arg2, 'filtered')
        filtered = true();        
    elseif ismatrix(arg2)
        theP = true();
        P = arg2;
    else
        error('Expected a char or matrix as second argument');
    end
elseif nargin == 0
    error('Required image data argument not provided');
end

% No matter what, we must import image data and return it in I. Handle path
% or image array values.
if ischar(imdata)
    I = imread(imdata);
else
    I = imdata;
end
mRows = size(I,1);
nCols = size(I,2);

if ~theX && ~theY && ~theP
    % User supplied nothing; run cone detector
    Id = im2double(I);
    if filtered
        [X,Y] = AOimf2xy(Id);
    else
        [X,Y] = AOim2xy(Id);
    end    
    P = AOxy2plot(X, Y, mRows, nCols);
elseif ~theX && ~theY
    % User supplied P; populate X and Y
    [Y,X] = ind2sub(size(P),find(P));
elseif ~theP
    % User supplied X and Y; populate P
    P = AOxy2plot(X, Y, mRows, nCols);
end

% Make I a known format, 8-bit RGB
I = im2uint8(I);
if size(I,3) == 1
    I = cat(3,I,I,I);
end

% Transparency data is a grayscale image with bitdepth of the parent image.
T = im2uint8(P);

% Cone plot in color
C = uint8(zeros(size(I)));
C(:,:,2) = (P * 255);
C(:,:,1) = (P * 128);

psnewdocmatrix(I);
psnewlayermatrix(C, T);