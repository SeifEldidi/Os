/*
 * RTOS_svc_calls.c
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */
#include "RTOS_svc_calls.h"

extern   Task * CurrentTask ;
extern   Task * IdleTaskPtr ;
extern   uint8 HighestPriority ;
extern   Queue  OsReadyQueue[ConfigPriorities];
uint32	 KernelStack[RTOSKernelStackSize];
uint32 	*KernelStackPtr = &KernelStack[RTOSKernelStackSize];
extern uint8 KernelrStatus ;

void  OsSelectTask();

void NAKED SVC_Handler(void)
{
  __asm(
    ".global SVC_Handler_Main\n"
    "TST lr, #4\n"
    "ITE EQ\n"
    "MRSEQ r0, MSP\n"
    "MRSNE r0, PSP\n"
    "B SVC_Handler_Main\n"
  ) ;
  __asm volatile("BX LR");
}

void NAKED OSGetPrivlegde()
{
	__asm volatile("MRS R0,CONTROL");
	__asm volatile("AND R0,R0,#0xFFFFFFFC");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");
}

void NAKED OSReleasePrivlegde()
{
	__asm volatile("MOV R0,SP");
	__asm volatile("MSR PSP,R0");
	__asm volatile("MRS R0,CONTROL");
	__asm volatile("ORR R0,R0,#3");
	__asm volatile("MSR CONTROL,R0");
	__asm volatile("BX LR");
}


void NAKED SVC_Handler_Main( unsigned int *svc_args )
{
  unsigned int svc_number = 0;

  svc_number = ( ( char * )svc_args[ 6 ] )[ -2 ] ;
  switch( svc_number )
  {
    case 0:  /* EnablePrivilegedMode */
      CS_OFF;
      break;
    case 1:
    	CS_ON;
    	break;
    case 2:
    	ICSR = PENDSV_PENDING;
    	break;
    case 3:
    	OS_LOCK_SCHEDULER();
    	break;
    case 4:
    	OS_UNLOCK_SCHEDULER();
    	break;
    default:    /* unknown SVC */
      break;
  }
  __asm volatile("BX LR");
}

///Dispatcher Code
void NAKED PendSV_Handler(void)
{
	CS_ON;
	if(KernelrStatus != 1)
	{
		/*--------------Save Context of Current Task-------------*/
		__asm volatile("MRS R0 , PSP");
		__asm volatile("MOV SP , R0");
		__asm volatile("PUSH {R4-R11}");
		/*-------------Store SP new Value into TCB of old Task-----*/
		__asm volatile("LDR  R0, =CurrentTask");
		__asm volatile("LDR  R1, [R0]");
		__asm volatile("STR  SP, [R1,#16]");
		/*------------Switch to kernel Stack--------*/
		__asm volatile ("LDR R1,=KernelStackPtr");
		__asm volatile ("LDR SP,[R1]");
		/*-------------Select Next Task-------------*/
		__asm volatile ("PUSH {R0,LR}");
		/*-------------Check For StackOverflow/UnderFlow ----------*/
		__asm volatile ("BL OsSelectTask");
		__asm volatile ("POP  {R0,LR}");
		/*-------------Restore Context of Next Task-------*/
		__asm volatile("LDR  R0, =CurrentTask");
		__asm volatile("LDR  R1, [R0]");
		__asm volatile("LDR  SP, [R1,#16]");
		__asm volatile("POP {R4-R11}");
		__asm volatile ("MRS R0 , MSP");
		__asm volatile ("MSR PSP, R0");
	}else{
		__asm volatile("LDR  R0, =CurrentTask");
		__asm volatile("LDR  R1, [R0]");
		__asm volatile("LDR  SP, [R1,#16]");
		__asm volatile("POP {R4-R11}");
		__asm volatile ("MRS R0 , MSP");
		__asm volatile ("MSR PSP, R0");
	}
	//Set Current Task To Next Task
	CS_OFF;
	 __asm volatile("BX LR");
}

void  OsSelectTask()
{
	sint8 Priority = 0;
	for(Priority = ConfigPriorities -1; Priority > 0 ;Priority--)
	{
		if(OsReadyQueue[Priority].Front != NULL)
		{
			HighestPriority = Priority;
			DequeQueueFront(&OsReadyQueue[Priority] ,(Task **)&CurrentTask);
			CurrentTask->OwnerList = NULL;
			return ;
		}else{
		}
	}
	CurrentTask = IdleTaskPtr;
}

