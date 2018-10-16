
#include "button.h"

/**
  ******************************************************************
													   变量声明
  ******************************************************************
  */ 

Button_t Button1;
Button_t Button2; 


/**
  ******************************************************************
														函数声明
  ******************************************************************
  */ 
  
static void BSP_Init(void);


uint8_t Read_KEY1_Level(void)
{
  return GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN);
}

uint8_t Read_KEY2_Level(void)
{
  return GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN);
}



void Btn1_Dowm_CallBack(void *btn)
{
  PRINT_INFO("Button1 连击!");
}

void Btn1_Double_CallBack(void *btn)
{
  PRINT_INFO("Button1 双击!");
}

void Btn1_Long_CallBack(void *btn)
{
  PRINT_INFO("Button1 长按!");
  
  Button_Delete(&Button2);
  PRINT_INFO("删除Button1");
  Search_Button();
}

void Btn2_Dowm_CallBack(void *btn)
{
  PRINT_INFO("Button2 连击!");
}

void Btn2_Double_CallBack(void *btn)
{
  PRINT_INFO("Button2 双击!");
}

void Btn2_Long_CallBack(void *btn)
{
  PRINT_INFO("Button2 长按!");
}


/**
  ******************************************************************
  * @brief   主函数
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */ 
int main(void)
{
  uint32_t i = 100000;
  
	BSP_Init();
  
//  PRINT_DEBUG("当前电平：%d",Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN));
  
  Button_Create("Button1",
                &Button1, 
                Read_KEY1_Level, 
                KEY_ON);
  Button_Attach(&Button1,BUTTON_DOWM,Btn1_Dowm_CallBack);
//  Button_Attach(&Button1,BUTTON_DOUBLE,Btn1_Double_CallBack);
  Button_Attach(&Button1,BUTTON_LONG,Btn1_Long_CallBack);
  
  Button_Create("Button2",
                &Button2, 
                Read_KEY2_Level, 
                KEY_ON);
  Button_Attach(&Button2,BUTTON_DOWM,Btn2_Dowm_CallBack);
//  Button_Attach(&Button2,BUTTON_DOUBLE,Btn2_Double_CallBack);
  Button_Attach(&Button2,BUTTON_LONG,Btn2_Long_CallBack);
 
	while(1)                            
	{

    Button_Process();     //需要周期调用按键处理函数

    Delay_ms(20);
    
    
	}
}

/**
  ******************************************************************
  * @brief   BSP_Init，用于所有板级初始化
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */ 
static void BSP_Init(void)
{
	/* LED 初始化 */
	LED_GPIO_Config();
	
	/* 串口初始化 */
	USART_Config();
  
	/* 按键初始化 */
  Key_GPIO_Config();
  
	
	/* 打印信息 */
	PRINT_INFO("welcome to learn jiejie stm32 library!\n");
	
}


/********************************END OF FILE***************************************/
