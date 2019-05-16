function Hw = AOfirKernel(cutOff, W)
% Hw = AOfirKernel(cutOff)  Get finite impulse response (FIR) filter kernel.
%   The filter is a low-pass filter multiplied by a window. The window defaults
%   to a disk averaging filter with radius 7.
%
%   cutOff     Set the cutoff frequency of the filter to (cutOff)*pi radians 
%              per degree.
%
%   W          (Optional) The desired window for the filter.
%

if nargin < 2
    W = fspecial('disk', 7);
    W = W./max(W(:));
end

freqsize = 512;
[F1, F2] = freqspace(freqsize, 'meshgrid');
Fr = sqrt(F1.^2 + F2.^2);

H = ones(freqsize, freqsize);
H(Fr > cutOff) = 0;

Hw = fwind2(H, W);

end

