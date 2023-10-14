/*
 * Task.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Seif pc
 */

#ifndef TASK_H_
#define TASK_H_

#include "TaskTypes.h"
#include "Mem.h"
#include "RtosConfig.h"
#include "RTOS_svc_calls.h"
#include "Queue.h"

#define OS_CODE		AUTOMATIC
#define OS_TYPE		AUTOMATIC

#define OS_XPSR_OFFSET			1
#define OS_PC_OFFSET			2
#define OS_LR_OFFSET			3
#define OS_R12_OFFSET			4
#define OS_R3_OFFSET			5
#define OS_R2_OFFSET			6
#define OS_R1_OFFSET			7
#define OS_R0_OFFSET			8
#define OS_R11_OFFSET			9
#define OS_R10_OFFSET			10
#define OS_R9_OFFSET			11
#define OS_R8_OFFSET			12
#define OS_R7_OFFSET			13
#define OS_R6_OFFSET			14
#define OS_R5_OFFSET			15
#define OS_R4_OFFSET			16
#define OS_EPSR_TBIT_SET		(1<<24)

/**
 *
 * @param TaskPriority 		To set the priority of the Created Task
 * @param TaskID	   		To set ID of the Created Task
 * @param TaskStackSize		To set the Stack Size of the created Task
 * @param TaskEntryAddress	To set the initial Value of the Program Counter to point to a certain piece of code stored in ROM
 * @param Vparam			To Declare an initial Variable in Stack of Task
 * @brief The Following API Creates a Task by using dynamic memory allocation from a predefined buffer through the configuration file of the RTOS
 * @return Std_ReturnType   Whether The The Task was successfully Created or Not
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskCreate(VAR(uint8,OS_TYPE) TaskPriority,VAR(uint8,OS_TYPE) TaskID,
										  VAR(uint32,OS_TYPE) TaskStackSize,
										  VAR(TaskCode,OS_TYPE) TaskEntryAddress,P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC) TaskHandle,
										  P2VAR(void,AUTOMATIC,AUTOMATIC) Vparam);
/**
 *
 * @param 	TaskHandle  		Pointer to a certain Task
 * @brief 	The Following API Terminates the Execution of the Task Passed in the TaskHandle if NULL is Passed the Current Task is terminated
 * @return  Std_ReturnType	Whether The The Task was successfully terminated or Not
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskKill(P2VAR(Task,OS_TYPE,OS_TYPE) TaskHandle);
/**
 *
 * @param TaskHandle
 * @param Priority
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskSetPriority(P2VAR(Task,OS_TYPE,OS_TYPE) TaskHandle ,VAR(uint8,AUTOMATIC) Priority);
/**
 *
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskSchedule();
/**
 *
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskLockScheduler();
/**
 *
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskResumeScheduler();
/**
 *
 * @param TaskHandle
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskSleep(P2VAR(Task,OS_TYPE,OS_TYPE) TaskHandle);
/**
 *
 * @param TaskHandle
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskWakeup(P2VAR(Task,OS_TYPE,OS_TYPE) TaskHandle);
/**
 *
 * @param DelayTime
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskDelay(VAR(uint32,OS_TYPE) DelayTime);
/**
 * @brief Function To know the Current Task executing in the System
 * @param CurrentTask[o/p] Pointer to Pointer of Type Task to return CurrentTask
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) INLINE  OsGetCurrentTask(P2VAR(TaskHandle_t,OS_TYPE,OS_TYPE) CurrentRunningTask);

#endif /* TASK_H_ */
