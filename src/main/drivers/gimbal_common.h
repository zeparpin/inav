/*
 * This file is part of INAV.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with INAV.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "platform.h"

#ifdef USE_SERIAL_GIMBAL

#include <stdint.h>

#include "config/feature.h"
#include "common/time.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GIMBAL_DEV_UNSUPPORTED = 0,
    GIMBAL_DEV_SERIAL,
    GIMBAL_DEV_UNKNOWN=0xFF
} gimbalDevType_e;


struct gimbalVTable_s;

typedef struct gimbalDevice_s {
    const struct gimbalVTable_s *vTable;
} gimbalDevice_t;

// {set,get}BandAndChannel: band and channel are 1 origin
// {set,get}PowerByIndex: 0 = Power OFF, 1 = device dependent
// {set,get}PitMode: 0 = OFF, 1 = ON

typedef struct gimbalVTable_s {
    void (*process)(gimbalDevice_t *gimbalDevice, timeUs_t currentTimeUs);
    gimbalDevType_e (*getDeviceType)(const gimbalDevice_t *gimbalDevice);
    bool (*isReady)(const gimbalDevice_t *gimbalDevice);
} gimbalVTable_t;


typedef struct gimbalConfig_s {
    uint8_t panChannel;
    uint8_t tiltChannel;
    uint8_t rollChannel;
    uint8_t sensitivity;
} gimbalConfig_t;

PG_DECLARE(gimbalConfig_t, gimbalConfig);

typedef enum {
    GIMBAL_MODE_PITCH_ROLL_LOCK = 0,
    GIMBAL_MODE_PITCH_LOCK = 1,
    GIMBAL_MODE_FOLLOW = 2
} gimbal_htk_mode_e;

#define GIMBAL_MODE_DEFAULT GIMBAL_MODE_FOLLOW

void gimbalCommonInit(void);
void gimbalCommonSetDevice(gimbalDevice_t *gimbalDevice);
gimbalDevice_t *gimbalCommonDevice(void);

// VTable functions
void gimbalCommonProcess(gimbalDevice_t *gimbalDevice, timeUs_t currentTimeUs);
gimbalDevType_e gimbalCommonGetDeviceType(gimbalDevice_t *gimbalDevice);
bool gimbalCommonIsReady(gimbalDevice_t *gimbalDevice);


void taskUpdateGimbal(timeUs_t currentTimeUs);

#ifdef __cplusplus
}
#endif

#endif