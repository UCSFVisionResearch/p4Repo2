cones = {};
for ix = 1:3
    [X,Y] = AOim2xy(montages{ix});
    XY = zeros(max(size(X)),2);
    XY(1:end,1) = X;
    XY(1:end,2) = Y;
    cones{ix} = XY;
end