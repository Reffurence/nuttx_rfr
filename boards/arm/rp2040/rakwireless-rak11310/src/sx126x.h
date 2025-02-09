#ifndef _TESTTHING_H
#define _TESTTHING_H
#define TESTDRIVER1 124

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/spi/spi.h>
#include <nuttx/irq.h>
#include <nuttx/wireless/ioctl.h>

#include <stdint.h>
#include <stdbool.h>

/****************************************************************************
 * Settings
 ****************************************************************************/
#define SX126X_MAX_DEVICES 2


/****************************************************************************
 * Public Data Types
 ****************************************************************************/

struct sx126x_lower_s
{
    void (*reset)(void);
};

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

void sx126x_register(FAR struct spi_dev_s *spi,
                     FAR const struct sx126x_lower_s *lower);

#endif