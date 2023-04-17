clear all;
N = 15;
Np = 1000;

window = ones(1,N);
window = [window,zeros(1,Np-length(window))];
y=fftshift(fft(window));
subplot(5,2,1);plot(window);title('���δ�ʱ��');xlim([0 20]);
subplot(5,2,2);plot(abs(y));title('���δ�Ƶ��');

window = hann(N)';
window = [window,zeros(1,Np-length(window))];
y=fftshift(fft(window));
subplot(5,2,3);plot(window);title('������ʱ��');xlim([0 20]);
subplot(5,2,4);plot(abs(y));title('������Ƶ��');

window = hamming(N)';
window = [window,zeros(1,Np-length(window))];
y=fftshift(fft(window));
subplot(5,2,5);plot(window);title('������ʱ��');xlim([0 20]);
subplot(5,2,6);plot(abs(y));title('������Ƶ��');

window = kaiser(N)';
window = [window,zeros(1,Np-length(window))];
y=fftshift(fft(window));
subplot(5,2,7);plot(window);title('��ɪ����ʱ��');xlim([0 20]);
subplot(5,2,8);plot(abs(y));title('��ɪ����Ƶ��');

window = blackman(N)';
window = [window,zeros(1,Np-length(window))];
y=fftshift(fft(window));
subplot(5,2,9);plot(window);title('����������ʱ��');xlim([0 20]);
subplot(5,2,10);plot(abs(y));title('����������Ƶ��');
