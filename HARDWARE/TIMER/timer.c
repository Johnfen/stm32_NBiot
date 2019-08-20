#include "timer.h"
#include "led.h"
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr:�Զ���װֵ��
//psc:ʱ��Ԥ��Ƶ��
//����ʹ�õ�Ϊ��ʱ��3
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //ʱ��TIM3ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period=arr; //�����Զ����ؼĴ������ڵֵֵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler=psc; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1;//����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;//TIM���ϼ���
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);   //2��ʼ��TIM3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);      //3��������ж�
	
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);              //��ʼ��NVIC�Ĵ���
	
	TIM_Cmd(TIM3,ENABLE);                    //5ʹ��TIM3
}

//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}


//TIM1 CH1 PWM������� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{		 					 
	
 	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʹ��TIMx����
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOA����ʱ��ʹ��
	
 
  //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM1_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //���ù������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��GPIO
 
	TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����Ԥ��Ƶֵ ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH1 PWM2ģʽ	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //OC1 �͵�ƽ��Ч 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����ָ���Ĳ�����ʼ������TIMx

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1 Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��,�߼���ʱ�����뿪����� 
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx
	   										  
} 
/**
  * @brief  TIM2_CAP_Init
  * @param  arr: This parameter must be a number between 0x0000 and 0xFFFF.
  * @param  prescaler: This parameter can be a number between 0x0000 and 0xFFFF;
  * @retval None
  */
void TIM2_CAP_Init() //u16 arr, u16 prescaler
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    // ����TIM2��GPIOA��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // PA0��ʼ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;                   // ����Ϊ�������� 
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    // TIM2��ʱ����ʼ��
//    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
//    TIM_TimeBaseInitStruct.TIM_Period = arr;
//    TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    // TIM2_CH1���벶���ʼ��
    TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;   
    TIM_ICInitStruct.TIM_ICFilter = 0x00;                        // ���˲�
    TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;     // �����ز���
    TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // �����Ƶ������Ƶ
    TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽IC1
    TIM_ICInit(TIM2, &TIM_ICInitStruct);
    // �жϷ����ʼ��
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
     
    TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1, ENABLE);   // �����жϺ�CC1IE�����ж�
    TIM_Cmd(TIM2, ENABLE);
}
 
 










