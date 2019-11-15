#ifndef DSY_SDMMC_H
#define DSY_SDMMC_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "libdaisy.h"

// We might not need a pinconfig for this because  I think
// the six pins for the SDMMC are in fixed locations.

void dsy_sdmmc_init();
void dsy_sdmmc_init_fatfs();

#ifdef __cplusplus
}
#endif 
#endif // DSY_SDMMC_H