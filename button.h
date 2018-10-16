#ifndef BUTTON_H
#define BUTTON_H

#include "include.h"

#define BTN_NAME_MAX  32     //名字最大为32字节

/* 按键消抖时间40ms, 建议调用周期为20ms
 只有连续检测到40ms状态不变才认为有效，包括弹起和按下两种事件
*/

#define CONTINUOS_TRIGGER     0  //是否支持连续触发 	

#define BUTTON_DEBOUNCE_TIME 	2   //消抖时间      2*调用周期
#define BUTTON_CYCLE          2	 //连按触发时间  2*调用周期  
#define BUTTON_DOUBLE_TIME    15 	//双击间隔时间  20*调用周期  建议在200-600ms
#define BUTTON_LONG_TIME 	    50		/* 持续1秒(50*调用周期)，认为长按事件 */

#define TRIGGER_CB(event)   \
        if(btn->CallBack_Function[event]) \
          btn->CallBack_Function[event]((Button_t*)btn)

typedef void (*Button_CallBack)(void*);   /* 按键触发回调函数，需要用户实现 */



typedef enum {
  BUTTON_DOWM = 0,
  BUTTON_UP,
  BUTTON_DOUBLE,
  BUTTON_LONG,
  BUTTON_ALL_RIGGER,
  number_of_event, /* 触发回调的事件 */
  NONE_TRIGGER
}Button_Event;

/*
	每个按键对应1个全局的结构体变量。
	其成员变量是实现消抖和多种按键状态所必须的
*/
typedef struct button
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*Read_Button_Level)(void); /* 读取按键电平函数，需要用户实现 */
  
  char Name[BTN_NAME_MAX];
  
  uint8_t Button_Trigger_Event      :   3;	  /* 按键触发事件，单击，双击，长按等 */
	uint8_t Button_State              :   3;	  /* 按键当前状态（按下还是弹起） */
  uint8_t Button_Trigger_Level      :   1;    /* 按键触发电平 */
  uint8_t Button_Last_Level         :   1;    /* 按键当前电平 */
  
  uint8_t Button_Last_State;      /* 上一次的按键状态，用于判断双击 */
  
  Button_CallBack CallBack_Function[number_of_event];
  
	uint8_t Button_Cycle;	           /* 连续按键周期 */
  
  uint8_t Timer_Count;			/* 计时 */
	uint8_t Debounce_Time;		/* 消抖时间 */
  
	uint8_t Long_Time;		  /* 按键按下持续时间 */
  
//  struct button *Prve;
  struct button *Next;
  
}Button_t;




/* 供外部调用的函数声明 */

void Button_Create(const char *name,
                  Button_t *btn, 
                  uint8_t(*read_btn_level)(void),
                  uint8_t btn_trigger_level);
                  
void Button_Attach(Button_t *btn,Button_Event btn_event,Button_CallBack btn_callback);   
                  
void Button_Cycle_Process(Button_t *btn);   
                  
void Button_Process(void);
                  
void Button_Delete(Button_t *btn);   
  
void Search_Button(void);     
                  
uint8_t Get_Button_Event(Button_t *btn);
uint8_t Get_Button_State(Button_t *btn);

                  
#endif
