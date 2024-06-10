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

#include "ll_init.h"

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

void init_sdram(void) {
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
  command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = 0;
  FMC_SDRAM_SendCommand(hsdram1.Instance, &command, 0);

  LL_Init1msTick(216000000);
  LL_mDelay(1);
  

  command.CommandMode = FMC_SDRAM_CMD_PALL;
  FMC_SDRAM_SendCommand(hsdram1.Instance, &command, 0);

  command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  command.AutoRefreshNumber = 8;
  FMC_SDRAM_SendCommand(hsdram1.Instance, &command, 0);

 static const uint32_t SDRAM_modeRegisterContent = SDRAM_MODEREG_BURST_LENGTH_1          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_2           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;  //[10:4]     Reserved

  command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  command.ModeRegisterDefinition = SDRAM_modeRegisterContent;
	command.AutoRefreshNumber = 1;
  FMC_SDRAM_SendCommand(hsdram1.Instance, &command, 0);

  FMC_SDRAM_ProgramRefreshRate(hsdram1.Instance, ((uint32_t)((1292)<< 1)));
}
