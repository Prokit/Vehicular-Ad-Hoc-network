#include <reg51.h>                                  
typedef unsigned char uchar;                          
typedef unsigned int uint;                           
#define ModeLed P0                                  //�����������ʾ�˿�
uchar code  LedShowData[]={0x03,0x9F,0x25,0x0D,0x99,  //�����������ʾ����
                            0x49,0x41,0x1F,0x01,0x19};//0,1,2,3,4,5,6,7,8,9
static unsigned int RecvData;                         //������պ������ݱ���
static unsigned char AddData;                         //������������
static unsigned int LedFlash;                         //��������Ƶ�ʼ�������
unsigned char HeardData;                              //������յ����ݵĸ�λ�� ��
unsigned char carMode;								  //����С������ģʽ��0������ģʽ��9���˹�ģʽ
bit RunFlag=0;                                        //�������б�־λ
sbit LeftIR=P3^5;                                     //����ǰ��������̽ͷ
sbit RightIR=P3^6;                                    //����ǰ���Ҳ����̽ͷ	 ������
sbit FontIR=P3^7;                                      //������ǰ������̽ͷ	     �̵���
sbit M1A=P1^0;                                        //������1����˿�
sbit M1B=P1^1;                                        //������1����˿�
sbit M2A=P1^2;                                        //������2����˿�
sbit M2B=P1^3;                                        //������2����˿�
sbit M3A=P1^4;
sbit M3B=P1^5;
sbit M4A=P1^6;
sbit M4B=P1^7;
void Delay()                                          
{ uint DelayTime=35000;                               
  while(DelayTime--);                                 
  return;                                            
}

void Delay2()
{
  uint DelayTime2 = 15000;
  while(DelayTime2--);
  return;
}

void ControlCar(uchar CarType)                        //����С�������ӳ���
{
      M1A=0;
	    M1B=0;                                         
      M2A=0;
	    M2B=0;  
	    M3A=0;
      M3B=0;
      M4A=0;
      M4B=0;                                       
 switch(CarType)                                      
 {  
	case 0: //����ģʽǰ��                                 
    {   M1A=1;
	    M1B=0;                                         
        M2A=1;
	    M2B=0;  
	    M3A=1;
        M3B=0;
        M4A=1;
        M4B=0;                     
      break;                                        
    }
	case 1: //ǰ��                                 
    {   M1A=1;
	    M1B=0;                                         
        M2A=1;
	    M2B=0;  
	    M3A=1;
        M3B=0;
        M4A=1;
        M4B=0;                      
      break;                                        
    }
    case 2: //����                                    
    {   M1A=0;
	    M1B=1;                                         
        M2A=0;
	    M2B=1;  
	    M3A=0;
        M3B=1;
        M4A=0;
        M4B=1;                                                                                                                  
      break;                                        
    }
    case 3: //��ת                                    
    {    
	  M1A=0;
	  M1B=0;                                         
      M2A=1;
	  M2B=0;  
	  M3A=1;
      M3B=0;
      M4A=0;
      M4B=0;                                                                                             
      break;                                         
    }
    case 4: //��ת                                   
    {  

	  M1A=1;
	  M1B=0;                                         
      M2A=0;
	  M2B=0;  
	  M3A=0;
      M3B=0;
      M4A=1;
      M4B=0;                                                 
      break;                                         
    }
	case 8:  //ֹͣ
	{
	  break;
	}
    default:                                          
    {
     break;                                          
    }
 } 
}

void ComBreak() interrupt 4                         //���崮��ͨ���ӳ���
{unsigned char RecvData;                            //���崮�����ݽ��ձ���
 if(RI==1)                                          //�ж��Ƿ��������
 { RecvData=SBUF;                                   //�����յ������ݷ�����ʱ����
   RecvData=RecvData-'0';
   if(RecvData==9)                                  //�жϽ��յ��������Ƿ�С��10
   {
	carMode = 9;
    
    ControlCar(7);                                  //��������������С��״̬
   }
   else if (RecvData == 0)
	{
   	 carMode = 0;
	 ControlCar(0);
	}
	else if(RecvData ==5)
	{
	  carMode = 0;
		ControlCar(8);
	}
	else if(RecvData ==6)
	{
	   if(carMode == 0)
		ControlCar(0);
		 if(carMode !=0)
		 carMode==9;
		 
	}
   else if (RecvData!=5 && RecvData!=6 && RecvData!=7 &&RecvData!=0 &&RecvData<9 && carMode == 9)	   //�˹�ģʽ
   {
    ControlCar(RecvData);  
   }
   RI=0;                                            //��������жϱ�־λ
 }
 if(TI==1)                                          //�ж��Ƿ��Ƿ����ж�
 {
   TI=0;                                            //��������жϱ�־λ
 }
}

void main(void)                                     //���������
{
	RecvData=0;                                      //�����ձ�����ֵ��ʼ��
	TMOD=0x01;                                       //ѡ��ʱ��0Ϊ����16λ��ʱ��
	TH0=0xFF;                                        //�Զ�ʱ�����м���ֵ���г�ʼ��
	TL0=0x19;                                        
	TR0=1;                                           
	EX1=1;                                         
	IT1=1;                                         
	ET0=0;
	SCON=80;                                         //���ô���ģʽΪ8λ����
	TMOD=33;                                         //���ö�ʱ/������ģʽ
	TH1=0xFD;                                        //����ʱ��1�߰�λ��ʼ����ֵ
	TL1=0xFD;                                        //����ʱ��1�Ͱ�λ��ʼ����ֵ
	TR1=1;                                           //������ʱ��1
	ES=1;                                            //��������ͨ�Ź���
	REN=1;                                           //���������жϱ�־
	EA=1;                                            //���жϿ���
	carMode = 0;                                    
	ControlCar(8);                                   //��С������ǰ��״̬
	ModeLed=LedShowData[0];                          //�������ʾ����0
    PCON=0;                     //SMOD=0
	while(1)                                         //������ѭ��
	{
	    
			if(carMode ==0 )
			{
			  
				if(FontIR==0 )     //�ж���ǰ��,ǰ���,ǰ�Ҳ����̽ͷ״̬
				{
					ControlCar(2);                       
					Delay();                             
					Delay();                             
					ControlCar(3);                       
					Delay();                             
					Delay();                           
					ControlCar(0);                        
				}
				if(LeftIR==0 && RightIR ==1)      //��ǰ�����ϰ����ǰ��û��
				{
					ControlCar(2);                        
					Delay();                             
					Delay();                              	
					ControlCar(3);
					Delay();
					Delay();					
					ControlCar(0); 
				}				
				if(LeftIR==1 && RightIR ==0)      //��ǰ�����ϰ����ǰ��û��
				{
					ControlCar(2);                        
					Delay();                             
					Delay();                              					
					ControlCar(4);
					Delay();
					Delay();				
					ControlCar(0); 
				}
			}
		}	
}
