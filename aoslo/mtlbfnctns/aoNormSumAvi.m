function [] = aoNormSumAvi(filePath, frameRange, newDir, prefix)
% aoNormSumAvi     Sum frames from AVI into a TIFF, normalizing by number of 
%                  frames per pixel and overall level. Write result to file.
%
% [] = aoNormSumAvi(filePath)
% [] = aoNormSumAvi(filePath, frameRange)
% [] = aoNormSumAvi(filePath, frameRange, newDir)
% [] = aoNormSumAvi(filePath, frameRange, [],     prefix)
% [] = aoNormSumAvi(filePath, frameRange, newDir, prefix)
%
% Algorithm:
%     accumulator frame A
%     binary accumulator frame B
%     for each frame f in the movie:
%         sum f into A
%         create binary frame b from f, with 0s for black, 1s otherwise
%         sum b into B
%     averaged accumulator frame A2
%     A2 = A ./ B
%     normalized accumulator frame N
%     N = A2 / (maximum pixel value of A2)
%     write N to file
%
% Input:
%    filePath = Absolute path to AVI file. Required.
%
%    frameRange = Array of integral type specifying a range of frames, not
%                 necessarily contiguous. Optional; if not supplied, sum all
%                 frames in file.
%
%    newDir = The new directory in which to write the summed TIFF file. 
%             Optional; if omitted, use same dir as input file.
%
%    prefix = Prefix for TIFF file names. Optional; default = 'sumnorm_'.
%
% Output:
% 
% Adapted by James V. Botelho (2013) from A. Roorda (2007).
%   

% ------------------------------------------------------------------------
% [] = aoNormSumAvi(filePath, frameRange, newDir, prefix)
% ------------------------------------------------------------------------

[currentDir, fileName, fileExt] = fileparts(filePath);

outDir = currentDir;
if exist('newDir', 'var') && ~isempty(newDir)
    outDir = newDir;
end

outPrefix = 'sumnorm_';
if exist('prefix', 'var') && ~isempty(prefix)
    outPrefix = prefix;
end

aviDetails = aviinfo(filePath);
frameIndices = 1:aviDetails.NumFrames;
if exist('frameRange', 'var') && ~isempty(frameRange)
    frameIndices = frameRange(frameRange <= aviDetails.NumFrames);
end

newFilePath = [outDir '\' outPrefix fileName '.tif'];
movieFrames = aviread(filePath, frameIndices);
imwrite(aoNormSumFrames(movieFrames), newFilePath, 'tif', 'Compression', 'none');
