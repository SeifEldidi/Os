#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_
/*
 * @brief : Define For 8bit CPU
 */
#define CPU_TYPE_8		8
/*
 * @brief : Define For 16bit CPU
 */
#define CPU_TYPE_16		16
/*
 * @brief : Define For 32bit CPU
 */
#define CPU_TYPE_32		32
/*
 * @brief : Define For 64bit CPU
 */
#define CPU_TYPE_64		64

/* [SWS_Platform_00064]
 * CPU_TYPE
 *@brief : This symbol shall be defined as #define having one of the values CPU_TYPE_8,
			CPU_TYPE_16, CPU_TYPE_32 or CPU_TYPE_64 according to the platform.
 */
#define CPU_TYPE		CPU_TYPE_32


/*
 * @brief : The most significant bit is the first bit of the bit sequence.
 */
#define MSB_FIRST		1
/*
 * @brief :  The least significant bit is the first bit of the bit sequence.
 */
#define LSB_FIRST		0
/* [SWS_Platform_00038]
 * CPU_BIT_ORDER
 *@brief : This symbol shall be defined as #define having one of the values
 * 		   HIGH_BYTE_FIRST or LOW_BYTE_FIRST according to the platform
 */
#define CPU_BIT_ORDER	LSB_FIRST

/*
 * @brief : Within uint16, the high byte is located before the low byte
 */
#define HIGH_BYTE_FIRST		1
/*
 * @brief :  Within uint16, the low byte is located before the high byte.
 */
#define LOW_BYTE_FIRST		0
/* [SWS_Platform_00039]
 * CPU_BYTE_ORDER
 *@brief : This symbol shall be defined as #define having one of the values
 *		   HIGH_BYTE_FIRST or LOW_BYTE_FIRST according to the platform
 */
#define CPU_BYTE_ORDER	LOW_BYTE_FIRST

/*
 * @brief :  TRUE Which is used with boolean data type
 */
#ifndef TRUE
#define TRUE 1
#endif

/*
 * @brief : False Which is used with boolean data type
 */
#ifndef FALSE
#define FALSE 0
#endif

#if CPU_TYPE == CPU_TYPE_8


#endif

#if CPU_TYPE == CPU_TYPE_16


#endif

#if CPU_TYPE == CPU_TYPE_32
/*
 * @brief : This standard AUTOSAR type shall only be used together with the definitions TRUE
			and FALSE.
 */
typedef unsigned char 		boolean;
/*
 * @brief : This standard AUTOSAR type shall be of 8 bit signed
 */
typedef signed char 		sint8;
/*
 * @brief : This standard AUTOSAR type shall be of 8 bit unsigned.
 */
typedef unsigned char 		uint8;
/*
 * @brief : This standard AUTOSAR type shall be of 16 bit signed
 */
typedef signed short 		sint16;
/*
 * @brief : This standard AUTOSAR type shall be of 16 bit unsigned
 */
typedef unsigned short 		uint16;
/*
 * @brief : This standard AUTOSAR type shall be of 32 bit signed
 */
typedef signed long 		sint32;
/*
 * @brief : This standard AUTOSAR type shall be of 64 bit signed
 */
typedef signed long long 	sint64;
/*
 * @brief : This standard AUTOSAR type shall be of 32 bit unsigned
 */
typedef unsigned long 		uint32;
/*
 * @brief : This standard AUTOSAR type shall be of 64 bit unsigned
 */
typedef unsigned long long  uint64;

/*
 * @brief : This optimized AUTOSAR type shall be at least 8 bit unsigned.
 */
typedef unsigned short 		uint8_least;
/*
 * @brief : This optimized AUTOSAR type shall be at least 16 bit unsigned.
 */
typedef unsigned short 		uint16_least;
/*
 * @brief : This optimized AUTOSAR type shall be at least 32 bit unsigned.
 */
typedef unsigned long 		uint32_least;
/*
 * @brief : This optimized AUTOSAR type shall be at least 8 bit signed.
 */
typedef signed short 		sint8_least;
/*
 * @brief : This optimized AUTOSAR type shall be at least 16 bit signed.
 */
typedef signed short 		sint16_least;
/*
 * @brief : This optimized AUTOSAR type shall be at least 32 bit signed.
 */
typedef signed long 		sint32_least;

/*
 * @brief : This standard AUTOSAR type shall follow the 32-bit binary interchange format
 *			according to IEEE 754-2008 with encoding parameters specified in chapter 3.6, table
 *			3.5, column "binary32".
 */
typedef float 				float32;
/*
 * @brief : This standard AUTOSAR type shall follow the 64-bit binary interchange format
 *			according to IEEE 754-2008 with encoding parameters specified in chapter 3.6,
 *			table 3.5, column "binary64".
 */
typedef double 				float64;
#endif

#if CPU_TYPE == CPU_TYPE_64


#endif

#endif /* PLATFORM_TYPES_H_ */
