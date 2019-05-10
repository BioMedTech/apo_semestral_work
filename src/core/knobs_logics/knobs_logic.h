#ifndef MZAPO_TEMPLATE_KNOBS_H
#define MZAPO_TEMPLATE_KNOBS_H

#include <stdio.h>
#include "../../mzapo_parlcd.h";
#include "../../mzapo_regs.h";

typedef struct Knobs_state{
    int rk, gk, bk, rb, gb, bb;
} Knobs_state;


unsigned char * init_knobs();
Knobs_state getKnobsValue(unsigned char *mem_base);

#endif //MZAPO_TEMPLATE_KNOBS_H