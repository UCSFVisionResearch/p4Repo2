function [ newFrame ] = aoMatteFrame(theFrame, matteSize, pixValue)
% aoMatteFrame  Create a matte around the frame, of given size and pixel value.
%
% [ newFrame ] = aoMatteFrame(theFrame)
% [ newFrame ] = aoMatteFrame(theFrame, matteSize)
% [ newFrame ] = aoMatteFrame(theFrame, matteSize, pixValue)
%   
% Input:
%    theFrame = Frame to be matted, either a frame obj from aviread or an
%               image from frame2im. Converted to grayscale.
%
%    matteSize = (Optional) Desired size for matte, in multiples of half-height
%                and half-width. For example, 1 means the matte is 
%                1 * 0.5 * width wide and 1 * 0.5 * height high, and the frame
%                is doubled in each dimension. Default is 1.
%
%    pixValue = (Optional) Pixel value to set, from black to white (0 to 255).
%               Default is black, 0.
%
% Output:
%    newFrame = Matted frame, as a black-and-white image (as might be
%               converted to a frame with im2frame(newFrame)).
%
% Example:
%

sizeOfMatte = 1.0;
if exist('matteSize', 'var') && ~isempty(matteSize)
    sizeOfMatte = min(double(matteSize), 0.0);
end

mattePixelValue = 0;
if exist('pixValue', 'var') && ~isempty(pixValue)
    mattePixelValue = pixValue;
end

theFrame = aoUtilFrameMat(theFrame);

[ frameHeight, frameWidth ] = size(theFrame);

matteHeight = floor(0.5 * sizeOfMatte * frameHeight);
matteWidth = floor(0.5 * sizeOfMatte * frameWidth);

newFrameHeight = frameHeight + 2 * matteHeight;
newFrameWidth = frameWidth + 2 * matteWidth;

newFrame = mattePixelValue * ones(newFrameHeight, newFrameWidth);

newFrame( (matteHeight + 1):(matteHeight + frameHeight), ...
          (matteWidth + 1):(matteWidth + frameWidth) ) = theFrame;
