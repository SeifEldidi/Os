/*
 * TaskTypes.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Seif pc
 */

#ifndef TASKTYPES_H_
#define TASKTYPES_H_

#include "Std_Types.h"

#define OS_APPL_DATA AUTOMATIC
#define OS_VAR_FAST  AUTOMATIC

typedef void (*TaskCode)(void *);
typedef sint32 *Stackptr;

typedef struct
{
	VAR(Stackptr,AUTOMATIC) StackTop; //4bytes
	VAR(Stackptr,AUTOMATIC) StackBottom; //4bytes
}StackMemPtr;


typedef struct PACKED
{
	VAR(uint8,AUTOMATIC) TaskState; //1 byte
	VAR(uint8,AUTOMATIC) TaskPriority;//1 byte
}TCB;

typedef struct
{
	VAR(uint32,AUTOMATIC) TaskExecTime; // 4 bytes
	VAR(uint32,AUTOMATIC) TaskSleepTime;//4bytes
}TaskTimingInfo;

typedef struct PACKED Task
{
	P2VAR(TCB,AUTOMATIC,AUTOMATIC) 		   TaskStatus; // 4bytes
	VAR(TaskCode,AUTOMATIC)    			   TaskEntryAddress; //4bytes
	VAR(TaskTimingInfo,AUTOMATIC) 		   Timing ;//8bytes
	/*--------------Stack  -------------*/
	VAR(StackMemPtr,AUTOMATIC) 			   Stack ; //8 bytes
	/*--------------Owner List-------------*/
	P2VAR(void,AUTOMATIC,AUTOMATIC) 	   OwnerList ; //4 bytes
	/*--------------List 1 -------------*/
	P2VAR(struct Task,AUTOMATIC,AUTOMATIC) Next; //4 bytes
	P2VAR(struct Task,AUTOMATIC,AUTOMATIC) Prev; //4 bytes
	/*---------List 2 System List--------*/
	P2VAR(struct Task,AUTOMATIC,AUTOMATIC) SystemNext; //4 bytes
	P2VAR(struct Task,AUTOMATIC,AUTOMATIC) SystemPrev; //4 bytes
}Task;

typedef enum
{
	RUNNING = 0x00,
	READY,
	BLOCKED,
	SUSPENDED,
}TaskState;

typedef Task * TaskHandle_t;

#endif /* TASKTYPES_H_ */
