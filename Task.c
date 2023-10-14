/*
 * Task.C
 *
 *  Created on: Oct 8, 2023
 *      Author: Seif pc
 */


#include "Task.h"

Queue OsReadyQueue[ConfigPriorities];
Queue OsSystemList;
Queue OsSystemWaitingList;
extern uint8 SchedulerStatus ;
extern uint8 KernelrStatus ;
extern Queue   OsKernelTrackTasksList ;
Task *CurrentTask = NULL;
uint8 HighestPriority = 0;
/**
 * @param TaskPriority 		To set the priority of the Created Task
 * @param TaskID	   		To set ID of the Created Task
 * @param TaskStackSize		To set the Stack Size of the created Task
 * @param TaskEntryAddress	To set the initial Value of the Program Counter to point to a certain piece of code stored in ROM
 * @param TaskHandle		Pointer to Created Task
 * @brief The Following API Creates a Task by using dynamic memory allocation from a predefined buffer through the configuration file of the RTOS
 * @return Std_ReturnType   Whether The The Task was successfully Created or Not
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskCreate(VAR(uint8,OS_TYPE) TaskPriority,VAR(uint8,OS_TYPE) TaskID,
										  VAR(uint32,OS_TYPE) TaskStackSize,
										  VAR(TaskCode,OS_TYPE) TaskEntryAddress,P2VAR(TaskHandle_t,AUTOMATIC,AUTOMATIC) TaskHandle,
										  P2VAR(void,AUTOMATIC,AUTOMATIC) Vparam)
{
	OS_CS_EN();
	VAR(Std_ReturnType,OS_TYPE) Ret  = E_OK;
	if(KernelrStatus != 1)
	{
		/*---------Allocate Memory for TASK -------*/
		VAR(Stackptr,AUTOMATIC) 		StackMemory = NULL;
		P2VAR(Task,AUTOMATIC,AUTOMATIC) CreatedTask = (P2VAR(Task,AUTOMATIC,AUTOMATIC) )OsMalloc(sizeof(Task));
		/*---------if Task Reached Limit Memory Return Err -------*/
		if(CreatedTask != NULL)
		{
			CreatedTask->Next = NULL;
			CreatedTask->Prev = NULL;

			StackMemory =  (VAR(Stackptr,AUTOMATIC) )OsMalloc(TaskStackSize*sizeof(sint32));
			/*----------Set up Stack Memory ------*/
			if(StackMemory != NULL)
			{
				#if OS_FLOATING_POINT == DIS
					/*----------Init Stack Pointer to point to block below registers-----*/
					CreatedTask->Stack.StackTop    = (VAR(sint32 *,AUTOMATIC)) (&StackMemory[TaskStackSize - OS_R4_OFFSET]);
					CreatedTask->Stack.StackBottom = StackMemory;
					/*--------Set T bit to 1------*/
					StackMemory[TaskStackSize - OS_XPSR_OFFSET] = OS_EPSR_TBIT_SET;
					/*-------Program Counter initialization---*/
					StackMemory[TaskStackSize - OS_PC_OFFSET]  = (VAR(uint32,OS_TYPE)) (TaskEntryAddress);
					/*--------Init R0->R12-----*/
					uint8 StackInitCounter = OS_LR_OFFSET ;
					for(; StackInitCounter<=OS_R4_OFFSET ; StackInitCounter++)
					{
						//According To APCS the First Function Paramter is passed to R0 of the CPU
						if(OS_R0_OFFSET == StackInitCounter)
						{
							StackMemory[TaskStackSize - StackInitCounter] = (VAR(uint32,OS_TYPE))Vparam;
						}else{
							StackMemory[TaskStackSize - StackInitCounter] = 0xDEADBEEF;
						}
					}
				#endif
					/*---------------Allocate Task Control Block --------*/
					CreatedTask->TaskEntryAddress = TaskEntryAddress;
					CreatedTask->TaskStatus = (P2VAR(TCB,AUTOMATIC,AUTOMATIC) )OsMalloc(1*sizeof(TCB));
					if(CreatedTask->TaskStatus != NULL)
					{
						CreatedTask->TaskStatus->TaskPriority = TaskPriority;
						CreatedTask->TaskStatus->TaskState    = ((VAR(uint8,OS_TYPE))(READY));
						CreatedTask->Timing.TaskExecTime = 0;
						CreatedTask->Timing.TaskSleepTime = 0;
						CreatedTask->OwnerList = &OsReadyQueue[CreatedTask->TaskStatus->TaskPriority];
						*TaskHandle = CreatedTask;
						/*------------- Add Task To Kernel Task Lists to Track All Tasks in System in Case of Shutdown-------*/
						InsertQueueTail(&OsKernelTrackTasksList , CreatedTask ,SYSTEM_QUEUE );
						/*------------- Add Task To Ready Queue but Check if a running Task is preempted only if Scheduler is Running-------*/
						InsertQueueTail(&OsReadyQueue[CreatedTask->TaskStatus->TaskPriority],CreatedTask , NORMAL_QUEUE);
						/*--------Find Highest Priority ----------*/
						if(HighestPriority < CreatedTask->TaskStatus->TaskPriority)
							HighestPriority = CreatedTask->TaskStatus->TaskPriority;
						else{}
						/*------------- Compare with Current Running Task ---------*/
						if(1 == SchedulerStatus)
						{
							if(CreatedTask->TaskStatus->TaskPriority > CurrentTask->TaskStatus->TaskPriority)
							{
								//Add Current List to Ready Queue
								CurrentTask->TaskStatus->TaskState = READY;
								InsertQueueTail(&OsReadyQueue[CurrentTask->TaskStatus->TaskPriority],CurrentTask,NORMAL_QUEUE );
								///Request A context Switch
								OS_DISPATCHER;
							}else{

							}
						}else{

						}
					}else{
						Ret = E_NOT_OK;
					}
			}else{
				Ret = E_NOT_OK;
			}
		}else{
			Ret = E_NOT_OK;
		}
	}
	OS_CS_DIS();
	return Ret;
}

/**
 *
 * @param 	TaskHandle  		Pointer to a certain Task
 * @brief 	The Following API Terminates the Execution of the Task Passed in the TaskHandle if NULL is Passed the Current Task is terminated
 * @return  Std_ReturnType	Whether The The Task was successfully terminated or Not
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskKill(P2VAR(Task,OS_TYPE,OS_TYPE) TaskHandle)
{
	Std_ReturnType Ret = E_OK;
	OS_CS_EN();
	if(1 == SchedulerStatus && KernelrStatus != 1 )
	{
		if(TaskHandle == NULL)
		{
			/*------Release All Resources Of Task*/
			CurrentTask->Next = NULL;
			CurrentTask->Prev = NULL;
			CurrentTask->OwnerList = NULL;
			/*----- Free Resources -------*/
			Ret |= OsFree((P2VAR(void,AUTOMATIC,AUTOMATIC))(CurrentTask->Stack.StackBottom));
			Ret |= OsFree((P2VAR(void,AUTOMATIC,AUTOMATIC))(CurrentTask->TaskStatus));
			Ret |= OsFree(((P2VAR(void,AUTOMATIC,AUTOMATIC) )CurrentTask));
			/*----- Request Context Switch -----*/
			OS_DISPATCHER;
		}else{
			//Remove from Current List
			DequeQueueElement(TaskHandle->OwnerList, TaskHandle,NORMAL_QUEUE );
			//Remove from System Queue
			DequeQueueElement(TaskHandle->OwnerList, TaskHandle,SYSTEM_QUEUE );
			/*------Release All Resources Of Task*/
			TaskHandle->Next = NULL;
			TaskHandle->Prev = NULL;
			TaskHandle->OwnerList = NULL;
			/*----- Free Resources -------*/
			Ret |= OsFree((P2VAR(void,AUTOMATIC,AUTOMATIC)) (TaskHandle->Stack.StackBottom));
			Ret |= OsFree((P2VAR(void,AUTOMATIC,AUTOMATIC)) (TaskHandle->TaskStatus));
			Ret |= OsFree(((P2VAR(void,AUTOMATIC,AUTOMATIC) ) TaskHandle));
		}
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
/**
 * @brief Dynamically modify priority of the Current Task if the Priority of the Current Task is higher than than
 * 		  the current Task priority context switch occurs and the current task is blocked
 * @param TaskHandle Pointer to The Selected Task in which priority is to be changed dynamically
 * @param Priority Priority to be set which should be less than the maximum priority in the System
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskSetPriority(P2VAR(Task,OS_TYPE,OS_TYPE) TaskHandle ,VAR(uint8,AUTOMATIC) Priority)
{
	Std_ReturnType Ret = E_OK;
	OS_CS_EN();
	if(1 == SchedulerStatus && KernelrStatus != 1)
	{
		if (TaskHandle == NULL) {
			TaskHandle->TaskStatus->TaskPriority = Priority;
		} else {

			if(Priority >= ConfigPriorities )
				Priority = ConfigPriorities -1;
			else{}

			if(Priority > CurrentTask->TaskStatus->TaskPriority)
			{
				//Add Current Task to Ready Queue
				HighestPriority = Priority;
				CurrentTask->TaskStatus->TaskState = READY;
				InsertQueueTail(&OsReadyQueue[CurrentTask->TaskStatus->TaskPriority],CurrentTask,NORMAL_QUEUE);
				//Remove Task from Ready of Previous to new Queue
				TaskHandle->TaskStatus->TaskState = RUNNING;
				DequeQueueElement(TaskHandle->OwnerList,TaskHandle,NORMAL_QUEUE);
				TaskHandle->TaskStatus->TaskPriority = Priority;
				InsertQueueTail(&OsReadyQueue[Priority],TaskHandle,NORMAL_QUEUE);
				//Request Context Switch won't Be activated until critcial Section is executed
				OS_DISPATCHER;
			}else{
				TaskHandle->TaskStatus->TaskPriority = Priority;
			}
		}
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
/**
 *
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskSchedule()
{
	Std_ReturnType Ret = E_OK;
	OS_CS_EN();
	if(1 == SchedulerStatus && KernelrStatus != 1)
	{
		OS_DISPATCHER;
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
/**
 *
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskLockScheduler()
{
	Std_ReturnType Ret = E_OK;
	OS_SCHEDULER_LOCK;
	if (1 == SchedulerStatus && KernelrStatus != 1) {
		SchedulerStatus = 0;
	} else {
		Ret = E_NOT_OK;
		OS_SCHEDULER_UNLOCK;
	}
	return Ret;
}
/**
 *
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskResumeScheduler()
{
	Std_ReturnType Ret = E_OK;
	if (0 == SchedulerStatus && KernelrStatus != 1) {
		SchedulerStatus = 1 ;
		OS_SCHEDULER_UNLOCK;
	} else {
		Ret = E_NOT_OK;
	}
	return Ret;
}
/**
 *
 * @param TaskHandle
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskSleep(P2VAR(Task,OS_TYPE,OS_TYPE) TaskHandle)
{
	Std_ReturnType Ret = E_OK;
	OS_CS_EN();
	if(SchedulerStatus == 1 && KernelrStatus != 1)
	{
		if(TaskHandle == NULL)
		{
			//Remove from Current List
			DequeQueueElement(CurrentTask->OwnerList, CurrentTask,NORMAL_QUEUE);

			//Set Task State to Blocked
			CurrentTask->OwnerList =(P2VAR(void,OS_TYPE,OS_TYPE)) &OsSystemList;
			CurrentTask->TaskStatus->TaskState = BLOCKED;
			//Add Task to List of system Tasks
			InsertQueueSorted(&OsSystemList,CurrentTask,NORMAL_QUEUE);
			//Request a context switch to Switch to another task
			OS_DISPATCHER;
		}else{
			//Remove from Current List
			DequeQueueElement(TaskHandle->OwnerList,TaskHandle,NORMAL_QUEUE);
			//Set Task State to Blocked
			TaskHandle->TaskStatus->TaskState = BLOCKED;
			TaskHandle->OwnerList =(P2VAR(void,OS_TYPE,OS_TYPE)) &OsSystemList;
			//Add Task to List of system Tasks
			InsertQueueSorted(&OsSystemList, CurrentTask,NORMAL_QUEUE);
		}
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
/**
 *
 * @param TaskHandle
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskWakeup(P2VAR(Task,OS_TYPE,OS_TYPE) TaskHandle)
{
	Std_ReturnType Ret = E_OK;
	OS_CS_EN();
	if(SchedulerStatus == 1 && KernelrStatus != 1)
	{
		if (TaskHandle == NULL) {
			Ret = E_NOT_OK;
		} else {
			//Remove from Current List
			if(TaskHandle->OwnerList != &OsSystemWaitingList)
			{
				DequeQueueElement(TaskHandle->OwnerList, TaskHandle,NORMAL_QUEUE);
				//Set State to Ready
				TaskHandle->OwnerList =(P2VAR(void,OS_TYPE,OS_TYPE)) &OsReadyQueue[TaskHandle->TaskStatus->TaskPriority];
				TaskHandle->TaskStatus->TaskState = READY;
				InsertQueueTail(&OsReadyQueue[TaskHandle->TaskStatus->TaskPriority], TaskHandle,NORMAL_QUEUE);
				//Request Context Switch if priority is Higher than Current Task
				if(TaskHandle->TaskStatus->TaskPriority > CurrentTask->TaskStatus->TaskPriority)
				{
					//Add Current Task to Ready List
					InsertQueueTail(&OsReadyQueue[CurrentTask->TaskStatus->TaskPriority], CurrentTask,NORMAL_QUEUE);
					CurrentTask->TaskStatus->TaskState = READY;
					TaskHandle->TaskStatus->TaskState = RUNNING;
					OS_DISPATCHER;
				}else{

				}
			}else{
				Ret = E_NOT_OK;
			}
		}
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
/**
 *
 * @param DelayTime
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) OsTaskDelay(VAR(uint32,OS_TYPE) DelayTime)
{
	VAR(Std_ReturnType , OS_CODE) Ret = E_OK;
	OS_CS_EN();
	if(SchedulerStatus == 1 && KernelrStatus != 1 )
	{
		if(DelayTime > 0)
		{
			//Task Executing Must be Blocked and Added to Blocking Queue
			CurrentTask->Timing.TaskSleepTime  = DelayTime;
			CurrentTask->TaskStatus->TaskState = BLOCKED;
			CurrentTask->OwnerList = (P2VAR(void,OS_TYPE,OS_TYPE)) &OsSystemWaitingList;
			//Add Task to List of system Tasks
			InsertQueueSorted(&OsSystemWaitingList, CurrentTask,NORMAL_QUEUE);
			//Request a context switch to Switch to another task
			OS_DISPATCHER;
		}else{
			//Add Task to Ready Queue
			InsertQueueTail(&OsReadyQueue[CurrentTask->TaskStatus->TaskPriority], CurrentTask,NORMAL_QUEUE);
			//Request Context Switch
			OS_DISPATCHER;
		}
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
 	return Ret;
}

/**
 * @brief Function To know the Current Task executing in the System
 * @param CurrentTask[o/p] Pointer to Pointer of Type Task to return CurrentTask
 * @return
 */
FUNC(Std_ReturnType,OS_CODE) INLINE  OsGetCurrentTask(P2VAR(TaskHandle_t,OS_TYPE,OS_TYPE) CurrentRunningTask)
{
	//Enter Critical Section To prevent Pre-emption by other Tasks
	OS_CS_EN();
	VAR(Std_ReturnType , OS_CODE) Ret = E_OK;
	if(CurrentRunningTask != NULL)
	{
		if(SchedulerStatus == 1 )
		{
			*CurrentRunningTask = CurrentTask;
		}else{
			*CurrentRunningTask = NULL;
		}
	}else{
		Ret = E_NOT_OK;
	}
	OS_CS_DIS();
	return Ret;
}
