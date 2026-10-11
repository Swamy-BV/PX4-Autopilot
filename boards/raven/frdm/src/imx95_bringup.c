/****************************************************************************
 * boards/arm/imx95/imx95-evk/src/imx95_bringup.c
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

#include <nuttx/config.h>
#include <nuttx/fs/fs.h>
#include <errno.h>
#include "imx95-evk.h"
#ifdef CONFIG_RPTUN
# include <imx9_rptun.h>
#endif
#ifdef CONFIG_RPMSG_UART
# include <nuttx/serial/uart_rpmsg.h>

void rpmsg_serialinit(void)
{
	uart_rpmsg_init("netcore", "proxy", 1024, false);
	uart_rpmsg_init("netcore", "mavmin", 1024, false);
	uart_rpmsg_init("netcore", "ros", 1024, false);
	uart_rpmsg_init("netcore", "nsh", 1024, false);
}
#endif

int imx95_bringup(void)
{
#ifdef CONFIG_RPTUN
	int ret = imx9_rptun_init("imx9-shmem", "netcore");

	if (ret < 0) {
		return ret;
	}

#endif
	/* Parameters are volatile until persistent storage is validated. */
	int mount_ret = nx_mount(NULL, "/fs/ram", "tmpfs", 0, NULL);
	return mount_ret == -EBUSY ? 0 : mount_ret;
}
