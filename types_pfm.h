/**********************************************************************************************************************
 *
 * This firmware, architecture, comments and all information contained within are the property of Creekside Controls 
 * LLC.  Creekside may provide this firmware as an aid to customer understanding or for further development on projects 
 * started by Creekside Controls in the case that Creekside Controls is no longer working on the project.  This 
 * firmware is NOT to be used for any design unless Creekside Controls started the design, or the firmware is licensed 
 * for the specific use at www.creeksidecontrols.com.
 * 
 * This firmware is provided AS IS.  Creekside Controls does not provide warranty on any use of this firmware.  
 * Creekside Controls shall not be held liable for any direct, indirect or consequential damages with respect to any
 * claims arising from the content of such firmware and/or the usage of this firmware in a product.
 *  
 * COPYRIGHT 2010 Creekside Controls LLC 
 * 
 **********************************************************************************************************************/  
#ifndef _TYPES_PFM_H_
#define _TYPES_PFM_H_

//TODO: This is carry over from the RC400 project. Should be removed at the
//       earliest convenience.

#ifndef NULL
#define NULL 0
#endif

typedef unsigned char      u8_t; 
typedef signed char        i8_t; 
typedef unsigned short     u16_t;
typedef signed short       i16_t;
typedef unsigned long      u32_t;
typedef signed long        i32_t;
typedef unsigned long long u64_t;
typedef signed long long   i64_t;
typedef float              float32_t;
typedef double             float64_t;

//typedef u8_t               bool_t;
//#define

typedef void (*void_func_t)(void);
typedef void (*void_param_func_t)(u8_t u8_param);
typedef void (*void_two_param_func_t)(u8_t param1, u8_t param2);
typedef void (*void_msg_func_t)(u8_t u8_param, u8_t *pau8_msg);

/*******************************************************************************
// Global Constants
*******************************************************************************/
/* Maximum signed 8-bit value */
#define MAX_INT8   ((i8_t)127)
/* Minimum signed 8-bit value */
#define MIN_INT8   ((i8_t)(-MAX_INT8 - 1))
/* Maximum unsigned 8-bit value */
#define MAX_UINT8  ((u8_t)255)
/* Minimum unsigned 8-bit value */
#define MIN_UINT8  ((u8_t)0)
/* Maximum signed 16-bit value */
#define MAX_INT16  ((i16_t)32767)
/* Minimum signed 16-bit value */
#define MIN_INT16  ((i16_t)(-MAX_INT16 - 1))
/* Maximum unsigned 16-bit value */
#define MAX_UINT16 ((u16_t)65535)
/* Minimum unsigned 16-bit value */
#define MIN_UINT16 ((u16_t)0)
/* Maximum signed 32-bit value */
#define MAX_INT32  ((i32_t)2147483647ul)
/* Minimum signed 32-bit value */
#define MIN_INT32  ((i32_t)(-MAX_INT32 - 1))
/* Maximum unsigned 32-bit value */
#define MAX_UINT32 ((u32_t)4294967295ul)
/* Minimum unsigned 32-bit value */
#define MIN_UINT32 ((u32_t)0)

#endif /* _TYPES_PFM_H_ */
