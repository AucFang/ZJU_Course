for i=-10:10
    n=i+11;
    if(mod(n,2) == 0) h(n) = 2/pi/i;
    else h(n) = 0;
    end
end
i=-10:10;
w=-pi:0.01:pi
H=1j*(w>=-pi&w<=0)-1j*(w>=0&w<=pi);
subplot(2,1,1);stem(i,h,'linewidth',1.3);title('��ɢϣ�����ر任����h[n]');axis([-10,10,-0.8,0.8]);
subplot(2,1,2);plot(w,H/j,'linewidth',1.3);title('��ɢϣ�����ر任����H(e^{j\omega})�ķ�ֵ');axis([-pi,pi,-1.2,1.2]);