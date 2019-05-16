function [ ] = viewfft2( Y )
%viewfft2 Visualize the 2D FFT result Y
%   viewfft2(Y)     Visualize the matrix Y, where Y = fft2(I) for some matrix 
%                   I, optimized for the case where I is a grayscale image.

Y = fftshift(Y); % Center FFT

M = abs(Y); % Get the magnitude
M = log(M+1); % Use log, for perceptual scaling, and +1 since log(0) is undefined
M = mat2gray(M); % Use mat2gray to scale the image between 0 and 1

P = angle(Y);

figure
title('Magnitude spectrum')
imshow(M,[]); % Display the result

figure
title('Phase spectrum')
imshow(P, []);