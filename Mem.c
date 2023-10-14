/*
 * MemManagment.C
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */
#include "Mem.h"

/**
 * Os Heap Memory Array Static stored in .BSS Section in RAM
 */
static VAR(uint32,AUTOMATIC) OSHEAPMemory[OSHeapSize];
static P2VAR(Header_T, AUTOMATIC, AUTOMATIC) HeapHead;
static VAR(uint32,AUTOMATIC)  BytesLeft;

/**
 * @brief Thread Safe API to Allocate a memory block from the total available System Memory
 * @param NoBytes No of Bytes to be allocated from Heap
 * @brief API dynamically Allocates Memory For Task to use
 * @return void * Pointer to Memory Allocated
 */
FUNC_P2VAR(void, AUTOMATIC, AUTOMATIC) OsMalloc(VAR(uint32,AUTOMATIC) NoBytes)
{
	OS_CS_EN();
	P2VAR(void, AUTOMATIC, AUTOMATIC)  HeaderBlock = NULL;
	if( NoBytes > 0)
	{
		// No of Bytes in respect to Header Size
		uint32 Nuints = ((NoBytes+HEADER_SIZE-1)>>HEADER_SHIFT) + 1;
		//init
		if(HeapHead == NULL)
		{
			HeapHead 	   = (Header_T *)&OSHEAPMemory[0];
			HeapHead->Size = ((P2VAR(char,AUTOMATIC,AUTOMATIC))(&OSHEAPMemory[OSHeapSize]) -
					(P2VAR(char,AUTOMATIC,AUTOMATIC))(&OSHEAPMemory[0]))>>HEADER_SHIFT;
			HeapHead->Size |= OSheapFree;
			HeapHead->NextFree = NULL;
			BytesLeft = HeapHead->Size;
		}else{}
		if(BytesLeft > Nuints)
		{
			// Dynamically Allocate Memory
			P2VAR(Header_T, AUTOMATIC, AUTOMATIC)  PreviousBlock = NULL;
			P2VAR(Header_T, AUTOMATIC, AUTOMATIC)  CurrBlock = HeapHead;
			/*-------Apply A first Fit Alogrithim To Look For Next Fit Block -----*/
			while(CurrBlock != NULL)
			{
				if(CurrBlock->Size >= Nuints)
				{
					if(CurrBlock->Size > Nuints)
					{
						CurrBlock->Size -= Nuints;
						CurrBlock+= CurrBlock->Size;
						CurrBlock->Size  = Nuints;
						CurrBlock->Size |= OSheapOcc;
						CurrBlock->NextFree = NULL;
						BytesLeft -= Nuints ;
					}else{
						//BytesLeft == Nuints
						if(PreviousBlock != NULL)
							PreviousBlock->NextFree = CurrBlock->NextFree;
						BytesLeft -= Nuints ;
					}
					// return The free memory 8 bytes after Header
					HeaderBlock = (P2VAR(void,AUTOMATIC,AUTOMATIC))((P2VAR(Header_T,AUTOMATIC,AUTOMATIC))CurrBlock+1);
					break;
				}else{
					PreviousBlock = CurrBlock;
					CurrBlock = CurrBlock->NextFree;
				}
			}
		}else{HeaderBlock = NULL;}
	}else{
		HeaderBlock = NULL;
	}
	OS_CS_DIS();
	return HeaderBlock;
}
/**
 * @brief Thread Safe API to Allocate a memory block from the total available System Memory
 * @param NoBytes amount of memory to be allocated from a certain Type
 * @param Type sizeof(type) to be provided
 * @return void * Pointer to Memory Allocated
 */
FUNC_P2VAR(void, AUTOMATIC, AUTOMATIC) OsCalloc(VAR(uint32,AUTOMATIC) NoBytes,VAR(uint32,AUTOMATIC) Type)
{
	OS_CS_EN();
	P2VAR(void, AUTOMATIC, AUTOMATIC)  HeaderBlock = NULL;
	uint32 Nuints = NoBytes*Type;
	HeaderBlock = OsMalloc(Nuints);
	OS_CS_DIS();
	return HeaderBlock;
}
/**
 * @brief Thread Safe API to free a memory block allocated from the total Available System Memory
 * @param Free Pointer to The Memory Block to be Freed from the System
 * @return Std_ReturnType Returns E_OK for Successful free or E_NOT_OK for Unsuccessful Free Operation
 */
FUNC(Std_ReturnType, AUTOMATIC)  OsFree(P2VAR(void,AUTOMATIC,AUTOMATIC) Free)
{
	OS_CS_EN();
	Std_ReturnType Ret = E_OK;
	if(Free != NULL)
	{
		P2VAR(Header_T, AUTOMATIC, AUTOMATIC)  PrevBlock 	= NULL;
		P2VAR(Header_T, AUTOMATIC, AUTOMATIC)  HeadBlock 	= HeapHead;
		P2VAR(Header_T, AUTOMATIC, AUTOMATIC)  CurrOccBlock = ((P2VAR(Header_T, AUTOMATIC, AUTOMATIC))Free - 1);

		if((((CurrOccBlock->Size) &( OSheapOcc) )>>30) == 1)
		{
			//Clear Size Element to free element
			CurrOccBlock->Size &= ~ (OSheapOcc);
			BytesLeft += CurrOccBlock->Size;
			//Look for block
			while(HeadBlock != NULL )
			{
				if(HeadBlock > CurrOccBlock )
					break;
				PrevBlock = HeadBlock;
				HeadBlock = HeadBlock->NextFree;
			}
			if(PrevBlock == NULL)
			{
				if(CurrOccBlock + CurrOccBlock->Size == HeadBlock)
				{
					CurrOccBlock->Size = CurrOccBlock->Size + HeadBlock->Size;
					HeadBlock = NULL;
					CurrOccBlock->NextFree = HeadBlock->NextFree;
					HeapHead = CurrOccBlock;
				}else{
					CurrOccBlock->NextFree = HeadBlock;
					HeapHead = CurrOccBlock;
				}
			}else
			{
				if(HeadBlock != NULL)
				{
					if(CurrOccBlock + CurrOccBlock->Size == HeadBlock)
					{
						CurrOccBlock->Size = CurrOccBlock->Size + HeadBlock->Size;
						CurrOccBlock->NextFree = HeadBlock->NextFree;
						HeadBlock = NULL;
						HeadBlock->Size = 0;
					}else{
						CurrOccBlock->NextFree = HeadBlock;
					}
					if(PrevBlock  + PrevBlock->Size == CurrOccBlock)
					{
						PrevBlock->Size += CurrOccBlock->Size;
						PrevBlock->NextFree = CurrOccBlock->NextFree;
						CurrOccBlock->Size = 0;
						CurrOccBlock->NextFree = NULL;
					}else{
						PrevBlock->NextFree = CurrOccBlock;
					}
				}else{
					if (PrevBlock + PrevBlock->Size == CurrOccBlock)
					{
						PrevBlock->Size += CurrOccBlock->Size;
						PrevBlock->NextFree = CurrOccBlock->NextFree;
						CurrOccBlock->Size = 0;
						CurrOccBlock->NextFree = NULL;
					} else {
						PrevBlock->NextFree = CurrOccBlock;
					}
				}
			}
		}else{
			Ret = E_NOT_OK;
		}
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}


