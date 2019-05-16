function [maxCoeff, yOffset, xOffset, coeffArray] = aoFrameXcorr(templateFrame, parentFrame)
% aoFrameXcorr  Compute normalized xcorr between two frames, returning the max
%               correlation coefficient and the corresponding offsets.
%
% [maxCoeff, yOffset, xOffset, coeffArray] = aoFrameXcorr(templateFrame, parentFrame)
%   
% Input:
%    templateFrame = Truecolor AVI frame object, or numerical matrix 
%                    resulting from frame2im(). Matrix must be smaller than
%                    parent and not all the same.
%
%    parentFrame = Truecolor AVI frame object, or numerical matrix 
%                  resulting from frame2im().
%
% Output:
%    maxCoeff = The maximum correlation coefficient, in the range [-1,1].
%
%    yOffset = The offset, 0-indexed, by which to shift templateFrame for
%              the correlation given by maxCoeff.
%
%    xOffset = The offset, 0-indexed, by which to shift templateFrame for
%              the correlation given by maxCoeff.
%
%    coeffArray = The entire array of correlation coefficients.
%
% Example:
%       grayCross = .2*ones(11); % Light gray + on dark gray background
%       grayCross(6,3:9) = .6;   
%       grayCross(3:9,6) = .6;
%       bwCross = grayCross > 0.5;    % White + on black background
%       figure, imshow(bwCross), figure, imshow(grayCross)
%
%       % Make new image that offsets the template
%       offsetGray = .2*ones(21); 
%       offset = [3 5];  % Shift by 3 rows, 5 columns
%       offsetGray( (1:size(grayCross, 1)) + offset(1),...
%                   (1:size(grayCross, 2)) + offset(2) ) = grayCross;
%       figure, imshow(offsetGray)
%     
%       % Cross-correlate bwCross and offsetGray to recover offset  
%       [maxCoeff, yOffset, xOffset] = aoFrameXcorr(bwCross, offsetGray);
%       isequal(offset, [yOffset, xOffset]) % 1 means offset was recovered
%

    

templateFrame = aoUtilFrameMat(templateFrame);
parentFrame = aoUtilFrameMat(parentFrame);

% Debug statement
%figure, imshow(templateFrame), figure, imshow(parentFrame);

coeffArray = normxcorr2(templateFrame, parentFrame);
[maxCoeff, index] = max(abs(coeffArray(:)));
[yBR, xBR] = ind2sub(size(coeffArray), index(1));
yOffset = yBR - size(templateFrame, 1);
xOffset = xBR - size(templateFrame, 2);

