rem ##########################################################################
rem # Post-build step to copy the executable to stim machine
rem ##########################################################################

IF NOT EXIST \\ophth-k335-test\EyeTrackingTest GOTO donenow

IF EXIST \\ophth-k335-test\EyeTrackingTest\merle GOTO docopy

rem Make directory merle
mkdir \\ophth-k335-test\EyeTrackingTest\merle

:docopy
copy /Y %1 \\ophth-k335-test\EyeTrackingTest\merle

:donenow
echo Done