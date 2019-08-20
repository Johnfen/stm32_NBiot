/****************SPI ���ö˿�ģ��**********************************/
#include "math.h"
#include "db_amme.h"
#include "delay.h"
#include "usart.h"

u32 CurrentElectric;				//�ܵ���
//u32 CurrentElectricA;
//u32 CurrentElectricB;
//u32 CurrentElectricC;


 #define HFCONST   369//((2.592 * ATT_G * ATT_G*10000*INATTV*INATTI)/(EC*UI*VI)) 
 #define P_K    0.0259//2.592*pow(10,10)	/(HFCONST *EC *pow(2,23))  0.01225 //0.965595/HFCONST//0.0025745

//float data0;
//float data1;

u32 Device_ID = 0;


u8 VoltageA = 0;   //A ���ѹ��Чֵ

//u32 PA_Q = 0;  // A����
float CurrentA = 0;	 //A �������Чֵ
float  TempA = 0; //A �๦������ 
unsigned int Up_VolA = 0;
unsigned int Up_CurrentA = 0;
u32 Up_PA_Q = 0;
u32 Up_PA_Power = 0;
u32 Up_QA_Power = 0;


u8 VoltageB = 0;   //B ���ѹ��Чֵ
//u32 PB_Q = 0;  // B����
float CurrentB = 0;	 //B �������Чֵ
float TempB = 0; //B �๦������


unsigned int Up_VolB = 0;
unsigned int Up_CurrentB = 0;
u32 Up_PB_Q = 0;
u32 Up_PB_Power = 0;
u32 Up_QB_Power = 0;


u8 VoltageC = 0;   //C ���ѹ��Чֵ
//u32 PC_Q = 0;  // C����
float CurrentC = 0;	 //C �������Чֵ
float  TempC = 0; //C �๦������
unsigned int Up_VolC = 0;
unsigned int Up_CurrentC = 0;
u32 Up_PC_Q = 0;
u32 Up_PC_Power = 0;
u32 Up_QC_Power = 0;

u16 Huganqibeilv = 1;                             //����������

u32 EST_Q = 0;  // �¼����ĺ������
u32 EST_Q_buff;  // ֮ǰ���ܺ������
u8 SetGainData;
 /*****************SPI GPIO ���ܶ���******************************/
void SPI_GPIO_Config(void)
{		
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����GPIOB�͵�����ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF,ENABLE); 

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		 		/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	
		/*���ÿ⺯������ʼ��GPIOB*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);
																	   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//��������

		GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/************************* READ_Att7022*************************************************/
u32 READ_Att7022(u8 Address)
{
        u8 i, bTemp;
        u32 dwData;

        SPI_CS_HIGH();
        SPI_CLK_LOW();
        SPI_CS_LOW();    // ����SPI����
        bTemp = 0x80;
        for(i=0; i<8; i++)   //Write the Address
        {
                SPI_CLK_HIGH();
				
                if(Address & bTemp)
				{
                   SPI_DI_HIGH();
				}
                else
                     SPI_DI_LOW();
                bTemp >>= 1;
				delay_us(1) ;
                SPI_CLK_LOW();
				delay_us(1) ;
        }
        delay_us(3);

        dwData = 0x00000000;//Read 24bit
        for(i=0; i<24; i++)
        {
                dwData <<= 1;
                SPI_CLK_HIGH();
               // Delay_us(3);
                if (RDSPIData)
                {
                      dwData |= 1;
                }
                else
                        ;
		    //	Delay_us(1);
                SPI_CLK_LOW();
				delay_us(1);
        }
        SPI_CS_HIGH();	  //�ر�SPI����
        delay_us(2);
        return dwData;
}
/**************************Wr_Dat******************************/

void Write_Att7022(u8 Address, u32 dwData)
{
        u8 i, bTemp;
        u32 dwTemp;

        SPI_CS_HIGH();
        SPI_CLK_LOW();
        SPI_CS_LOW();	 //����SPI����

        Address |= 0x80;//write
        bTemp = 0x80;
        for(i=0; i<8; i++)         //Write the Address
        {
               SPI_CLK_HIGH();
                if(Address & bTemp)
				{
					SPI_DI_HIGH();
				}
                else
                     SPI_DI_LOW();
                bTemp >>= 1;
				delay_us(1) ;
                SPI_CLK_LOW();
				delay_us(1) ;
        }
        delay_us(3);

        dwTemp = 0x00800000;
        for(i=0; i<24; i++)  //Write Data
        {
                if(dwData & dwTemp)
                {
                    SPI_DI_HIGH();
                }
                else
                     SPI_DI_LOW();
                SPI_CLK_HIGH();
                delay_us(1);
                SPI_CLK_LOW();
				delay_us(1);
                dwTemp >>= 1;
        }
        
        SPI_CS_HIGH();	    //��������
        delay_us(2);
}
void EMU_init(void)
{

		Write_Att7022(0xD3,0x0000);//rest
		delay_us(2);
		Write_Att7022(0xC3,0x000000);  //���У������
		Write_Att7022(0xc9,0x00005A);  //ʹ��У��
		//Wr_Dat(0xC5,0x000000); //�ر�ͬ��
		Write_Att7022(0x01,0xB97E);  //����ģʽ�Ĵ���
		Write_Att7022(0x03,0xF884); //EMUcfg   ��������		 0xF884   �͹����й�
		Write_Att7022(0x02,0x0200);   	//��������1  ��ѹ8 

		//   Write_Att7022(0x30,0x0000); // �ر������ж�
		Write_Att7022(0x31,0x3437); //ģ��ģ��Ĵ���
		Write_Att7022(0x6D, 0xFF00);                //Vrefgain �Ĳ�������ϵ��TCcoffA ��TCcoffB ��TCcoffC(0x6D~0x6F)
		Write_Att7022(0x6E, 0x0DB8);                //Vrefgain �Ĳ�������ϵ��TCcoffA ��TCcoffB ��TCcoffC(0x6D~0x6F)
		Write_Att7022(0x6F, 0xD1DA);                //Vrefgain �Ĳ�������ϵ��TCcoffA ��TCcoffB ��TCcoffC(0x6D~0x6F)
		Write_Att7022(0x1E,HFCONST); // HFconst 3200  109	
	
		Write_Att7022(0x17,(int)(0.98*32768));	 //A���ѹ����У׼
		Write_Att7022(0x18,(int)(0.98*32768));  //B ���ѹ����У׼
		Write_Att7022(0x19,(int)(0.98*32768));  //C ���ѹ����У׼ 
	
		Write_Att7022(0x1A,1.918*32768); //A��������� У׼		
		Write_Att7022(0x1B,1.918*32768); //B��������� У׼	
		Write_Att7022(0x1C,1.908*32768); //C��������� У׼
						
		Write_Att7022(0x04,1.442*32768); //A�й������� У׼    
		Write_Att7022(0x07,1.442*32768); //A�޹������� У׼
		Write_Att7022(0x0A,1.442*32768);	//A���ڹ������� У׼

		Write_Att7022(0x05,1.442*32768); //B�й������� У׼	
		Write_Att7022(0x08,1.442*32768); //B�޹������� У׼
		Write_Att7022(0x0B,1.442*32768);	//B���ڹ������� У׼
		
		Write_Att7022(0x06,1.43*32768); //C�й������� У׼
		Write_Att7022(0x09,1.43*32768); //C�޹������� У׼1.378*(pow(2,15))
		Write_Att7022(0x0C,1.43*32768);//C���ڹ������� У׼   1.378*(pow(2,15)

		//A ���ѹ����У׼ (int)(1.9827*
	/*	Write_Att7022(0x24,0x036C); //  A���ѹ��Чֵ��У׼0x036C

		Write_Att7022(0x25,0x0301); //  B���ѹ��Чֵ��У׼0x0301

		Write_Att7022(0x26,0x02E2); //  C���ѹ��Чֵ��У׼0x02E2
*/

		Write_Att7022(0x35,0x080F); 	  //   I/O
		Write_Att7022(0xC5,0x0002);			///��ͬ��
		Write_Att7022(0xc9,0x000000); 	//Close the write protection
		Write_Att7022(0xC6,0x0000) ; 	  //�򿪼����Ĵ�������
      Device_ID = READ_Att7022(0x00);	  // Device ID 
}
void InitAmmeter(void)
{
//	data0 = ((2.592 * ATT_G * ATT_G*10000*INATTV*INATTI)/(EC*UI*VI));
//	data1 = 25920000000	/(HFCONST *EC *8388608);//0.0025745//
	
	SPI_GPIO_Config(); //�ܽų�ʼ��
	EMU_init();    // ����ʼ��
	Device_ID = READ_Att7022(0x00);	  // Device ID 
	delay_us(100);
}
void ReadAmmeterData(void)
{
	u8 ReadPowerFlag = 0;//�Ƿ��ȡ������־  //�û��Լ�������Ҫ�����ԵĶ�ȡ��������ȡ֮��оƬ�ڲ����������û��Լ����ۼ� 
	

	u32 Device_ID_buff;
	u32 TempSSAPwoer[3];
	u32 VolA = 0; // A  ���ѹ
	u32 CurrA = 0; //A �����
	u32 PA_Power = 0; //A ���й���
	u32 QA_Power = 0; //A ���޹�����
	u32 SA_Power = 0; //A �� ���ڹ���
	float PPA_Power = 0;
	float QQA_Power = 0;
	float SSA_Power = 0;
	u32 VolB = 0; // B  ���ѹ
	u32 CurrB = 0; //B �����
	u32 PB_Power = 0; //B ���й�����
	u32 QB_Power = 0; //B ���޹�����
	u32 SB_Power = 0; // B�����ڹ���
	float PPB_Power = 0;
	float QQB_Power = 0;
	float SSB_Power = 0;
	u32 VolC = 0; // C  ���ѹ
	u32 CurrC = 0; //C �����
	u32 PC_Power = 0; //C ���й�����
	u32 QC_Power = 0; //C ���޹�����
	u32 SC_Power = 0; //C�����ڹ���
	float PPC_Power = 0;
	float QQC_Power = 0;
	float SSC_Power = 0;
	/*************A ��********************************/
	
		Device_ID_buff = READ_Att7022(0x00);	  // Device ID 
	  delay_us(100);
		if (Device_ID_buff != Device_ID)
		{
				EMU_init();    // ����ʼ��
				delay_ms(500);	
         return;
		}

	  VolA =  READ_Att7022(0x0D);  //Vp2 A��ѹ ,
	  delay_us(100);
	  CurrA = READ_Att7022(0x10);   //
	  delay_us(100);
	  PA_Power = READ_Att7022(0x01);  //A���й�����
	  delay_us(100);  
	  QA_Power = READ_Att7022(0x05);  //A���޹�����
	  delay_us(100); 
	  SA_Power = READ_Att7022(0x09);  //A�����ڹ���
	  delay_us(100); 
//	  PA_Q = (READ_Att7022(0x35))/3200;  //A�����ڵ���
//	  delay_us(100);
	  VoltageA = (u8)(VolA/8192)	;   //��ѹ��Чֵ   								
	  CurrentA = ((float)CurrA/8192)/UI_K;   //A�������Чֵ
//	CurrentA = (float)(CurrA/(pow(2,13)))/176;   //A�������Чֵ
	  CurrentA =CurrentA*1000;//  ��λ0.01A
	  delay_us(100);
	  TempA = READ_Att7022(0x14); //A�๦������
	  if(TempA > 8388608)
	  {
	  	TempA =(float) 	((16777216 - TempA)/8388608);
	  }
	  else 
		{
			TempA = (float) (TempA /8388608);
		}
  
	  if(PA_Power > 8388608) //         A���й�����
	  {
	     PPA_Power = ((16777216 - (PA_Power))*P_K);//	
		  
		 // PPA_Power = ((PA_Power)- ((pow(2,24))))*P_K;
	  } 
	  else 
		{
	    PPA_Power =  (PA_Power)*P_K;		   //*0.000865
		}
	   if (QA_Power > 8388608)	 //         A���޹�����
	   {
	   		QQA_Power = ((16777216 - QA_Power )*P_K);
	   }
	   else 
		 {
	   		QQA_Power = (QA_Power*P_K);
		 }

	    if (SA_Power> 8388608)	//         A�����ڹ�����
	   {
	   		SSA_Power = ((16777216 - SA_Power )*P_K);
	   }
	   else 
         {
                SSA_Power = (SA_Power*P_K);	
         }
	   	
	   Up_VolA = (unsigned int)VoltageA;          						//�ϴ���ѹ
	   Up_CurrentA = (unsigned int)(CurrentA*Huganqibeilv + 0.5);     						//�ϵ���
	   Up_PA_Power = (unsigned int)(PPA_Power*Huganqibeilv + 0.5);		       			    //�й�����
				 
				 
		
//      UsartSend_Bit1(Up_PA_Power,0);
//      UsartSend_Bit1((Up_PA_Power>>8),0);
//      UsartSend_Bit1(Up_PA_Power>>16,0);
//     // UsartSend_Bit3((Up_PA_Power>>24),0);
	   Up_QA_Power = (unsigned int)(QQA_Power*Huganqibeilv + 0.5);			   			    //�޹����� 
         
//	   CurrentElectricA = PA_Q*Huganqibeilv;							//A�����ڵ����
	   
	   TempSSAPwoer[0] = (unsigned int)(SSA_Power*Huganqibeilv + 0.5);			    	    //���ʺ�����ڹ���
//	   UsartSend_Bit1(TempSSAPwoer[0],0);
//      UsartSend_Bit1((TempSSAPwoer[0]>>8),0);
//      UsartSend_Bit1(TempSSAPwoer[0]>>16,0);
//      UsartSend_Bit1((TempSSAPwoer[0]>>24),0);
	  /***********B��**************************/

	  VolB =  READ_Att7022(0x0E);  //Vp4 B��ѹ ,
	  delay_us(100);
	  CurrB = READ_Att7022(0x11);   //Vp5 C��ѹ  Vp1 A��ѹ	  VP3 B
	  delay_us(100);
	  PB_Power = READ_Att7022(0x02);  //B���й�����
	  delay_us(100);  
	  QB_Power = READ_Att7022(0x06);  //B���޹�����
	  delay_us(100); 
//	  PB_Q =(READ_Att7022(0x36))/3200;   //B���й�����+PB_Q
//	  delay_us(100);
	  SB_Power = READ_Att7022(0x0A);
	  VoltageB = (u8)(VolB/8192)	;   //��ѹ��Чֵ   �д�
	  CurrentB = ((float)CurrB/8192)/UI_K;  //         B�������Чֵ
	  delay_us(100);
	  TempB = READ_Att7022(0x15); //A�๦������
	  if(TempB > 8388608)
	  {
	  	TempB =(float)((16777216 - TempB)/8388608);
	  }
	  else 
		{
		  TempB = (float)(TempB /8388608) ; 
		}
					
	  if(PB_Power > 8388608)    //B���й�����
	  {
	     PPB_Power = (float)((16777216- PB_Power  )*P_K);
	  } 
	  else 
		{
	      PPB_Power = (float) (PB_Power*P_K);
		}

	   if (QB_Power> 8388608)	  //B���޹�����
	   {
	   		QQB_Power = (float)((16777216 - QB_Power)*P_K);
	   }
	   else 
		 {
	   		QQB_Power = (float)(QB_Power*P_K);
		 }

	   if (SB_Power> 8388608)	 //B�����ڹ�����
	   {
	   		SSB_Power = (float)((16777216- SB_Power )*P_K);
	   }
	   else 
		 {
	   		SSB_Power = (float)(SB_Power*P_K);	 
		 }
		   
	   
	   Up_VolB = (unsigned int)VoltageB;          						//�ϴ���ѹ
	   Up_CurrentB = (unsigned int)(CurrentB*Huganqibeilv + 0.5);     						//�ϵ���
	   Up_PB_Power = (unsigned int)(PPB_Power*Huganqibeilv + 0.5);		       			    //�й�����
//      UsartSend_Bit1(Up_PB_Power,0);
//      UsartSend_Bit1((Up_PB_Power>>8),0);
//      UsartSend_Bit1(Up_PB_Power>>16,0);
//      UsartSend_Bit1((Up_PB_Power>>24),0);
	   Up_QB_Power = (unsigned int)(QQB_Power*Huganqibeilv + 0.5);			   			    //�޹����� 
//	   CurrentElectricB = PB_Q*Huganqibeilv;							//A�����ڵ����
	   
 	   TempSSAPwoer[1] = (unsigned int)(SSB_Power*Huganqibeilv + 0.5);			    	    //���ʺ�����ڹ���
//	   UsartSend_Bit1(TempSSAPwoer[1],0);
//      UsartSend_Bit1((TempSSAPwoer[1]>>8),0);
//      UsartSend_Bit1(TempSSAPwoer[1]>>16,0);
//      UsartSend_Bit1((TempSSAPwoer[1]>>24),0);
	  /********************C��*************************/

	  VolC =  READ_Att7022(0x0F);  //Vp4 C��ѹ ,
	  delay_us(100);
	  CurrC = READ_Att7022(0x12);   //Vp5 C��ѹ  Vp1 A��ѹ	  VP3 B
	  delay_us(100);
	  PC_Power = READ_Att7022(0x03);  //C���й�����
	  delay_us(100);  
	  QC_Power = READ_Att7022(0x07);  //C���޹�����
	  delay_us(100); 
//	  PC_Q = (READ_Att7022(0x37));   //C�����ڵ���/3200+PC_Q
//	  delay_us(100);
	  SC_Power = READ_Att7022(0x0B);		//c�����ڹ���	 
	  VoltageC = (u8)(VolC/8192);   //��ѹ��Чֵ   �д�
	  CurrentC = ((float)CurrC/8192)/UI_K;  //C �������Ч
	  delay_us(100);
	  TempC = READ_Att7022(0x16); //A�๦������
	  if(TempC > 8388608)
	  {
	  	TempC =(float) 	((16777216 - TempC)/8388608);
	  }
	  else 
		{
			TempC = (float) (TempC /8388608) ; 

		}
		
	  if(PC_Power > 8388608)   //C���й�����
	  {
	     PPC_Power = (float)((16777216 - PC_Power )*P_K);
	  } 
	  else 
		{
	     PPC_Power = (float) (PC_Power*P_K);
		}

	   if (QC_Power> 8388608)  //C���޹�����
	   {
	   		QQC_Power = (float)((16777216 - QC_Power)*P_K);
	   }
	   else 
		 {
				QQC_Power = (float)(QC_Power*P_K);
		 }
	   		
	    if (SC_Power> 8388608)	    //C�����ڹ�����
	   {
	   		SSC_Power = (float)((16777216 - SC_Power)*P_K);
	   }
	   else 
		 {
				SSC_Power = (float)(SC_Power*P_K);
		 }
		 
		if (ReadPowerFlag)							//����֮ǰ�������Ա�����ʱ�ϱ�
		{
			 EST_Q = READ_Att7022(0x38);
			
			 delay_us(100);
			
			 EST_Q_buff += ((EST_Q+16)>>5)*Huganqibeilv;						//	 
			 CurrentElectric = EST_Q_buff/100;			//�ϴ�����	
		}
		 
	   Up_VolC = (unsigned int)VoltageC;          						//�ϴ���ѹ
	   Up_CurrentC = (unsigned int)(CurrentC*Huganqibeilv + 0.5);     						//�ϵ���
	   Up_PC_Power = (unsigned int)(PPC_Power*Huganqibeilv + 0.5);		       			    //�й�����
//      UsartSend_Bit1(Up_PC_Power,0);
//      UsartSend_Bit1((Up_PC_Power>>8),0);
//      UsartSend_Bit1(Up_PC_Power>>16,0);
//      UsartSend_Bit1((Up_PC_Power>>24),0);
//	   Up_QC_Power = (unsigned int)(QQC_Power*Huganqibeilv + 0.5);			   			    //�޹����� 
//	   CurrentElectricC = PC_Q*Huganqibeilv;							//B�����ڵ����
	   
	   TempSSAPwoer[2] = (unsigned int)(SSC_Power*Huganqibeilv + 0.5);			    	    //���ʺ�����ڹ���
//      UsartSend_Bit1(TempSSAPwoer[2],0);
//      UsartSend_Bit1((TempSSAPwoer[2]>>8),0);
//      UsartSend_Bit1(TempSSAPwoer[2]>>16,0);
//      UsartSend_Bit1((TempSSAPwoer[2]>>24),1);
//	   CurrentElectric = CurrentElectricC+CurrentElectricB+CurrentElectricA; //�ܵ���
	   
	   	   
}



