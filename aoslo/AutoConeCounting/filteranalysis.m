% Analysis of FIR kernels, using embedded AOfirKernel function
% Plots desired frequency response, mesh of window, and actual frequency
% response of the FIR filter. Gaussian seems better; lots of ripple from the
% disk window

freqsize = 512;
[F1, F2] = freqspace(freqsize, 'meshgrid');
Fr = sqrt(F1.^2 + F2.^2);

H = ones(freqsize, freqsize);
H(Fr > 0.61) = 0;

figure, mesh(F1, F2, H);

ix = 1;
Win = {};
Win{ix} = fspecial('disk', 7); ix = ix + 1;
Win{ix} = fspecial('gaussian', 15, 0.5); ix = ix + 1;
Win{ix} = fspecial('gaussian', 15, 1.0); ix = ix + 1;
Win{ix} = fspecial('gaussian', 15, 2.0); ix = ix + 1;
Win{ix} = fspecial('gaussian', 15, 3.0); ix = ix + 1;
Win{ix} = fspecial('gaussian', 15, 4.0); ix = ix + 1;
Win{ix} = fspecial('gaussian', 15, 5.0); ix = ix + 1;

maxIx = max(size(Win));

for ix = 1:maxIx
    W = Win{ix};
    W = W ./ max(W(:));
    Hw = fwind2(H, W);
    figure
    subplot(1,2,1), mesh(W);
    subplot(1,2,2), freqz2(Hw, freqsize);
end