#include "sx126x.h"

#include <nuttx/config.h>
#include <nuttx/arch.h>

#include <nuttx/spi/spi.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <debug.h>


/****************************************************************************
 * Private prototypes
 ****************************************************************************/
typedef FAR struct file		file_t;

static int     sx126x_open(file_t *filep);
static int     sx126x_close(file_t *filep);
static ssize_t sx126x_read(file_t *filep, FAR char *buffer, size_t buflen);
static ssize_t sx126x_write(file_t *filep, FAR const char *buf, size_t buflen);



/****************************************************************************
 * Private data types
 ****************************************************************************/

struct sx126x_dev_s {
	struct spi_dev_s *spi;
	struct sx126x_lower_s *lower;
};

/****************************************************************************
 * Globals
 ****************************************************************************/

const struct sx126x_dev_s g_sx126x_devices[SX126X_MAX_DEVICES];

static const struct file_operations sx126x_ops = {
	sx126x_open,		/* open */
	sx126x_close,		/* close */
	sx126x_read,		/* read */
	sx126x_write,		/* write */
	0,			/* seek */
	0,			/* ioctl */
};

/****************************************************************************
 * Private functions
 ****************************************************************************/

static int sx126x_open(file_t *filep)
{
	syslog(LOG_INFO, "Opening SX126x\n");



	return OK;
}

static int sx126x_close(file_t *filep)
{
	syslog(LOG_INFO, "Closing SX126x\n");
	return OK;
}

static ssize_t sx126x_read(file_t *filep, FAR char *buf, size_t buflen)
{

	if(buf == NULL || buflen < 1)
		/* Well... nothing to do */
		return -EINVAL;

	printf("Reading\n");

	return 1;
}

static ssize_t sx126x_write(file_t *filep, FAR const char *buf, size_t buflen)
{

	if(buf == NULL || buflen < 1)
		/* Well... nothing to do */
		return -EINVAL;


	printf("Trying to write\n");


	return 1;
}


/****************************************************************************
 * Public functions
 ****************************************************************************/

void sx126x_register(FAR struct spi_dev_s *spi,
                     FAR const struct sx126x_lower_s *lower)
{

	(void)register_driver("/dev/sx126x", &sx126x_ops, 0444, NULL);
}