function [ normIm ] = aoNormSumLayers(layerMat)
% aoNormSumLayers  Sum image matrix layers and normalize.
%
% normIm = aoNormSumFrames(layerMat)
%
% Algorithm:
%     accumulator image A
%     binary accumulator image B
%     for each image Im in layerMat:
%         sum f into A
%         create binary frame b from f, with 0s for black, 1s otherwise
%         sum b into B
%     % averaged accumulator image A2: 
%     A2 = A ./ B
%     % normalized accumulator frame Na: 
%     Na = A2 / (maximum pixel value of A2)
%
% Input:
%    layerMat = An HxWxN matrix, where H and W are height and width of the
%               individual images and N is the number of images.
%
% Output:
%    normIm = The normalized image, a HxW matrix.
% 
% Adapted by James V. Botelho (2013) from A. Roorda (2007).
%   

imHeight = size(layerMat, 1);
imWidth = size(layerMat, 2);

% Accumulator image
sumIm = zeros(imHeight, imWidth);
% Binary accumulator image, counting number of non-black frames per pixel
sumImBin = ones(imHeight, imWidth);

for ix = 1:size(layerMat, 3)
    curIm = double(layerMat(:,:,ix));
    sumIm = sumIm + curIm;
    % im2bw(im, lev) Returns a binary matrix with 0s (black) for input pixels 
    % <= thresh and 1s (white) for input pixels > thresh. The threshold is
    %     thresh = lev * range_of_values_for_image_class
    curImBin = im2bw(curIm, 0); 
    sumImBin = sumImBin + curImBin;
    drawnow;
end

sumImNew = sumIm ./ sumImBin;
normIm = sumImNew / max(sumImNew(:));
