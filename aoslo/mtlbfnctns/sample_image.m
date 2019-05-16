% Must have generateSample in your path

% ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
% Configuration variables
% ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

% (Constant) The time, for use with file naming
DATE_AND_TIME = round(clock());

% (Constant) The name of the file that will receive the file names
DECODER_FILE_NAME = sprintf('sample-codes_%d-%d-%d_%d-%d-%d.csv', DATE_AND_TIME);

% (Constant) Foveal coordinate, x; 0 to query user.
FOVEA_X = 5897;

% (Constant) Foveal coordinate, y; 0 to query user.
FOVEA_Y = 841;

% (Constant) The name of the output folder
OUTPUT_FOLDER_NAME = sprintf('samples_%d-%d-%d_%d-%d-%d', DATE_AND_TIME);

% (Constant) Number of samples
NUM_SAMPLES = 500;

% (Constant) First sample index
FIRST_SAMPLE_INDEX = 1;

% (Constant) Height of sample
SAMPLE_HEIGHT = 100;

% (Constant) Width of sample
SAMPLE_WIDTH = 100;

% (Constant) Offset in x coordinate to get center of sample
CTR_OFFSET_X = round(SAMPLE_WIDTH / 2);

% (Constant) Offset in y coordinate to get center of sample
CTR_OFFSET_Y = round(SAMPLE_HEIGHT / 2);

% (Constant) Pixels per degree of retinal eccentricity.
PIX_PER_DEG = 370.3719;

% ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
% Body
% ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

filter = {'*.tif;*.tiff;*.png','Uncompressed image files'};
[fname,g_pName] = uigetfile(filter,'Choose the image');

% Image file name; the image data itself; and the "spent" matrix.
% The image *must* be black-bordered.
g_imageFile = fullfile(g_pName, fname);
g_Image = imread(g_imageFile);
g_Sp = (g_Image == 0);

if FOVEA_X && FOVEA_Y
    g_xFovea = FOVEA_X;
    g_yFovea = FOVEA_Y;
else
    hF = figure('name','Click on the fovea');
    imshow(g_Image,'InitialMagnification',70);
    [g_xFovea,g_yFovea] = ginput(1);
    close(hF);
end

g_xFoveaInt = round(g_xFovea);
g_yFoveaInt = round(g_yFovea);

% Output file and directory
g_outFolderPath = fullfile(g_pName, OUTPUT_FOLDER_NAME);
mkdir(g_outFolderPath);
outFile = fopen(fullfile(g_outFolderPath, DECODER_FILE_NAME), 'w');
% Close output file in case of error; otherwise, Matlab keeps it open.
try
    fprintf(outFile, 'Decoder file for sampled montage\n\n');
    fprintf(outFile, 'Montage file: %s\n', g_imageFile);
    fprintf(outFile, 'Fovea location (xy): %d %d\n\n', g_xFoveaInt, g_yFoveaInt);
    fprintf(outFile, [...
        'Sample_ID,'    ...
        'Sample_X_pix,' ... 
        'Sample_Y_pix,' ... 
        'Sample_X_deg,' ... 
        'Sample_Y_deg,' ... 
        'Ctr_X_pix,'    ... 
        'Ctr_Y_pix,'    ... 
        'Ctr_X_deg,'    ... 
        'Ctr_Y_deg,'    ... 
        'Dist_pix,'     ... 
        'Dist_deg\n']);

    % Number of digits required to contain the sample number; smaller values are
    % padded with 0s so that the strings will sort lexically.
    numDigits = floor(log10(NUM_SAMPLES)) + 1;

    for ix = 1:NUM_SAMPLES
        [r,c,Is,g_Sp] = generateSample(SAMPLE_HEIGHT, SAMPLE_WIDTH, g_Image, g_Sp);
        imwrite(Is, fullfile(g_outFolderPath, sprintf('s%0*d.tiff', numDigits, ix)));
        sampCtrX = c + CTR_OFFSET_X;
        sampCtrY = r + CTR_OFFSET_Y;
        distPix = pdist([sampCtrX,sampCtrY ; g_xFoveaInt,g_yFoveaInt]);
        fprintf(outFile, '%d,%d,%d,%f,%f,%d,%d,%f,%f,%f,%f\n', ...
            ix,                   ... % Sample_ID
            c,                    ... % Sample_X_pix
            r,                    ... % Sample_Y_pix
            c/PIX_PER_DEG,        ... % Sample_X_deg
            r/PIX_PER_DEG,        ... % Sample_Y_deg
            sampCtrX,             ... % Ctr_X_pix
            sampCtrY,             ... % Ctr_Y_pix
            sampCtrX/PIX_PER_DEG, ... % Ctr_X_deg
            sampCtrY/PIX_PER_DEG, ... % Ctr_Y_deg
            distPix,              ... % Dist_pix
            distPix/PIX_PER_DEG);     % Dist_deg
    end
catch err
    if strcmp(err.identifier, 'generatesample:couldnot')
        % User-generated message; ran out of usable frames
        fprintf('Found last unused sample (%d)\n', ix - 1);
    else
        fclose(outFile); % Error
        rethrow(err);
    end
end

fclose(outFile); % Normal flow
