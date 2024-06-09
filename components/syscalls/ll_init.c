#include "stm32f7xx_ll_fmc.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_cortex.h"
#include "stm32f7xx_hal_cortex.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_sdram.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_rcc_ex.h"

#define SDRAM_TIMEOUT     ((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)
#define SDRAM_BANK_ADDR ((uint32_t)0xC0000000)

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

static void MX_FMC_Init(void)
{
  LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_FMC);
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
  SDRAM_HandleTypeDef hsdram1;
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  
  /* SdramTiming */
  FMC_SDRAM_TimingTypeDef SdramTiming = {
    .LoadToActiveDelay = 2,
    .ExitSelfRefreshDelay = 6,
    .SelfRefreshTime = 4,
    .RowCycleDelay = 6,
    .WriteRecoveryTime = 2,
    .RPDelay = 2,
    .RCDDelay = 2,
  };

  (void)FMC_SDRAM_Init(hsdram1.Instance, &(hsdram1.Init));
  (void)FMC_SDRAM_Timing_Init(hsdram1.Instance, &SdramTiming, hsdram1.Init.SDBank);

  FMC_SDRAM_CommandTypeDef command = {};
  HAL_StatusTypeDef status;
  command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = 0;
  status = FMC_SDRAM_SendCommand(hsdram1.Instance, &command, 0);

  LL_Init1msTick(216000000);
  LL_mDelay(100);

  command.CommandMode = FMC_SDRAM_CMD_PALL;
  status = FMC_SDRAM_SendCommand(hsdram1.Instance, &command, 0);

  command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  command.AutoRefreshNumber = 8;
  status = FMC_SDRAM_SendCommand(hsdram1.Instance, &command, 0);

 static const uint32_t SDRAM_modeRegisterContent = SDRAM_MODEREG_BURST_LENGTH_1          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_2           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;  //[10:4]     Reserved

  command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  command.ModeRegisterDefinition = SDRAM_modeRegisterContent;
	command.AutoRefreshNumber = 1;
  status = FMC_SDRAM_SendCommand(hsdram1.Instance, &command, 0);

  FMC_SDRAM_ProgramRefreshRate(hsdram1.Instance, ((uint32_t)((1292)<< 1)));
}

void SystemClock_Config(void)
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
  while(LL_RCC_PLLI2S_IsReady() != 1);
  LL_RCC_PLLSAI_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLLSAI_IsReady() != 1);
}

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as WB for SDRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = SDRAM_BANK_ADDR;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes FMC control registers */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xA0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  
  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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

  MPU_Config();
  SCB_EnableICache();
  SCB_EnableDCache();

  MX_FMC_Init();
}