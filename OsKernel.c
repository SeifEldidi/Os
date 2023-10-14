/*
 * OsKernel.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */

#include "OsKernel.h"

uint8 SchedulerStatus;
uint8 KernelrStatus;
extern uint8 HighestPriority ;

extern Queue OsReadyQueue[ConfigPriorities] ;

extern Task *CurrentTask ;

static Task MainFunction ;

Queue   OsKernelTrackTasksList ;

P2VAR(Task,AUTOMATIC,AUTOMATIC) IdleTaskPtr = NULL;

/**
 * @brief Main Function Address to Jump to The Function in Case of OS shutdown
 */
FUNC(void,AUTOMATIC) main();
/**
 * @brief Idle Task in Case No Task is Active in the System
 */
FUNC(static void,AUTOMATIC) IdleTask();
/**
 * @brief Api to Initiliaze A dummy Task to point and return to the Main function
 */
FUNC(static void,AUTOMATIC) MainTaskInit();
/**
 * @brief Api to Switch to privleged Access Level
 */
FUNC(static void,AUTOMATIC) OsUserMode()
{
	/*------ Enter User Mode -------*/
	/*-----Set Stack Pointer to PSP and No privelegde---*/
	__asm volatile("MRS R0 , CONTROL");
	__asm volatile("ORR R0,R0,#3");
	__asm volatile("MSR CONTROL,R0");
}
/**
 *
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) OsKernelStart()
{
	VAR(Std_ReturnType,AUTOMATIC) Ret = E_OK;
	/*---------Lock Scheduler -----*/
	OS_SCHEDULER_LOCK;
	if(KernelrStatus != 1)
	{
		//Create Idle Task
		OsTaskCreate(IDLE_TASK_PRIORITY,0,IDLE_TASK_STACK,IdleTask,&IdleTaskPtr,NULL);
		//Start Scheduler
		if(IdleTaskPtr != NULL)
		{
			//Create Task to Point To Main Function To Shutdown OS
			MainTaskInit();
			/*------------- Add Task To Kernel Task Lists to Track All Tasks in System in Case of Shutdown-------*/
			InsertQueueTail(&OsKernelTrackTasksList, IdleTaskPtr);
			/*------Init Systick Timer ------*/
			SYSTICK_PRI |= (SYSTICK_PRI_LOWEST << SYSTICK_PRI_OFFSET);
			SYSTICK_PRI |= (PENDSV_PRI_LOWEST << PENDSV_PRI_OFFSET);
			/*------Call Startup Hook --------*/
			//OsStartupHook();
			/*------Start Scheduler -------*/
			SchedulerStatus = OsSchedulerStarted;
			if(HighestPriority == 0)
				CurrentTask = IdleTaskPtr;
			/*------Select Highest Priority Task ----*/
			else
				DequeQueueFront(&OsReadyQueue[HighestPriority], &CurrentTask);
			/*------ Enter User Mode -------*/
			/*-----Set Stack Pointer to PSP and No privelegde---*/
			OsUserMode();
			/*------Load StackPointer -----*/
			__asm volatile ("LDR R0 ,= CurrentTask");
			__asm volatile ("LDR R1 ,[R0]");
			__asm volatile ("LDR SP ,[R1,#16]");
			/*------Restore Context -----*/
			__asm volatile("POP {R4-R11}");
			__asm volatile("POP {R0-R3}");
			__asm volatile("POP {R12}");
			__asm volatile ("ADD SP,SP,#4");
			/*----Create New Start Location----*/
			__asm volatile ("POP {LR}");
			__asm volatile ("ADD SP,SP,#4");
			/*----Unlock Scheduler-----*/
			OS_SCHEDULER_UNLOCK;
			__asm volatile ("BX LR");
		}else{
			Ret = E_NOT_OK;
		}
	}else{
		Ret = E_NOT_OK;
	}
	return Ret;
}

/**
 * @brief API to Shutdown osKernel
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) OsKernelShutdown()
{
	OS_CS_EN();
	VAR(Std_ReturnType,AUTOMATIC) Ret = E_OK;
	/*----------- deinitiliaze All System Tasks ----------*/
	P2VAR(Task,AUTOMATIC,AUTOMATIC) WalkList   = OsKernelTrackTasksList.Front;
	P2VAR(Task,AUTOMATIC,AUTOMATIC) DeleteTask = OsKernelTrackTasksList.Front;
	if(WalkList != NULL)
	{
		//Remove All Tasks from System
		while(WalkList != NULL)
		{
			/*---------Select Current Task----------*/
			DeleteTask = WalkList;
			/*---------Move To Next Task----------*/
			WalkList = WalkList->Next;
			/*--------- Remove Task From Current List --------*/
			DequeQueueElement(DeleteTask->OwnerList, DeleteTask);
			/*------Release All Resources Of Task*/
			DeleteTask->Next = NULL;
			DeleteTask->Prev = NULL;
			DeleteTask->OwnerList = NULL;
			/*----- Free Resources -------*/
			Ret |= OsFree((P2VAR(void,AUTOMATIC,AUTOMATIC)) (DeleteTask->Stack.StackBottom));
			Ret |= OsFree((P2VAR(void,AUTOMATIC,AUTOMATIC)) (DeleteTask->TaskStatus));
			Ret |= OsFree(((P2VAR(void,AUTOMATIC,AUTOMATIC) )DeleteTask));
		}
		//Release All Kernel Objects such as Semaphores and Mutexs and Queues and So on
		//Return To Main Function
		CurrentTask = &MainFunction;
		KernelrStatus = 1;
		OS_DISPATCHER;
	}else{

	}
	OS_CS_DIS();
	return Ret;
}

/**
 * @brief Api to Initiliaze A dummy Task to point and return to the Main function
 */
FUNC(static void,AUTOMATIC) MainTaskInit()
{
	//Create Task to Point To Main Function
	MainFunction.Next = NULL;
	MainFunction.Prev = NULL;
	MainFunction.OwnerList = NULL;
	MainFunction.Stack.StackTop = ((VAR(Stackptr,AUTOMATIC)) 0x20005000);
	MainFunction.Stack.StackBottom = NULL;
	MainFunction.TaskEntryAddress = main;
	//Set Stack of Main Function
	MainFunction.Stack.StackTop[-1] = (1 << 24);
	MainFunction.Stack.StackTop[-2] = (VAR(uint32,AUTOMATIC)) MainFunction.TaskEntryAddress;
	MainFunction.Stack.StackTop     = (VAR(sint32 *,AUTOMATIC)) (&MainFunction.Stack.StackTop[-16]);
}

/**
 * @brief Idle Task in Case No Task is Active in the System
 */
FUNC(static void,AUTOMATIC)  IdleTask()
{
	while(1)
	{

	}
}
