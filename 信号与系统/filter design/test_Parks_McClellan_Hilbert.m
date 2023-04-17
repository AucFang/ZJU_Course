clear all;
N=80;%����
f=[0.05 0.95];%���ô���
m=[1 1];%�����˲����ķ�Ƶ����
b=firpm(N,f,m,'h');%Hilbert�任��
dt=0.01;
t=0:dt:100;
x=cos(2*pi*0.01*t).*cos(2*pi*10*t+0.01*t);%�����ź�
Ad=abs(cos(2*pi*0.01*t));

y=filter(b,1,x);%ϣ�����ر任���õ��Ľ����ź�Ϊx+1j*y;
A=(abs(x).^2+abs(y).^2).^0.5;%�������

p=atan2(y,x);%������λ
pd=atan2(sin(2*pi*10*t+0.01*t),cos(2*pi*10*t+0.01*t));%������λ

q=x+1j*y;%�����ź�
f = zeros(1,length(q));%����Ƶ��
temp = diff(q);
for n=1:length(f)-1
    f(n)=imag(100*temp(n)/q(n))/2/pi;
end
f(end)=p(end)/2/pi;
for n=1:length(f)
    fd(n)=10+0.01/2/pi;%����Ƶ��
end
figure(1)
subplot(4,1,1);
plot(t,y);xlabel('ʱ��');title('��������ź�');axis([48,50,-1.3,1.3]);hold on;
plot(t,x, 'r--' , 'linewidth' ,2);axis([48,50,-1.3,1.3]);
legend('����','����');

subplot(4,1,2);
plot(t,A);title('�������');axis([0,100,0,1.3]);hold on;
plot(t,Ad, 'r--' , 'linewidth' ,2);axis([0,100,0,1.3]);
legend('����','����');

subplot(4,1,3);
plot(t,p);title('��λ');axis([0,2,-3.15,3.15]);hold on;
plot(t,pd, 'r--' , 'linewidth' ,2);axis([0,2,-3.15,3.15]);
legend('����','����');

subplot(4,1,4);
plot(t,f);title('˲ʱƵ��');axis([0,100,-10,30]);hold on;
plot(t,fd, 'r--' , 'linewidth' ,2);axis([0,100,-10,30]);
legend('����','����');

%%%%%%����Ϊ����MATLAB�Դ�Hilbert�任��˲ʱƵ����ǰ������Ƶ��˲������ĶԱ�%%%%%
fs=100;
z = hilbert(x);
instfrq = fs/(2*pi)*diff(unwrap(angle(z)));


figure(2)
plot(t,f);title('˲ʱƵ�ʶԱ�');axis([0,100,-10,30]);hold on;
plot(t(2:end),instfrq, 'r--' , 'linewidth' ,2);axis([0,100,-10,30]);
legend('����','����');
