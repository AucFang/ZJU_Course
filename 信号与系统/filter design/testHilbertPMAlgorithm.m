clear all;
N=81;
f=[0.05 0.95];%����
m=[1 1];%��Ƶ����
fs=2000;%����Ƶ��
b=firpm(N,f,m,'h');%����Parks-McClellan�������Hilbert�任��

T=0.01;
t=0:T:100;
%x=0.3*t.*cos(3*t);
%x = cos(0.02*pi*t).*cos(pi*10*t);
%x=cos(0.2*t).*cos(30*t);
x=cos(2*pi*0.01*t).*cos(2*pi*10*t+0.01*t);
y =filter(b,1,x);%�˲�
A=(abs(x).^2+abs(y).^2).^0.5;
subplot(3,1,1);plot(t,A);title('��Ƶ');

phase=atan2(y,x);
subplot(3,1,2);plot(t,phase);title('��λ');

q=x+j*y;
w = zeros(1,length(q));
for n=1:length(w)-1
    w(n)=imag(2*(q(n+1)-q(n))./((q(n+1)+q(n))*T));
end
w(end)=phase(end);
subplot(3,1,3);plot(t,w);title('˲ʱƵ��');