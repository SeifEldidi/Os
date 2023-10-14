/*
 * Semaphores.c
 *
 *  Created on: Oct 12, 2023
 *      Author: Seif pc
 */

#include "Semaphores.h"

extern Queue OsSystemList;
extern Task *CurrentTask ;
extern Queue OsReadyQueue[];
/**
 * @brief Should be called from a Critical Section
 * to Block Current Task
 * @param Handler to Mutex/Semaphore
 */
LOCAL_INLINE void  BlockCurrentTask(P2VAR(Semaphore, AUTOMATIC, AUTOMATIC) Handler)
{
	//Since Task is Currently Running
	CurrentTask->OwnerList = Handler->SemaphoreBlockingQueue;
	CurrentTask->TaskStatus->TaskState = BLOCKED;
	//Add List to Sempahore Waiting List
	InsertQueueSorted(Handler->SemaphoreBlockingQueue, CurrentTask,NORMAL_QUEUE);
}
/**
 * @brief API to Signal for Semaphore or Mutex
 * @param Handler Pointer to Current Semaphore or Mutex
 */
STATIC void OsSemphMutexSig(P2VAR(Semaphore, AUTOMATIC, AUTOMATIC) Handler)
{
	P2VAR(Task, AUTOMATIC, AUTOMATIC) TaskHandle = NULL;

	if (Handler->Currval < Handler->StoredVal) {
		Handler->Currval++;
		//Check if a Task Exists in the Queue of Tasks waiting for the Semaphore
		if (Handler->SemaphoreBlockingQueue->Front != NULL) {
			VAR(uint8,AUTOMATIC) Priority_Curr =CurrentTask->TaskStatus->TaskPriority;
			VAR(uint8,AUTOMATIC) Priority_Rel = 0;
			//Remove The Last Task Requesting Semaphore
			DequeQueueFront(Handler->SemaphoreBlockingQueue,&TaskHandle,NORMAL_QUEUE);
			Priority_Rel = TaskHandle->TaskStatus->TaskPriority;
			//Set to Ready
			TaskHandle->OwnerList = &OsReadyQueue[Priority_Rel];
			TaskHandle->TaskStatus->TaskState = READY;
			//Check if the Released Task is Higher in Priority
			InsertQueueTail(&OsReadyQueue[Priority_Rel], TaskHandle,NORMAL_QUEUE);
			if (Priority_Rel > Priority_Curr) {
				InsertQueueTail(&OsReadyQueue[Priority_Curr], CurrentTask,NORMAL_QUEUE);
				//Reqeust Context Switch
				OS_DISPATCHER;
			} else {

			}
		} else {
		}
	} else {
		//Block Current Task
		//Since Task is Currently Running
		BlockCurrentTask(Handler);
		//Request Context Switch
		OS_DISPATCHER;
	}
}
/**
 * @brief Semaphore Init Function to Create Either Binary Semaphore or Counting Semaphore
 * @param SemaphoreInitVal Initial Semaphore Value if One than Maximum Value for Semaphore is 1 otherwise SemaphoreMax = InitVal
 * @param SemaphoreMode Defines Whether Semaphhore is Binary or Counting
 * @return
 */
FUNC(SemaphoreHandle , AUTOMATIC) OsSemaphoreInit(VAR(uint32,AUTOMATIC) SemaphoreInitVal,VAR(uint8,AUTOMATIC) SemaphoreMode)
{
	OS_CS_EN();
	SemaphoreHandle SemaphoreRet = NULL;
	if ((SemaphoreMode == BinarySempaphore && SemaphoreInitVal > 1))
	{
		SemaphoreRet = NULL;
	} else {
		SemaphoreRet = OsMalloc(sizeof(Semaphore));
		if (SemaphoreRet != NULL)
		{
			SemaphoreRet->SemaphoreBlockingQueue = OsMalloc(sizeof(Queue));
			if (SemaphoreRet->SemaphoreBlockingQueue != NULL)
			{
				SemaphoreRet->Currval   = SemaphoreInitVal;
				if(SemaphoreMode == BinarySempaphore )
				{
					SemaphoreRet->StoredVal = BinarySemaphoreActive;
				}else{
					SemaphoreRet->StoredVal = SemaphoreRet->Currval;
				}
				//Semaphore Not Mutex
				SemaphoreRet->OwnerTask = NULL;
			} else {
				SemaphoreRet = NULL;
			}
		} else {
			SemaphoreRet = NULL;
		}
	}
	OS_CS_DIS();
	return SemaphoreRet;
}
/**
 *
 * @param MutexInitVal
 * @param MutexMode
 * @return
 */
FUNC(MutexHandle , AUTOMATIC) OsMutexInit(VAR(uint32,AUTOMATIC) MutexInitVal,VAR(uint8,AUTOMATIC) MutexMode)
{
	OS_CS_EN();
	MutexHandle MutexHandle_t = NULL;
	if(MutexMode == NonRecurssiveMutex && MutexInitVal < 1)
	{
		MutexHandle_t = OsSemaphoreInit(MutexInitVal,MutexMode);
	}else if(MutexMode == RecurssiveMutex)
	{
		MutexHandle_t = OsSemaphoreInit(MutexInitVal,MutexMode);
	}else{
		MutexHandle_t = NULL;
	}
	OS_CS_DIS();
	return MutexHandle_t;
}
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsSemaphoreDelete(VAR(SemaphoreHandle,AUTOMATIC) SemaphoreHandle)
{
	OS_CS_EN();
	Std_ReturnType Ret = E_OK;
	if(SemaphoreHandle != NULL)
	{
		P2VAR(Task,AUTOMATIC,AUTOMATIC) 		QueueHead  = SemaphoreHandle->SemaphoreBlockingQueue->Front;
		P2VAR(Task,AUTOMATIC,AUTOMATIC) 		TaskHandle = NULL;

		SemaphoreHandle->Currval   = 0;
		SemaphoreHandle->OwnerTask = NULL;
		SemaphoreHandle->StoredVal = 0;

		//Dequeu all Tasks from Queue
		while(QueueHead != NULL)
		{
			DequeQueueFront(SemaphoreHandle->SemaphoreBlockingQueue ,&TaskHandle,NORMAL_QUEUE);
			//Add Task To suspended List
			TaskHandle->OwnerList  = &OsSystemList;
			CurrentTask->TaskStatus->TaskState = SUSPENDED;
			//Add to Suspended Queue since Queue is Sorted Add to Tail of OsSystemList
			InsertQueueTail(&OsSystemList , TaskHandle,NORMAL_QUEUE);
			//Proceed to Next Task
			QueueHead = QueueHead->Next;
		}
		//Free Semaphore Queue and Semaphore TCB
		OsFree(SemaphoreHandle->SemaphoreBlockingQueue);
		OsFree(SemaphoreHandle);
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsSemaphoreWait(VAR(SemaphoreHandle,AUTOMATIC) SemaphoreHandle)
{
	OS_CS_EN();
	Std_ReturnType Ret = E_OK;
	if (SemaphoreHandle != NULL)
	{
		//Check if Current Semaphore Value is Valid or not otherwise block Task
		if(SemaphoreHandle->Currval > 0 )
		{
			SemaphoreHandle->Currval --;
		}else{
			//Since Task is Currently Running
			BlockCurrentTask(SemaphoreHandle);
			//Reqeust Context Switch
			OS_DISPATCHER;
		}
	} else {
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}

/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsMutexWait(VAR(MutexHandle,AUTOMATIC) MutexHandler)
{
	OS_CS_EN();
	Std_ReturnType Ret = E_OK;
	if (MutexHandler != NULL) {
		//Check if Current Semaphore Value is Valid or not otherwise block Task
		if (MutexHandler->Currval > 0) {
			MutexHandler->Currval--;
		} else {
			//Since Task is Currently Running
			BlockCurrentTask(MutexHandler);
			//Reqeust Context Switch
			OS_DISPATCHER;
		}
	} else {
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsSemaphoreSignal(VAR(SemaphoreHandle,AUTOMATIC) SemaphoreHandle)
{
	OS_CS_EN();
	Std_ReturnType Ret = E_OK;
	if (SemaphoreHandle != NULL)
	{
		//Check if Current Semaphore Value is Valid or not otherwise block Task
		OsSemphMutexSig(SemaphoreHandle);
	} else {
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}

/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsMutexSignal(VAR(MutexHandle,AUTOMATIC) MutexHandler)
{
	OS_CS_EN();
	Std_ReturnType Ret = E_OK;
	if (MutexHandler != NULL)
	{
		//Check if Current Semaphore Value is Valid or not otherwise block Task
		if(MutexHandler->OwnerTask == NULL || MutexHandler->OwnerTask == CurrentTask )
		{
			if (MutexHandler->OwnerTask == NULL)
				MutexHandler->OwnerTask = CurrentTask;
			else {
			}
			OsSemphMutexSig(MutexHandler);
		}else{
			BlockCurrentTask(MutexHandler);
			//Request Context Switch
			OS_DISPATCHER;
		}
	} else {
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsSemaphoreReset(VAR(SemaphoreHandle,AUTOMATIC) SemaphoreHandle)
{
	OS_CS_EN();
	Std_ReturnType Ret = E_OK;
	if (SemaphoreHandle != NULL) {
		P2VAR(Task,AUTOMATIC,AUTOMATIC) QueueHead  = SemaphoreHandle->SemaphoreBlockingQueue->Front;
		P2VAR(Task,AUTOMATIC,AUTOMATIC) TaskHandle = NULL;

		SemaphoreHandle->Currval   = SemaphoreHandle->StoredVal;
		SemaphoreHandle->OwnerTask = NULL;
		VAR(uint8,AUTOMATIC) Priority 	  = 0;
		VAR(uint8,AUTOMATIC) Priority_Curr = CurrentTask->TaskStatus->TaskPriority;

		//Dequeu all Tasks from Queue and Add to Ready Queue
		while (QueueHead != NULL) {
			DequeQueueFront(SemaphoreHandle->SemaphoreBlockingQueue,&TaskHandle,NORMAL_QUEUE);
			//Add Task To suspended List
			TaskHandle->OwnerList  = &OsReadyQueue[TaskHandle->TaskStatus->TaskPriority];
			CurrentTask->TaskStatus->TaskState = READY;
			//Add to Suspended Queue since Queue is Sorted Add to Tail of OsSystemList
			InsertQueueTail(&OsReadyQueue[TaskHandle->TaskStatus->TaskPriority], TaskHandle,NORMAL_QUEUE);
			//Proceed to Next Task
			if(TaskHandle->TaskStatus->TaskPriority > Priority)
			{
				Priority = TaskHandle->TaskStatus->TaskPriority;
			}
			QueueHead = QueueHead->Next;
		}
		if(Priority > Priority_Curr)
		{
			//Reqeust Context Switch
			OS_DISPATCHER;
		}
	} else {
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
