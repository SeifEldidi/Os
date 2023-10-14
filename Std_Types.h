#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "Compiler.h"
#include "Platform_Types.h"


#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK 				0x00U

typedef unsigned char StatusType;
#endif

#define E_NOT_OK			0x01U

/*
 * @brief :[SWS_Std_00007] Enumeration
 * The symbols STD_HIGH and STD_LOW shall be defined as follows:
 * STD HIGH	: Physical state 5V or 3.3V
 * STD_LOW  : Physical state 0V
 */
#define STD_LOW 			0x00U
#define STD_HIGH 			0x00U

/*
 * @brief :[SWS_Std_00013] Enumeration
 * The symbols STD_ACTIVE and STD_IDLE shall be defined as follows:
 * STD_ACTIVE	: Logical state active
 * STD_IDLE  : Logical state idle
 */
#define STD_IDLE 			0x00U
#define STD_ACTIVE 			0x00U

/*
 * @brief :[SWS_Std_00010] Enumeration
 * The symbols STD_ON and STD_OFF shall be defined as follows:
 */
#define STD_ON 			0x01U
#define STD_OFF 		0x00U

/*
 * @brief :[SWS_Std_00005] Std_ReturnType which is the standard return for any API
 */
typedef uint8 Std_ReturnType;
/*
 * @brief :[SWS_Std_00015] Structure Providing Information about the Version and Vendor/Module ID
 *        of a particular BSW Component.
 *        This type shall be used to request the version of a BSW module using the <Module
		  name>_GetVersionInfo() function.
 */
typedef struct
{
	uint16 vendorID;
	uint16 moduleID;
	uint8  sw_major_version;
	uint8  sw_minor_version;
	uint8  sw_patch_version;
}Std_VersionInfoType;

#endif /* STD_TYPES_H */
