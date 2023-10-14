/*
 * Queue.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */
#include "Queue.h"

FUNC(Std_ReturnType,AUTOMATIC) InsertQueueTail(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											   P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem)
{
	Std_ReturnType Ret = E_OK;
	if(CurrQueue != NULL && Elem != NULL)
	{
		if(CurrQueue->No_Tasks == 0)
		{
			CurrQueue->Front = Elem;
			CurrQueue->Tail  = Elem;
			CurrQueue->Front->Prev = NULL;
			CurrQueue->Front->Next = NULL;
			CurrQueue->No_Tasks++;
		}else{
			CurrQueue->Tail->Next = Elem;
			Elem->Prev = CurrQueue->Tail;
			CurrQueue->Tail = CurrQueue->Tail->Next;
			CurrQueue->No_Tasks++;
		}
	}else{
		Ret = E_NOT_OK;
	}
	return Ret;
}

FUNC(Std_ReturnType,AUTOMATIC) InsertQueueSorted(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											    	P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Elem != NULL) {
		if (CurrQueue->No_Tasks == 0) {
			CurrQueue->Front = Elem;
			CurrQueue->Tail = Elem;
			CurrQueue->Front->Prev = NULL;
			CurrQueue->Front->Next = NULL;
			CurrQueue->No_Tasks++;
		} else {
			VAR(uint8 , AUTOMATIC) 			 Priority_Inserted = Elem->TaskStatus->TaskPriority;
			P2VAR(Task,AUTOMATIC,AUTOMATIC)  Insert = CurrQueue->Front;
			P2VAR(Task,AUTOMATIC,AUTOMATIC)  Prev   = NULL;

			while( Priority_Inserted >= Insert->TaskStatus->TaskPriority)
				Insert = Insert->Next;

			if(Insert != NULL)
			{
				Prev = Insert->Prev;
				Prev->Next = Elem;
				Elem->Prev = Prev;
				Elem->Next = Insert;
				Insert->Prev = Elem;
				CurrQueue->No_Tasks++;
			}else{
				InsertQueueTail(CurrQueue,Elem);
			}
		}
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}

FUNC(Std_ReturnType,AUTOMATIC) InsertQueueFront(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											    P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Elem != NULL)
	{
		if (CurrQueue->No_Tasks == 0) {
			CurrQueue->Front = Elem;
			CurrQueue->Tail = Elem;
			CurrQueue->Front->Prev = NULL;
			CurrQueue->Front->Next = NULL;
			CurrQueue->No_Tasks++;
		} else
		{
			Elem->Next = CurrQueue->Front;
			Elem->Prev = NULL;
			CurrQueue->Front->Prev = Elem;
			CurrQueue->Front = Elem;
			CurrQueue->No_Tasks++;
		}
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}

FUNC(Std_ReturnType,AUTOMATIC) DequeQueueTail(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											  P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC)Deque)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Deque != NULL) {
		if (CurrQueue->No_Tasks == 0) {
			*Deque = NULL;
		} else {
			*Deque = CurrQueue->Tail;
			CurrQueue->Tail = CurrQueue->Tail->Prev;
			(*Deque)->Prev = NULL;
			(*Deque)->Next = NULL;
			if (CurrQueue->No_Tasks == 1)
				CurrQueue->Front = NULL;
			CurrQueue->No_Tasks--;
		}
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}

FUNC(Std_ReturnType,AUTOMATIC) DequeQueueElement(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											  P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Elem != NULL) {
		if(CurrQueue->No_Tasks > 0)
		{
			 if(Elem == CurrQueue->Front)
			 {
				 P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC) Deque = &Elem;
				 DequeQueueFront(CurrQueue, Deque);
			 }else if(Elem == CurrQueue->Tail)
			 {
				P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC) Deque = &Elem;
				DequeQueueTail(CurrQueue,Deque);
			 }else{
				 P2VAR(Task,AUTOMATIC,AUTOMATIC) Prev = Elem->Prev;
				 P2VAR(Task,AUTOMATIC,AUTOMATIC) Next = Elem->Next;
				 Elem->Next = NULL;
				 Elem->Prev = NULL;
				 if(Prev != NULL)
				 {
					 Prev->Next = Next;
				 }else{}
				 if(Next != NULL)
				 {
					 Next->Prev = Prev;
				 }
				 CurrQueue->No_Tasks--;
			 }
		}else{}
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}

FUNC(Std_ReturnType,AUTOMATIC) DequeQueueFront(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
												P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC)Deque)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Deque != NULL)
	{
		if (CurrQueue->No_Tasks == 0) {
			*Deque = NULL;
		} else {
			*Deque = CurrQueue->Front;

			CurrQueue->Front = CurrQueue->Front->Next;
			(*Deque)->Next = NULL;
			CurrQueue->Front->Prev = NULL;
			if(CurrQueue->No_Tasks == 1)
				CurrQueue->Tail = NULL;
			CurrQueue->No_Tasks--;

		}
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}
