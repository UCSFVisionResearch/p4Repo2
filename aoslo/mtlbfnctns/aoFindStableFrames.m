% aoFindStableFrames

[fname, pname] = uigetfile('*.avi', 'Open AVI file'); %, 'MultiSelect', 'on');

% Number of neighbors per side of image under consideration
neighbors = 1;
% Coeff threshold
minCoeff = 0.7;
% Translation offset threshold
maxOffset = 50;

filePath = [ pname fname ];
aviDetails = aviinfo(filePath);
    
height = aviDetails.Height;
width = aviDetails.Width;
%numFrames = aviDetails.NumFrames; 
numFrames = 20; % Debugging: specify a subset of frames
movie = aviread(filePath);

% Dynamic programming
% We do multiple comparisons of Xcorrs, so calculate the Xcorrs first and
% put in a table.
xcorrTable = zeros(4, numFrames - 1);

for ix = 1:(numFrames-1)
    leftCtr = aoUtilFrameMat(movie(ix));
    rightCtr = aoUtilFrameMat(movie(ix+1));
    avgL = mean(mean(leftCtr));
    maxL = max(max(leftCtr));
    avgR = mean(mean(rightCtr));
    maxR = max(max(rightCtr));
    %leftCtr = imcrop(leftCtr, [width/4, height/4, width/2, height/2]);
    %rightCtr = imcrop(rightCtr, [width/4, height/4, width/2, height/2]);
    leftCtr = im2bw(leftCtr, avgL + (maxL - avgL)/2);
    rightCtr = im2bw(rightCtr, avgR + (maxR - avgR)/2);
    [maxCoeff, yOffset, xOffset] = aoFrameXcorr(leftCtr, rightCtr);
    xcorrTable(:,ix) = [ix, maxCoeff, yOffset, xOffset];
    fprintf(1, '----------Processed frame %4d out of %4d\n', ix, numFrames - 1);
end

maxX = max(xcorrTable(4,:));
minX = min(xcorrTable(4,:));
maxY = max(xcorrTable(3,:));
minY = min(xcorrTable(3,:));

reqWidth = abs(minX) + abs(maxX) + width;
reqHeight = abs(minY) + abs(maxY) + height;
docSize = max([reqWidth reqHeight]);
refX = 1 + abs(minX);
refY = 1 + abs(minY);
psnewdoc(docSize, docSize, 72, ['layers_' fname], 'grayscale', 'white', 1.0, 8);
frameMat = zeros(docSize, docSize);
frameMat(refY:(refY+height-1), refX:(refX+width-1)) = aoUtilFrameMat(movie(1));
frame1 = im2uint8(frameMat);
psnewlayermatrix(frame1);

for ix = 1:(numFrames - 1)
    frameMat = zeros(docSize, docSize);
    xOffset = xcorrTable(4,ix);
    yOffset = xcorrTable(3,ix);
    startX = refX + xOffset;
    endX = startX + width - 1;
    startY = refY + yOffset;
    endY = startY + height - 1;
    frameRes = aoUtilFrameMat(movie(ix + 1));
    frameMat(startY:endY, startX:endX) = frameRes;
    psnewlayermatrix(im2uint8(frameMat));
end
