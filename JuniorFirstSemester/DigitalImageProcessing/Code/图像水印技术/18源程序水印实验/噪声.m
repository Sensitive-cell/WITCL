%����Ϊ0.002 �ĸ�˹������ǿ��Ϊ0.02 �Ľ�������
I=imread('watermarked.bmp');
subplot(1,3,1);
imshow(I);
title('��ˮӡͼ��');
J=imnoise(I,'gaussian',0,0.002);
imwrite(J,'watermarked1.bmp');
subplot(1,3,2);
imshow(J);
title('��˹����');
k=imnoise(I,'salt & pepper',0.02);
imwrite(k,'watermarked2.bmp');
subplot(1,3,3);
imshow(k);
title('��������');
