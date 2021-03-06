/*--------------------------------------------------------------------
This source distribution is placed in the public domain by its author,
Jason Papadopoulos. You may use it for any purpose, free of charge,
without having to notify anyone. I disclaim any responsibility for any
errors.

Optionally, please be nice and tell me if you find this source to be
useful. Again optionally, if you add to the functionality present here
please consider making those additions public too, so that others may 
benefit from your work.	

$Id$
--------------------------------------------------------------------*/

#ifndef _MSIEVE_H_
#define _MSIEVE_H_

#ifdef __cplusplus
extern "C" {
#endif

	/* Lightweight factoring API */

#include <util.h>
#include <mp.h>


/* version info */

#define MSIEVE_MAJOR_VERSION 1
#define MSIEVE_MINOR_VERSION 46

enum msieve_factor_type {
	MSIEVE_COMPOSITE,
	MSIEVE_PRIME,
	MSIEVE_PROBABLE_PRIME
};

typedef struct msieve_factor {
	enum msieve_factor_type factor_type;
	char *number;
	struct msieve_factor *next;
} msieve_factor; 


typedef struct {
#if defined(WIN32) || defined(_WIN64)
	HANDLE file_handle;
	uint32 read_size;
	uint32 eof;
#else
	FILE *fp;
#endif 

	char *name;
	char *buf;
	uint32 buf_off;
} savefile_t; 

/* These flags are used to 'communicate' status and
   configuration info back and forth to a factorization
   in progress */

enum msieve_flags {
	MSIEVE_DEFAULT_FLAGS = 0,		/* just a placeholder */
	MSIEVE_FLAG_USE_LOGFILE = 0x01,	    /* append log info to a logfile */
	MSIEVE_FLAG_LOG_TO_STDOUT = 0x02,   /* print log info to the screen */
	MSIEVE_FLAG_STOP_SIEVING = 0x04,    /* tell library to stop sieving
					       when it is safe to do so */
	MSIEVE_FLAG_FACTORIZATION_DONE = 0x08,  /* set by the library if a
						   factorization completed */
	MSIEVE_FLAG_SIEVING_IN_PROGRESS = 0x10, /* set by the library when 
						   any sieving operations are 
						   in progress */
	MSIEVE_FLAG_SKIP_QS_CYCLES = 0x20,  /* do not perform exact tracking of
	                                    the number of cycles while sieving
					    is in progress; for distributed
					    sieving where exact progress info
					    is not needed, sieving clients can
					    save a lot of memory with this */
	MSIEVE_FLAG_NFS_POLY1 = 0x40,     /* if input is large enough, perform
	                                    stage 1 polynomial selection for NFS */
	MSIEVE_FLAG_NFS_POLY2 = 0x80,     /* if input is large enough, perform
	                                    stage 2 polynomial selection for NFS */
	MSIEVE_FLAG_NFS_SIEVE = 0x100,   /* if input is large enough, perform
	                                    sieving for NFS */
	MSIEVE_FLAG_NFS_FILTER = 0x200,  /* if input is large enough, perform
	                                    filtering phase for NFS */
	MSIEVE_FLAG_NFS_LA = 0x400,      /* if input is large enough, perform
	                                    linear algebra phase for NFS */
	MSIEVE_FLAG_NFS_SQRT = 0x800,    /* if input is large enough, perform
	                                    square root phase for NFS */
	MSIEVE_FLAG_NFS_LA_RESTART = 0x1000,/* restart the NFS linear algbra */
	MSIEVE_FLAG_DEEP_ECM = 0x2000    /* perform nontrivial-size ECM */
};
	


/* One factorization is represented by a msieve_obj
   structure. This contains all the static information
   that gets passed from one stage of the factorization
   to another. If this was C++ it would be a simple object */

typedef struct {
	char *input;		  /* pointer to string version of the 
				     integer to be factored */
	msieve_factor *factors;   /* linked list of factors found (in
				     ascending order */
	volatile uint32 flags;	  /* input/output flags */
	savefile_t savefile;      /* data for savefile */
	char *logfile_name;       /* name of the logfile that will be
				     used for this factorization */
	uint32 seed1, seed2;      /* current state of random number generator
				     (updated as random numbers are created) */
	char *nfs_fbfile_name;    /* name of factor base file */
	uint32 max_relations;      /* the number of relations that the sieving
	                              stage will try to find. The default (0)
				      is to keep sieving until all necessary 
				      relations are found. */
	uint64 nfs_lower;         /* lower bound for NFS-related thing to do */
	uint64 nfs_upper;         /* upper bound for NFS-related thing to do */

	uint32 cache_size1;       /* bytes in level 1 cache */
	uint32 cache_size2;       /* bytes in level 2 cache */
	enum cpu_type cpu;
	uint32 num_threads;

	uint32 mem_mb;            /* megabytes usable for NFS filtering */

	uint32 which_gpu;         /* ordinal ID of GPU to use */

	char mp_sprintf_buf[32 * MAX_MP_WORDS+1]; /* scratch space for 
						printing big integers */
} msieve_obj;

typedef struct {

} msieve_exported_relation_t;

typedef struct {
} msieve_exported_factor_base_t; 

typedef struct {
} msieve_exported_polys_t; 

typedef struct {
	msieve_exported_factor_base_t fb;
	msieve_exported_polys_t polys; 

} msieve_exported_relation_ctx_t; 


msieve_obj * msieve_obj_new(char *input_integer,
			    uint32 flags,
			    char *savefile_name,
			    char *logfile_name,
			    char *nfs_fbfile_name,
			    uint32 seed1,
			    uint32 seed2,
			    uint32 max_relations,
			    uint64 nfs_lower,
			    uint64 nfs_upper,
			    enum cpu_type cpu,
			    uint32 cache_size1,
			    uint32 cache_size2,
			    uint32 num_threads,
			    uint32 mem_mb,
			    uint32 which_gpu);

msieve_obj * msieve_obj_free(msieve_obj *obj);

void msieve_run(msieve_obj *obj);

/* Needed: new function to run until a certain number of relations are found */ 

/* Needed: extract relations from msieve object */ 

/* Needed: given a relation, given an msieve object, check if the relation is correct */ 
				

#ifdef __cplusplus
}
#endif

#endif /* _MSIEVE_H_ */
