#include "knobs_logic.h"


unsigned char * init_knobs(){
    unsigned char *mem_base;
    mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
    // if (mem_base == NULL) exit(1);
    return mem_base;
}

Knobs_state getKnobsValue(unsigned char *mem_base){
    uint32_t rgb_knobs_value;
    rgb_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    int rk, gk, bk, rb, gb, bb;

    bk =  rgb_knobs_value      & 0xFF; // blue knob position
    gk = (rgb_knobs_value>>8)  & 0xFF; // green knob position
    rk = (rgb_knobs_value>>16) & 0xFF; // red knob position
    bb = (rgb_knobs_value>>24) & 1;    // blue button
    gb = (rgb_knobs_value>>25) & 1;    // green button
    rb = (rgb_knobs_value>>26) & 1;    // red buttom

    Knobs_state state = { rk,
                          gk,
                          bk,
                          rb,
                          gb,
                          bb };
                          
     return state;

}
