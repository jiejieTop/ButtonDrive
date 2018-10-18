# ButtonDrive
自己写的一个按键驱动，支持单双击、连按、长按；采用回调处理按键事件（自定义消抖时间），使用只需3步，创建按键，按键事件与回调处理函数链接映射，周期检查按键。
源码地址：[https://github.com/jiejieTop/ButtonDrive](https://github.com/jiejieTop/ButtonDrive)

## 前言
前几天写了个按键驱动，参考了[MulitButton](https://github.com/0x1abin/MultiButton)的数据结构的用法，逻辑实现并不一样。
在这里感谢所有的开源开发者，让我从中学到了很多，同时网络也是一个好平台，也希望所有的开发者能形成良性循环，从网络中学知识，回馈到网络中去。感谢[MulitButton](https://github.com/0x1abin/MultiButton)的作者[0x1abin](https://github.com/0x1abin)，感谢两位rtt的大佬：[大法师](https://github.com/uestczyh222)、[流光](https://github.com/liu2guang)。
## Button_drive简介
Button_drive是一个小巧的按键驱动，支持单击、双击、长按、连续触发等（后续可以在按键控制块中添加触发事件），理论上可无限量扩展Button，Button_drive采用按键触发事件回调方式处理业务逻辑，支持在RTOS中使用，我目前仅在[RT-Thread](https://github.com/RT-Thread/rt-thread)上测试过。
写按键驱动的目的是想要将用户按键逻辑与按键处理事件分离，用户无需处理复杂麻烦的逻辑事件。

## Button_drive使用效果
1. **单击与长按**

![单击与长按](https://github.com/jiejieTop/ButtonDrive/blob/master/png/1.png?raw=true)

2. **双击**

![双击](https://github.com/jiejieTop/ButtonDrive/blob/master/png/2.png?raw=true)

3. **连按**

![连按](https://github.com/jiejieTop/ButtonDrive/blob/master/png/3.png?raw=true)

4. **连按释放**

![连按释放](https://github.com/jiejieTop/ButtonDrive/blob/master/png/4.png?raw=true)

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

## 关于[rtkpgs](https://github.com/liu2guang/buildpkg)

## 简介 ([English](/readme.en.md)) 

buildpkg 是用于生成 RT-Thread package 的快速构建工具。

一个优秀的 package 应该是这样的：
 1. 代码优雅, 规范化。
 2. examples 例程，提供通俗易懂的使用例程。
 3. SConscript 文件，用于和 RT-Thread 环境一起进行编译。
 4. README.md 文档，向用户提供必要的功能说明。
 5. docs 文件夹, 放置除了 README 之外的其他细节文档。
 6. license 许可文件，版权说明。

为了方便快速的生成 RT-Thread package 规范化模板 以及 减轻开源仓库迁移 RT-Thread 的前期准备工作的负担，基于此目的的 buildpkg 应运而生，为开发 Rt-Thread 的 package 的开发者提供辅助开发工具。

| 序号  | 支持功能 | 描述 |
| :--- | :--- | :--- |
| 1 | 构建 package 模板 | 创建指定名称 package , 自动添加 readme /版本号/ github ci脚本/demo/开源协议文件 |
| 2 | 迁移开源仓库 | 从指定 git 仓库构建 package , 自动添加readme/版本号/ github ci脚本/demo/开源协议文件, 但是迁移的仓库需要用户自己按照实际情况修改 |
| 3 | 更新 package | 生成package后可以再次更新之前设定的版本号，开源协议或者scons脚本等 | 

## 使用说明

### 1. 构建package
> buildpkg.exe make pkgdemo

### 2. 迁移开源仓库
> buildpkg.exe make cstring https://github.com/liu2guang/cstring.git

### 3. 更新package
> buildpkg.exe update pkgname

### 4. 可选配置 
| 长参数  | 短参数 | 描述 |
| :--- | :--- | :--- |
| --version=v1.0.0 | -v v1.0.0 | 设置 package 的版本 |
| --license=MIT | -l MIT | 设置 package 所遵循的版权协议 |
| --submodule | -s |删除 git 子模块 |

## Windows10 及 Linux 平台的演示动图
![buildpkg](/figures/buildpkg.gif)

## 测试平台

| 序号 | 测试平台 | 测试结果 | 
|:---|:---|:---|
| 1 | win10 | exe测试通过, py测试通过 |
| 2 | win7  | exe待测试, py待测试 | 
| 3 | mac   | py脚本不知道是否兼容, 没有测试条件, 后面维护下 |
| 4 | linux | py脚本不知道是否兼容, 没有测试条件, 后面维护下 |

## 联系人

* 邮箱：[1004383796@qq.com](mailto:1004383796@qq.com)
* 主页：[liu2guang](https://github.com/liu2guang)
* 仓库：[Github](https://github.com/liu2guang), [Gitee](https://github.com/liu2guang) 
