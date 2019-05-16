function [ layerMat ] = aoFrames2Layers(frameMat)
% aoFrames2Layers  Convert column vector of frame structs to a matrix of layers
%
% layerMat = aoFrames2Layers(frameMat)
%
% Input:
%    frameMat = A 1xN Matrix of frames (AVI frames)
%
% Output:
%    layerMat = HxWxN matrix of images.
% 
% Adapted by James V. Botelho (2013) from A. Roorda (2007).
%   

% ------------------------------------------------------------------------
% [] = aoNormSumFrames(filePath, frameRange, newDir, prefix)
% ------------------------------------------------------------------------

firstFrame = aoFrame2Im(frameMat(1,1));
aviHeight = size(firstFrame, 1);
aviWidth = size(firstFrame, 2);
numFrames = size(frameMat, 2);

layerMat = zeros(aviHeight, aviWidth, numFrames);

for ix = 1:numFrames
    layerMat(:,:,ix) = aoFrame2Im(frameMat(1,ix));
end
