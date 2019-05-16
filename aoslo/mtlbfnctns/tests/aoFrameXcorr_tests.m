function aoFrameXcorr_tests(simpleTest, aviTest)
% aoFrameXcorrTests  Test the aoFrameXcorr function.

if simpleTest
    grayCross = .2 * ones(11); % Light gray + on dark gray background
    grayCross(6, 3:9) = .6;   
    grayCross(3:9, 6) = .6;
    bwCross = grayCross > 0.5;    % White + on black background
    figure, imshow(bwCross), figure, imshow(grayCross)

    % Make new image that offsets the template
    offsetGray = .2 * ones(21); 
    offset = randi([0,10], [1,2]);
    offsetGray((1:size(grayCross, 1)) + offset(1), ...
               (1:size(grayCross, 2)) + offset(2)) = grayCross;
    figure, imshow(offsetGray)

    % Cross-correlate bwCross and offsetGray to recover offset  
    [maxCoeff, yOffset, xOffset] = aoFrameXcorr(bwCross, offsetGray);
    isequal(offset, [yOffset, xOffset]) % 1 means offset was recovered
end

if aviTest
    aviFile = 'testAvi56fr.avi';
    frame1 = aviread(aviFile, 1);
    frame2 = frame2im(aviread(aviFile, 1));
    
    [maxCoeff, yOffset, xOffset, cc] = aoFrameXcorr(frame1, frame1);
    fprintf(1, '\nObj X Obj: Coeff %g at offset [%d, %d]\n\n', maxCoeff, ...
        yOffset, xOffset);
    
    [maxCoeff, yOffset, xOffset] = aoFrameXcorr(frame1, frame2);
    fprintf(1, '\nObj X Mat: Coeff %g at offset [%d, %d]\n\n', maxCoeff, ...
        yOffset, xOffset);
    
    [maxCoeff, yOffset, xOffset] = aoFrameXcorr(frame2, frame1);
    fprintf(1, '\nMat X Obj: Coeff %g at offset [%d, %d]\n\n', maxCoeff, ...
        yOffset, xOffset);
    
    aviFile2 = '40075L_V001_stabilized_final.avi';
    frame3 = aviread(aviFile2, 1);
    
    [maxCoeff, yOffset, xOffset, cc] = aoFrameXcorr(frame3, frame3);
    fprintf(1, '\nObj X Obj Indexed: Coeff %g at offset [%d, %d]\n\n', maxCoeff, ...
        yOffset, xOffset);
end