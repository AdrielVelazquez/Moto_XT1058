/* Copyright (c) 2011-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __PM8XXX_BMS_H
#define __PM8XXX_BMS_H

#include <linux/errno.h>
#include <linux/mfd/pm8xxx/batterydata-lib.h>
#include <linux/types.h>

#define PM8921_BMS_DEV_NAME	"pm8921-bms"

struct pm8xxx_bms_core_data {
	unsigned int	batt_temp_channel;
	unsigned int	vbat_channel;
	unsigned int	ref625mv_channel;
	unsigned int	ref1p25v_channel;
	unsigned int	batt_id_channel;
};

/**
 * struct pm8921_bms_platform_data -
 * @batt_type:		allows to force chose battery calibration data
 * @r_sense_uohm:	sense resistor value in (micro Ohms)
 * @i_test:		current at which the unusable charger cutoff is to be
 *			calculated or the peak system current (mA)
 * @v_cutoff:		the loaded voltage at which the battery
 *			is considered empty(mV)
 * @enable_fcc_learning:	if set the driver will learn full charge
 *				capacity of the battery upon end of charge
 * @normal_voltage_calc_ms:	The period of soc calculation in ms when battery
 *				voltage higher than cutoff voltage
 * @low_voltage_calc_ms:	The period of soc calculation in ms when battery
 *				voltage is near cutoff voltage
 * @disable_flat_portion_ocv:	feature to disable ocv updates while in sleep
 * @ocv_dis_high_soc:		the high soc percent when ocv should be disabled
 * @ocv_dis_low_soc:		the low soc percent when ocv should be enabled
 * @get_batt_info:	a board specific function to return battery data If NULL
 *			default palladium data will be used to meter the battery
 */
struct pm8921_bms_platform_data {
	struct pm8xxx_bms_core_data	bms_cdata;
	enum battery_type		battery_type;
	int				r_sense_uohm;
	unsigned int			i_test;
	unsigned int			v_cutoff;
	unsigned int			max_voltage_uv;
	unsigned int			rconn_mohm;
	int				enable_fcc_learning;
	int				shutdown_soc_valid_limit;
	int				ignore_shutdown_soc;
	int				adjust_soc_low_threshold;
	int				chg_term_ua;
	int				normal_voltage_calc_ms;
	int				low_voltage_calc_ms;
	int				disable_flat_portion_ocv;
	int				ocv_dis_high_soc;
	int				ocv_dis_low_soc;
#ifdef CONFIG_PM8921_EXTENDED_INFO
	int64_t (*get_batt_info) (int64_t battery_id,
				  struct bms_battery_data *data);
#endif
};

#if defined(CONFIG_PM8921_BMS) || defined(CONFIG_PM8921_BMS_MODULE)
/**
 * pm8921_bms_get_vsense_avg - return the voltage across the sense
 *				resitor in microvolts
 * @result:	The pointer where the voltage will be updated. A -ve
 *		result means that the current is flowing in
 *		the battery - during battery charging
 *
 * RETURNS:	Error code if there was a problem reading vsense, Zero otherwise
 *		The result won't be updated in case of an error.
 *
 *
 */
int pm8921_bms_get_vsense_avg(int *result);

/**
 * pm8921_bms_get_battery_current - return the battery current based on vsense
 *				resitor in microamperes
 * @result:	The pointer where the voltage will be updated. A -ve
 *		result means that the current is flowing in
 *		the battery - during battery charging
 *
 * RETURNS:	Error code if there was a problem reading vsense, Zero otherwise
 *		The result won't be updated in case of an error.
 *
 */
int pm8921_bms_get_battery_current(int *result);

/**
 * pm8921_bms_get_percent_charge - returns the current battery charge in percent
 *
 */
int pm8921_bms_get_percent_charge(void);

/**
 * pm8921_bms_get_fcc - returns fcc in mAh of the battery depending on its age
 *			and temperature
 *
 */
int pm8921_bms_get_fcc(void);

/**
 * pm8921_bms_get_cc_uah - returns cc_uah in microampere_hour of
			    the battery
 *
 * @result:	The pointer where the cc_uah will be updated.
 *
 * RETURNS:	Error code if there was a problem reading, Zero otherwise
 *              The result won't be updated in case of an error.
 */
int pm8921_bms_get_cc_uah(int *result);

/**
 * pm8921_bms_get_aged_capacity - returns percentage of full battery capacity taking
 *					  aging into acccount
 *
 * @result:	The pointer where the percentage will be updated.
 *
 * RETURNS:	Error code if there was a problem reading, Zero otherwise
 *              The result won't be updated in case of an error.
 */
int pm8921_bms_get_aged_capacity(int *result);

/**
 * pm8921_bms_charging_began - function to notify the bms driver that charging
 *				has started. Used by the bms driver to keep
 *				track of chargecycles
 */
void pm8921_bms_charging_began(void);
/**
 * pm8921_bms_charging_end - function to notify the bms driver that charging
 *				has stopped. Used by the bms driver to keep
 *				track of chargecycles
 */
void pm8921_bms_charging_end(int is_battery_full);

void pm8921_bms_calibrate_hkadc(void);
/**
 * pm8921_bms_get_simultaneous_battery_voltage_and_current
 *		- function to take simultaneous vbat and vsense readings
 *		  this puts the bms in override mode but keeps coulumb couting
 *		  on. Useful when ir compensation needs to be implemented
 */
int pm8921_bms_get_simultaneous_battery_voltage_and_current(int *ibat_ua,
								int *vbat_uv);
/**
 * pm8921_bms_get_current_max
 *	- function to get the max current that can be drawn from
 *	  the battery before it dips below the min allowed voltage
 */
int pm8921_bms_get_current_max(void);
/**
 * pm8921_bms_invalidate_shutdown_soc - function to notify the bms driver that
 *					the battery was replaced between reboot
 *					and so it should not use the shutdown
 *					soc stored in a coincell backed register
 */
void pm8921_bms_invalidate_shutdown_soc(void);
#ifdef CONFIG_PM8921_FLOAT_CHARGE
/**
 * pm8921_bms_charging_full - function to notify the bms driver that charging
 *				is Full.
 */
void pm8921_bms_charging_full(void);
/**
 * pm8921_bms_no_external_accy - function to notify the bms driver that No Accy
 *				is attached.
 */
void pm8921_bms_no_external_accy(void);
#else
static inline void pm8921_bms_charging_full(void)
{
}
#endif
#ifdef CONFIG_PM8921_EXTENDED_INFO
/**
 * pm8921_bms_voltage_based_capacity - function toadjust meter offset
 */
void pm8921_bms_voltage_based_capacity(int batt_mvolt,
					int batt_mcurr,
					int batt_temp);
/**
 * pm8921_bms_calculate_chrg_fcc - function to calculate charge fcc
 */
void pm8921_bms_calculate_chrg_fcc(int ocv_mv,
				   int ocv_cc_uah,
				   int full_cc_uah,
				   int full_temp_c);
/**
 * pm8921_bms_get_chrg_ocv_time - function to get time of Charge OCV
 */
int pm8921_bms_get_chrg_ocv_time(void);

#endif
#ifdef CONFIG_PM8921_TEST_OVERRIDE
int pm8921_override_get_charge_status(int *status);
#endif

/**
 * pm8921_bms_cc_uah -	function to get the coulomb counter based charge. Note
 *			that the coulomb counter are reset when the current
 *			consumption is low (below 8mA for more than 5 minutes),
 *			This will lead in a very low coulomb counter charge
 *			value upon wakeup from sleep.
 */
int pm8921_bms_cc_uah(int *cc_uah);
#else
static inline int pm8921_bms_get_vsense_avg(int *result)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_battery_current(int *result)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_percent_charge(void)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_fcc(void)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_cc_uah(int *result)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_aged_capacity(int *result)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_cc_mas(int64_t *result)
{
	return -ENXIO;
}
static inline void pm8921_bms_charging_began(void)
{
}
static inline void pm8921_bms_charging_end(int is_battery_full)
{
}
static inline void pm8921_bms_calibrate_hkadc(void)
{
}
static inline int pm8921_bms_get_simultaneous_battery_voltage_and_current(
						int *ibat_ua, int *vbat_uv)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_rbatt(void)
{
	return -EINVAL;
}
static inline void pm8921_bms_invalidate_shutdown_soc(void)
{
}
static inline void pm8921_bms_no_external_accy(void)
{
}
static inline int pm8921_bms_cc_uah(int *cc_uah)
{
	return -ENXIO;
}
static inline int pm8921_bms_get_current_max(void)
{
	return -ENXIO;
}
#endif

#endif
