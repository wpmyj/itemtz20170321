#ifndef _SIM_H 
#define _SIM_H

		#include "stm32f10x.h"
		
		uint8 SimNumToTermId(uint8 sim_num[],uint8 term_id[]);
		uint8 SimSetSimNum(uint8 data[]);
		void SimTermIdToAsciiNum(uint8 term_id[],uint8 sim_num[]);
		uint8 SimNumToTermId23(uint8 sim_num[],uint8 term_id[]);
		void SimTermId23ToAsciiNum(uint8 term_id[],uint8 sim_num[]);

#endif
