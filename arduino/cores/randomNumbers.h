#ifndef randomNumbers_h
#define randomNumbers_h

/* to suport C code, provide C wrapper to call C++ code */
#ifndef __cplusplus

/**
	* if we compile in plain C,
	* redefine random function name as
	* random_c
	*/
#define  random      __random_c

#endif



/* provide these function as C function */

#ifdef __cplusplus
extern "C"
{
#endif

void randomSeed(unsigned long seed);

#ifdef __cplusplus
} /* extern "C" */
#endif



/**
	* to work around polymorphism, when __cplusplus defined
	* we need to declare extern c to c wrapper function
	*/
#ifndef __cplusplus

/* C */

/* no polymorphism in C */
long random(long max);

/* not part of standard */
long __random_c_with_range(long min, long max);

/* ! defined(__cplusplus) */

#else /* defined(__cplusplus) */

/* C++ */

/* provide polymorphism in C++ */
long random(long max);
long random(long min, long max);

/* provide c low level function */

extern "C" long __random_c(long max);
extern "C" long __random_c_with_range(long min, long max);

#endif /* defined(__cplusplus) */

#endif