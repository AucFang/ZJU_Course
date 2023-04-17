#include <reg52.h>
#include <STC12C.h>
#include <intrins.h>              

#define Uchar unsigned char
#define Uint unsigned int
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define Ulong unsigned long 

/* �������� */
#define ISP_IAP_BYTE_READ		0x01        /*  �ֽڶ����ݴ洢��   */
#define ISP_IAP_BYTE_PROGRAM    0x02        /*  �ֽڱ�����ݴ洢�� */
#define ISP_IAP_SECTOR_ERASE    0x03        /*  �����������ݴ洢�� */

/* ����Flash �����ȴ�ʱ�� */
#define ENABLE_ISP        0x84
#define DATA_FLASH_START_ADDRESS           0x2800

typedef     unsigned char	INT8U;		/* 8 bit �޷�������  */
typedef     unsigned int    INT16U;     /* 16 bit �޷������� */
#define		DELAY_CONST         60000

/* ���峣�� */
#define ERROR   0
#define OK      1

// �������Ŷ��壬��ͬ�����ӱ����޸ĵĲ���
sbit RS=P3^2;		 /*LCD�Ĵ���ѡ���ź�*/
sbit RW=P3^3; 		 /*LCD��д�ź�*/
sbit Elcm=P3^4;		 /*LCDƬѡ�ź�*/  

unsigned int data AdcMem;
unsigned char data tempAdc;
unsigned char lcd_data[5];

unsigned int i,k,j;
//unsigned char tempC;
unsigned char overflag;
unsigned char changeflag;
  	
#define DataPort P2                // lcd���ݶ˿�
#define Busy    0x80

char idata word_storeroom[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; 
        																		//���ֽ� idata ��ʾ����λ���ڲ�������
void Delay400Ms(void);
void Delay5Ms(void);
void WaitForEnable( void );
void LcdWriteData( char dataW );
void LcdWriteCommand( Uchar CMD,Uchar AttribC );
void LcdReset( void );

void Display( Uchar dd );
void DispOneChar(Uchar x,Uchar y,Uchar Wdata);
//void ePutstr(Uchar x,Uchar y, Uchar code *ptr);
void SmallCur(void);
void BigCur(void);
void send(uchar i);

/*=======================================================
��ʾ��궨λ
=======================================================*/
void Disp_XY( char posx,char posy) {

uchar temp;

    temp = posx %40;// & 0x07;
    posy &= 0x01;
    if ( posy==1)  {temp |= 0x40;}
    temp |= 0x80;
    LcdWriteCommand(temp,1);
}

/*=======================================================
��ָ��λ����ʾ����һ���ַ�
=======================================================*/
void DispOneChar(Uchar x,Uchar y,Uchar Wdata) {

    Disp_XY( x, y );                // ��λ��ʾ��ַ
    LcdWriteData( Wdata );            // д�ַ�
}

/*=======================================================
��ʼ������, ���밴�ղ�Ʒ���Ͻ��ܵĳ�ʼ�����̽���
=======================================================*/
void LcdReset( void ) {

     LcdWriteCommand( 0x38, 0);            // ��ʾģʽ����(�����æ�ź�)
        Delay5Ms();
    LcdWriteCommand( 0x38, 0);            // ������
        Delay5Ms();
    LcdWriteCommand( 0x38, 0);
        Delay5Ms();

    LcdWriteCommand( 0x38, 1);            // ��ʾģʽ����(�Ժ�����æ�ź�)
    LcdWriteCommand( 0x08, 1);            // ��ʾ�ر�
    LcdWriteCommand( 0x01, 1);            // ��ʾ����
    LcdWriteCommand( 0x06, 1);            // ��ʾ����ƶ�����
    LcdWriteCommand( 0x0c, 1);            // ��ʾ�����������
    LcdWriteCommand( 0x02, 1);            // ��ʾ�����������
}

/*=======================================================
д�����ַ��ӳ���: E=1 RS=0 RW=0
=======================================================*/
void LcdWriteCommand( Uchar CMD,Uchar AttribC ) {

    if (AttribC) WaitForEnable();                // ���æ�ź�?

    RS = 0;    RW = 0; _nop_();    //һ��nopԼ����1/32us

    DataPort = CMD; _nop_();
    // �Ϳ������ӳ���

    Elcm = 1;_nop_();_nop_();Elcm = 0;            // �������������ź�
}

/*=======================================================
��ǰλ��д�ַ��ӳ���: E =1 RS=1 RW=0
=======================================================*/
void LcdWriteData( char dataW ) {

    WaitForEnable();
        // ���æ�ź�

    RS = 1; RW = 0; _nop_();

    DataPort = dataW; _nop_();

    Elcm = 1; _nop_(); _nop_(); Elcm = 0;        // �������������ź�

}

/*=======================================================
������д����֮ǰ������LCD������״̬:    CS=1 RS=0 RW=1
DB7:    0  LCD����������; 1  LCD������æ
========================================================*/
void WaitForEnable( void ) {
    DataPort = 0xff;
    RS =0;
	RW = 1; 
	_nop_();    
	Elcm = 1; 
	_nop_(); 
	_nop_();
    while( DataPort & Busy ); //��DB7
    Elcm = 0;
}


// ����ʱ
void Delay5Ms(void)
{
    Uint i = 5552;
    while(i--);
}

//����ʱ
void Delay400Ms(void)
{
    Uchar i = 5;
    Uint j;
    while(i--)
    {
        j=7269;
        while(j--);
    };
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

uint Adc(unsigned char n)
{
	unsigned char i;	
	uint result=0;

	ADC_CONTR = ADC_CONTR|0x80;
	delay(10);
	i = 0x01<<n;
    P1M0 = P1M0|i;
   	P1M1 = P1M1|i;
    delay(10); 
    ADC_CONTR = 0xE0|n;
    delay(10);
	ADC_DATA = 0;
	ADC_LOW2 = 0;
	ADC_CONTR = ADC_CONTR|0x08;
	delay(10);
    ADC_CONTR = ADC_CONTR&0xE7;
	result = ADC_DATA;
	result<<=2;
	result = result&0x03FC;
	result = result|(ADC_LOW2 & 0x03);
	return(result);
}


void cominit(void)
{
	SCON=0X50;
	TMOD=0X20;
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	//ES=1;
	//EA=1;
}

uint Adc6(void)
{
	uint i,j,k;
	uint result=0;
	uint Tmin=0x3ff,Tmax=0;
	ulong T_tatol=0;
	uint temp1;
    uint comp;

	k=0;
    j=0;
    overflag=1;

    comp=3;// AdcMem; //ֵ����

    for(i=0;i<1000;i++)
	{
        temp1=Adc(2);// AdcMem; //ֵ����
		if(temp1<comp)
		{
			result=Adc(1);// AdcMem; //ֵ����
            temp1=Adc(2);// AdcMem; //ֵ����
			if(temp1>comp)
				continue;
		}
		else
        { 
            j++;
            if(j>900)
                return 0;
			continue;
        }
		k++;
		if(Tmin>result)
			Tmin=result;
		if(Tmax<result)
			Tmax=result;
		T_tatol=T_tatol+result;
	
		if(k==21)  
		{
			T_tatol=T_tatol-Tmin;
			T_tatol=T_tatol-Tmax;
			T_tatol>>=2;
            overflag=0;
			return((uint)T_tatol);
		}
	}
	return 0;
}


//void disptempC(uchar data1)
//{
//		tempAdc = data1%0x000A;
//		lcd_data[0] = word_storeroom[tempAdc];
//		data1 = data1/0x000A;
//		tempAdc = data1%0x000A;
//		lcd_data[1] = word_storeroom[tempAdc];
//		data1 = data1/0x000A;
//		tempAdc = data1%0x000A;
//		lcd_data[2] = word_storeroom[tempAdc];
//
//		//��ʾBCD��
//	    DispOneChar(4,0,lcd_data[2]);
//		DispOneChar(5,0,'.');
//		DispOneChar(6,0,lcd_data[1]);
//		DispOneChar(7,0,lcd_data[0]);
//}
void send(uchar i)
{
	TI=0;
	SBUF=i;
	while(TI==0);
	TI=0;

}



void dispint(uint data1)
{
 uint tempAdc;
		tempAdc = data1%0x000A;
		lcd_data[0] = word_storeroom[tempAdc];
		data1 = data1/0x000A;
		tempAdc = data1%0x000A;
		lcd_data[1] = word_storeroom[tempAdc];
		data1 = data1/0x000A;
		tempAdc = data1%0x000A;
		lcd_data[2] = word_storeroom[tempAdc];

		//��ʾBCD��
	    DispOneChar(4,1,lcd_data[2]);
		DispOneChar(5,1,'.');
		DispOneChar(6,1,lcd_data[1]);
		DispOneChar(7,1,lcd_data[0]);
}


void dispchar(uint data1)
{
        uchar tempAdc;
		tempAdc = data1%0x000A;
		lcd_data[0] = word_storeroom[tempAdc];
		data1 = data1/0x000A;
		tempAdc = data1%0x000A;
		lcd_data[1] = word_storeroom[tempAdc];
		data1 = data1/0x000A;
		tempAdc = data1%0x000A;
		lcd_data[2] = word_storeroom[tempAdc];
		data1 = data1/0x000A;
		tempAdc = data1%0x000A;
		lcd_data[3] = word_storeroom[tempAdc];

		//��ʾBCD��
	    DispOneChar(4,1,lcd_data[3]);
		DispOneChar(5,1,lcd_data[2]);
		DispOneChar(6,1,lcd_data[1]);
		DispOneChar(7,1,lcd_data[0]);
}

/* �ر� ISP,IAP ���� */
void ISP_IAP_disable(void)
{
	ISP_CONTR	=	0;	/* 0111,1111 */
	ISP_CMD	=	0;
	ISP_TRIG	=	0;
//	EA			=   1;                	/* ���ж� */
}
/* �ֽڶ� */
uchar byte_read(uint byte_addr)
{
	uchar tmp=0;

	ISP_CONTR		=	ENABLE_ISP;
	ISP_CMD		=	ISP_IAP_BYTE_READ;        /* 1111,1000 */
	ISP_ADDRH	=	(uchar)(byte_addr >> 8);
	ISP_ADDRL	=	(uchar)(byte_addr & 0x00ff);


	ISP_TRIG	=	0x46;
	ISP_TRIG	=	0xb9;
	_nop_();
	tmp=ISP_DATA;
	ISP_IAP_disable();
	return (tmp);
}
/* �������� */
uchar sector_erase(uint sector_addr)
{
	ISP_CONTR   =  ENABLE_ISP;
	ISP_CMD	=	ISP_IAP_SECTOR_ERASE;

	ISP_ADDRH	=	(uchar)(sector_addr >> 8);
	ISP_ADDRL	=	(uchar)(sector_addr & 0x00ff);

	ISP_TRIG	=	0x46;        /* ����ISP_IAP���� */
	ISP_TRIG	=	0xb9;        /* ����ISP_IAP���� */
	 _nop_();

	ISP_IAP_disable();
	return OK;
}

/* �ֽڱ�� */
uchar byte_program(uint byte_addr, uchar original_data)
{
	ISP_CONTR   =  ENABLE_ISP;
	ISP_CMD		=	ISP_IAP_BYTE_PROGRAM;
	ISP_ADDRH	=	(uchar)(byte_addr >> 8);
	ISP_ADDRL	=	(uchar)(byte_addr & 0x00ff);
	ISP_DATA	=	original_data;

	ISP_TRIG	=	0x46;        /* ����ISP_IAP���� */
	ISP_TRIG	=	0xb9;        /* ����ISP_IAP���� */
	_nop_();

	ISP_IAP_disable();
	return	OK;
}


main()
{ 
    uint tmp=0;
    uint i;
	uint tmp_alarm;
    char code *number1="www.zju.edu.cn  ";
	char code *number6="edu.cn  ";
    char code *number2="         Ver2.71";
    char code *number3="Voltage: ";
 

    LcdReset();
	Delay400Ms();
	cominit();	

// Display "www.zju.edu.cn"
		for(i=0;i<16;i++)
	    {
	        DispOneChar(i,0,*(number1+i));
	        DispOneChar(i,1,*(number6+i));
	    }
		Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
		Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
		Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
		Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();

		//ƽ��
		for(i=0;i<16;i++)
	    {
	        DispOneChar(i,0,*(number1+i));
	        DispOneChar(i,1,*(number2+i));
	    }
	    for(i=0;i<8;i++)
	    {
	        LcdWriteCommand( 0x18, 1);            //ƽ��
	        Delay400Ms();
	        Delay400Ms();
	        Delay400Ms();
	    }

	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    Delay400Ms();
	    LcdWriteCommand( 0x02, 1);            // ��ʾ�����������

// clear Display

	for(i=0;i<8;i++)
    {
        DispOneChar(i,0,' ');
        DispOneChar(i,1,' ');
    }

	for(;;) 
   {
	 	for(i=0;i<8;i++)
	    {
	        DispOneChar(i,0,*(number3+i));
	    }

	   tmp_alarm=Adc(7);
	   dispchar(tmp_alarm);

   }

} //end_main
