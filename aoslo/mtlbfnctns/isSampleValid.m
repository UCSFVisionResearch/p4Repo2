function [ isValid, Is, nSp ] = isSampleValid( r, c, h, w, I, Sp )
% isSampleValid  True if given sample contains no spent pixels.
%
%               isValid  = isSampleValid( r, c, h, w, I, Sp )
%
%        [ isValid, Is ] = isSampleValid( r, c, h, w, I, Sp )
%
%   [ isValid, Is, nSp ] = isSampleValid( r, c, h, w, I, Sp )
%
% Input:
%
%      r = Row of sample to test
%
%      c = Column of sample to test
%
%      h = Height of sample to test
%
%      w = Width of sample to test
%
%      I = HxWxN matrix containing the image from which the sample is drawn.
%
%     Sp = HxW boolean matrix indicating whether a pixel is "spent", i.e.
%          should not be included in the sample. A value of 1 indicates the
%          pixel is spent and unavailable.
%
% Output:
%
%     isValid = True if the sample under test contains no spent pixels.
%
%          Is = If the sample is valid, receives an HxWxN matrix containing the
%               valid sample. If the sample is invalid, the matrix contains 
%               arbitrary values.
%
%         nSp = If the sample is valid, receives a new matrix of spent pixels, 
%               HxW, reflecting the pixels used in the valid sample. If the 
%               sample is invalid, the matrix contains arbitrary values.
%


assert(nargin == 6, 'Incorrect number of arguments.');

szI = size(I);
szSp = size(Sp);
assert(all(szI(1:2) == szSp(1:2)), 'Size mismatch between I and Sp');

rr = r + h - 1;
cc = c + w - 1;
spent = Sp(r:rr,c:cc);
isValid = all(~spent(:));
    
Is = I(r:rr,c:cc,:);
nSp = Sp;
nSp(r:rr,c:cc) = 1;


end % function

