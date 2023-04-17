N=21;
Hd=[-1j*ones(1,10),0,1j*ones(1,10)];
k = 0:N-1;
A = exp(-1j*pi*k*(N-1)/N);

H = Hd.*A;%Ƶ�ʲ������õ�ϣ�����ر任����Ƶ���ʾ

h = ifft(H);%������Ҷ���任�õ�ʱ��
figure(1);
freqz(h1);

T=0.01;
t=0:T:100;
x=cos(2*pi*0.01*t).*cos(2*pi*10*t+0.01*t);

%%%%%%%%�����ź�����ֵ%%%%%%%%
Ad=abs(cos(2*pi*0.01*t));%���۷���
pd=atan2(sin(2*pi*10*t+0.01*t),cos(2*pi*10*t+0.01*t));%������λ
for i=1:length(x)
    fd(i)=10+0.01/2/pi;%����Ƶ��
end

%%%%%%%%������ɢϣ�����ر任������%%%%%%%%%
h=real(h);
y=conv(x,h,'same');
A=(abs(x).^2+abs(y).^2).^0.5;
p=atan2(y,x);%������λ
q=x+1j*y;%�����ź�
f = zeros(1,length(q));%����Ƶ��
temp = diff(q);
for n=1:length(f)-1
    f(n)=imag(100*temp(n)/q(n))/2/pi;
end
f(end)=p(end)/2/pi;

figure(2);
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