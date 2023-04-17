clear all;
N=72;%����
f=[0.05 0.95];%����
m=[1 1];%��Ƶ����
fs=2000;%����Ƶ��
b=firpm(N,f,m,'h');%����Parks-McClellan�������Hilbert�任��
[H,w]=freqz(b,1,512,fs);%����Hilbert�任����������Ӧ
subplot(2,1,1);plot(w,20*log10(abs(H)));axis([0,1000,-30,10]);xlabel('Ƶ��/Hz');ylabel('��ֵ/dB');title('��ɢϣ�����ر任����Ƶ����');
subplot(2,1,2);plot(w,angle(H)/pi*180);xlabel('Ƶ��/Hz');ylabel('���');title('��ɢϣ�����ر任����Ƶ����');
