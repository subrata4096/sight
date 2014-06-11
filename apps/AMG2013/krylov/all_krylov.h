/*BHEADER**********************************************************************
 * Copyright (c) 2008,  Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * This file is part of HYPRE.  See file COPYRIGHT for details.
 *
 * HYPRE is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License (as published by the Free
 * Software Foundation) version 2.1 dated February 1999.
 *
 * $Revision: 2.4 $
 ***********************************************************************EHEADER*/




/******************************************************************************
 *
 * WARNING: The file krylov.h should NOT be edited by hand.
 * It is automatically generated by the script "headers", which should be run
 * frequently.  Almost every other hypre directory is set up this way, the
 * krylov directory is no different.
 *
 *****************************************************************************
 *
 * krylov solver headers
 *
 *****************************************************************************/

#ifndef HYPRE_ALL_KRYLOV_HEADER
#define HYPRE_ALL_KRYLOV_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
#ifndef max
#define max(a,b)  (((a)<(b)) ? (b) : (a))
#endif
*/

#define hypre_CTAllocF(type, count, funcs) \
( (type *)(*(funcs->CAlloc))\
((unsigned int)(count), (unsigned int)sizeof(type)) )

#define hypre_TFreeF( ptr, funcs ) \
( (*(funcs->Free))((char *)ptr), ptr = NULL )

/* A pointer to a type which is never defined, sort of works like void* ... */
#ifndef HYPRE_SOLVER_STRUCT
#define HYPRE_SOLVER_STRUCT
struct hypre_Solver_struct;
typedef struct hypre_Solver_struct *HYPRE_Solver;
/* similar pseudo-void* for Matrix and Vector: */
#endif
#ifndef HYPRE_MATRIX_STRUCT
#define HYPRE_MATRIX_STRUCT
struct hypre_Matrix_struct;
typedef struct hypre_Matrix_struct *HYPRE_Matrix;
#endif
#ifndef HYPRE_VECTOR_STRUCT
#define HYPRE_VECTOR_STRUCT
struct hypre_Vector_struct;
typedef struct hypre_Vector_struct *HYPRE_Vector;
#endif

typedef int (*HYPRE_PtrToSolverFcn)(HYPRE_Solver,
                                    HYPRE_Matrix,
                                    HYPRE_Vector,
                                    HYPRE_Vector);


#ifndef HYPRE_MODIFYPC
#define HYPRE_MODIFYPC
typedef int (*HYPRE_PtrToModifyPCFcn)(HYPRE_Solver,
                                      int,
                                      double);

#endif

