/*
 * Copyright (c) 2020 George Gkinis
 * Copyright (c) 2021 Jan Gnip
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_DRIVERS_SENSOR_HX711_H_
#define ZEPHYR_INCLUDE_DRIVERS_SENSOR_HX711_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <device.h>
#include <drivers/sensor.h>

/** @brief Sensor specific attributes of hx711. */
enum hx711_attribute {

	/**
	 * The sensor value returned will be altered by the amount indicated by
	 * slope: final_value = sensor_value * slope.
	 */
	SENSOR_ATTR_SLOPE = SENSOR_ATTR_PRIV_START,

	/**
	 * The sensor gain.
	 */
	SENSOR_ATTR_GAIN,
};

/** @brief Sensor specific channels of hx711. */
enum hx711_channel{

	/** Weight in grams */
	SENSOR_CHAN_WEIGHT = SENSOR_CHAN_PRIV_START,


};

 
enum hx711_gain {
	HX711_GAIN_128X = 1,
	HX711_GAIN_32X,
	HX711_GAIN_64X,
};

enum hx711_rate {
	HX711_RATE_10HZ,
	HX711_RATE_80HZ,
};

/**
 * @brief Zero the HX711.
 *
 * @param dev Pointer to the hx711 device structure
 * @param readings Number of readings to get average offset.
 *        5~10 readings should be enough, although more are allowed.
 * @retval The offset value
 *
 */
int avia_hx711_tare(const struct device *dev, uint8_t readings);

/**
 * @brief Callibrate the HX711.
 *
 * Given a target value of a known weight the slope gets calculated.
 * This is actually unit agnostic.
 * If the target weight is given in grams, lb, Kg or any other weight unit,
 * the slope will be calculated accordingly.
 *
 * @param dev Pointer to the hx711 device structure
 * @param target Target weight in grams.
 *        If target is represented in another unit (lb, oz, Kg) then the
 *        value returned by sensor_channel_get() will represent that unit.
 * @param readings Number of readings to take for calibration.
 *        5~10 readings should be enough, although more are allowed.
 * @retval The slope value
 *
 */
struct sensor_value avia_hx711_calibrate(const struct device *dev, uint8_t readings);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_DRIVERS_SENSOR_HX711_H_ */
