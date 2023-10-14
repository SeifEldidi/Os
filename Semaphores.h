/*
 * Semaphores.h
 *
 *  Created on: Oct 12, 2023
 *      Author: Seif pc
 */

#ifndef SEMAPHORES_H_
#define SEMAPHORES_H_

#include "Std_Types.h"
#include "RTOS_svc_calls.h"
#include "TaskTypes.h"
#include "Mem.h"
#include "Queue.h"

#define BinarySempaphore	0
#define BinarySemaphoreActive 	1
#define BinarySemaphoreLocked	0
#define CountingSemaphore 	1

#define RecurssiveMutex		1
#define NonRecurssiveMutex	0
#define MutexActive 		1
#define MutexLocked			0

typedef struct
{
	VAR(uint32,AUTOMATIC) Currval;
	VAR(uint32,AUTOMATIC) StoredVal;
	P2VAR(Task,AUTOMATIC,AUTOMATIC)	OwnerTask; // For Mutex
	P2VAR(Queue,AUTOMATIC,AUTOMATIC)  SemaphoreBlockingQueue;// For Semaphore && Mutes
}Semaphore;

typedef Semaphore * MutexHandle ;
typedef Semaphore * SemaphoreHandle ;
/**
 * @brief Semaphore Init Function to Create Either Binary Semaphore or Counting Semaphore
 * @param SemaphoreInitVal Initial Semaphore Value if One than Maximum Value for Semaphore is 1 otherwise SemaphoreMax = InitVal
 * @param SemaphoreMode Defines Whether Semaphhore is Binary or Counting
 * @return
 */
FUNC(SemaphoreHandle , AUTOMATIC) OsSemaphoreInit(VAR(uint32,AUTOMATIC) SemaphoreInitVal,VAR(uint8,AUTOMATIC) SemaphoreMode);
/**
 *
 * @param MutexInitVal
 * @param MutexMode
 * @return
 */
FUNC(MutexHandle , AUTOMATIC) OsMutexInit(VAR(uint32,AUTOMATIC) MutexInitVal,VAR(uint8,AUTOMATIC) MutexMode);
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsSemaphoreDelete(VAR(SemaphoreHandle,AUTOMATIC) SemaphoreHandle);
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsSemaphoreWait(VAR(SemaphoreHandle,AUTOMATIC) SemaphoreHandle);
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsMutexWait(VAR(MutexHandle,AUTOMATIC) MutexHandler);
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsSemaphoreSignal(VAR(SemaphoreHandle,AUTOMATIC) SemaphoreHandle);
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsMutexSignal(VAR(MutexHandle,AUTOMATIC) MutexHandler);
/**
 *
 * @param SemaphoreHandle
 * @return
 */
FUNC(Std_ReturnType , AUTOMATIC) OsSemaphoreReset(VAR(SemaphoreHandle,AUTOMATIC) SemaphoreHandle);
#endif /* SEMAPHORES_H_ */
