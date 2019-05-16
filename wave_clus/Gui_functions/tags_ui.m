function varargout = tags_ui(varargin)
% TAGS_UI MATLAB code for tags_ui.fig
%      TAGS_UI, by itself, creates a new TAGS_UI or raises the existing
%      singleton*.
%
%      H = TAGS_UI returns the handle to a new TAGS_UI or the handle to
%      the existing singleton*.
%
%      TAGS_UI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TAGS_UI.M with the given input arguments.
%
%      TAGS_UI('Property','Value',...) creates a new TAGS_UI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before tags_ui_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to tags_ui_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help tags_ui

% Last Modified by GUIDE v2.5 17-Jan-2017 10:34:52

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @tags_ui_OpeningFcn, ...
                   'gui_OutputFcn',  @tags_ui_OutputFcn, ...
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


% --- Executes just before tags_ui is made visible.
function tags_ui_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to tags_ui (see VARARGIN)

% Choose default command line output for tags_ui
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
%classes_names = classes_names(classes_names>0); to exclude Cluster0
set(handles.classMenu,'String',classes_names);
classMenu_Callback(handles.classMenu,eventdata,handles);
% UIWAIT makes tags_ui wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = tags_ui_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in classMenu.
function classMenu_Callback(hObject, eventdata, handles)
% hObject    handle to classMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns classMenu contents as cell array
%        contents{get(hObject,'Value')} returns selected item from classMenu
selClass = get(hObject,'Value')-1;
h_figs = get(0,'children');
h_fig = findobj(h_figs, 'tag','wave_clus_figure');
USER_DATA = get(h_fig,'userdata');
tags = USER_DATA{17};
classes = USER_DATA{6};
ind = find(classes==selClass,1); %find first instance

switch tags(ind) 
    case -1
        set(handles.tagTrash,'Value',1);
        set(handles.tag0,'Value',0);
        set(handles.tag1,'Value',0);
    case 0
        set(handles.tagTrash,'Value',0);
        set(handles.tag0,'Value',1);
        set(handles.tag1,'Value',0);
    case 1
        set(handles.tagTrash,'Value',0);
        set(handles.tag0,'Value',0);
        set(handles.tag1,'Value',1);
    otherwise
        set(handles.tagTrash,'Value',0);
        set(handles.tag0,'Value',1);
        set(handles.tag1,'Value',0);
end
    

% --- Executes during object creation, after setting all properties.
function classMenu_CreateFcn(hObject, eventdata, handles)
% hObject    handle to classMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in tag0.
function tag0_Callback(hObject, eventdata, handles)
% hObject    handle to tag0 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of tag0
%Default Value will always be zero
set(handles.tagTrash,'Value',0);
set(handles.tag0,'Value',1);
set(handles.tag1,'Value',0);




% --- Executes on button press in tag1.
function tag1_Callback(hObject, eventdata, handles)
% hObject    handle to tag1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of tag1
set(handles.tagTrash,'Value',0);
set(handles.tag0,'Value',0);
set(handles.tag1,'Value',1);


% --- Executes on button press in tagTrash.
function tagTrash_Callback(hObject, eventdata, handles)
% hObject    handle to tagTrash (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of tagTrash
set(handles.tagTrash,'Value',1);
set(handles.tag0,'Value',0);
set(handles.tag1,'Value',0);


% --- Executes on button press in saveTag.
function saveTag_Callback(hObject, eventdata, handles)
% hObject    handle to saveTag (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
selClass = get(handles.classMenu,'Value')-1;
selTag = -1;
if get(handles.tag0,'Value') ==1
    selTag =0;
elseif get(handles.tag1,'Value') ==1
    selTag =1;
end
h_figs = get(0,'children');
h_fig = findobj(h_figs, 'tag','wave_clus_figure');
USER_DATA = get(h_fig,'userdata');
tags = USER_DATA{17};
classes = USER_DATA{6};
tags(classes ==selClass) = selTag;
USER_DATA{17} = tags;
set(h_fig,'userdata',USER_DATA);




