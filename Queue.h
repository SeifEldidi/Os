/*
 * Queue.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "TaskTypes.h"

#define SYSTEM_QUEUE 1
#define NORMAL_QUEUE 0

typedef struct
{
	Task *Front;
	Task *Tail;
	uint16 No_Tasks;
}Queue;

/**
 *
 * @param CurrQueue
 * @param Elem
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) InsertQueueTail(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											   P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem ,VAR(uint8,AUTOMATIC) Flag);

/**
 *
 * @param CurrQueue
 * @param Elem
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) InsertQueueSorted(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											    	P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem
													,VAR(uint8,AUTOMATIC) Flag);
/**
 *
 * @param CurrQueue
 * @param Elem
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) InsertQueueFront(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											    P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem
												,VAR(uint8,AUTOMATIC) Flag);
/**
 *
 * @param CurrQueue
 * @param Deque
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) DequeQueueTail(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											  P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC)Deque
											  ,VAR(uint8,AUTOMATIC) Flag);
/**
 *
 * @param CurrQueue
 * @param Elem
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) DequeQueueElement(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											  P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem
											  ,VAR(uint8,AUTOMATIC) Flag);
/**
 *
 * @param CurrQueue
 * @param Deque
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) DequeQueueFront(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
												P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC)Deque
												,VAR(uint8,AUTOMATIC) Flag);

#endif /* QUEUE_H_ */
