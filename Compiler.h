/*
 * Compiler.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Seif pc
 */

#ifndef COMPILER_H_
#define COMPILER_H_

#define _ARMGCC_C_STM32F401CC

#define AUTOMATIC
#define TYPEDEF

#ifdef _ARMGCC_C_STM32F401CC
	#define INLINE			inline
	#define LOCAL_INLINE	static inline
	#define NAKED 			__attribute__((naked))
	#define PACKED			__attribute__((packed))
	#define NULL   			((void *)0)
	#define STATIC			static
	#define	__IO				volatile
#endif

#ifdef _ARMGCC_C_STM32F401CC

#define FUNC(rettype, memclass) 					rettype memclass
#define FUNC_P2CONST(rettype, ptrclass, memclass)  	const  ptrclass rettype * memclass
#define FUNC_P2VAR(rettype, ptrclass, memclass) 	ptrclass rettype * memclass

#define P2VAR(ptrtype, memclass, ptrclass)  		ptrclass ptrtype * memclass
#define P2CONST(ptrtype, memclass, ptrclass) 		const ptrclass ptrtype * memclass
#define CONSTP2VAR (ptrtype, memclass, ptrclass) 	ptrclass ptrtype * const memclass
#define CONSTP2CONST(ptrtype, memclass, ptrclass) 	const ptrclass ptrtype *const memclass
#define P2FUNC(rettype, ptrclass, fctname) 			ptrclass rettype (*fctname)
#define CONSTP2FUNC(rettype, ptrclass, fctname) 	ptrclass rettype (*const fctname)
#define CONST(type, memclass) 						memclass const type

#define VAR(vartype, memclass) 						memclass vartype

#endif

#endif /* COMPILER_H_ */
