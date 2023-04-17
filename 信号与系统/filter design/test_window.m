clear all;
T=0.01;
t=0:T:100;
x=cos(2*pi*0.01*t).*cos(2*pi*10*t+0.01*t);
n=-1000:1000;
N=length(n);
h=zeros(1,N);
for i=1:N
    if(mod(i,2) == 0) 
        h(i) = 2/pi/n(i);%ϣ�����ر任��
    end
end
%%%%%%%%�����ź�����ֵ%%%%%%%%
Ad=abs(cos(2*pi*0.01*t));%���۷���
pd=atan2(sin(2*pi*10*t+0.01*t),cos(2*pi*10*t+0.01*t));%������λ
for i=1:length(x)
    fd(i)=10+0.01/2/pi;%����Ƶ��
end

%%%%%%%%���δ�%%%%%%%%%%
[y1,A1,p1,f1]=cal_window(1,N,h,x);

%%%%%%%%%������%%%%%%%%%
[y2,A2,p2,f2]=cal_window(2,N,h,x);

%%%%%%%%%������%%%%%%%%%
[y3,A3,p3,f3]=cal_window(3,N,h,x);

%%%%%%%%%��ɪ����%%%%%%%%%
[y4,A4,p4,f4]=cal_window(4,N,h,x);

%%%%%%%%%����������%%%%%%%%%
[y5,A5,p5,f5]=cal_window(5,N,h,x);

figure(1)
subplot(4,5,1);
plot(t,y1);xlabel('ʱ��');title('���δ���������ź�');axis([50,50.5,-1.3,1.3]);hold on;
plot(t,x, 'r--' , 'linewidth' ,2);axis([50,50.5,-1.3,1.3]);
legend('����','����');
subplot(4,5,2);
plot(t,y2);xlabel('ʱ��');title('��������������ź�');axis([50,50.5,-1.3,1.3]);hold on;
plot(t,x, 'r--' , 'linewidth' ,2);axis([50,50.5,-1.3,1.3]);
legend('����','����');
subplot(4,5,3);
plot(t,y3);xlabel('ʱ��');title('��������������ź�');axis([50,50.5,-1.3,1.3]);hold on;
plot(t,x, 'r--' , 'linewidth' ,2);axis([50,50.5,-1.3,1.3]);
legend('����','����');
subplot(4,5,4);
plot(t,y4);xlabel('ʱ��');title('��ɪ������������ź�');axis([50,50.5,-1.3,1.3]);hold on;
plot(t,x, 'r--' , 'linewidth' ,2);axis([50,50.5,-1.3,1.3]);
legend('����','����');
subplot(4,5,5);
plot(t,y5);xlabel('ʱ��');title('������������������ź�');axis([50,50.5,-1.3,1.3]);hold on;
plot(t,x, 'r--' , 'linewidth' ,2);axis([50,50.5,-1.3,1.3]);
legend('����','����');

subplot(4,5,6);
plot(t,A1);title('�������');axis([0,100,0,1.3]);hold on;
plot(t,Ad, 'r--' , 'linewidth' ,2);axis([0,100,0,1.3]);
legend('����','����');
subplot(4,5,7);
plot(t,A2);title('�������');axis([0,100,0,1.3]);hold on;
plot(t,Ad, 'r--' , 'linewidth' ,2);axis([0,100,0,1.3]);
legend('����','����');
subplot(4,5,8);
plot(t,A3);title('�������');axis([0,100,0,1.3]);hold on;
plot(t,Ad, 'r--' , 'linewidth' ,2);axis([0,100,0,1.3]);
legend('����','����');
subplot(4,5,9);
plot(t,A4);title('�������');axis([0,100,0,1.3]);hold on;
plot(t,Ad, 'r--' , 'linewidth' ,2);axis([0,100,0,1.3]);
legend('����','����');
subplot(4,5,10);
plot(t,A5);title('�������');axis([0,100,0,1.3]);hold on;
plot(t,Ad, 'r--' , 'linewidth' ,2);axis([0,100,0,1.3]);
legend('����','����');

subplot(4,5,11);
plot(t,p1);title('��λ');axis([1,1.5,-3.15,3.15]);hold on;
plot(t,pd, 'r--' , 'linewidth' ,2);axis([1,1.5,-3.15,3.15]);
legend('����','����');
subplot(4,5,12);
plot(t,p2);title('��λ');axis([1,1.5,-3.15,3.15]);hold on;
plot(t,pd, 'r--' , 'linewidth' ,2);axis([1,1.5,-3.15,3.15]);
legend('����','����');
subplot(4,5,13);
plot(t,p3);title('��λ');axis([1,1.5,-3.15,3.15]);hold on;
plot(t,pd, 'r--' , 'linewidth' ,2);axis([1,1.5,-3.15,3.15]);
legend('����','����');
subplot(4,5,14);
plot(t,p4);title('��λ');axis([1,1.5,-3.15,3.15]);hold on;
plot(t,pd, 'r--' , 'linewidth' ,2);axis([1,1.5,-3.15,3.15]);
legend('����','����');
subplot(4,5,15);
plot(t,p5);title('��λ');axis([1,1.5,-3.15,3.15]);hold on;
plot(t,pd, 'r--' , 'linewidth' ,2);axis([1,1.5,-3.15,3.15]);
legend('����','����');

subplot(4,5,16);
plot(t,f1);title('˲ʱƵ��');axis([0,100,-10,30]);hold on;
plot(t,fd, 'r--' , 'linewidth' ,2);axis([0,100,-10,30]);
legend('����','����');
subplot(4,5,17);
plot(t,f2);title('˲ʱƵ��');axis([0,100,-10,30]);hold on;
plot(t,fd, 'r--' , 'linewidth' ,2);axis([0,100,-10,30]);
legend('����','����');
subplot(4,5,18);
plot(t,f3);title('˲ʱƵ��');axis([0,100,-10,30]);hold on;
plot(t,fd, 'r--' , 'linewidth' ,2);axis([0,100,-10,30]);
legend('����','����');
subplot(4,5,19);
plot(t,f4);title('˲ʱƵ��');axis([0,100,-10,30]);hold on;
plot(t,fd, 'r--' , 'linewidth' ,2);axis([0,100,-10,30]);
legend('����','����');
subplot(4,5,20);
plot(t,f5);title('˲ʱƵ��');axis([0,100,-10,30]);hold on;
plot(t,fd, 'r--' , 'linewidth' ,2);axis([0,100,-10,30]);
legend('����','����');