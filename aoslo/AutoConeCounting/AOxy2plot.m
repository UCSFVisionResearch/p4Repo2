function P = AOxy2plot(X, Y, sz1, sz2)
%AOXY2PLOT   Convert arrays of X and Y values into a plot of sufficient size.
%
%   AOim2xy produces two arrays of equal size, X and Y, where (X(n),Y(n))
%   are the (x, y) coordinates for cone n. It is a simple matter of converting
%   the arrays into a logical plot of size (m,n) where the original image was
%   size (m,n,d) for some bitdepth d (1 for grayscale, 3 for rgb). Since it's
%   a common operation, a function was worthwhile to write.
%
%   P = AOxy2plot(X, Y, I)   Return an m-by-n logical array containing 1s at
%   the coordinates given by X and Y, where m and n are the first two 
%   dimensions of I.
%
%   P = AOxy2plot(X, Y, m, n)   Return an m-by-n logical array containing 1s at
%   the coordinates given by X and Y.
%
% James V. Botelho 13 Feb 2014

if nargin == 4
    assert(isscalar(sz1), 'Expected scalar as third param');
    assert(isscalar(sz2), 'Expected scalar as fourth param');
    mRows = sz1;
    nCols = sz2;
elseif nargin == 3
    assert(ismatrix(sz1), 'Expected vector as third param');
    mRows = size(sz1,1);
    nCols = size(sz1,2);
end

P = false(mRows, nCols);
P(sub2ind([mRows, nCols],Y,X)) = 1;

end

