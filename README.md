# ButtonDrive
自己写的一个按键驱动，支持单双击、连按、长按；采用回调处理按键事件（自定义消抖时间），使用只需3步，创建按键，按键事件与回调处理函数链接映射，周期检查按键

## 前言
前几天写了个按键驱动，参考了[MulitButton](https://github.com/0x1abin/MultiButton)的数据结构的用法，逻辑实现并不一样。
在这里感谢所有的开源开发者，让我从中学到了很多，同时网络也是一个好平台，也希望所有的开发者能形成良性循环，从网络中学知识，回馈到网络中去。感谢[MulitButton](https://github.com/0x1abin/MultiButton)的作者[0x1abin](https://github.com/0x1abin)，感谢两位rtt的大佬：[大法师](https://github.com/uestczyh222)、[流光](https://github.com/liu2guang)。
## Button_drive简介
Button_drive是一个小巧的按键驱动，支持单击、双击、长按、连续触发等（后续可以在按键控制块中添加触发事件），理论上可无限量扩展Button，Button_drive采用按键触发事件回调方式处理业务逻辑，支持在RTOS中使用，我目前仅在RT-Thread上测试过。

## Button_drive使用效果
1. **单击与长按**
![单击与长按](https://img-blog.csdn.net/2018101621325972)
2. **双击**
![双击](https://img-blog.csdn.net/20181016213335944)

3. **连击**
![连击](https://img-blog.csdn.net/20181016213538180)

## 使用方法
1. 创建按键句柄
```
Button_t Button1;
Button_t Button2; 
```
2. 创建按键，初始化按键信息，包括按键名字、按键电平检测函数接口、按键触发电平。
```
  Button_Create("Button1",				//按键名字
                &Button1, 				//按键句柄
                Read_Button1_Level, 	//按键电平检测函数接口
                BTN_TRIGGER);		   	//触发电平
                
                ......
```
3. 按键触发事件与事件回调函数链接映射，当按键事件被触发的时候，自动跳转回调函数中处理业务逻辑。
```
  Button_Attach(&Button1,BUTTON_DOWM,Btn2_Dowm_CallBack);		//按键单击
  Button_Attach(&Button1,BUTTON_DOUBLE,Btn2_Double_CallBack);	//双击
  Button_Attach(&Button1,BUTTON_LONG,Btn2_Long_CallBack);		//长按
				
				.......
```
4. 周期调用回调按键处理函数即可，建议调用周期20-50ms。
```
Button_Process();     //需要周期调用按键处理函数
```

---
需要用户实现的 **2** 个函数：

- 按键电平检测接口：
```
uint8_t Read_Button1_Level(void)
{
  return GPIO_ReadInputDataBit(BTN1_GPIO_PORT,BTN1_GPIO_PIN);
}

uint8_t Read_Button2_Level(void)
{
  return GPIO_ReadInputDataBit(BTN2_GPIO_PORT,BTN2_GPIO_PIN);
}

// 这是我在stm32上简单测试的伪代码，以实际源码为准

```

- 按键逻辑处理
```
void Btn1_Dowm_CallBack(void *btn)
{
  PRINT_INFO("Button1 单击!");
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
```

##  特点
Button_drive开放源码，按键控制块采用数据结构方式，按键事件采用枚举类型，确保不会重复，也便于添加用户需要逻辑，采用宏定义方式定义消抖时间、连按触发时间、双击时间间隔、长按时间等，便于修改。
同时所有被创建的按键采用单链表方式连击，用户只管创建，无需理会按键处理，只需调用`Button_Process()`即可，在函数中会自动遍历所有被创建的按键。
支持按键删除操作，用户无需在代码中删除对应的按键创建于映射链接代码，也无需删除关于按键的任何回调事件处理函数，只需调用`Button_Delete()`函数即可，这样子，就不会处理关于被删除按键的任何状态。当然目前按键内存不会释放，如果使用os的话，建议释放按键内存。
##### 按键控制块
```
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
  
  struct button *Next;
  
}Button_t;

```
#####  触发事件
```
typedef enum {
  BUTTON_DOWM = 0,
  BUTTON_UP,
  BUTTON_DOUBLE,
  BUTTON_LONG,
  BUTTON_ALL_RIGGER,
  number_of_event, /* 触发回调的事件 */
  NONE_TRIGGER
}Button_Event;

```

## 后续
[流光](https://github.com/liu2guang)大佬的要求，让我玩一玩RTT的[rtkpgs](https://github.com/liu2guang/buildpkg)，打算用Button_drive练一练手吧。
