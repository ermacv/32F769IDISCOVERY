#include "stm32f7xx_ll_fmc.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_cortex.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_rcc_ex.h"

#include "ll_init.h"

void init_clock(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_7);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_7);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_PWR_EnableOverDriveMode();
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1);
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1);
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1);
  LL_PWR_EnableBkUpAccess();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 432, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1);
  while (LL_PWR_IsActiveFlag_VOS() == 0);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
  LL_SetSystemCoreClock(216000000);

  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_HSI, LL_RCC_MCO1_DIV_1);

   /* Set Timers Clock Prescalers */
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);

  LL_RCC_PLLSAI_ConfigDomain_SAI(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLSAIQ_DIV_3, LL_RCC_PLLSAIDIVQ_DIV_1);
  LL_RCC_PLLSAI_ConfigDomain_LTDC(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLSAIR_DIV_2, LL_RCC_PLLSAIDIVR_DIV_2);
  LL_RCC_PLLSAI_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLSAIP_DIV_4);
  LL_RCC_PLLI2S_ConfigDomain_SPDIFRX(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, RCC_PLLP_DIV2);
  LL_RCC_PLLI2S_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLLI2S_IsReady() != 1);
  LL_RCC_PLLSAI_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLLSAI_IsReady() != 1);
}
