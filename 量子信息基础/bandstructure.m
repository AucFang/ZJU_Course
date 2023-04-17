%%%%%%%%edited by lx on 20220316 for matrix form band structure calculation%%%%%%%
clc; clf; clear all;
h=6.626E-34;
h_bar=h/(2*pi);%%%Լ�����ʿ˳���
eV2J=1.60E-19;%%%����ת��1 eV=1.602E-19 J
J2eV=1/eV2J;%%����ת�� 1J=1/(1.6E-19) eV
m_frele=9.109E-31;%%%��������
delta_x=0.5E-12;%%%%%%%%%%%eigen state step in unit of meter����������ķָ��(step)����λΪm�������趨Ϊ0.5 pm�����0.01����ԭ�Ӳ����뾶
N=300;%%%%��������Ϊһ���������ڣ���С��150pm(��ѧ������)��
p=N*delta_x;
kp_list=linspace(-pi,pi,100);
x=((1:1:N)-N/2)*delta_x;%%����һ�����飬���е�Ԫ�ذ������ʾ����ĸ�����ꡣ[1 2 3 4 ... 100]-50=[ -49 -48...48 49 50]���ٽ�����Բ���5pm
E_K=zeros(5,length(kp_list));%%%%ǰ5���ܴ���E-K��ϵ�ݴ�
kai=(-h_bar^2/(2*m_frele*delta_x^2));%%%%ǰ��ϵ��
T=kai*(-2*diag(ones(N,1))+diag(ones(N-1,1),1)+diag(ones(N-1,1),-1));%%%���������ע��diag��ʹ�÷�����
type='K_P';
switch type%%%%%����ϵͳ��eVΪ������ȱȽϷ��㡣%%%���Է����ҵ�д�������޸�Сv�Ķ��壬�½�case
    case 'K_P'%%%%���ڷ�����ģ��
        height=50;%well depth in unit of eV���Ӷ����������������ȣ����ߴӵײ�������������ݸ߶ȣ����ߵȼۣ�ֻ�ǲο���㲻ͬ������λeV
        aN=150;
        bN=N-aN;
        a=aN*delta_x;
        b=bN*delta_x;
        vb=eV2J*(height)*ones(1,bN);
        va=eV2J*(0)*ones(1,aN);
        v=[vb va];
        V=diag(v);%%%�����������ɷ���
    case 'Coul'%%%%�����һά��������
        epsilon=8.85E-12;%%��ս�糣��
        q=1.6021E-19;%%���ӵ��
        v=-(q*q)./(4*pi*epsilon*abs(x-0.15E-12).^1);%%%��Ҫ����ɵ�����λ�������ڸ���ϣ����������.����������ϵ���ܲ���1�η�
        V=diag(v);%%%�����������ɷ���
    case 'delta'%%%%PKģ��b->0, U->inf����
        height=1000;%well depth in unit of eV���Ӷ����������������ȣ����ߴӵײ�������������ݸ߶ȣ����ߵȼۣ�ֻ�ǲο���㲻ͬ������λeV
        aN=290;
        bN=N-aN;
        a=aN*delta_x;
        b=bN*delta_x;
        vb=eV2J*(height)*ones(1,bN);
        va=eV2J*(0)*ones(1,aN);
        v=[vb va];
        V=diag(v);%%%�����������ɷ���
    otherwise
        warning('No such potential type.')
end
H=T+V;%%���ɹ��ܶپ���

for ii=1:1:length(kp_list)
    H(1,N)=kai*exp(1i*kp_list(ii));%%%%����ն�����������Ա߽�����1
    H(N,1)=kai*exp(-1i*kp_list(ii));%%%%%%%%����ն�����������Ա߽�����2
    [phi,E]=eig(H);%%����ֵ�ͱ���������ע��E����һ�����󣬶Խ�Ԫ��������ֵ��Ϊ�˷��㣬�����ٲ���һ��e
    e=eig(H);%%%������ֵ�����ص�e�С�
    E_K(:,ii)=J2eV*real(e(1:5));%%%%��ǰ5������ֵ���浽E_K�ĵ�ii����
end
figure(1)
%plot(kp_list,E_K(1,:))

%plot(kp_list(2:100),diff(E_K(1,:))./diff(kp_list))
%hold on
%plot(kp_list(3:100),diff(diff(E_K(1,:))./diff(kp_list))./diff(kp_list(2:100)))
plot(kp_list(3:100),h_bar^2*1./(diff(diff(E_K(1,:))./diff(kp_list))./diff(kp_list(2:100))))
xlabel('k*p');
ylabel('m(k*p)');


%%%%%%%%%%%����kp=pi������Ԩ���߽磩�����Ĳ�����ģƽ��
% figure(2)
% subplot(2,2,1)
% plot(x*1E9,abs(phi(:,1)).^2);
% yyaxis right
% plot(x*1E9,v*J2eV,'r');%%�������ݵ���״���ο�
% xlim([min(x*1E9) max(x*1E9)]);
% legend(['E_1=' num2str(e(1)*J2eV,3) 'eV']);
% 
% subplot(2,2,2)
% plot(x*1E9,abs(phi(:,2)).^2);
% yyaxis right
% plot(x*1E9,v*J2eV,'r');%%�������ݵ���״���ο�
% xlim([min(x*1E9) max(x*1E9)]);
% legend(['E_2=' num2str(e(2)*J2eV,3) 'eV']);
% 
% subplot(2,2,3)
% plot(x*1E9,abs(phi(:,3)).^2);
% yyaxis right
% plot(x*1E9,v*J2eV,'r');%%�������ݵ���״���ο�
% xlim([min(x*1E9) max(x*1E9)]);
% legend(['E_3=' num2str(e(3)*J2eV,3) 'eV']);
% 
% subplot(2,2,4)
% plot(x*1E9,abs(phi(:,4)).^2);
% xlabel('X position (nm)')
% ylabel('|psi|^2')
% yyaxis right
% plot(x*1E9,v*J2eV,'r');%%�������ݵ���״���ο�
% xlim([min(x*1E9) max(x*1E9)]);
% ylabel('potential energy (eV)')
% legend(['E_4=' num2str(e(4)*J2eV,3) 'eV']);
% 
% 
% 
% 
% 
