function AutoChoose(handles)
global signal;
global fs;
global d;
y=signal;
Nn=256;
DigitalFilter_value=get(handles.DigitalFilter,'Value'); %�˲�������
FilterDesign_value=get(handles.FilterDesign,'Value');   %IIR�����˲���
Windows_value=get(handles.Windows,'Value');             %FIR������ѡ��
FilterType_value=get(handles.FilterType,'Value');  %��ͨ��ͨ��ͨ��ͨ����
DisplayType_value=get(handles.DisplayType,'Value');%��ʾ�������ԵĻ������

Rp_value=str2double(get(handles.Rp,'String'));
Rs_value=str2double(get(handles.Rs,'String'));
Fs_value=str2double(get(handles.Fs,'String'));
fs=Fs_value;
% Fs=Fs_value;
Fp1_value=str2double(get(handles.Fp1,'String'));
Fp2_value=str2double(get(handles.Fp2,'String'));
Fs1_value=str2double(get(handles.Fs1,'String'));
Fs2_value=str2double(get(handles.Fs2,'String'));
wp1=tan((pi*Fp1_value/Fs_value)/2);
wp2=tan((pi*Fp2_value/Fs_value)/2);
ws1=tan((pi*Fs1_value/Fs_value)/2);
ws2=tan((pi*Fs2_value/Fs_value)/2);
wp=[wp1,wp2];ws=[ws1,ws2];
wap=2*Fs_value*tan(wp./2);was=2*Fs_value*tan(ws./2);
if(DigitalFilter_value==1)
    if(FilterDesign_value==1)
       if((FilterType_value==1)||(FilterType_value==2))
         % set(handles.Fp1,'string','30');set(handles.Fs1,'string','40')
          [n,Wn]=buttord(wp1,ws1,Rp_value,Rs_value,'s')
         set(handles.MinOrderDisplay,'string',num2str(n))
       else
          if((FilterType_value==3)||(FilterType_value==4))
              [n,Wn]=buttord(wap,was,Rp_value,Rs_value,'s')
              set(handles.MinOrderDisplay,'string',num2str(n))
          end
       end
%          switch FilterType_value
%                     case 1:
%                          [n,Wn]=buttord(wp1,ws1,Rp_value,Rs_value);
%                          set(handles.MinOrderDisplay,'string',num2str(n))
%                          set(handles.textFp1,'string','ͨ����ֹƵ��');
%                          set(handles.textFs1,'string','�����ֹƵ��');
%                     case 2:
%                         [n,Wn]=buttord(wp1,ws1,Rp_value,Rs_value);
%                         set(handles.MinOrderDisplay,'string',num2str(n))
%                         set(handles.textFp1,'string','�����ֹƵ��');
%                         set(handles.textFs1,'string','ͨ����ֹƵ��');
%                     case 3:
%                         [n,Wn]=buttord(wap,was,Rp_value,Rs_value);
%                         set(handles.MinOrderDisplay,'string',num2str(n))
%                         set(handles.textFp1,'string','��ͨ����ֹƵ��Fp1');
%                         set(handles.textFs1,'string','�������ֹƵ��Fs1');
%                         et(handles.textFp2,'string','��ͨ����ֹƵ��Fp2');
%                         set(handles.textFs2,'string','�������ֹƵ��Fs2');
%                         warn('�뱣֤Fs1<Fp1<Fp2<Fs1','My Warn Dialog','modal');
%                     case 4:
%                         [n,Wn]=buttord(wap,was,Rp_value,Rs_value);
%                         set(handles.MinOrderDisplay,'string',num2str(n))
%                         set(handles.textFp1,'string','�������ֹƵ��Fs1');
%                         set(handles.textFs1,'string','��ͨ����ֹƵ��Fp1');
%                         et(handles.textFp2,'string','�������ֹƵ��Fs2');
%                         set(handles.textFs2,'string','��ͨ����ֹƵ��Fp2');
%                         warn('�뱣֤Fp1<Fs1<Fs2<Fp1','My Warn Dialog','modal');   
%                 end
    else
        if(FilterDesign_value==2)
            if((FilterType_value==1)||(FilterType_value==2))
                [n,Wn]=cheb1ord(wp1,ws1,Rp_value,Rs_value,'s')
                set(handles.MinOrderDisplay,'string',num2str(n))
            else
                if((FilterType_value==3)||(FilterType_value==4))
                    [n,Wn]=cheb1ord(wap,was,Rp_value,Rs_value,'s')
                    set(handles.MinOrderDisplay,'string',num2str(n))
                end
            end
        else
            if(FilterDesign_value==3)
                if((FilterType_value==1)||(FilterType_value==2))
                    [n,Wn]=cheb2ord(wp1,ws1,Rp_value,Rs_value,'s')
                    set(handles.MinOrderDisplay,'string',num2str(n))
                else
                    if((FilterType_value==3)||(FilterType_value==4))
                        [n,Wn]=cheb2ord(wap,was,Rp_value,Rs_value,'s')
                        set(handles.MinOrderDisplay,'string',num2str(n))
                    end
                end
     else
        if(FilterDesign_value==4)
            if((FilterType_value==1)||(FilterType_value==2))
                [n,Wn]=ellipord(wp1,ws1,Rp_value,Rs_value,'s')
                set(handles.MinOrderDisplay,'string',num2str(n))
            else
                if((FilterType_value==3)||(FilterType_value==4))
                    [n,Wn]=ellipord(wap,was,Rp_value,Rs_value)
                    set(handles.MinOrderDisplay,'string',num2str(n))
                end
            end               
            end
        end 
        end
    end
else
    if(DigitalFilter_value==2)
      if((FilterType_value==1)||(FilterType_value==2))
          [n,Wn]=buttord(wp1,ws1,Rp_value,Rs_value);
          set(handles.MinOrderDisplay,'string',num2str(n))
       else
          if((FilterType_value==3)||(FilterType_value==4))
              [n,Wn]=buttord(wp,ws,Rp_value,Rs_value);
              set(handles.MinOrderDisplay,'string',num2str(n))
          end
       end  
    end
end
     
%�����˲���IIR����FIRѡ��
switch DigitalFilter_value
    %ѡ��IIR�˲���    
    case 1
%IIR�е�Butterworth��Chebyshev1��Chebyshev2ѡ��        
switch FilterDesign_value  
    %ѡ�����Butterworth�˲���
    case 1
        %ѡ���˲�������
        switch FilterType_value
            %��ͨ�˲���
            case 1          
%                  [b,a]=butter(n,Wn);
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                 [z,p,k]=buttap(n);
                 [bp,ap]=zp2tf(z,p,k); 
                 [bs,as]=lp2lp(bp,ap,wp1); 
                 [bz,az]=bilinear(bs,as,1/2); 
                 [h,f]=freqz(bz,az,256,fs);  
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');            
                 d=filter(bz,az,y);
            %��ͨ�˲���
            case 2                            
%                  [b,a]=butter(n,Wn,'high');
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                 [z,p,k]=buttap(n);
                 [bp,ap]=zp2tf(z,p,k); 
                 [bs,as]=lp2hp(bp,ap,wp1); 
                 [bz,az]=bilinear(bs,as,1/2); 
                 [h,f]=freqz(bz,az,256,fs);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
                 d=filter(bz,az,y);
            %��ͨ�˲���
            case 3   
%                  [b,a]=butter(n,Wn);
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=buttap(n);
                [bp,ap]=zp2tf(z,p,k);
                bw=wap(2)-wap(1);
                w0=sqrt(wap(2)*wap(1))
                [bs,as]=lp2bp(bp,ap,w0,bw);
                [bz,az]=bilinear(bs,as,fs);
                [h,f]=freqz(bz,az,fs/2,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲���
            case 4
%                  [b,a]=butter(n,Wn,'stop');
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=buttap(n);
                [bp,ap]=zp2tf(z,p,k);
                bw=wap(2)-wap(1);
                w0=sqrt(wap(2)*wap(1))
                [bs,as]=lp2bs(bp,ap,w0,bw);
                [bz,az]=bilinear(bs,as,fs);
                [h,f]=freqz(bz,az,fs/2,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');             
        end
    %ѡ�����Cheybyshev1�˲���
    case 2
         %ѡ���˲������� 
       switch FilterType_value
             %��ͨ�˲���
             case 1                   
%                  [b,a]=cheby1(n,Rp_value,Wn);
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=cheb1ap(n,Rp_value);
                [bp,ap]=zp2tf(z,p,k);
                [bs,as]=lp2lp(bp,ap,wp1);
                [bz,az]=bilinear(bs,as,1/2);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
             %��ͨ�˲���
             case 2                  
%                  [b,a]=cheby1(n,Rp_value,Wn,'high');
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=cheb1ap(n,Rp_value);
                [bp,ap]=zp2tf(z,p,k);
                [bs,as]=lp2hp(bp,ap,wp1);
                [bz,az]=bilinear(bs,as,1/2);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
               title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���     
            case 3
%                  [b,a]=cheby1(n,Rp_value,Wn);
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=cheb1ap(n,Rp_value);
                [bp,ap]=zp2tf(z,p,k);
                [bs,as]=lp2bp(bp,ap,wap);
                [bz,az]=bilinear(bs,as,1/2);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲��� 
            case 4
%                  [b,a]=cheby1(n,Rp_value,Wn,'stop');
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=cheb1ap(n,Rp_value);
                [bp,ap]=zp2tf(z,p,k);
                [bs,as]=lp2bs(bp,ap,wap);
                [bz,az]=bilinear(bs,as,1/2);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');              
       end
        %ѡ�����Cheybyshev2�˲���
        case 3
          %ѡ���˲�������    
        switch FilterType_value
             %��ͨ�˲���
             case 1                  
%                  [b,a]=cheby2(n,Rp_value,Wn);
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=cheb2ap(n,Rp_value);
                [bp,ap]=zp2tf(z,p,k);
                [bs,as]=lp2lp(bp,ap,wp1);
                [bz,az]=bilinear(bs,as,1/2);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
             %��ͨ�˲��� 
             case 2                    
%                  [b,a]=cheby2(n,Rp_value,Wn,'high');
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=cheb2ap(n,Rp_value);
                [bp,ap]=zp2tf(z,p,k);
                [bs,as]=lp2hp(bp,ap,wp1);
                [bz,az]=bilinear(bs,as,1/2);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 3
%                  [b,a]=cheby2(n,Rp_value,Wn);
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=cheb2ap(n,Rp_value);
                [bp,ap]=zp2tf(z,p,k);
                [bs,as]=lp2bp(bp,ap,wap);
                [bz,az]=bilinear(bs,as,1/2);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲���
            case 4
%                  [b,a]=cheby2(n,Rp_value,Wn,'stop');
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=cheb2ap(n,Rp_value);
                [bp,ap]=zp2tf(z,p,k);
                [bs,as]=lp2bs(bp,ap,wap);
                [bz,az]=bilinear(bs,as,1/2);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');     
        end 
    %ѡ�������Բ�˲���
    case 4
         %ѡ���˲������� 
       switch FilterType_value
             %��ͨ�˲���
             case 1                   
%                  [b,a]=ellip(n,Rp_value,Rs_value,Wn);
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=ellipap(n,Rp_value,Rs_value);
                [A,B,C,D]=zp2ss(z,p,k);
                [AT,BT,CT,DT]=lp2lp(A,B,C,D,Wn);
                [num1,den1]=ss2tf(AT,BT,CT,DT);
                [bz,az]=bilinear(num1,den1,fs);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
             %��ͨ�˲���
             case 2                  
%                  [b,a]=ellip(n,Rp_value,Rs_value,Wn,'high');
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=ellipap(n,Rp_value,Rs_value);
                [A,B,C,D]=zp2ss(z,p,k);
                [AT,BT,CT,DT]=lp2hp(A,B,C,D,Wn);
                [num1,den1]=ss2tf(AT,BT,CT,DT);
                [bz,az]=bilinear(num1,den1,fs);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���     
            case 3
%                  [b,a]=ellip(n,Rp_value,Rs_value,Wn);
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=ellipap(n,Rp_value,Rs_value);
                [A,B,C,D]=zp2ss(z,p,k);
                [AT,BT,CT,DT]=lp2bp(A,B,C,D,Wn);
                [num1,den1]=ss2tf(AT,BT,CT,DT);
                [bz,az]=bilinear(num1,den1,fs);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                axes(handles.Magnitude);
                if(DisplayType_value==1) plot(f,abs(h))
                else plot(f,20*log10(abs(h)))
                end
                grid on;
               title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲��� 
            case 4
%                  [b,a]=ellip(n,Rp_value,Rs_value,Wn,'stop');
%                  [h,f]=freqz(b,a,Nn,Fs_value);
                [z,p,k]=ellipap(n,Rp_value,Rs_value);
                [A,B,C,D]=zp2ss(z,p,k);
                [AT,BT,CT,DT]=lp2bs(A,B,C,D,Wn);
                [num1,den1]=ss2tf(AT,BT,CT,DT);
                [bz,az]=bilinear(num1,den1,fs);
                [h,f]=freqz(bz,az,256,fs);
                d=filter(bz,az,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');           
       end                
end
    %ѡ��FIR�˲���    
    case 2
%FIR�е�Windowsѡ��        
switch Windows_value 
    %ѡ�����boxcar�˲���
    case 1
        %ѡ���˲�������
        switch FilterType_value
            %��ͨ�˲���
            case 1
                 w=boxcar(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                 
                 d=filter(b,1,y);
                 
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 2           
                 w=boxcar(n+1);
                 b=fir1(n,Wn,'high',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                 d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 3
                 w=boxcar(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲���
            case 4
                 w=boxcar(n+1);
                 b=fir1(n,Wn,'stop',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                  title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');             
        end
    %ѡ�����bartlett�˲���
    case 2
         %ѡ���˲������� 
        switch FilterType_value
            %��ͨ�˲���
            case 1
                 w=bartlett(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 2           
                 w=bartlett(n+1);
                 b=fir1(n,Wn,'high',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                  title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 3
                 w=bartlett(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲���
            case 4
                 w=bartlett(n+1);
                 b=fir1(n,Wn,'stop',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');          
        end
        %ѡ�����blackman�˲���
    case 3
        %ѡ���˲�������
        switch FilterType_value
            %��ͨ�˲���
            case 1
                 w=blackman(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 2            
                 w=blackman(n+1);
                 b=fir1(n,Wn,'high',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 3
                 w=blackman(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲���
            case 4
                 w=blackman(n+1);
                 b=fir1(n,Wn,'stop',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');       
        end
    %ѡ�����hanning�˲���
    case 4
        %ѡ���˲�������
        switch FilterType_value
            %��ͨ�˲���
            case 1
                 w=hanning(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 2            
                 w=hanning(n+1);
                 b=fir1(n,Wn,'high',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                  title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 3
                 w=hanning(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲���
            case 4
                 w=hanning(n+1);
                 b=fir1(n,Wn,'stop',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');        
        end
    %ѡ�����hamming�˲���
    case 5
        %ѡ���˲�������
        switch FilterType_value
            %��ͨ�˲���
            case 1
                 w=hamming(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 2            
                 w=hamming(n+1);
                 b=fir1(n,Wn,'high',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 3
                 w=hamming(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲���
            case 4
                 w=hamming(n+1);
                 b=fir1(n,Wn,'stop',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');         
        end
    %ѡ�����kaiser�˲���
    case 6
        %ѡ���˲�������
        switch FilterType_value
            %��ͨ�˲���
            case 1
                 w=kaiser(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 2            
                 w=kaiser(n+1);
                 b=fir1(n,Wn,'high',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %��ͨ�˲���
            case 3
                 w=kaiser(n+1);
                 b=fir1(n,Wn,w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                 title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
            %�����˲���
            case 4
                 w=kaiser(n+1);
                 b=fir1(n,Wn,'stop',w);
                 [h,f]=freqz(b,1,Nn,Fs_value);
                  d=filter(b,1,y);
                 axes(handles.Magnitude);
                 if(DisplayType_value==1) plot(f,abs(h))
                 else plot(f,20*log10(abs(h)))
                 end
                 grid on;
                  title('�˲�����Ƶ��Ӧ');
                 axes(handles.Phase);
                 plot(f,angle(h));
                 grid on;
                 title('�˲�����Ƶ��Ӧ');    
        end        
end    
end



