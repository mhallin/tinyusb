/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "hal/include/hal_gpio.h"
#include "pico/stdlib.h"
#include "bsp/board.h"
#include "tusb.h"
#include "device/usbd_pvt.h"
#include "XInputPad.h"

uint8_t endpoint_in=0;
uint8_t endpoint_out=0;

bool btn_last=0;

#define PIN_IN_UP 26
#define PIN_IN_RIGHT 27
#define PIN_IN_DOWN 28
#define PIN_IN_LEFT 29
#define PIN_IN_Y 7
#define PIN_IN_B 6
#define PIN_IN_X 5
#define PIN_IN_A 4

static void sendReportData(void) {

  // Poll every 1ms
    const uint32_t interval_ms = 1;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms) return;  // not enough time
    start_ms += interval_ms;

    // Remote wakeup
    if (tud_suspended()) {
      // Wake up host if we are in suspend mode
      // and REMOTE_WAKEUP feature is enabled by host
      tud_remote_wakeup();
    }

    // bool dpad_up = !gpio_get_pin_level(PIN_PA02);
    // bool dpad_right = !gpio_get_pin_level(PIN_PB08);
    // bool dpad_down = !gpio_get_pin_level(PIN_PB09);
    // bool dpad_left = !gpio_get_pin_level(PIN_PA04);

    // bool btn_y = !gpio_get_pin_level(PIN_PA05);
    // bool btn_b = !gpio_get_pin_level(PIN_PB02);
    // bool btn_a = !gpio_get_pin_level(PIN_PB11);
    // bool btn_x = !gpio_get_pin_level(PIN_PB10);

    bool dpad_up = !gpio_get(PIN_IN_UP);
    bool dpad_right = !gpio_get(PIN_IN_RIGHT);
    bool dpad_down = !gpio_get(PIN_IN_DOWN);
    bool dpad_left = !gpio_get(PIN_IN_LEFT);
    bool btn_y = !gpio_get(PIN_IN_Y);
    bool btn_b = !gpio_get(PIN_IN_B);
    bool btn_a = !gpio_get(PIN_IN_X);
    bool btn_x = !gpio_get(PIN_IN_A);

    bool btn = dpad_up || dpad_right || dpad_down || dpad_left || btn_y || btn_b || btn_a || btn_x;

    uint8_t btn_1 = (uint8_t)(
      (dpad_up ? 0x01 : 0x00)
      | (dpad_down ? 0x02 : 0x00)
      | (dpad_left ? 0x04 : 0x00)
      | (dpad_right ? 0x08 : 0x00));

    uint8_t btn_2 = (uint8_t)(
      (btn_a ? 0x10 : 0x00)
      | (btn_b ? 0x20 : 0x00)
      | (btn_x ? 0x40 : 0x00)
      | (btn_y ? 0x80 : 0x00));

    bool change =
      XboxButtonData.digital_buttons_1 != btn_1
      || XboxButtonData.digital_buttons_2 != btn_2;

    XboxButtonData.digital_buttons_1 = btn_1;
    XboxButtonData.digital_buttons_2 = btn_2;
    if (btn && !btn_last) {
      board_led_write(1);
    } else if (!btn && btn_last) {
      board_led_write(0);
    }
    btn_last = btn;

    XboxButtonData.rsize = 20;
    if (change && (tud_ready()) && ((endpoint_in != 0)) && (!usbd_edpt_busy(0, endpoint_in))){
      usbd_edpt_claim(0, endpoint_in);
      usbd_edpt_xfer(0, endpoint_in, (uint8_t*)&XboxButtonData, 20);
      usbd_edpt_release(0, endpoint_in);
    }
}

int main(void) {
  board_init();

  uint8_t pins[] = {
    PIN_IN_UP,
    PIN_IN_RIGHT,
    PIN_IN_DOWN,
    PIN_IN_LEFT,
    PIN_IN_Y,
    PIN_IN_B,
    PIN_IN_X,
    PIN_IN_A,
  };
  int pin_count = sizeof(pins)/sizeof(*pins);

  for (int i = 0; i < pin_count; ++i) {
    uint8_t pin = pins[i];
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
  }

  // board_led_on();

  gpio_init(18);
  gpio_set_dir(18, GPIO_OUT);
  gpio_put(18, 1);

  gpio_init(19);
  gpio_set_dir(19, GPIO_OUT);
  gpio_put(19, 1);

  gpio_init(20);
  gpio_set_dir(20, GPIO_OUT);
  gpio_put(20, 1);

  // gpio_set_pin_direction(PIN_PA02, GPIO_DIRECTION_IN);
  // gpio_set_pin_direction(PIN_PB08, GPIO_DIRECTION_IN);
  // gpio_set_pin_direction(PIN_PB09, GPIO_DIRECTION_IN);
  // gpio_set_pin_direction(PIN_PA04, GPIO_DIRECTION_IN);
  // gpio_set_pin_direction(PIN_PA05, GPIO_DIRECTION_IN);
  // gpio_set_pin_direction(PIN_PB02, GPIO_DIRECTION_IN);
  // gpio_set_pin_direction(PIN_PB11, GPIO_DIRECTION_IN);
  // gpio_set_pin_direction(PIN_PB10, GPIO_DIRECTION_IN);

  // gpio_set_pin_pull_mode(PIN_PA02, GPIO_PULL_UP);
  // gpio_set_pin_pull_mode(PIN_PB08, GPIO_PULL_UP);
  // gpio_set_pin_pull_mode(PIN_PB09, GPIO_PULL_UP);
  // gpio_set_pin_pull_mode(PIN_PA04, GPIO_PULL_UP);
  // gpio_set_pin_pull_mode(PIN_PA05, GPIO_PULL_UP);
  // gpio_set_pin_pull_mode(PIN_PB02, GPIO_PULL_UP);
  // gpio_set_pin_pull_mode(PIN_PB11, GPIO_PULL_UP);
  // gpio_set_pin_pull_mode(PIN_PB10, GPIO_PULL_UP);

  tusb_init();
  while (1) {
    sendReportData();
    tud_task();  // tinyusb device task
  }
  return 0;
}


static void xinput_init(void) {

}

static void xinput_reset(uint8_t __unused rhport) {

}

static uint16_t xinput_open(uint8_t __unused rhport, tusb_desc_interface_t const *itf_desc, uint16_t max_len) {
  //+16 is for the unknown descriptor
  uint16_t const drv_len = (uint16_t)(sizeof(tusb_desc_interface_t) + itf_desc->bNumEndpoints*sizeof(tusb_desc_endpoint_t) + 16);
  TU_VERIFY(max_len >= drv_len, 0);

  uint8_t const * p_desc = tu_desc_next(itf_desc);
  uint8_t found_endpoints = 0;
  while ( (found_endpoints < itf_desc->bNumEndpoints) && (drv_len <= max_len)  )
  {
    tusb_desc_endpoint_t const * desc_ep = (tusb_desc_endpoint_t const *) p_desc;
    if ( TUSB_DESC_ENDPOINT == tu_desc_type(desc_ep) )
    {
      TU_ASSERT(usbd_edpt_open(rhport, desc_ep));

      if ( tu_edpt_dir(desc_ep->bEndpointAddress) == TUSB_DIR_IN )
      {
        endpoint_in = desc_ep->bEndpointAddress;
      }else
      {
        endpoint_out = desc_ep->bEndpointAddress;
      }
      found_endpoints += 1;
    }
    p_desc = tu_desc_next(p_desc);
  }
  return drv_len;
}

// static bool xinput_device_control_request(uint8_t __unused rhport, tusb_control_request_t const __unused *request) {
//   return true;
// }

// static bool xinput_control_complete_cb(uint8_t __unused rhport, tusb_control_request_t __unused const *request) {
//     return true;
// }
//callback after xfer_transfer
static bool xinput_xfer_cb(uint8_t __unused rhport, uint8_t __unused ep_addr, xfer_result_t __unused result, uint32_t __unused xferred_bytes) {
  return true;
}


static usbd_class_driver_t const xinput_driver ={
  #if CFG_TUSB_DEBUG >= 2
    .name = "XINPUT",
#endif
    .init             = xinput_init,
    .reset            = xinput_reset,
    .open             = xinput_open,
    .xfer_cb          = xinput_xfer_cb,
    .sof              = NULL
};

// Implement callback to add our custom driver
usbd_class_driver_t const *usbd_app_driver_get_cb(uint8_t *driver_count) {
    *driver_count = 1;
    return &xinput_driver;
}