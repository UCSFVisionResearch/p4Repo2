for sz = 3:2:7
    for sigma = 0.5:0.5:4
        Tg = fspecial('gaussian', sz, sigma);
        xcr = xcorr2(Id, Tg);
        offset = floor((sz - 1) / 2);
        xcorrMat = cat(3, xcorrMat, xcr(1+offset:end-offset,1+offset:end-offset));
    end
end