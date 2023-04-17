
#include <reg52.h>
#include <STC12C.h>
#include <intrins.h>              

sbit LCD_RS=P3^2;    /*LCD�Ĵ���ѡ���ź�*/
sbit LCD_R_W=P3^3;   /*LCD��д�ź�*/
sbit LCD_E=P3^4;     /*LCDƬѡ�ź�*/

//�������ͱ�ʶ�ĺ궨�壬��Ҷ�ϲ����ô��
#define Uchar unsigned char
#define Uint unsigned int
#define uchar unsigned char
#define uint unsigned int

/* �������� */
#define ISP_IAP_BYTE_READ				0x01        /*  �ֽڶ����ݴ洢��   */
#define ISP_IAP_BYTE_PROGRAM          0x02        /*  �ֽڱ�����ݴ洢�� */
#define ISP_IAP_SECTOR_ERASE     0x03        /*  �����������ݴ洢�� */

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
sbit RS=P3^2;
sbit RW=P3^3; 
sbit Elcm=P3^4;  

sbit SelA=P1^1;
sbit SelB=P3^7;

sbit PA=P1^5;
sbit PB=P1^6;
sbit PC=P1^7;



unsigned char lcd_data[5];
unsigned int i,k,j;
unsigned char disp_bool;
unsigned char tempC;
unsigned char overflag;


#define DataPort P2                // ���ݶ˿�
#define Busy    0x80
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
#define _Nop() _nop_()

char idata word_storeroom[]=
{'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'}; 

void Delay400Ms(void);
void Delay5Ms(void);
void WaitForEnable( void );
void LcdWriteData( char dataW );
void LcdWriteCommand( Uchar CMD,Uchar AttribC );
void LcdReset( void );
void Display( Uchar dd );
void DispOneChar(Uchar x,Uchar y,Uchar Wdata);
void ePutstr(Uchar x,Uchar y, Uchar code *ptr);


/*=======================================================
��ʾ�ַ���
=======================================================*/
void ePutstr(Uchar x,Uchar y, Uchar code *ptr) {
Uchar i,l=0;
    while (ptr[l] >31) {l++;};
    for (i=0;i<16;i++) { DispOneChar(x++,y,ptr[i]);
        if ( x == 16 ){
            x = 0; y ^= 1;
        }
    }
}

/*=======================================================
��ʾһ�������ַ����������λ������ʾ�ƶ��ִ�
=======================================================*/
void Display( Uchar dd ) {

Uchar i;

    for (i=0;i<16;i++) {
        DispOneChar(i,1,dd++);
        dd &= 0x7f;
        if (dd<32) dd=32;
    }
}

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

    RS = 0;    RW = 0; _nop_();

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

    RS =0; RW = 1; _nop_();    Elcm = 1; _nop_(); _nop_();

    while( DataPort & Busy );

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

main()
{ 
    uint tmp=0;
    uint i;
	uint tmp_alarm;
    unsigned long limit;
    char code *number1="www.zju.edu.cn  ";
	char code *number6="edu.cn  ";
    char code *number2="         Ver2.71";
    char code *number3="Voltage: ";
    char code *number4="Thinknes";
    char code *number5="s:      ";


    LcdReset();
	Delay400Ms();


	for(;;) {	

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

 }
} //end_main
