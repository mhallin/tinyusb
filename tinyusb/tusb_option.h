/*
 * tusb_option.h
 *
 *  Created on: Nov 26, 2012
 *      Author: hathach
 */

/*
 * Software License Agreement (BSD License)
 * Copyright (c) 2012, hathach (tinyusb.net)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the tinyUSB stack.
 */

/** \file
 *  \brief Configure File
 *
 *  \note TBD
 */

/** 
 *  \defgroup Group_TinyUSB_Configure Configuration tusb_option.h
 *  @{
 */

#ifndef _TUSB_TUSB_OPTION_H_
#define _TUSB_TUSB_OPTION_H_

#ifdef __cplusplus
 extern "C" {
#endif

/// define this symbol will make tinyusb look for external configure file
#include "tusb_config.h"

/// 0: no debug information 3: most debug information provided
#ifndef TUSB_CFG_DEBUG
  #define TUSB_CFG_DEBUG 3
  #warning TUSB_CFG_DEBUG is not defined, default value is 3
#endif

/// Enable Host Support
#ifdef TUSB_CFG_HOST

#ifndef TUSB_CFG_HOST_CONTROLLER_NUM
  #define TUSB_CFG_HOST_CONTROLLER_NUM 1
  #warning TUSB_CFG_HOST_CONTROLLER_NUM is not defined, default value is 1
#endif

#ifndef TUSB_CFG_HOST_DEVICE_MAX
  #define TUSB_CFG_HOST_DEVICE_MAX 1
  #warning TUSB_CFG_HOST_DEVICE_MAX is not defined, default value is 1
#endif

#endif

#ifndef TUSB_CFG_CONFIGURATION_MAX
  #define TUSB_CFG_CONFIGURATION_MAX 1
  #warning TUSB_CFG_CONFIGURATION_MAX is not defined, default value is 1
#endif

/// Enable Device Support
//#define TUSB_CFG_DEVICE

/// USB RAM Section Placement, MCU's usb controller often has limited access to specific RAM region. This will be used to declare internal variables as follow:
/// uint8_t tinyusb_data[10] TUSB_ATTR_RAM_SECTION;
/// if your mcu's usb controller has no such limit, define TUSB_ATTR_RAM_SECTION as empty macro.
#ifndef TUSB_ATTR_RAM_SECTION
 #error TUSB_ATTR_RAM_SECTION is not defined, needed to place data in accessible RAM for usb controller
#endif

#define DEVICE_CLASS_HID ( (defined TUSB_CFG_DEVICE_HID_KEYBOARD) || (defined TUSB_CFG_DEVICE_HID_MOUSE) )
#define HOST_EHCI

// TODO APP
#define USB_MAX_IF_NUM          8
#define USB_MAX_EP_NUM          5

#define USB_FS_MAX_BULK_PACKET  64
#define USB_HS_MAX_BULK_PACKET  USB_FS_MAX_BULK_PACKET /* Full speed device only */

// Control Endpoint
#define USB_MAX_PACKET0         64

/* HID In/Out Endpoint Address */
#define    HID_KEYBOARD_EP_IN       USB_ENDPOINT_IN(1)
//#define  HID_KEYBOARD_EP_OUT      USB_ENDPOINT_OUT(1)
#define    HID_MOUSE_EP_IN          USB_ENDPOINT_IN(4)

/* CDC Endpoint Address */
#define  CDC_NOTIFICATION_EP                USB_ENDPOINT_IN(2)
#define  CDC_DATA_EP_OUT                    USB_ENDPOINT_OUT(3)
#define  CDC_DATA_EP_IN                     USB_ENDPOINT_IN(3)

#define  CDC_NOTIFICATION_EP_MAXPACKETSIZE  8
#define  CDC_DATA_EP_MAXPACKET_SIZE         16

#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_TUSB_OPTION_H_ */

/** @} */