function varargout = correlate_ui(varargin)
% CORRELATE_UI MATLAB code for correlate_ui.fig
%      CORRELATE_UI, by itself, creates a new CORRELATE_UI or raises the existing
%      singleton*.
%
%      H = CORRELATE_UI returns the handle to a new CORRELATE_UI or the handle to
%      the existing singleton*.
%
%      CORRELATE_UI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in CORRELATE_UI.M with the given input arguments.
%
%      CORRELATE_UI('Property','Value',...) creates a new CORRELATE_UI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before correlate_ui_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to correlate_ui_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help correlate_ui

% Last Modified by GUIDE v2.5 11-Jan-2017 16:11:56

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @correlate_ui_OpeningFcn, ...
                   'gui_OutputFcn',  @correlate_ui_OutputFcn, ...
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


% --- Executes just before correlate_ui is made visible.
function correlate_ui_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to correlate_ui (see VARARGIN)

% Choose default command line output for correlate_ui
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);
h_figs = get(0,'children');
h_fig = findobj(h_figs, 'tag','wave_clus_figure');
USER_DATA = get(h_fig, 'userdata');
classes = USER_DATA{6};
classes = classes';
% Find all the unique classes
classes_names = sort(unique(classes));
classes_names = classes_names(classes_names>0);
set(handles.cluster1,'String',classes_names);
set(handles.cluster2,'String',classes_names);
% UIWAIT makes correlate_ui wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = correlate_ui_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in cluster1.
function cluster1_Callback(hObject, eventdata, handles)
% hObject    handle to cluster1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns cluster1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from cluster1


% --- Executes during object creation, after setting all properties.
function cluster1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cluster1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in cluster2.
function cluster2_Callback(hObject, eventdata, handles)
% hObject    handle to cluster2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns cluster2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from cluster2


% --- Executes during object creation, after setting all properties.
function cluster2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cluster2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

h_figs = get(0,'children');
h_fig = findobj(h_figs, 'tag','wave_clus_figure');
USER_DATA = get(h_fig, 'userdata');
classes = USER_DATA{6};
classes = classes';
index = USER_DATA{3};

c1Times = index(classes==get(handles.cluster1,'Value'));
c2Times = index(classes==get(handles.cluster2,'Value'));
c1Sig = histc(c1Times,1:max(c1Times));
c2Sig = histc(c2Times,1:max(c2Times));

axes(handles.axes1);
[acor,lag] = xcorr(c1Sig,c2Sig,100);
plot(lag,acor);
