function [ L ] = psLayersFromDoc(docPath)
% psLayersFromDoc Opens the given doc in Photoshop; reads in all layers; and
%                 returns as an array of images.
%
%   Photoshop will be launched if it is not already running.
%
%   L = psLayersFromDoc(docName)
%
%   L is a M by N by X matrix, where M by N are the canvas size and X is the 
%   number of layers.
%
%   docName The name of the document to open; must be supported by Photoshop.
%

if nargin == 1
    pslaunch();
    psopendoc(docPath);
end

%layerNames = pslayernames();
layerNames = { 'Layer 1' 'Layer 19' 'Layer 5' 'Layer 11' 'Layer 15' }
%numFrames = psnumlayers();
numFrames = 5;
[docW, docH] = psdocinfo();
L = uint8(zeros(docH, docW, numFrames));

for ix = 1:numFrames
    pssetactivelayer(char(layerNames(ix)));
    lPix = psgetpixels();
    if size(lPix,3) > 1
        lPix = rgb2gray(lPix);
    end
    L(:,:,ix) = im2uint8(lPix);
end

if nargin == 1
    psclosedoc(1);
end