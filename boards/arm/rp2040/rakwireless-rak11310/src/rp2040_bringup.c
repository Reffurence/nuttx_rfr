/****************************************************************************
 * boards/arm/rp2040/raspberrypi-pico/src/rp2040_bringup.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <debug.h>
#include <stddef.h>

#include <nuttx/fs/fs.h>

#include <arch/board/board.h>

#include "rp2040_pico.h"

#ifdef CONFIG_ARCH_BOARD_COMMON
#include "rp2040_common_bringup.h"
#endif /* CONFIG_ARCH_BOARD_COMMON */

#ifdef CONFIG_USERLED
#  include <nuttx/leds/userled.h>
#endif

/* Custom driver */
#include "rp2040_spi.h"
#include <arch/board/rp2040_spidev.h>
#include <nuttx/spi/spi.h>
#include "sx126x.h"
#include "rp2040_gpio.h"


/****************************************************************************
 * Private prototypes
 ****************************************************************************/

void sx_reset(void);

/****************************************************************************
 * Private data
 ****************************************************************************/

struct sx126x_lower_s sx126x = {
  .reset=sx_reset
};


/****************************************************************************
 * Private Functions
 ****************************************************************************/

void sx_reset(void)
{
  rp2040_gpio_put(14, false);
  usleep(100);
  rp2040_gpio_put(14, true);
  usleep(100);  
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rp2040_bringup
 ****************************************************************************/

int rp2040_bringup(void)
{
#ifdef CONFIG_ARCH_BOARD_COMMON

  int ret = rp2040_common_bringup();
  if (ret < 0)
    {
      return ret;
    }

#endif /* CONFIG_ARCH_BOARD_COMMON */

  /* --- Place any board specific bringup code here --- */

#ifdef CONFIG_USERLED
  /* Register the LED driver */

  ret = userled_lower_initialize("/dev/userleds");
  if (ret < 0)
    {
      syslog(LOG_ERR, \
      "ERROR: userled_lower_initialize() failed: %d\n", ret);
    }
#endif

#ifdef CONFIG_INPUT_BUTTONS
  /* Register the BUTTON driver */

  ret = btn_lower_initialize("/dev/buttons");
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: btn_lower_initialize() failed: %d\n", ret);
    }
#endif
  
  rp2040_gpio_init(14);
  rp2040_gpio_setdir(14, true);



// 14.2 Circuit Configuration for Basic Tx Operation
// This chapter describes the sequence of operations needed to send or receive a frame starting from a power up.
// After power up (battery insertion or hard reset) the chip runs automatically a calibration procedure and goes to STDBY_RC
// mode. This is indicated by a low state on BUSY pin. From this state the steps are:
// 1. If not in STDBY_RC mode, then go to this mode with the command SetStandby(...)
// 2. Define the protocol (LoRa® or FSK) with the command SetPacketType(...)
// 3. Define the RF frequency with the command SetRfFrequency(...)
// 4. Define output power and ramping time with the command SetTxParams(...)
// 5. Define where the data payload will be stored with the command SetBufferBaseAddress(...)
// 6. Send the payload to the data buffer with the command WriteBuffer(...)
// 7. Define the modulation parameter according to the chosen protocol with the command SetModulationParams(...)
// 8. Define the frame format to be used with the command SetPacketParams(...)
// 9. Configure DIO and IRQ: use the command SetDioIrqParams(...) to select TxDone IRQ and map this IRQ to a DIO (DIO1,
// DIO2 or DIO3)
// 10. Define Sync Word value: use the command WriteReg(...) to write the value of the register via direct register access
// 11. Set the circuit in transmitter mode to start transmission with the command SetTx(). Use the parameter to enable
// Timeout
// 12. Wait for the IRQ TxDone or Timeout: once the packet has been sent the chip goes automatically to STDBY_RC mode
// 13. Clear the IRQ TxDone flag

  struct spi_dev_s *spi;
  spi=rp2040_spibus_initialize(1);

  sx126x_register(spi, &sx126x);

  return OK;
}
