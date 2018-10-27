#ifndef __REDEF_H
#define __REDEF_H

/************************************************************
  * @brief   redeg.h
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    这个文件重定义一些变量与错误的宏
  ***********************************************************/

#include "include.h"

/* exact-width signed integer types */
typedef   signed            char int8_t;
typedef   signed short      int int16_t;
typedef   signed            int int32_t;


/* exact-width unsigned integer types */
typedef unsigned            char uint8_t;
typedef unsigned short      int uint16_t;
typedef unsigned            int uint32_t;


typedef int32_t              err_t;       /**< Type for error number */


    /* minimum values of exact-width signed integer types */
#define INT8_MIN                   -128
#define INT16_MIN                -32768
#define INT32_MIN          (~0x7fffffff)   /* -2147483648 is unsigned */
#define INT64_MIN  __INT64_C(~0x7fffffffffffffff) /* -9223372036854775808 is unsigned */

    /* maximum values of exact-width signed integer types */
#define INT8_MAX                    127
#define INT16_MAX                 32767
#define INT32_MAX            2147483647
#define INT64_MAX  __INT64_C(9223372036854775807)

    /* maximum values of exact-width unsigned integer types */
#define UINT8_MAX                   255
#define UINT16_MAX                65535
#define UINT32_MAX           4294967295u
#define UINT64_MAX __UINT64_C(18446744073709551615)

/* 32bit CPU */
typedef long                base_t;      /**< Nbit CPU related date type */
typedef unsigned long       ubase_t;     /**< Nbit unsigned CPU related data type */

/************************** 暂时不用区域 **********************************/
//typedef uint32_t            time_t;      /**< Type for time stamp */
//typedef uint32_t            tick_t;      /**< Type for tick count */
//typedef base_t              flag_t;      /**< Type for flags */
//typedef ubase_t             size_t;      /**< Type for size number */
//typedef ubase_t             dev_t;       /**< Type for device */
//typedef base_t              off_t;       /**< Type for offset */
/************************** 暂时不用区域 **********************************/

/* Compiler Related Definitions */
#ifdef __CC_ARM                         /* ARM Compiler */
  #include <stdarg.h>
  #define SECTION(x)               __attribute__((section(x)))
  #define UNUSED                   __attribute__((unused))
  #define USED                     __attribute__((used))
  #define ALIGN(n)                 __attribute__((aligned(n)))
  #define WEAK                     __weak
  #define inline                   static __inline

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
  #include <stdarg.h>
  #define SECTION(x)               @ x
  #define UNUSED
  #define USED                     __root
  #define PRAGMA(x)                _Pragma(#x)
  #define ALIGN(n)                 PRAGMA(data_alignment=n)
  #define WEAK                     __weak
  #define inline                   static inline

#elif defined (__GNUC__)                /* GNU GCC Compiler */
  
  #define SECTION(x)               __attribute__((section(x)))
  #define UNUSED                   __attribute__((unused))
  #define USED                     __attribute__((used))
  #define ALIGN(n)                 __attribute__((aligned(n)))
  #define WEAK                     __attribute__((weak))
  #define inline                   static __inline

#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
  #include <stdarg.h>
  #define SECTION(x)               __attribute__((section(x)))
  #define UNUSED                   __attribute__((unused))
  #define USED                     __attribute__((used))
  #define ALIGN(n)                 __attribute__((aligned(n)))
  #define WEAK                     __attribute__((weak))
  #define inline                   static inline
#elif defined (_MSC_VER)
  #include <stdarg.h>
  #define SECTION(x)
  #define UNUSED
  #define USED
  #define ALIGN(n)                 __declspec(align(n))
  #define WEAK
  #define inline                   static __inline
#elif defined (__TI_COMPILER_VERSION__)
  #include <stdarg.h>
  /* The way that TI compiler set section is different from other(at least
   * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
   * details. */
  #define SECTION(x)
  #define UNUSED
  #define USED
  #define PRAGMA(x)                _Pragma(#x)
  #define ALIGN(n)
  #define WEAK
  #define inline                   static inline
#else
  #error not supported tool chain
#endif

/************************************************************ 
  * @note    下面是记录ERR返回值的宏
  ***********************************************************/
#define NULL          ( 0 )

#define ERR_OK        ( 0 )
#define ERR_NOK       ( -1 )
#define ERR_NULL      ( -2 )
#define ERR_UNUSE     ( -3 )
    
    
#endif /* __REDEF_H */


/********************************END OF FILE***************************************/

