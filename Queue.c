/*
 * Queue.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */
#include "Queue.h"

FUNC(Std_ReturnType,AUTOMATIC) InsertQueueTail(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											   P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem ,VAR(uint8,AUTOMATIC) Flag)
{
	Std_ReturnType Ret = E_OK;
	if(CurrQueue != NULL && Elem != NULL)
	{
		if(CurrQueue->No_Tasks == 0)
		{
			CurrQueue->Front = Elem;
			CurrQueue->Tail  = Elem;
			if(Flag == NORMAL_QUEUE )
			{
				CurrQueue->Front->Prev = NULL;
				CurrQueue->Front->Next = NULL;
			}else{
				CurrQueue->Front->SystemNext = NULL;
				CurrQueue->Front->SystemPrev = NULL;
			}
			CurrQueue->No_Tasks++;
		}else{
			if(Flag == NORMAL_QUEUE)
			{
				CurrQueue->Tail->Next = Elem;
				Elem->Prev = CurrQueue->Tail;
				CurrQueue->Tail = CurrQueue->Tail->Next;
			}else{
				CurrQueue->Tail->SystemNext = Elem;
				Elem->SystemPrev = CurrQueue->Tail;
				CurrQueue->Tail = CurrQueue->Tail->SystemNext;
			}
			CurrQueue->No_Tasks++;
		}
	}else{
		Ret = E_NOT_OK;
	}
	return Ret;
}

FUNC(Std_ReturnType,AUTOMATIC) InsertQueueSorted(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											    	P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem
													,VAR(uint8,AUTOMATIC) Flag)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Elem != NULL) {
		if (CurrQueue->No_Tasks == 0) {
			CurrQueue->Front = Elem;
			CurrQueue->Tail = Elem;
			if(Flag == NORMAL_QUEUE )
			{
				CurrQueue->Front->Prev = NULL;
				CurrQueue->Front->Next = NULL;
			}else{
				CurrQueue->Front->SystemPrev = NULL;
				CurrQueue->Front->SystemNext = NULL;
			}
			CurrQueue->No_Tasks++;
		} else {
			VAR(uint8 , AUTOMATIC) 			 Priority_Inserted = Elem->TaskStatus->TaskPriority;
			P2VAR(Task,AUTOMATIC,AUTOMATIC)  Insert = CurrQueue->Front;
			P2VAR(Task,AUTOMATIC,AUTOMATIC)  Prev   = NULL;

			while( (Insert != NULL )&&( Priority_Inserted >= Insert->TaskStatus->TaskPriority))
			{
				if(Flag == NORMAL_QUEUE)
				{
					Insert = Insert->Next;
				}else{
					Insert = Insert->SystemNext;
				}
			}
			if(Insert != NULL)
			{
				if(Flag == NORMAL_QUEUE)
				{
					Prev = Insert->Prev;
					Prev->Next = Elem;
					Elem->Prev = Prev;
					Elem->Next = Insert;
					Insert->Prev = Elem;
				}else{
					Prev = Insert->SystemPrev;
					Prev->SystemNext = Elem;
					Elem->SystemPrev = Prev;
					Elem->SystemNext = Insert;
					Insert->SystemPrev = Elem;
				}
				CurrQueue->No_Tasks++;
			}else{
				InsertQueueTail(CurrQueue,Elem,Flag);
			}
		}
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}

FUNC(Std_ReturnType,AUTOMATIC) InsertQueueFront(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											    P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem
												,VAR(uint8,AUTOMATIC) Flag)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Elem != NULL)
	{
		if (CurrQueue->No_Tasks == 0) {
			CurrQueue->Front = Elem;
			CurrQueue->Tail = Elem;
			if(Flag == NORMAL_QUEUE)
			{
				CurrQueue->Front->Prev = NULL;
				CurrQueue->Front->Next = NULL;
			}else{
				CurrQueue->Front->SystemPrev = NULL;
				CurrQueue->Front->SystemNext = NULL;
			}
			CurrQueue->No_Tasks++;
		} else
		{
			if(Flag == NORMAL_QUEUE)
			{
				Elem->Next = CurrQueue->Front;
				Elem->Prev = NULL;
				CurrQueue->Front->Prev = Elem;
			}else{
				Elem->SystemNext = CurrQueue->Front;
				Elem->SystemPrev = NULL;
				CurrQueue->Front->SystemPrev = Elem;
			}
			CurrQueue->Front = Elem;
			CurrQueue->No_Tasks++;
		}
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}

FUNC(Std_ReturnType,AUTOMATIC) DequeQueueTail(P2VAR(Queue,AUTOMATIC,AUTOMATIC) CurrQueue ,
											  P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC)Deque
											  ,VAR(uint8,AUTOMATIC) Flag)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Deque != NULL) {
		if (CurrQueue->No_Tasks == 0) {
			*Deque = NULL;
		} else {
			*Deque = CurrQueue->Tail;
			if(Flag == NORMAL_QUEUE)
			{
				CurrQueue->Tail = CurrQueue->Tail->Prev;
				(*Deque)->Prev = NULL;
				(*Deque)->Next = NULL;
			}else{
				CurrQueue->Tail = CurrQueue->Tail->SystemPrev;
				(*Deque)->SystemPrev = NULL;
				(*Deque)->SystemNext = NULL;
			}
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
											  P2VAR(Task,AUTOMATIC,AUTOMATIC)Elem
											  ,VAR(uint8,AUTOMATIC) Flag)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Elem != NULL) {
		if(CurrQueue->No_Tasks > 0)
		{
			 if(Elem == CurrQueue->Front)
			 {
				 P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC) Deque = &Elem;
				 DequeQueueFront(CurrQueue, Deque,Flag);
			 }else if(Elem == CurrQueue->Tail)
			 {
				P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC) Deque = &Elem;
				DequeQueueTail(CurrQueue,Deque,Flag);
			 }else{
				 if(Flag == NORMAL_QUEUE)
				 {
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
				 }else{
					P2VAR(Task,AUTOMATIC,AUTOMATIC) Prev = Elem->SystemPrev;
					P2VAR(Task,AUTOMATIC,AUTOMATIC) Next = Elem->SystemNext;
					Elem->SystemNext = NULL;
					Elem->SystemPrev = NULL;
					if (Prev != NULL) {
						Prev->SystemNext = Next;
					} else {
					}
					if (Next != NULL) {
						Next->SystemPrev = Prev;
					}
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
												P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC)Deque
												,VAR(uint8,AUTOMATIC) Flag)
{
	Std_ReturnType Ret = E_OK;
	if (CurrQueue != NULL && Deque != NULL)
	{
		if (CurrQueue->No_Tasks == 0) {
			*Deque = NULL;
		} else {
			*Deque = CurrQueue->Front;
			if(Flag == NORMAL_QUEUE)
			{
				CurrQueue->Front = CurrQueue->Front->Next;
				(*Deque)->Next = NULL;
				CurrQueue->Front->Prev = NULL;
			}else{
				CurrQueue->Front = CurrQueue->Front->SystemNext;
				(*Deque)->SystemNext = NULL;
				CurrQueue->Front->SystemPrev = NULL;
			}
			if(CurrQueue->No_Tasks == 1)
				CurrQueue->Tail = NULL;
			CurrQueue->No_Tasks--;

		}
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}
