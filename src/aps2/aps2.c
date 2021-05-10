/*
 * config.c
 *
 * Created: 07/05/2021 22:51:32
 *  Author: RafaelCF4
 */ 

#include "aps2.h"

// extern volatile int ecgCnt = 0;
// 
// void TC0_Handler(void) {
//   volatile uint32_t ul_dummy;
// 
//   ul_dummy = tc_get_status(TC0, 0);
//   
//   int ecgSize = sizeof(ecg)/sizeof(ecg[0]);
//   if (ecgCnt >= ecgSize) {  ecgCnt=0; }
//   else { ecgCnt++; }
//   
//   dacc_write_conversion_data(DACC_BASE, ecg[ecgCnt], DACC_CHANNEL);
// }

static void config_DAC(void) {
  /* Enable clock for DACC */
  sysclk_enable_peripheral_clock(DACC_ID);

  /* Reset DACC registers */
  dacc_reset(DACC_BASE);

  /* Half word transfer mode */
  dacc_set_transfer_mode(DACC_BASE, 0);
  dacc_set_osr(DACC_BASE, DACC_CHANNEL, 5 );

  /* Enable output channel DACC_CHANNEL */
  dacc_enable_channel(DACC_BASE, DACC_CHANNEL);

  /* Set up analog current */
  dacc_set_analog_control(DACC_BASE, DACC_ANALOG_CONTROL);
}

void TC_init_modificado(Tc * TC, int ID_TC, int TC_CHANNEL, int freq){
  uint32_t ul_div;
  uint32_t ul_tcclks;
  uint32_t ul_sysclk = sysclk_get_cpu_hz();

  pmc_enable_periph_clk(ID_TC);
  tc_find_mck_divisor(freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
  tc_init(TC, TC_CHANNEL, TC_CMR_TCCLKS_TIMER_CLOCK2 | TC_CMR_CPCTRG);
  tc_write_rc(TC, TC_CHANNEL, (ul_sysclk / 16) / freq);

  NVIC_SetPriority(ID_TC, 4);
  NVIC_EnableIRQ((IRQn_Type) ID_TC);
  tc_enable_interrupt(TC, TC_CHANNEL, TC_IER_CPCS);

  tc_start(TC, TC_CHANNEL);
}