function [ ] = randcrops ( fname, N, sz, dir )
%randcrops Create random cropped images from fname
%  Return vector of file names of crops
%  randcrops(fname) Create 10 crops
%  randcrops(fname, N) Create N crops
%  randcrops(fname, N, sz) Create N crops sz X sz in size.

switch nargin
    case 0;
        error('Need fname');
    case 1;
        warning('Creating 10 crops at 25x25 to current dir');
        N = 10;
        sz = 25;
        dir = '';
    case 2;
        warning('Creating crops at 25x25 to current dir')
        sz = 25;
        dir = '';
    case 3;
        warning('Creating crops to current dir')
        dir = '';
end

imgV = imread(fname);
numRows = size(imgV, 1);
numCols = size(imgV, 2);
sz = min(sz, min(numRows, numCols));

if strcmp(dir, '')
    dirPrefix = '';
else
    dirPrefix = strcat(dir, '\');
end

ixes = strfind(fname, '.');
if ~isempty(ixes)
    rootN = fname(1:(ixes(length(ixes))-1));
else
    rootN = '';
end
    
for ix = 1:N
    row = floor(rand(1) * numRows);
    col = floor(rand(1) * numCols);
    % Handle edge limits
    row = row - max(0, (row+sz) - numRows);
    col = col - max(0, (col+sz) - numCols);
    sub = imgV(row:row+sz-1, col:col+sz-1);
    newFname = strcat(dirPrefix, rootN, '.sub',  int2str(ix), '.tif');
    imwrite(sub, newFname, 'tiff');
end


%imshow(sub)
%hold on
%[xx,yy] = AOim2xy(sub);
%hplot = plot(X, Y, 'g.');
%set(hplot, 'MarkerSize', 1);