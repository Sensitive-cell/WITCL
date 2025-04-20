
function varargout = Cluster(varargin)

% CLUSTER M-file for Cluster.fig
%      CLUSTER, by itself, creates a new CLUSTER or raises the existing
%      singleton*.
%
%      H = CLUSTER returns the handle to a new CLUSTER or the handle to
%      the existing singleton*.
%
%      CLUSTER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in CLUSTER.M with the given input arguments.
%
%      CLUSTER('Property','Value',...) creates a new CLUSTER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Cluster_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Cluster_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Cluster

% Last Modified by GUIDE v2.5 02-Dec-2011 01:07:46

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Cluster_OpeningFcn, ...
                   'gui_OutputFcn',  @Cluster_OutputFcn, ...
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


% --- Executes just before Cluster is made visible.
function Cluster_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Cluster (see VARARGIN)

% Choose default command line output for Cluster
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes Cluster wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = Cluster_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;
global patternStatus;
patternStatus=0;

% --- Executes on button press in Clear.
function Clear_Callback(hObject, eventdata, handles)
% hObject    handle to Clear (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global activex;
activex.invoke('ResetNum');

%handlse.activex2.DrawStandardNum;

% --- Executes on button press in Input1.
function Input1_Callback(hObject, eventdata, handles)
InputStandardNum(1,handles);

% --- Executes on button press in Input2.
function Input2_Callback(hObject, eventdata, handles)
InputStandardNum(2,handles);

% --- Executes on button press in Input3.
function Input3_Callback(hObject, eventdata, handles)
InputStandardNum(3,handles);

% --- Executes on button press in Input4.
function Input4_Callback(hObject, eventdata, handles)
InputStandardNum(4,handles);

% --- Executes on button press in Input5.
function Input5_Callback(hObject, eventdata, handles)
InputStandardNum(5,handles);

% --- Executes on button press in Input6.
function Input6_Callback(hObject, eventdata, handles)
InputStandardNum(6,handles);

% --- Executes on button press in Input7.
function Input7_Callback(hObject, eventdata, handles)
InputStandardNum(7,handles);

% --- Executes on button press in Input8.
function Input8_Callback(hObject, eventdata, handles)
InputStandardNum(8,handles);

% --- Executes on button press in Input9.
function Input9_Callback(hObject, eventdata, handles)
InputStandardNum(9,handles);

% --- Executes on button press in Input0.
function Input0_Callback(hObject, eventdata, handles)
InputStandardNum(0,handles);


function InputStandardNum(num,handles)
global activex;
activex.invoke('DrawStandardNum',num);


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over Clear.
function Clear_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to Clear (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --- Executes on button press in ClearRight.
function ClearRight_Callback(hObject, eventdata, handles)
% hObject    handle to ClearRight (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
axes(handles.ShowResult);
temp=imread('Clear.bmp');
imshow(temp);
set(handles.textResult,'String','');
%cla reset;


% --- Executes on button press in HandWrite.
function HandWrite_Callback(hObject, eventdata, handles)
% hObject    handle to HandWrite (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
a=get(handles.PicLeft,'Position');
b=get(handles.Input,'Position');
c=[a(1)+b(1)-1 a(2)+b(2)+2 a(3) a(4)];
set(handles.PicLeft,'Visible','Off');
global activex;
activex=actxcontrol('CLUSTERL.Clusterl1ctrl.1',c);
set(handles.InputStandard,'visible','On');
set(handles.Clear,'visible','On');
set(handles.OpenPic,'visible','Off');
set(handles.HandWrite,'Enable','Off');
set(handles.PicPattern,'Enable','On');
set(handles.InputPattern,'String','��ǰΪ��дģʽ');

% --- Executes on button press in PicPattern.
function PicPattern_Callback(hObject, eventdata, handles)
% hObject    handle to PicPattern (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global activex;
delete(activex); 

set(handles.PicLeft,'Visible','On');
set(handles.InputStandard,'visible','Off');
set(handles.Clear,'visible','Off');
set(handles.OpenPic,'visible','On');
set(handles.HandWrite,'Enable','On');
set(handles.PicPattern,'Enable','Off');
set(handles.InputPattern,'String','��ǰΪλͼģʽ');
axes(handles.PicLeft);
temp=imread('Clear.bmp');
imshow(temp);



% --- Executes on button press in OpenPic.
function OpenPic_Callback(hObject, eventdata, handles)
% hObject    handle to OpenPic (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[fname,pname]=uigetfile({'*.bmp';'*.jpg';'*.*'},'File Selector');
global Data;%Դͼ��
global pic;%��ֵͼ��
Data=imread([pname fname]);
axes(handles.PicLeft);
pic=im2bw(Data,0.3);
imshow(Data);
global patternStatus;
patternStatus=1;


%��ʾ������
function ShowResult( pattern,patternnum, handles )
global Data;
global height;
global width;
axes(handles.ShowResult);
imshow(Data);
pos=get(handles.ShowResult,'Position');
widthRatio=pos(3)/width;
heightRatio=pos(4)/height;
if(widthRatio<heightRatio)
    heightRatio=widthRatio;
else
    widthRatio=heightRatio;
end
for i=1:patternnum
    str1=num2str(pattern(i).index);
    str2=['���:' num2str(pattern(i).category)];
    text((pattern(i).rightbottom_x*widthRatio),(height*heightRatio-pattern(i).lefttop_y*heightRatio+3),str1,'Units','pixels','Color','Blue');%��ʾ����
    text((pattern(i).lefttop_x*widthRatio-3),(height*heightRatio-pattern(i).rightbottom_y*heightRatio-6),str2,'Units','pixels','Color','Red');%��ʾ���
end


% --------------------------------------------------------------------
function ZuiJinLin_Callback(hObject, eventdata, handles)
% hObject    handle to ZuiJinLin (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ZuiLinJin( m_pattern,patternNum );%���ٽ�����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���ٽ����������');
set(gcf,'Pointer','arrow');





% --------------------------------------------------------------------
function Cluster_Callback(hObject, eventdata, handles)
% hObject    handle to Cluster (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function Unknown_Callback(hObject, eventdata, handles)
% hObject    handle to Unknown (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)




% --------------------------------------------------------------------
function ZuiDaZuiXiaoJuLi_Callback(hObject, eventdata, handles)
% hObject    handle to ZuiDaZuiXiaoJuLi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ZuiDaZuiXiaoJuLi( m_pattern,patternNum );%�����С���뷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','�����С���뷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function Layer_Callback(hObject, eventdata, handles)
% hObject    handle to Layer (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function ZuiDuanJuLi_Callback(hObject, eventdata, handles)
% hObject    handle to ZuiDuanJuLi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ZuiDuanJuLi( m_pattern,patternNum );%��̾��뷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��̾��뷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function ZuiChangJuLi_Callback(hObject, eventdata, handles)
% hObject    handle to ZuiChangJuLi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ZuiChangJuLi( m_pattern,patternNum );%����뷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','����뷨������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function ZhongJianJuLi_Callback(hObject, eventdata, handles)
% hObject    handle to ZhongJianJuLi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ZhongJianJuLi( m_pattern,patternNum );%�м���뷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','�м���뷨������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function ZhongXin_Callback(hObject, eventdata, handles)
% hObject    handle to ZhongXin (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ZhongXin( m_pattern,patternNum );%���ķ�����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���ķ�������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function LePingJunJuLi_Callback(hObject, eventdata, handles)
% hObject    handle to LePingJunJuLi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_LeiPingJunJuLi( m_pattern,patternNum );%��ƽ�����뷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ƽ�����뷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function Dynamic_Callback(hObject, eventdata, handles)
% hObject    handle to Dynamic (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function KJunZhi_Callback(hObject, eventdata, handles)
% hObject    handle to KJunZhi (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_KJunZhi( m_pattern,patternNum );%K��ֵ������
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','K��ֵ��������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function ISODATA_Callback(hObject, eventdata, handles)
% hObject    handle to ISODATA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ISODATA( m_pattern,patternNum );%ISODATA����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','ISODATA������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function Fuzzy_Callback(hObject, eventdata, handles)
% hObject    handle to Fuzzy (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --------------------------------------------------------------------
function FuzzyCluster_Callback(hObject, eventdata, handles)
% hObject    handle to FuzzyCluster (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_FuzzyCluster( m_pattern,patternNum );%ģ������
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','ģ��������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function Genetic_Callback(hObject, eventdata, handles)
% hObject    handle to Genetic (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --------------------------------------------------------------------
function GA_Callback(hObject, eventdata, handles)
% hObject    handle to GA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_GA( m_pattern,patternNum );%�Ŵ��㷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','�Ŵ��㷨������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function CSA_Callback(hObject, eventdata, handles)
% hObject    handle to CSA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_CSA( m_pattern,patternNum );%�Ŵ��㷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��¡ѡ���㷨������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function EP_Callback(hObject, eventdata, handles)
% hObject    handle to EP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_EP( m_pattern,patternNum );%�����滮�㷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','�����滮�㷨������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function ES_Callback(hObject, eventdata, handles)
% hObject    handle to ES (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ES( m_pattern,patternNum );%���������㷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���������㷨������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function popCluster_Callback(hObject, eventdata, handles)
% hObject    handle to popCluster (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --------------------------------------------------------------------
function AF_Callback(hObject, eventdata, handles)
% hObject    handle to C_ArtificialFish (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_AF( m_pattern,patternNum );%�˹���Ⱥ�㷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','�˹���Ⱥ�㷨������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function SFLA_Callback(hObject, eventdata, handles)
% hObject    handle to SFLA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_SFLA( m_pattern,patternNum );%�����㷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','�����㷨������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function AntUnknown_Callback(hObject, eventdata, handles)
% hObject    handle to AntUnknown (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_AntCluster( m_pattern,patternNum );%��Ⱥ�㷨����(δ֪������Ŀ)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��Ⱥ�㷨������(δ֪������Ŀ)');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function AntKnown_Callback(hObject, eventdata, handles)
% hObject    handle to AntKnown (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_AntClusterKnown( m_pattern,patternNum );%��Ⱥ�㷨����(��֪������Ŀ)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��Ⱥ�㷨������(��֪������Ŀ)');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function PSO_Callback(hObject, eventdata, handles)
% hObject    handle to PSO (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_PSO( m_pattern,patternNum );%����Ⱥ�㷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','����Ⱥ�㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function MoNiTuiHuo_Callback(hObject, eventdata, handles)
% hObject    handle to MoNiTuiHuo (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_MoNiTuiHuo( m_pattern,patternNum );%ģ���˻��㷨����
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','ģ���˻��㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function KJunZhi2_Callback(hObject, eventdata, handles)
% hObject    handle to KJunZhi2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_KJunZhi2( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','K��ֵ��������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function LIANGZI_Callback(hObject, eventdata, handles)
% hObject    handle to LIANGZI (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)




% --------------------------------------------------------------------
function UserHelp_Callback(hObject, eventdata, handles)
% hObject    handle to UserHelp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
str1='����������ʹ�ð�����';
str2='�����������������Ϊ������壨�󣩺������壨�ң�����������������������Ʒ����������ʾ��������';
strb1='';
str3='����������̰����²���������С�';
str4='1.    ������Ʒ��';
str5='      ������Ʒ�����ַ�ʽ��1) λͼ���� 2) ��д����';
str6='      1) λͼ���룺';
str7='                    ������Ϸ�^����ģʽ^����е�^λͼģʽ^��ť(���������Ĭ��Ϊλͼģʽ����';
str8='                    ������弴ת��Ϊλͼ���룬���^��һ��ͼƬ^��ť������^��λͼ�Ի���^��';
str9='                    ѡ��һ�ź��о�����Ʒ��ͼ�񣬵���򿪣���������м���ʾ����ǰλͼ��';
str10='     2) ��д���룺';
str11='                   ������Ϸ�^����ģʽ^����е�^��дģʽ^��ť��������弴ת��Ϊ��д���룬';
str12='                   ��ʱ����������������д���ֻ�ͼ����Ʒ��Ҳ����ͨ�����^�����׼����^';
str13='                   ����е����ְ�ť�����׼���֡�';
str14='2.	ѡ������㷨';
str15='     ������Ʒ�������˵���ѡ��һ�־����㷨�����о��������';
str16='3.	��ʾ������';
str17='     ������ɺ󣬻�������������ʾ����������Ʒ���Ϸ�Ϊ��Ʒ��ţ���Ʒ���·�Ϊ��Ʒ�������š�';
str=char(str1,str2,strb1,str3,strb1,str4,str5,strb1,str6,str7,str8,str9,strb1,str10,str11,str12,str13,strb1,str14,str15,strb1,str16,str17);
helpdlg(str,'����������ʹ�ð���');

% --------------------------------------------------------------------
function IGA_Callback(hObject, eventdata, handles)
% hObject    handle to IGA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_IGA( m_pattern,patternNum );
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','�����㷨������');
set(gcf,'Pointer','arrow');









% --------------------------------------------------------------------
function QGA2_Callback(hObject, eventdata, handles)
% hObject    handle to QGA2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handset(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_QGA2( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���ӽ����㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function QGA_Callback(hObject, eventdata, handles)
% hObject    handle to QGA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_QGA( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���ӽ����㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function exam_Callback(hObject, eventdata, handles)
% hObject    handle to exam (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=TEST( m_pattern,patternNum );
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','����');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function CSO_Callback(hObject, eventdata, handles)
% hObject    handle to CSO (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_CSO( m_pattern,patternNum );
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','èȺ�㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function CPSO_Callback(hObject, eventdata, handles)
% hObject    handle to CPSO (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_CPSO( m_pattern,patternNum );
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','èȺ+����Ⱥ�㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function BFO_Callback(hObject, eventdata, handles)
% hObject    handle to BFO (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_BFO( m_pattern,patternNum );
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','ϸ����ʳ�㷨������');
set(gcf,'Pointer','arrow');


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% --------------------------------------------------------------------
function IM_Callback(hObject, eventdata, handles)%�����㷨����
% hObject    handle to IM (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function C_IGA_Callback(hObject, eventdata, handles)
% hObject    handle to C_IGA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_IGA( m_pattern,patternNum );
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','�����㷨������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function C_AIGA_Callback(hObject, eventdata, handles)
% hObject    handle to C_AIGA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_AIGA( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','����Ӧ�����㷨������');
set(gcf,'Pointer','arrow');








% --------------------------------------------------------------------
function C_DIGA_Callback(hObject, eventdata, handles)
% hObject    handle to C_DIGA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_DIGA( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��̬�����㷨������');
set(gcf,'Pointer','arrow');








% --------------------------------------------------------------------
function C_CSA_Callback(hObject, eventdata, handles)
% hObject    handle to C_CSA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_CSA( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��¡ѡ���㷨������');
set(gcf,'Pointer','arrow');





% --------------------------------------------------------------------
function C_EAIGA_Callback(hObject, eventdata, handles)
% hObject    handle to C_EAIGA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_EAIGA( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ǿ����Ӧ�����㷨������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function C_EDIGA_Callback(hObject, eventdata, handles)
% hObject    handle to C_EDIGA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_EDIGA( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ǿ��̬�㷨������');
set(gcf,'Pointer','arrow');





% --------------------------------------------------------------------
function EAIGA_Callback(hObject, eventdata, handles)
% hObject    handle to C_EAIGA (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_EAIGA( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ǿ����Ӧ�����㷨������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function C_IP_Callback(hObject, eventdata, handles)%���߹滮����
% hObject    handle to C_IP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_IP( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���߹滮���������');
set(gcf,'Pointer','arrow');




% --------------------------------------------------------------------
function C_AIP_Callback(hObject, eventdata, handles)
% hObject    handle to C_AIP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_AIP( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','����Ӧ���߹滮�㷨������');
set(gcf,'Pointer','arrow');
% --------------------------------------------------------------------
function C_DIP_Callback(hObject, eventdata, handles)
% hObject    handle to C_DIP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_DIP( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��̬���߹滮�㷨������');
set(gcf,'Pointer','arrow');





% --------------------------------------------------------------------
function C_EAIP_Callback(hObject, eventdata, handles)
% hObject    handle to C_EAIP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_EAIP( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ǿ����Ӧ���߹滮�㷨������');
set(gcf,'Pointer','arrow');
% --------------------------------------------------------------------
function C_EDIP_Callback(hObject, eventdata, handles)
% hObject    handle to C_EDIP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_EDIP( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ǿ��̬���߹滮�㷨������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function IS_Callback(hObject, eventdata, handles)%���߲��Ծ���
% hObject    handle to IS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function C_IS_Callback(hObject, eventdata, handles)
% hObject    handle to C_IP (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_IS( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���߲����㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function C_AIS_Callback(hObject, eventdata, handles)
% hObject    handle to C_AIS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_AIS( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','����Ӧ���߲����㷨������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function C_EAIS_Callback(hObject, eventdata, handles)
% hObject    handle to C_EAIS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_EAIS( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ǿ���߲����㷨������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function C_DIS_Callback(hObject, eventdata, handles)
% hObject    handle to C_DIS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_DIS( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��̬���߲����㷨������');
set(gcf,'Pointer','arrow');


% --------------------------------------------------------------------
function C_EDIS_Callback(hObject, eventdata, handles)
% hObject    handle to C_EDIS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_EDIS( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ǿ��̬���߲����㷨������');
set(gcf,'Pointer','arrow');


% % --------------------------------------------------------------------
% function C_EDIP_Callback(hObject, eventdata, handles)
% % hObject    handle to C_EDIP (see GCBO)
% % eventdata  reserved - to be defined in a future version of MATLAB
% % handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function C_CSAE_Callback(hObject, eventdata, handles)
% hObject    handle to C_CSAE (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_CSAE( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��¡ѡ���㷨E������');
set(gcf,'Pointer','arrow');

% --------------------------------------------------------------------
function C_CSS_Callback(hObject, eventdata, handles)
% hObject    handle to C_CSS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_CSS( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��¡ѡ����Ծ�����');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function C_CSSE_Callback(hObject, eventdata, handles)
% hObject    handle to C_CSSE (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_CSSE( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��¡ѡ�����E������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function Untitled_2_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function Untitled_13_Callback(hObject, eventdata, handles)
% hObject    handle to Untitled_13 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



% --------------------------------------------------------------------
function ABC_Callback(hObject, eventdata, handles)
% hObject    handle to ABC (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_ABC( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��Ⱥ�㷨������');
set(gcf,'Pointer','arrow');





% --------------------------------------------------------------------
function Memetic_Callback(hObject, eventdata, handles)
% hObject    handle to Memetic (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_MTC( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','Memetic�㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function DE_Callback(hObject, eventdata, handles)
% hObject    handle to DE (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_DE( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��ֽ����㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function TS_Callback(hObject, eventdata, handles)
% hObject    handle to TS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function C_TS_Callback(hObject, eventdata, handles)
% hObject    handle to C_TS (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_TS( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���������㷨������');
set(gcf,'Pointer','arrow');



% --------------------------------------------------------------------
function QGA3_Callback(hObject, eventdata, handles)
% hObject    handle to QGA3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_QGA3( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','���Ӽ����㷨������');
set(gcf,'Pointer','arrow');




% --------------------------------------------------------------------
function WSO_Callback(hObject, eventdata, handles)
% hObject    handle to WSO (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(gcf,'Pointer','watch');
[m_pattern,patternNum]=GetFeature(handles);%�õ�����
if(patternNum==0)
    msgbox('�����������Ʒ��������������Ʒ');
    set(gcf,'Pointer','arrow');
    return;
end
m_pattern=C_WSO( m_pattern,patternNum );%K��ֵ�����ࣨmatlab�����亯��)
ShowResult(m_pattern,patternNum,handles);
set(handles.textResult,'String','��Ⱥ�㷨������');
set(gcf,'Pointer','arrow');
