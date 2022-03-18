/*
 * Copyright (c) 2020 George Gkinis
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <drivers/sensor.h>
#include <hx711.h>
#include <logging/log.h>
#include <pm/device.h>
#include <pm/pm.h>
#include <stddef.h>
#include <sys/reboot.h>
#include <sys/util.h>
#include <zephyr/types.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *hx711;

void set_rate(enum hx711_rate rate)
{
	static struct sensor_value rateVal;

	rateVal.val1 = rate;
	sensor_attr_set(hx711,
			SENSOR_CHAN_WEIGHT,
			SENSOR_ATTR_SAMPLING_FREQUENCY,
			&rateVal);
}

void measure(void)
{
	static struct sensor_value weight;
	int ret;

	ret = sensor_sample_fetch(hx711);
	if (ret != 0) {
		LOG_ERR("Cannot take measurement: %d  ", ret);
	} else {
		sensor_channel_get(hx711, SENSOR_CHAN_WEIGHT, &weight);
		LOG_INF("weight: %d.%06d", weight.val1, weight.val2);
	}
}

void main(void)
{
	hx711 = DEVICE_DT_GET(DT_ALIAS(hx711_sensor));

	__ASSERT(hx711 == NULL, "Failed to get device binding");

	LOG_INF("============= start =============");
	LOG_INF("device is %p, name is %s", hx711, hx711->name);

	LOG_INF("Calculating offset...");
	avia_hx711_tare(hx711, 5);

	struct sensor_value calibration_weight;

	sensor_attr_get(hx711,
			SENSOR_CHAN_WEIGHT,
			SENSOR_ATTR_CALIBRATION,
			&calibration_weight);

	LOG_INF("waiting for known weight of %d.%06d grams...",
		calibration_weight.val1,
		calibration_weight.val2);

	for (int i = 5; i >= 0; i--) {
		LOG_INF(" %d..", i);
		k_msleep(1000);
	}

	LOG_INF("Calculating slope...");
	avia_hx711_calibrate(hx711, 5);

	while (true) {
		k_msleep(1000);
		LOG_INF("== Test measure ==");
		LOG_INF("= Setting sampling rate to 10Hz.");
		set_rate(HX711_RATE_10HZ);
		measure();

		k_msleep(1000);
		LOG_INF("= Setting sampling rate to 80Hz.");
		set_rate(HX711_RATE_80HZ);
		measure();

		LOG_INF("====== Reboot in 5sec. =========");
		k_msleep(5000);
		//sys_reboot(SYS_REBOOT_COLD);
	}
}