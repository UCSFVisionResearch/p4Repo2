function [ fm ] = aoFrame2Im(frame)
% aoFrame2Im  Ensure the argument is a grayscale numerical matrix, 
%                 converting as necessary from an RGB image matrix or a Frame
%                 structure.
%
% [ fm ] = aoFrame2Im(frame)
%   
    
if isa(frame, 'struct') && isfield(frame, 'cdata')
    %warning('Converting frame to image.');
    [im, map] = frame2im(frame);
    if isempty(map)
        frame = im;
    else
        frame = ind2rgb(im, map);
    end
end

if isa(frame, 'numeric') && (size(frame, 3) > 1)
    %warning('Converting RGB to grayscale.');    
    frame = rgb2gray(frame);
end

fm = frame;
