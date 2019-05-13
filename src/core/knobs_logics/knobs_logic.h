#ifndef MZAPO_TEMPLATE_KNOBS_H
#define MZAPO_TEMPLATE_KNOBS_H

#include <stdio.h>
#include "../../mzapo_parlcd.h";
#include "../../mzapo_regs.h";

typedef struct Knobs_state{
    int rk, gk, bk, rb, gb, bb;
} Knobs_state;


void init_knobs();
Knobs_state getKnobsValue();
void setLedValues(uint32_t color);

#endif //MZAPO_TEMPLATE_KNOBS_H