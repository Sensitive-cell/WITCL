J=imread('watermarked.bmp','bmp');
subplot(1,2,1)
title('Դͼ��');
imshow(J);
imwrite(J,'watermarked1.jpg','jpeg','quality',45);
subplot(1,2,2);
title('ѹ����ͼ��');
imshow(J);