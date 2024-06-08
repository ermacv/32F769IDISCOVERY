#include "stm32f7xx_ll_fmc.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_hal_cortex.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_sdram.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_rcc_ex.h"

static void init_sdram_pins(GPIO_TypeDef *GPIOx, uint32_t Pins) {
  for (uint8_t i = 0; i < 32; i++) {
    if (Pins & (1 << i)) {
      uint32_t pin = (1 << i);
      LL_GPIO_SetPinMode(GPIOx, pin, LL_GPIO_MODE_ALTERNATE | LL_GPIO_OUTPUT_PUSHPULL);
      LL_GPIO_SetPinPull(GPIOx, pin, LL_GPIO_PULL_NO);
      LL_GPIO_SetPinSpeed(GPIOx, pin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
      if (pin > 7) {
        LL_GPIO_SetAFPin_8_15(GPIOx, pin, LL_GPIO_AF_12);
      } else {
        LL_GPIO_SetAFPin_0_7(GPIOx, pin, LL_GPIO_AF_12);
      }
    }
  }
}

static void HAL_FMC_MspInit(void){
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();

  init_sdram_pins(GPIOE, GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_11|GPIO_PIN_14|GPIO_PIN_7|GPIO_PIN_10
                          |GPIO_PIN_12|GPIO_PIN_15|GPIO_PIN_13);
  init_sdram_pins(GPIOG, GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_1|GPIO_PIN_2
                          |GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_4);
  init_sdram_pins(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_15|GPIO_PIN_10
                          |GPIO_PIN_14|GPIO_PIN_9|GPIO_PIN_8);
  init_sdram_pins(GPIOI, GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_5
                          |GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_6|GPIO_PIN_1
                          |GPIO_PIN_9|GPIO_PIN_0);
  init_sdram_pins(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_15
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_11);
  init_sdram_pins(GPIOH, GPIO_PIN_15|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_5
                          |GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_9
                          |GPIO_PIN_11|GPIO_PIN_8|GPIO_PIN_10);
}

static void MX_FMC_Init(void)
{
  SDRAM_HandleTypeDef hsdram1;
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  
  /* SdramTiming */
  FMC_SDRAM_TimingTypeDef SdramTiming = {
    .LoadToActiveDelay = 2,
    .ExitSelfRefreshDelay = 7,
    .SelfRefreshTime = 4,
    .RowCycleDelay = 7,
    .WriteRecoveryTime = 3,
    .RPDelay = 2,
    .RCDDelay = 2,
  };

  HAL_FMC_MspInit();
  (void)FMC_SDRAM_Init(hsdram1.Instance, &(hsdram1.Init));
  (void)FMC_SDRAM_Timing_Init(hsdram1.Instance, &SdramTiming, hsdram1.Init.SDBank);
}

void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_7);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_7)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_PWR_EnableOverDriveMode();
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_LSI_Enable();

   /* Wait till LSI is ready */
  while(LL_RCC_LSI_IsReady() != 1)
  {

  }
  LL_PWR_EnableBkUpAccess();
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 432, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_SetSystemCoreClock(216000000);

  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_HSI, LL_RCC_MCO1_DIV_1);

   /* Set Timers Clock Prescalers */
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  LL_RCC_PLLSAI_ConfigDomain_SAI(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLSAIQ_DIV_3, LL_RCC_PLLSAIDIVQ_DIV_1);
  LL_RCC_PLLSAI_ConfigDomain_LTDC(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLSAIR_DIV_2, LL_RCC_PLLSAIDIVR_DIV_2);
  LL_RCC_PLLSAI_ConfigDomain_48M(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, LL_RCC_PLLSAIP_DIV_4);
  LL_RCC_PLLI2S_ConfigDomain_SPDIFRX(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_25, 192, RCC_PLLP_DIV2);
  LL_RCC_PLLI2S_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLLI2S_IsReady() != 1)
  {

  }
  LL_RCC_PLLSAI_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLLSAI_IsReady() != 1)
  {

  }
}

void low_level_init(void) {
#if (ART_ACCELERATOR_ENABLE != 0)
  __HAL_FLASH_ART_ENABLE();
#endif /* ART_ACCELERATOR_ENABLE */

  /* Configure Flash prefetch */
#if (PREFETCH_ENABLE != 0U)
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

  SystemClock_Config();
  // PeriphCommonClock_Config();
  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOJ);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOK);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH);
  
  LL_RCC_EnableRTC();

  MX_FMC_Init();
}