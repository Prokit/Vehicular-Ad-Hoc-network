#include <reg51.h>                                  
typedef unsigned char uchar;                          
typedef unsigned int uint;                           
#define ModeLed P0                                  //�����������ʾ�˿�
uchar  idata trdata[]={'5'};
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
sbit SIGNAL=P0^1;
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
    { M1A=1;
	  M1B=0;                                         
      M2A=1;
	  M2B=0;  
	  M3A=1;
      M3B=0;
      M4A=1;
      M4B=0;                                 
      ModeLed=LedShowData[0];   //Һ����ʾ                   
      break;                                        
    }
	case 1: //ǰ��                                 
    { M1A=1;
	  M1B=0;                                         
      M2A=1;
	  M2B=0;  
	  M3A=1;
      M3B=0;
      M4A=1;
      M4B=0;                                    
      ModeLed=LedShowData[1];                      
      break;                                        
    }
    case 2: //����                                    
    { M1A=0;
	  M1B=1;                                         
      M2A=0;
	  M2B=1;  
	  M3A=0;
      M3B=1;
      M4A=0;
      M4B=1;       
	  //LedBit=0xfe;                                                              
      ModeLed=LedShowData[2];                                                           
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
      ModeLed=LedShowData[3];                                                           
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
      ModeLed=LedShowData[4];                                                     
      break;                                         
    }
 	
	case 8:  //ֹͣ
	{
	  ModeLed=LedShowData[8];
	  break;
	}
    default:                                          
    {
     break;                                          
    }
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
     SCON= 0x40;                                      //���ڷ�ʽ1
     PCON=0;                                         //SMOD=0
     TMOD= 0x20;                                     //��ʱ��1��ʱ��ʽ2
	while(1)                                         //������ѭ��
	{
	    int i,j;
		if(SIGNAL==0)
		 {			
              SBUF=trdata[0];			
              while(TI==0);
              TI=0;
              for (j=0;j<25000;j++);   
      		
         }
	    if(SIGNAL==1)
		 {     
              SBUF='A';
              while(TI==0);
              TI=0;
      
         } 
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
