/*
 * MemManagment.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */

#ifndef MEM_H_
#define MEM_H_

#include "Std_Types.h"
#include "RTOS_svc_calls.h"

/*----- Header Size is 8 Bytes = 2^3 ----*/
#define HEADER_SHIFT	3
#define HEADER_SIZE		8
#define OSHeapSize		2048
#define OSheapOcc		(1<<30)
#define OSheapFree		(0<<30)

typedef struct header
{
	P2VAR(struct header,AUTOMATIC,AUTOMATIC) NextFree;
	VAR(uint32, AUTOMATIC)  Size;
}Header_T;

/**
 * @brief Thread Safe API to Allocate a memory block from the total available System Memory
 * @param NoBytes No of Bytes to be allocated from Heap
 * @brief API dynamically Allocates Memory For Task to use
 * @return void * Pointer to Memory Allocated
 */
FUNC_P2VAR(void, AUTOMATIC, AUTOMATIC) OsMalloc(VAR(uint32,AUTOMATIC) NoBytes);
/**
 * @brief Thread Safe API to Allocate a memory block from the total available System Memory
 * @param NoBytes amount of memory to be allocated from a certain Type
 * @param Type sizeof(type) to be provided
 * @return void * Pointer to Memory Allocated
 */
FUNC_P2VAR(void, AUTOMATIC, AUTOMATIC) OsCalloc(VAR(uint32,AUTOMATIC) NoBytes,VAR(uint32,AUTOMATIC) Type);

/**
 * @brief Thread Safe API to free a memory block allocated from the total Available System Memory
 * @param Free Pointer to The Memory Block to be Freed from the System
 * @return Std_ReturnType Returns E_OK for Successful free or E_NOT_OK for Unsuccessful Free Operation
 */
FUNC(Std_ReturnType, AUTOMATIC)   	   OsFree(P2VAR(void,AUTOMATIC,AUTOMATIC) Free);

#endif /* MEM_H_ */
