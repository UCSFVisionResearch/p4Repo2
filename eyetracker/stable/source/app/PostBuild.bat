rem ##########################################################################
rem # Post-build step to copy the executable to stim machine
rem ##########################################################################

IF NOT EXIST \\ophth-330-stim2\EyeTrackingTest GOTO donenow

IF EXIST \\ophth-330-stim2\EyeTrackingTest\merle GOTO docopy

rem Make directory merle
mkdir \\ophth-330-stim2\EyeTrackingTest\merle

:docopy
copy /Y %1 \\ophth-330-stim2\EyeTrackingTest\merle

:donenow
echo Done