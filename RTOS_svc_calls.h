/*
 * RTOS_svc_calls.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */

#ifndef RTOS_SVC_CALLS_H_
#define RTOS_SVC_CALLS_H_

#include "TaskTypes.h"
#include "RtosConfig.h"
#include "Queue.h"
#include "Std_Types.h"

///System Priority = 2 below 2 interrupts are higher than the kernel;

#define ICSR					*((__IO uint32 *)(0x04 + 0xE000ED00))
#define PENDSV_PENDING			(1<<28)

#define OS_CS_EN()	 	({__asm volatile("SVC #1");})
#define CS_ON			{__asm volatile("MOV R0 , 0x02<<4");\
						 __asm volatile("MSR BASEPRI,R0");}

#define OS_CS_DIS()  	({__asm volatile("SVC #0");})
#define CS_OFF			{__asm volatile("MOV R0 , 0x00<<4");\
						 __asm volatile("MSR BASEPRI,R0");}

#define OS_DISPATCHER			__asm volatile("SVC #2")


#define OS_SCHEDULER_LOCK		__asm volatile("SVC #3")
#define OS_SCHEDULER_UNLOCK		__asm volatile("SVC #4")

#define OS_LOCK_SCHEDULER()		{__asm volatile("MOV R0 , 0x0E<<4");\
								 __asm volatile("MSR BASEPRI,R0");}

#define OS_UNLOCK_SCHEDULER()	{__asm volatile("MOV R0 , 0x00<<4");\
								 __asm volatile("MSR BASEPRI,R0");}


void NAKED OSGetPrivlegde();
void NAKED OSReleasePrivlegde();
void NAKED OsDispatcher();
void       OsSelectTask();

#endif /* RTOS_SVC_CALLS_H_ */
