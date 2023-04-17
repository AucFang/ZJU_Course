%%%%%%%%edited by lx on 20220203 for matrix form QM%%%%%%%
clc; clf; clear all;
h=6.626E-34;
h_bar=h/(2*pi);%%%Լ�����ʿ˳���
eV2J=1.60E-19;%%%����ת��1 eV=1.602E-19 J
J2eV=1/eV2J;%%����ת�� 1J=1/(1.6E-19) eV
m_frele=9.109E-31;%%%��������
delta_x=0.05E-9;%%%%%%%%%%%eigen state step in unit of meter����������ķָ��(step)����λΪm�������趨Ϊ50 pm�������ԭ�Ӳ����뾶
N=1002;%%%%���������ָܷ��������ܵļ��������Լ50 nm��
x=((1:1:N)-N/2)*delta_x;%%����һ�����飬���е�Ԫ�ذ������ʾ����ĸ�����ꡣ[1 2 3 4 ... 1000 1001 1002]-501=[-500 -500 -499 -498...498 499 500 501]���ٽ�����Բ���50pm

potential=['sho','square','triangle'];%%%Ԥ�������������״


kai=(-h_bar^2/(2*m_frele*delta_x^2));%%%%ǰ��ϵ��
T=kai*(-2*diag(ones(N,1))+diag(ones(N-1,1),1)+diag(ones(N-1,1),-1));%%%���������ע��diag��ʹ�÷�����

type='DoubleWell'
switch type%%%%%����ϵͳ��eVΪ������ȱȽϷ��㡣%%%���Է����ҵ�д�������޸�Сv�Ķ��壬�½�case
    case 'sho'%%%%��г����V=a*x^2
        v=eV2J*(20E18/625)*(x).^2;%%%%ע�������.^�÷�����ʾ�������ڵ�ÿһ��Ԫ����ƽ����û��.�Ļ��ᱨ��
        V=diag(v);%%%�����������ɷ���
    case 'square'
        height=0.05;%%well depth in unit of eV���Ӷ����������������ȣ����ߴӵײ�������������ݸ߶ȣ����ߵȼۣ�ֻ�ǲο���㲻ͬ������λeV
        v=[height*ones(1,floor(N/4)) 0*ones(1,N-floor(N/4)*2) height*ones(1,floor(N/4))]*eV2J;%%%%���ｫ��������˳��ƴ���������﷨Ϊ[a�ո�b�ո�c]����һ���͵����������Ӧ����һ���߶ȵ����ݣ����Ϊ250step���ڶ�����������Ϊ0�����~500step
        V=diag(v);%%ͬ��
    case 'triangle'
        v=linspace(0,2,N/2);%%%%linsapce����������һ����0�仯��2���ܹ�501��Ԫ�صĵȲ����У���Ϊ���������ݵ��Ұ�ߡ�
        v=[ones(1,N/2)*10 v]*eV2J;%%%%%������һ���߶�Ϊ10 eV�ĵ�Ч��������ݣ���������������ƴ�ӡ������У������������굽���������Ե���ܿ��Ϊ500��step����Ե���ݸ߶ȣ���500��step����Ϊ2eV
        V=diag(v);%%ͬ��
    case 'DoubleWell'
        width1=40E-9;%%%%%�ܿ�� ��λm
        N1=floor(width1/delta_x);
        width2=0.05E-9%%%�м����ݿ�� ��λnm
        N2=floor(width2/delta_x);
        height1=1;%%well depth in unit of eV���Ӷ����������������ȣ����ߴӵײ�������������ݸ߶ȣ����ߵȼۣ�ֻ�ǲο���㲻ͬ������λeV
        height2=1;%%%�м����ݸ߶ȣ���λeV
        v1=[height1*ones(1,floor((N-N1)/2)) 0*ones(1,N-2*floor((N-N1)/2)) height1*ones(1,floor((N-N1)/2))]*eV2J;%%%%���ｫ��������˳��ƴ���������﷨Ϊ[a�ո�b�ո�c]����һ���͵����������Ӧ����һ���߶ȵ����ݣ����Ϊ250step���ڶ�����������Ϊ0�����~500step
        v2=[0*ones(1,floor((N-N2)/2)) height2*ones(1,N-2*floor((N-N2)/2)) 0*ones(1,floor((N-N2)/2))]*eV2J;
        v=v1+v2;
        V=diag(v);%%ͬ��
        
    otherwise
        warning('No such potential type.')
end

H=T+V;%%���ɹ��ܶپ���
[phi,E]=eig(H);%%����ֵ�ͱ���������ע��E����һ�����󣬶Խ�Ԫ��������ֵ��Ϊ�˷��㣬�����ٲ���һ��e
e=eig(H);%%%������ֵ�����ص�e��
        
        
% x=0:0.001:0.100;
% plot(x,y),ylabel('\DeltaE=E_0-E_1(meV)'),xlabel('������ݸ߶�(eV)')
%%%%%%%%%%%%%%plot ��ͼ
for i=1:1:4%%%ѭ����ͼ��ǰ4��
    subplot(2,4,i)%%%2��5�У���i����ͼ
    
    plot(x*1E9,phi(:,i));%%%��������������Ϊλ�ã���λnm
    xlim([min(x*1E9) max(x*1E9)]);
    legend(['E_' num2str(i,1) '=' num2str(e(i)*J2eV*1000,4) 'meV']);
    xlabel('position (nm)')
    ylabel('\psi')
    subplot(2,4,4+i)
    plot(x*1E9,abs(phi(:,i)).^2);%%%������ģƽ����������Ϊλ�ã���λnm
    xlabel('position (nm)')
    ylabel('|\psi|^2')
    
    yyaxis right
    plot(x*1E9,v*J2eV,'k');%%�������ݵ���״���ο�
    hold on
    yyaxis right
    plot(x*1E9,e(i)*J2eV*ones(length(x),1),'r--');%%%����ֵ�ú�ɫ���߻�����
    
    ylim([0 0.1])
    hold off 
    xlim([min(x*1E9) max(x*1E9)]);
    ylabel('potential (eV)')
end

%%%%%%%%%%%%%T2
% aa=[0 1 2 3];
% m1=phi(:,1);
% m2=phi(:,2);
% m3=phi(:,3);
% m4=phi(:,4);
% mm0=[m1'*m1;m1'*m2;m1'*m3;m1'*m4];
% mm1=[m2'*m1;m2'*m2;m2'*m3;m2'*m4];
% mm2=[m3'*m1;m3'*m2;m3'*m3;m3'*m4];
% mm3=[m4'*m1;m4'*m2;m4'*m3;m4'*m4];
% bb=[mm0 mm1 mm2 mm3];
% bar3(bb),xlabel('n'),ylabel('m'),zlabel('\psi_m*\psi_n')


