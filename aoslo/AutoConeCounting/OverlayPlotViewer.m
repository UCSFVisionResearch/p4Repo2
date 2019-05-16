function varargout = OverlayPlotViewer(varargin)
%% OVERLAYPLOTVIEWER MATLAB code for OverlayPlotViewer.fig
%      OVERLAYPLOTVIEWER, by itself, creates a new OVERLAYPLOTVIEWER or raises the existing
%      singleton*.
%
%      H = OVERLAYPLOTVIEWER returns the handle to a new OVERLAYPLOTVIEWER or the handle to
%      the existing singleton*.
%
%      OVERLAYPLOTVIEWER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in OVERLAYPLOTVIEWER.M with the given input arguments.
%
%      OVERLAYPLOTVIEWER('Property','Value',...) creates a new OVERLAYPLOTVIEWER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before OverlayPlotViewer_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to OverlayPlotViewer_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help OverlayPlotViewer

% Last Modified by GUIDE v2.5 27-Jan-2014 13:08:40

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @OverlayPlotViewer_OpeningFcn, ...
                   'gui_OutputFcn',  @OverlayPlotViewer_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


function OverlayPlotViewer_OpeningFcn(hObject, eventdata, handles, varargin)
%% --- Executes just before OverlayPlotViewer is made visible.
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to OverlayPlotViewer (see VARARGIN)

% Choose default command line output for OverlayPlotViewer
handles.output = hObject;

% Add image to handles structure
[handles, chose] = getMosaicFile(hObject, handles);
while ~chose
    [handles, chose] = getMosaicFile(hObject, handles);
end

set(handles.cutoffDisplay, 'String', 0.61);
set(handles.thresholdDisplay, 'String', 0);
cutoffDisplay_Callback(handles.cutoffDisplay, eventdata, handles);
thresholdDisplay_Callback(handles.thresholdDisplay, eventdata, handles);

set(handles.coneToggle, 'Value', 1);

handles = updateConeCount(hObject, handles);
repaint(hObject, handles);


function varargout = OverlayPlotViewer_OutputFcn(hObject, eventdata, handles)
%% --- Outputs from this function are returned to the command line.
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


function coneToggle_Callback(hObject, eventdata, handles)
%% --- Executes on button press in coneToggle.
% hObject    handle to coneToggle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if get(hObject, 'Value')
    % Toggled on; untoggle other button and update image
    set(handles.overlayToggle, 'Value', 0);
    repaint(hObject, handles);
end


function overlayToggle_Callback(hObject, eventdata, handles)
%% --- Executes on button press in overlayToggle.
% hObject    handle to overlayToggle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if get(hObject, 'Value')
    % Toggled on; untoggle other button and update image
    set(handles.coneToggle, 'Value', 0);
    repaint(hObject, handles);
end


function cutoffSlider_Callback(hObject, eventdata, handles)
%% --- Executes on slider movement.
% hObject    handle to cutoffSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

set(handles.cutoffDisplay, 'String', get(hObject, 'Value'));

% Update cone plot
handles = updateConeCount(hObject, handles);
repaint(hObject, handles);


function cutoffSlider_CreateFcn(hObject, eventdata, handles)
%% --- Executes during object creation, after setting all properties.
% hObject    handle to cutoffSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


function cutoffDisplay_Callback(hObject, eventdata, handles)
%% --- Executes during object creation, after setting all properties.
% hObject    handle to cutoffDisplay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of cutoffDisplay as text
%        str2double(get(hObject,'String')) returns contents of cutoffDisplay as a double
val = str2double(get(hObject, 'String'));
min = get(handles.cutoffSlider, 'Min');
max = get(handles.cutoffSlider, 'Max');
if val < min
    val = min;
elseif val > max
    val = max;
end
set(handles.cutoffSlider, 'Value', val);
handles = updateConeCount(hObject, handles);
repaint(hObject, handles);


function cutoffDisplay_CreateFcn(hObject, eventdata, handles)
%% --- Executes during object creation, after setting all properties.
% hObject    handle to cutoffDisplay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function thresholdSlider_Callback(hObject, eventdata, handles)
%% --- Executes on slider movement.
% hObject    handle to thresholdSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

set(handles.thresholdDisplay, 'String', get(hObject, 'Value'));

% Update cone plot
handles = updateConeCount(hObject, handles);
repaint(hObject, handles);


function thresholdSlider_CreateFcn(hObject, eventdata, handles)
%% --- Executes during object creation, after setting all properties.
% hObject    handle to thresholdSlider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


function thresholdDisplay_Callback(hObject, eventdata, handles)
%% --- Executes during object creation, after setting all properties.
% hObject    handle to thresholdDisplay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of thresholdDisplay as text
%        str2double(get(hObject,'String')) returns contents of thresholdDisplay as a double
val = str2double(get(hObject, 'String'));
min = get(handles.thresholdSlider, 'Min');
max = get(handles.thresholdSlider, 'Max');
if val < min
    val = min;
elseif val > max
    val = max;
end
set(handles.thresholdSlider, 'Value', val);
handles = updateConeCount(hObject, handles);
repaint(hObject, handles);


function thresholdDisplay_CreateFcn(hObject, eventdata, handles)
%% --- Executes during object creation, after setting all properties.
% hObject    handle to thresholdDisplay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function browseButton_Callback(hObject, eventdata, handles)
%% --- Executes on button press in browseButton.
% hObject    handle to browseButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

[handles, chose] = getMosaicFile(hObject, handles);
if chose
    handles = updateConeCount(hObject, handles);
    repaint(hObject, handles);
end


function mosaicFileName_Callback(hObject, eventdata, handles)
%% --- Should never be called
% hObject    handle to mosaicFileName (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.mosaicFileName, 'String', handles.filePath);


function mosaicFileName_CreateFcn(hObject, eventdata, handles)
%% --- Executes during object creation, after setting all properties.
% hObject    handle to mosaicFileName (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function [handlesOut, chose] = getMosaicFile(hObject, handlesIn)
%% Chose a mosaic file name
% Edits handles, so you must re-read it from the application object.
chose = 0;
[fileName, pathName] = uigetfile({'*.tif;*.tiff';'*.png'}, 'Choose cone mosaic:');
if ~isequal(fileName, 0) && ~isequal(pathName, 0)
    handlesIn.filePath = fullfile(pathName, fileName);
    handlesIn.imOriginal = imread(handlesIn.filePath);
    % Update handles structure
    guidata(hObject, handlesIn);
    set(handlesIn.mosaicFileName, 'String', handlesIn.filePath);
    chose = 1;
    handlesOut = handlesIn;
end


function handlesOut = updateConeCount(hObject, handlesIn)
%% Update cone plot
% Edits handles, so you must re-read it from the application object.
cutoff = get(handlesIn.cutoffSlider, 'Value');
threshold = get(handlesIn.thresholdSlider, 'Value');
[handlesIn.conesX, handlesIn.conesY] = AOim2xy(handlesIn.imOriginal, cutoff, threshold);
guidata(hObject, handlesIn);
set(handlesIn.coneCount, 'String', size(handlesIn.conesX, 1));
handlesOut = handlesIn;


function repaint(hObject, handles)
%% Repaint the cone mosaic, possibly with plot.
if get(handles.coneToggle, 'Value')
    axes(handles.imageFrame);
    imshow(handles.imOriginal);
else
    axes(handles.imageFrame);
    imshow(handles.imOriginal);
    hold on;
    plot(handles.conesX, handles.conesY, '.g', 'MarkerSize', 1);
    hold off;
end