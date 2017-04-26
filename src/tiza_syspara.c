#define SYS_PARA_STRUCT_GLOBAL

#include "tiza_include.h"

void SysPara_Init(void)
{
	sysm_on_off_struct.canrx_switch 				= SYSM_OFF;
	sysm_on_off_struct.GPRSPacketTx_switch	= SYSM_OFF;	
	sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_OFF;
	
	sysm_on_off_struct.GPRSPacketTx_switch	= SYSM_ON;	
	sysm_on_off_struct.GPRSPeriodTx_switch 	= SYSM_ON;
}
