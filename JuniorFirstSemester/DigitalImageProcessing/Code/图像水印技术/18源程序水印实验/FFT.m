M=256;%ԭͼ�񳤶�
N=32;%ˮӡͼ�񳤶�
K=8;
%��ʾԭͼ��
f1='';
[f1,p]=uigetfile('*.jpg;*.bmp;*.tif;*.gif','��ԭʼͼ���ļ�');
addpath(p);
if f1==0;
return;
end;
I=imread(f1); 
subplot(2,2,1);

imshow(I);title(' ԭʼ����ͼ��');
%��ʾˮӡͼ��
f2='';
[f2,p]=uigetfile('*.jpg;*.bmp;*.tif;*.gif','��ԭʼͼ���ļ�');
addpath(p);
if f2==0;
return;
end;
subplot(2,2,2);J=imread(f2);imshow(J);title(' ˮӡͼ��');
%ˮӡǶ��
for p=1:N
for q=1:N
x=(p-1)*K+1;y=(q-1)*K+1;
BLOCK=I(x:x+K-1,y:y+K-1);
BLOCK=fft(BLOCK);
if J(p,q )==0
a=-1;
else
a=1;
end
BLOCK=BLOCK*(1+a*0.02);
BLOCK=ifft(BLOCK);
I(x:x+K-1,y:y+K-1)=BLOCK;
end
end
%��ʾǶ��ˮӡ���ͼ��
subplot(2,2,3);imshow(I);title(' ��ˮӡ��ͼ��')
imwrite(I,'watermarked.bmp','bmp');