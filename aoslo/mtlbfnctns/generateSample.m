function [ r, c, Is, nSp ] = generateSample(h, w, I, Sp)
% generateSample  Generate sample without reusing pixels.
%
%            [r, c] = generateSample(h, w, I, Sp);
%
%        [r, c, Is] = generateSample(h, w, I, Sp);
%
%   [r, c, Is, nSp] = generateSample(h, w, I, Sp);
%
% Select a rectangle-shaped sample of an image while avoiding spent pixels. If
% a randomly-generated sample includes spent pixels, the sample is regenerated.
% The returned sample has the same color depth and numeric class as the input
% image, and is a contiguous region.
%
% If a new sample cannot be found after a reasonable number of attempts, this
% function will return r == c == 0 and will raise an error.
%
% Input:
%
%      h = Height of desired sample
%
%      w = Width of desired sample
%
%      I = HxWxN matrix containing the image from which to draw the sample.
%
%     Sp = HxW boolean matrix indicating whether a pixel is "spent", i.e.
%          should not be included in the sample. A value of 1 indicates the
%          pixel is spent and unavailable.
%
% Output:
%
%       r = The row number of the top left of the sample. The bottom right will
%           be from row number (r + h - 1). If a new sample cannot be found, 
%           return 0.
%
%       c = The column number of the top left of the sample. The bottom right
%           will be from column number (c + w - 1). If a new sample cannot be
%           found, return 0.
%
%      Is = h x w x N matrix containing the sample. N is arbitrary and is 
%           preserved from the input matrix I.
%
%     nSp = The new matrix of spent pixels, HxW, reflecting the pixels used
%           in the returne sample.
%     

assert(nargout >= 2, 'You must supply row and column output variables.');

H = size(I,1);
W = size(I,2);
assert((H == size(Sp,1)) && (W == size(Sp,2)), 'Size mismatch');
xMultMax = floor(W / w); % rMax = (H - h + 1);
yMultMax = floor(H / h); % cMax = (W - w + 1);


% Tries only a set number of times before giving up with an error.
% In back-of-envelope testing, 1,000 attempts seemed plenty -- about half a
% second to time out, but capable of finding obscure samples.
count = 0;
countLimit = 1000;

while 1
    xMult = randi(xMultMax);
    yMult = randi(yMultMax);
    r = (yMult - 1) * h + 1;
    c = (xMult - 1) * w + 1;
    [isValid,Is,nSp] = isSampleValid(r,c,h,w,I,Sp);
    if isValid
        % Found a valid sample
        break;
     elseif count > countLimit
        error('generatesample:couldnot','Could not find new unspent sample.');
    end
    count = count + 1;
end


end % function
