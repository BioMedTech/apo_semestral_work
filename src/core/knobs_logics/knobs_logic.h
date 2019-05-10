#ifndef MZAPO_TEMPLATE_KNOBS_H
#define MZAPO_TEMPLATE_KNOBS_H

#include <stdio.h>
#include "../../mzapo_parlcd.h";
#include "../../mzapo_regs.h";

unsigned char * init_knobs();
void getKnobsValue(unsigned char *mem_base);

#endif //MZAPO_TEMPLATE_KNOBS_H