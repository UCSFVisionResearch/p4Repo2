function [ X, Y, hplot ] = OverlayPlot( img, xx, yy )
%OverlayPlot Plot cones detected by AOim2xy overlayed with image

figure;
imshow(img);
hold on
hplot = plot(xx, yy, 'g.');
set(hplot, 'MarkerSize', 1);
hold off

end

