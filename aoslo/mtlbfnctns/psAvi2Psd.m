fname = 'D:\p40075L\split\40075L_14-36_V029_stablized_8_209.avi';
aviDetails = aviinfo(fname);
%psnewdoc(aviDetails.Width, aviDetails.Height, 72, 'movie-frames', 'grayscale', 'transparent');

%for ix = 1:10 %1:aviDetails.NumFrames
%    curFrame = aviread(fname, ix);
%    imgMat = im2uint8(AOframe2im(curFrame));
%   psnewlayermatrix(imgMat);
%end

movie = aviread(fname, 1:20);
normImg = aoNormSumFrames(movie);

psnewlayermatrix(im2uint8(normImg));
%figure;
%imshow(normImg);
