function [  ] = aoAvi2Layers(aviPath, frameIxs)
% aoAvi2Layers  Opens the given AVI and reads its frames into a document in
%               photoshop as layers.
%
%   Photoshop will be launched if it is not already running.
%
%   aoAvi2Layers(aviPath)
%   aoAvi2Layers(aviPath, frameIxs)
%
%   aviPath = Full path to an avi file.
%
%   frameIxs = Range of frame indices to read. Indices out of range are
%              ignored. Range may have gaps.
%

pslaunch();

if ~exist(aviPath, 'file')
    error('%s does not exist', aviPath);
end

aviInfo = aviinfo(aviPath);
[~,docName,~] = fileparts(aviPath);

if nargin == 1:
    frameIxs = 1:aviInfo.numFrames;
end

psnewdoc(aviInfo.Width, aviInfo.Height, 72, docName, 'grayscale');

for ix = frameIxs
    frame = aviread(aviPath, ix);
    psnewlayermatrix(im2uint8(ind2gray(frame.cdata, frame.colormap)));
end
