/**
 *	@file	error.h
 *	@author	Michal Koval, FIT
 *	@date	14.3.2019 
 *	@brief	Protoypy funkcii v error.h
 *	@note	Compiler: gcc 7.3
 * 			Řešení IJC-DU1, příklad b)
 */		   	 

/**
 *	@fn	void warning_msg(const char *fmt, ...)
 *	@param	fmt	sprava na vypisanie
 *	@param	...	argumenty pre vypis
 *	@return	void
 * 	@brief	vypise chybovu spravu na standardny chybov vystup
 */
void warning_msg(const char *fmt, ...);

/**
 *	@fn void error_exit(const char *fmt, ...)
 *	@param	fmt	sprava na vypisanie
 *	@param	...	argumenty pre vypis
 * 	@return	void
 * 	@brief	vypise chybovu spravu na standardny chybov vystup a ukonci program s return kodom 1
 */
void error_exit(const char *fmt, ...);
