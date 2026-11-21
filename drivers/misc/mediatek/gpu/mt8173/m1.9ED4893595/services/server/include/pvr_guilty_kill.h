/*
 * Called when the firmware reports that a context caused a GPU lockup. Kills the owning process
 * if it is an app (uid >= 10000): a faulting app context otherwise keeps the GPU in recovery
 * and stalls every other GPU client, the compositor included.
 */
#ifndef PVR_GUILTY_KILL_H
#define PVR_GUILTY_KILL_H

#include "img_types.h"

void PVRKillGuiltyApp(IMG_PID pid, IMG_UINT32 reason);

#endif
