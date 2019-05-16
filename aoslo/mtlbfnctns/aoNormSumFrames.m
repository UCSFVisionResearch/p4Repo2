function [ normIm ] = aoNormSumFrames(frameMat)
% aoNormSumFrames  Sum frames from AVI into a TIFF, normalizing by number of 
%                  frames per pixel and overall level. Return result.
%
% normIm = aoNormSumFrames(frameMat)
%
% Algorithm:
%     accumulator frame A
%     binary accumulator frame B
%     for each frame f in the movie:
%         sum f into A
%         create binary frame b from f, with 0s for black, 1s otherwise
%         sum b into B
%     % averaged accumulator frame A2
%     A2 = A ./ B
%     % normalized accumulator frame Na
%     Na = A2 / (maximum pixel value of A2)
%
% Input:
%    frameMat = A 1xN Matrix of frames (AVI frames)
%
% Output:
%    normIm = The normalized image.
% 
% Adapted by James V. Botelho (2013) from A. Roorda (2007).
%   

% ------------------------------------------------------------------------
% [] = aoNormSumFrames(filePath, frameRange, newDir, prefix)
% ------------------------------------------------------------------------

layerMat = aoFrames2Layers(frameMat);
normIm = aoNormSumLayers(layerMat);
