function [ L ] = psTiffsToPsd(dirName)
%psTiffsToPds Send each tiff to a separate layer of a new PSD
%   Open each tiff (assumed to be same color profile); send each tiff to a
%   separate layer in one (new) photoshop document; and return the layers
%   in a layers matrix.
%
%   L = psTiffsToPsd( dirName )
%
%   dirName = Name of a directory containing the tiff files.
%
%   L = An array of grayscale images for each layer. 
%           M x N x I:
%               M rows
%               N cols
%               I layers
%

if ~isdir(dirName)
    error('Arg "%s" not a directory', dirName);
end

tiffFiles = dir(fullfile(dirName, '*.tif*'));
numFiles = size(tiffFiles,1);

if numFiles == 0
    error('Directory "%s" contains no tiffs', dirName);
end

I = imread(fullfile(dirName, tiffFiles(1).name));
rows = size(I,1);
cols = size(I,2);
L = im2uint8(zeros(rows, cols, numFiles));

L(:,:,1) = im2uint8(I);

for ix = 2:numFiles
    I = imread(fullfile(dirName, tiffFiles(ix).name));
    L(:,:,ix) = im2uint8(I);
end

[~, docName, ~] = fileparts(dirName);

pslaunch();
psnewdoc(cols, rows, 72, sprintf('tiffs_%s',docName), 'grayscale', 'transparent');
for ix = 1:size(L,3)
   psnewlayermatrix(L(:,:,ix));
end