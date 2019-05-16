function [ stdDevIm ]  = aoStdDevLayers( layerMat )
% aoStdDevLayers  Calculate standard deviation of pixels across frames.
%
% stdevIm = aoNormSumFrames(layerMat)
%
% Algorithm:
%     for each pixel position in image
%         value = standard deviation(all non-zero input pixel values at that 
%                                    position)
%                                     
%
% Input:
%    layerMat = An HxWxN matrix, where H and W are height and width of the
%               individual images and N is the number of images.
%
% Output:
%    stdevIm = The normalized image, a HxW matrix.
% 
% James V. Botelho (2014)
% 

imHeight = size(layerMat, 1);
imWidth = size(layerMat, 2);

stdDevIm = zeros(imHeight,imWidth);

for rr = 1:imHeight
    for cc = 1:imWidth
        pp = layerMat(rr,cc,:);
        nonzero = squeeze(pp(pp > 0));
        if size(nonzero,1) > 0
            val = std(double(nonzero));
            stdDevIm(rr,cc) = val;
    end
end

end

