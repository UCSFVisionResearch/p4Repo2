function [] = aoNormSumFrames_tests(testNum)
% aoNormSumFrames_Tests  Test the aoNormSumFrames function
%

testFileName = '40075L_V001_stabilized_final.avi';

[currentDir, dc1, dc2] = fileparts(mfilename('fullpath'));
testFilePath = [currentDir '\' testFileName];

fprintf(1, '\n--------------------------------------------------\n');
fprintf(1, 'Testing...\n');
testType = '';

switch testNum
    case 1
        testType = 'Test w/defaults';
        aoNormSumFrames(testFilePath);
    case 2
        testType = 'Test w/only a range';
        aoNormSumFrames(testFilePath, 1:10);
    case 3
        testType = 'Test w/only a non-cont range';
        aoNormSumFrames(testFilePath, [ 1:10 20:30 1900:1950]);
    case 4
        testType = 'Test w/only a path';
        aoNormSumFrames(testFilePath, [], ...
            'D:\p4root\specA\aoslo\mtlbfnctns\tests\somsub');
    case 5
        testType = 'Test w/only a prefix';
        aoNormSumFrames(testFilePath, [], [], 'othernorm_');
    case 6
        testType = 'Test w/range and path';
        aoNormSumFrames(testFilePath, 1:10, ...
            'D:\p4root\specA\aoslo\mtlbfnctns\tests\somsub');
    case 7
        testType = 'Test w/range and prefix';
        aoNormSumFrames(testFilePath, 1:10, [], 'paranorm_');
    case 8
        testType = 'Test w/path and prefix';
        aoNormSumFrames(testFilePath, [], ...
            'D:\p4root\specA\aoslo\mtlbfnctns\tests\somsub', 'abnorm_');
    otherwise
        testType = 'Test w/all arguments';
        aoNormSumFrames(testFilePath, 1:10, ...
            'D:\p4root\specA\aoslo\mtlbfnctns\tests\somsub', 'lognorm_');
end

fprintf(1, '--------------------------------------------------\nEND %s\n', ...
    testType);
fprintf(1, '--------------------------------------------------\n');
