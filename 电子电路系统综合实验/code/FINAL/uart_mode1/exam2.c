#include<reg52.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int

#define LCD1602_DB P2            //���ݶ˿�  D0~D7

sfr AUXR      = 0x8e;
sfr ADC_CONTR = 0xC5;
sfr ADC_DATA  = 0xC6;
sfr ADC_LOW2  = 0xBE;
sfr P1M0      = 0x91;
sfr P1M1      = 0x92;

sbit RED = P1^0;
sbit GREEN = P1^1;
sbit BLUE = P1^2;
sbit BEEP = P1^7;
sbit D3 = P1^4;
sbit LCD1602_RS = P3^2;         //1602������/ָ��ѡ�������
sbit LCD1602_RW = P3^3;
sbit LCD1602_E  = P3^4;         //1602��ʹ�ܿ�����

uint pwm_out_flag = 0;
uint pwm_renew_flag = 0;
uint clock_flag = 0;
uchar order = 0;
uchar R_OUT = 0, G_OUT = 0, B_OUT = 0; //����ռ�ձȼ�������
uint MAX=200,MIN=1;           //�����ɫ��ʼ��������Χ������0-200
uint mod = 9;
uint MODEL = 0;
uint info;
uint light_data;
uchar index = 0;
uint clock;

void ConfigUART(unsigned int baud)
{
 	SCON = 0X50;   //���ڷ�ʽ1
	TMOD &= 0X0F; //��ʱ��T1 ��������ʽ 2 
	TMOD |= 0X20;
	TH1 = 256 - (11059200/12/32)/baud;
	TL1 = TH1;

	ET1 = 0;   //  ʹ��T1�������ʷ���������Ҫ����T1�Ķ�ʱ�ж�
	ES = 1;	   // �����жϴ�
	TR1 = 1;   //  T1��ʼ��ʱ
}

void timer0init()//1ms�ж�
{
	TMOD &= 0XF0;  //��ʱ��T0��������ʽ16λ��ʱ
	TMOD |= 0X01;
	TH0 = 0XFC;
	TL0 = 0X67;
	TR0 = 1;
	ET0 = 1;
}

void delay(unsigned int t) //��ʱ
{       
  	unsigned int i, j; 
  	for(i=0;i<t;i++)
	{ 
  		for(j=0;j<10;j++)
		{;} 
    } 
} 

void InitADC()
{
        ADC_DATA = 0;
        ADC_LOW2 = 0;
        ADC_CONTR = 0x60 | 3;   //������check�ڣ�ת���ٶ����
        delay(10);
}

void InterruptUART() interrupt 4
{
	if (RI)
	{
		RI = 0;
		order = SBUF;
	}
	if (TI)
	{
	 	//SBUF = order;
		TI = 0;
	}
}
void timer0routine() interrupt 1
{
	static uint time_count;
	TH0 = 0XFC;
	TL0 = 0X67;
	time_count = (time_count + 1) % 1000;
	if (time_count % 2)
	{
		pwm_out_flag = 1;
	}
	if(time_count % 10 == 9)
	{
		pwm_renew_flag = 1;
	}
	if(time_count == 999)
	{
		clock_flag = 1;//1s
	}
}


uint Adc(unsigned char n)//�ɼ��ڼ�ͨ��������
{
	unsigned char i;	
	uint result=0;

	ADC_CONTR = ADC_CONTR|0x80;  //10000000 ����ת����Դ
	delay(10);
	i = 0x01<<n;
        P1M0 = P1M0|i;
   	P1M1 = P1M1|i;   //������ͨ����©
        delay(10); 
        ADC_CONTR = 0xE0|n;   //��adc��Դ,����ת������ ����P1.nģ��ͨ��  1110_0000|n
        delay(10);   	//��ʱһ��ʱ�䣬ʹ�����ѹ�ﵽ�ȶ�
	ADC_DATA = 0;   
	ADC_LOW2 = 0;	//��A/Dת������Ĵ���
	ADC_CONTR = ADC_CONTR|0x08;  //ADC_START=1,��ʼת��   0000_1000
	delay(10);
        ADC_CONTR = ADC_CONTR&0xE7;  //��ADC_FLAG�������   1110_0111  
	result = ADC_DATA;
	result<<=2;
	result = result&0x03FC; 	//0000_0011_1111_1100
	result = result|(ADC_LOW2 & 0x03);
	return(result);				//10λ�Ľ��
}

//���ռ�ձ�
void POUT() 
{
    unsigned char cnt;  
    for(cnt=0;cnt<MAX;cnt++)
    {
        if(cnt<R_OUT)
                RED=1;        //��ɫռ�ձȴ��ڼ���ֵʱ�����
        else
                RED=0;        //����͹رպ�ɫ
               
        if(cnt<G_OUT)               
                GREEN=1;                //��ɫռ�ձȴ��ڼ���ֵʱ�̵���
        else
                GREEN=0;                //����͹ر���ɫ
               
        if(cnt<B_OUT)
                BLUE=1;                //��ɫռ�ձȴ��ڼ���ֵʱ������
        else
                BLUE=0;                //����͹ر���ɫ
    }
}

void ConfigPWM(unsigned char t)  //PWMռ�ձȴ�����
{
        static unsigned char dir=0;    //���������䰵����
        static unsigned int time=0;    //�м����
        switch(index)
        {
		case 0: 
                        if(dir==0)      //��ɫ�ɰ�����
                        {
                                time+=t;
                                R_OUT=time;
                                G_OUT=0;
                                B_OUT=0;
                                if(time>=MAX)
                                {
                                        dir=1;
                                }
                        }
                        else         //��ɫ�����䰵
                        {
                                time-=t;
                                R_OUT=time;
                                G_OUT=0;
                                B_OUT=0;
                                if(time<=MIN)
                                {
                                        dir=0;
                                        time=0;
                                        if(mod == 1)
                                        {
                                                index = 0;
                                        }
                                        else if(mod == 9)
                                        {
                                                index = 1;
                                        }
                                        else
                                        {
                                                index = 7;
                                        }
                                }
                        }
                        break;
                                       
                case 1:            
                        if(dir==0)                //��ɫ�ɰ�����
                        {
                                time+=t;
                                R_OUT=time;
								G_OUT=time/4;
                                B_OUT=0;
                                if(time>=MAX)
                                {
                                        dir=1;
                                }
                        }
                        else                        //��ɫ�����䰵
                        {
                                time-=t;
                                R_OUT=time;
								G_OUT=time/4;
                                B_OUT=0;
                                if(time<=MIN)
                                {
                                        dir=0;
                                        time=0;
                                        if(mod == 2)
                                        {
                                                index = 1;
                                        }
                                        else if(mod == 9)
                                        {
												index = 2;
                                        }
                                        else
                                        {
                                                index = 7;
                                        }
                                }
                        }
                        break;
                                       
                case 2:            
                        if(dir==0)                //��ɫ�ɰ�����
                        {
                                time+=t;
                                R_OUT=time;
                                G_OUT=time/2;
                                B_OUT=0;
                                if(time>=MAX)
                                {
                                        dir=1;
                                }
                        }
                        else                        //��ɫ�����䰵
                        {
                                time-=t;
                                R_OUT=time;
                                G_OUT=time/2;
                                B_OUT=0;
                                if(time<=MIN)
                                {
                                        dir=0;
                                        time=0;
                                        if(mod == 3)
                                        {
                                                index = 2;
                                        }
                                        else if(mod == 9)
                                        {
                                                index = 3;
                                        }
                                        else
                                        {
                                                index = 7;
                                        }
                                }
                        }
                        break;
                                       
                case 3:            
                        if(dir==0)                //��ɫ�ɰ�����
                        {
                                time+=t;
                                R_OUT=0;
                                G_OUT=time;
                                B_OUT=0;
                                if(time>=MAX)
                                {
                                        dir=1;
                                }
                        }
                        else                        //��ɫ�����䰵
                        {
                                time-=t;
                                R_OUT=0;
                                G_OUT=time;
                                B_OUT=0;
                                if(time<=MIN)
                                {
                                        dir=0;
                                        time=0;
                                        if(mod == 4)
                                        {
                                                index = 3;
                                        }
                                        else if(mod == 9)
                                        {
                                                index = 4;
                                        }
                                        else
                                        {
                                                index = 7;
                                        }
                                }
                        }
                        break;
                                       
                case 4:           
                        if(dir==0)                //��ɫ�ɰ�����
                        {
                                time+=t;
                                R_OUT=0;
                                G_OUT=time;
                                B_OUT=time/2;
                                if(time>=MAX)
                                {
                                        dir=1;
                                }
                        }
                        else                        //��ɫ�����䰵
                        {
                                time-=t;
                                R_OUT=0;
                                G_OUT=time;
                                B_OUT=time/2;
                                if(time<=MIN)
                                {
                                        dir=0;
                                        time=0;
                                        if(mod == 5)
                                        {
                                                index = 4;
                                        }
                                        else if(mod == 9)
                                        {
                                                index = 5;
                                        }
                                        else
                                        {
                                                index = 7;
                                        }
                                }
                        }
                        break;
                                       
                case 5:            
                        if(dir==0)                //��ɫ�ɰ�����
                        {
                                time+=t;
                                R_OUT=0;
                                G_OUT=0;
                                B_OUT=time;
                                if(time>=MAX)
                                {
                                        dir=1;
                                }
                        }
                        else                        //��ɫ�����䰵
                        {
                                time-=t;
                                R_OUT=0;
                                G_OUT=0;
                                B_OUT=time;
                                if(time<=MIN)
                                {
                                        dir=0;
                                        time=0;
                                        if(mod == 6)
                                        {
                                                index = 5;
                                        }
                                        else if(mod == 9)
                                        {
                                                index = 6;
                                        }
                                        else
                                        {
                                                index = 7;
                                        }
                                }
                        }
                        break;
                                       
                case 6:            
                        if(dir==0)                //��ɫ�ɰ�����
                        {
                                time+=t;
                                R_OUT=time/3;
                                G_OUT=0;
                                B_OUT=time;
                                if(time>=MAX)
                                {
                                        dir=1;
                                }
                        }
                        else                        //��ɫ�����䰵
                        {
                                time-=t;
                                R_OUT=time/3;
                                G_OUT=0;
                                B_OUT=time;
                                if(time<=MIN)
                                {
                                        dir=0;
                                        time=0;
                                        if(mod == 7)
                                        {
                                                index = 6;
                                        }
                                        else
                                        {
                                                index = 7;
                                        }
                                }
                        }
                        break;
                                       
                case 7:            
                        if(dir==0)                //��ɫ�ɰ�����
                        {
                                time+=t;
                                G_OUT=time;
                                B_OUT=time;
                                R_OUT=time;
                                if(time>=MAX)
                                {
                                        dir=1;
                                }
                        }
                        else                        //��ɫ�����䰵
                        {
                                time-=t;
                                G_OUT=time;
                                B_OUT=time;
                                R_OUT=time;
                                if(time<=MIN)
                                {
                                        dir=0;
                                        time=0;
                                        if(mod == 8)
                                        {
                                                index = 7;
                                        }
                                        else if(mod == 9)
                                        {
												index = 0;
                                        }
                                        else
                                        {
                                                index = 7;
                                        }
                                }
                        }
                        break;
			           
        }
}

void Delay_ms(unsigned int nms)        //@11.0592MHz   nmsΪҪ��ʱ��ms��  
{
    unsigned char i, j;    
    while(nms--)
    {
        i = 15;
        j = 90;
        do
        {
            while (--j);
        } while (--i);
    }
}

void LcdWaitReady()    /* �ȴ�Һ��׼���� */
{
    unsigned char sta;
    
    LCD1602_DB = 0xFF;
    LCD1602_RS = 0;
    LCD1602_RW = 1;       //��״̬
    do {
        LCD1602_E = 1;
        sta = LCD1602_DB; //��ȡ״̬��
        LCD1602_E = 0;
    } while (sta & 0x80); //bit7����1��ʾҺ����æ���ظ����ֱ�������0Ϊֹ
}

void LCD1602_write_cmd(unsigned char cmd)    //д�����
{
    LcdWaitReady();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E  = 1;
    _nop_();_nop_();
    LCD1602_E  = 0;
}

void LCD1602_Init()    //1602��ʼ������ 
{
    LCD1602_write_cmd(0x01);    //��ʾ����
    LCD1602_write_cmd(0x38);    //��ʾģʽ����
    LCD1602_write_cmd(0x0C);    //��ʾ�����������
    LCD1602_write_cmd(0x06);    //��ʾ����ƶ�λ��
}

void LCD1602_write_data(unsigned char date)    //д���ݺ��� 
{
    LcdWaitReady();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = date;
    LCD1602_E  = 1;
    _nop_();_nop_();
    LCD1602_E  = 0;
}

void LCD1602_Clear()    //1602��������
{
    LCD1602_write_cmd(0x01);
}

void LCD1602_SetCursor(unsigned char x,unsigned char y)//����1602���λ�ú���
{
    unsigned char addr;
    if(y==0)     //���������Ļ���������ʾRAM�ĵ�ַ
        addr=0x00+x;
    else
        addr=0x40+x;
    LCD1602_write_cmd(addr | 0x80);//����RAM��ַ
}

void LCD1602_ShowNum(char x, char y, unsigned int num)  //ָ��λ����ʾ���ֺ���
{
    unsigned int i,j,k,l,n;
    i=num/10000;
    j=(num-10000*i)/1000;
    k=(num-10000*i-1000*j)/100;
    l=(num-10000*i-1000*j-100*k)/10;
    n=num%10;
    LCD1602_SetCursor(x,y);  //������ʼ��ַ
    if(i!=0)LCD1602_write_data(i+0x30);
    if((i!=0)||(j!=0))LCD1602_write_data(j+0x30);
    if((i!=0)||(j!=0)||(k!=0))LCD1602_write_data(k+0x30);
    if((i!=0)||(j!=0)||(k!=0)||(l!=0))LCD1602_write_data(l+0x30);
    LCD1602_write_data(n+0x30);
}

void LCD1602_ShowChar(unsigned char xpos,unsigned char ypos,char xsz)    //ָ��λ����ʾ�ַ�����
{
    ypos%=2;
    if(ypos==0)
    {
        LCD1602_write_cmd(0x80+xpos);
    }
    else
    {
        LCD1602_write_cmd(0x80+0x40+xpos);
    }
    LCD1602_write_data(xsz);
}

void LCD1602_ShowStr(unsigned char xpos,unsigned char ypos,char *p) //ָ��λ����ʾ�ַ�������
{
    if(ypos>1)return;
    while(*p!='\0')
    {
        LCD1602_ShowChar(xpos++,ypos,*p++);
        if(*p=='\n')
        {
            xpos=0;
            ypos++;
            p++;
        }
    }
}

void check(uint data1)
{
    if(data1 < 15)
    {
        EA = 0;//�ر�ȫ���ж�
        //�������ͱ���
        while(1)
        {
            LCD1602_ShowStr(15,1,"!");  //LCD��ʾ��̾��
            D3 = 1;                     //����˸����������Ъ�Ա���
            BEEP = 1;
            delay(40000);
            D3 = 0;
            BEEP = 0;
            delay(40000);
        }
    }
    else EA = 1;
}

/******************���ϼ��****************/
void LED_check()
{
		//�ж�����ͨ��
        RED = 0;
        GREEN = 0;
        BLUE = 1;
        Delay_ms(500);
        check(Adc(3));

        //�жϺ��ͨ��
        RED = 1;
        GREEN = 0;
        BLUE = 0;
        Delay_ms(500);
        check(Adc(3));
        
        //�ж��̵�ͨ��
        RED = 0;
        GREEN = 1;
        BLUE = 0;
        Delay_ms(500);
        check(Adc(3));
}

void main()
{
	static uint clock_count; 
	static uint MODEL_OLD;
	static uint info_old;
	static uint clock_enable;
	static uint change_enable;
	static uint light_count;

	P1M0 = 0x48;
	P1M1 = 0x4F;
	EA = 1;
	ConfigUART(9600);
	timer0init();
    LCD1602_Init();//LCD��ʼ��
	InitADC();//ADC�˿�ģʽ��ʼ��

	RED = 0;
	GREEN = 0;
	BLUE = 0;
	D3 = 0;
	light_count = 200;

	LCD1602_Clear();

        LED_check();//���ϼ��

	do
	{	
		LCD1602_ShowStr(0,1,"order:");
		MODEL = (order & 0xF0) >> 4;
		LCD1602_ShowNum(6,1,MODEL);	
		info = order & 0x0F;
		LCD1602_ShowNum(7,1,info);	

		light_data = Adc(6);

		if(MODEL != MODEL_OLD || info != info_old)
		{
			LCD1602_Clear();
			MODEL_OLD = MODEL;
			info_old = info;
			if(MODEL == 4 || MODEL == 5)
			{
				clock_enable = 1;
			}
			if(MODEL == 0)
			{
				change_enable = 1;
			}
		}

		switch(MODEL)
		{
			case 0:
				if(change_enable)
				{
					change_enable = 0;
					mod = info;
					index = (mod == 9)? 1: (mod - 1);
				}
				LCD1602_ShowStr(0,0,"mod:");
				LCD1602_ShowNum(5,0,mod);
				_nop_();_nop_();
				if(pwm_out_flag)
				{
					pwm_out_flag = 0;
					POUT();
				}
				if(pwm_renew_flag)
				{
					pwm_renew_flag = 0;
					ConfigPWM(1);
				}
				break;
			case 1:
				R_OUT = info*20;
				LCD1602_ShowStr(0,0,"R:");
				LCD1602_ShowNum(2,0,R_OUT/2);
				LCD1602_ShowStr(5,0,"G:");
				LCD1602_ShowNum(7,0,G_OUT/2);
				LCD1602_ShowStr(10,0,"B:");
				LCD1602_ShowNum(12,0,B_OUT/2);
				_nop_();_nop_();
				if(pwm_out_flag)
				{
					pwm_out_flag = 0;
					POUT();
				}
				break;
			case 2:
				G_OUT = info*20;
				LCD1602_ShowStr(0,0,"R:");
				LCD1602_ShowNum(2,0,R_OUT/2);
				LCD1602_ShowStr(5,0,"G:");
				LCD1602_ShowNum(7,0,G_OUT/2);
				LCD1602_ShowStr(10,0,"B:");
				LCD1602_ShowNum(12,0,B_OUT/2);
				_nop_();_nop_();
				if(pwm_out_flag)
				{
					pwm_out_flag = 0;
					POUT();
				}
				break;
			case 3:
				B_OUT = info*20;
				LCD1602_ShowStr(0,0,"R:");
				LCD1602_ShowNum(2,0,R_OUT/2);
				LCD1602_ShowStr(5,0,"G:");
				LCD1602_ShowNum(7,0,G_OUT/2);
				LCD1602_ShowStr(10,0,"B:");
				LCD1602_ShowNum(12,0,B_OUT/2);
				_nop_();_nop_();
				if(pwm_out_flag)
				{
					pwm_out_flag = 0;
					POUT();
				}
				break;
			case 4:
				if(clock_enable)//��һ����MODEL = 4
				{
					clock_enable = 0;
					clock = info;
					clock_count = 0;
					//��
					RED = 0;
					GREEN = 0;
					BLUE = 0;
				}

				LCD1602_ShowStr(0,0,"clock:");
				_nop_();_nop_();

				if(clock_flag)
				{
					clock_flag = 0;
					clock_count++;
				}
				if(clock_count >= clock * 60)
				{
					LCD1602_ShowNum(6,0,0);
					_nop_();_nop_();
					if(pwm_out_flag)
					{
						pwm_out_flag = 0;
						POUT();
					}
					if(pwm_renew_flag)
					{
						pwm_renew_flag = 0;
						ConfigPWM(1);
					}
				}
				else{
					if(clock*60 - clock_count < 10) LCD1602_ShowStr(7,0," ");
					else if(clock*60 - clock_count < 100) LCD1602_ShowStr(8,0," ");
					LCD1602_ShowNum(6,0,clock*60 - clock_count);
					_nop_();_nop_();
				}
				break;
			case 5:
				if(clock_enable)//��һ����MODEL = 5
				{
					clock_enable = 0;
					clock = info;
					clock_count = 0;
				}

				LCD1602_ShowStr(0,0,"clock:");
				_nop_();_nop_();
				
				if(clock_flag)
				{
					clock_flag = 0;
					clock_count++;
				}
				if(clock_count <= clock * 60)
				{
					if(clock*60 - clock_count < 10) LCD1602_ShowStr(7,0," ");
					else if(clock*60 - clock_count < 100) LCD1602_ShowStr(8,0," ");
					LCD1602_ShowNum(6,0,clock*60 - clock_count);
					_nop_();_nop_();
					if(pwm_out_flag)
					{
						pwm_out_flag = 0;
						POUT();
					}
					if(pwm_renew_flag)
					{
						pwm_renew_flag = 0;
						ConfigPWM(1);
					}
				}
				else
				{
					LCD1602_ShowNum(6,0,0);
					_nop_();_nop_();
					RED = 0;
					GREEN = 0;
					BLUE = 0;
				}
				break;
			case 6:
				light_count++;
				if(light_count >= 200)
				{
					light_count = 0;
					LCD1602_ShowStr(0,0,"light:");
					LCD1602_ShowNum(6,0,light_data);
					_nop_();_nop_();
				}
				
				if(light_data > 400)
				{
					RED = 0;
					GREEN = 0;
					BLUE = 0;
				}
				else
				{
					if(pwm_out_flag)
					{
						pwm_out_flag = 0;
						POUT();
					}
					if(pwm_renew_flag)
					{
						pwm_renew_flag = 0;
						ConfigPWM(1);
					}
				}
		}
		
	}while(1);
}