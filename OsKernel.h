/*
 * OsKernel.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Seif pc
 */

#ifndef OSKERNEL_H_
#define OSKERNEL_H_

#include "Std_Types.h"
#include "Task.h"
#include "RtosConfig.h"
#include "RTOS_svc_calls.h"

#define IDLE_TASK_PRIORITY 	0
#define IDLE_TASK_STACK		RTOSKernelStackSize

/** Systick Registers **/
#define STK_CTRL				*((__IO uint32 *)(0xE000E010UL))
#define STK_LOAD				*((__IO uint32 *)(0xE000E014UL))
#define STK_VAL					*((__IO uint32 *)(0xE000E018UL))
#define STK_CALIB				*((__IO uint32 *)(0xE000E01CUL))

/** Systick Masks **/
#define SYSTICK_TICK_INT		1U
#define SYSTICK_CNT_EN			0U
#define SYSTICK_AHB_PRE			2U
#define SYSTICK_PRI_OFFSET		28U
#define PENDSV_PRI_OFFSET		20U
#define PENDSV_PRI_LOWEST		0x0FU
#define SYSTICK_PRI_LOWEST		0x0EU
#define SYSTICK_PRI				*((__IO uint32 *)(0xE000ED20UL))

#define OsSchedulerStoped		0
#define OsSchedulerStarted		1

/**
 * @brief API to Initiliaze osKernel
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) OsKernelStart();
/**
 * @brief Startup Hook Called After Os is initiliazed
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) OsStartupHook();
/**
 * @brief Shutdown Hook Called when Os in Shut
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) OsShutDownHook();
/**
 * @brief API to Shutdown osKernel
 * @return
 */
FUNC(Std_ReturnType,AUTOMATIC) OsKernelShutdown();

#endif /* OSKERNEL_H_ */
