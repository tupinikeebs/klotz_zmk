#pragma once

#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TPS43 I2C address */
#define TPS43_I2C_ADDR              0x74

/* TPS43 controller register definitions with IQS572 - Based on the IQS5xx-B000 datasheet */
/* https://www.azoteq.com/images/stories/pdf/iqs5xx-b000_trackpad_datasheet.pdf */

/* Gesture events */
// Read-only
#define TPS43_REG_GESTURE_EVENTS_0  0x000D  /* 1 byte */
#define TPS43_REG_GESTURE_EVENTS_1  0x000E  /* 1 byte */

/* System information */
// Read-only
#define TPS43_REG_SYSTEM_INFO_0     0x000F  /* 1 byte - Status flags */
#define TPS43_REG_SYSTEM_INFO_1     0x0010  /* 1 byte - Status flags */


/* System control and configuration */
// Read-write
#define TPS43_REG_SYSTEM_CONTROL_0  0x0431  /* 1 byte - Main control (ACK_RESET, AUTO_ATI) */
#define TPS43_REG_SYSTEM_CONTROL_1  0x0432  /* 1 byte - Event mode control */
// Read-write
#define TPS43_REG_ATI_TARGET        0x056D  /* 2 bytes - ATI target value (gain); higher = more sensitive (overlay/glass compensation) */
#define TPS43_REG_SYSTEM_CONFIG_0   0x058E  /* 1 byte - System configuration */
#define TPS43_REG_SYSTEM_CONFIG_1   0x058F  /* 1 byte - Additional configuration */

/* Touch data - main coordinates */
// Read-only
#define TPS43_REG_NUM_FINGERS       0x0011  /* 1 byte */
#define TPS43_REG_REL_X             0x0012  /* 2 bytes - Relative movement along X */
#define TPS43_REG_REL_Y             0x0014  /* 2 bytes - Relative movement along Y */
#define TPS43_REG_ABS_X             0x0016  /* 2 bytes - Absolute position X */
#define TPS43_REG_ABS_Y             0x0018  /* 2 bytes - Absolute position Y */
#define TPS43_REG_TOUCH_STRENGTH    0x001A  /* 2 bytes */
#define TPS43_REG_TOUCH_AREA        0x001C  /* 1 byte */

/* XY configuration */
// Read-write
#define TPS43_REG_XY_CONFIG_0       0x0669  /* 1 byte */

/* Gesture configuration */
// Read-write // Low-level gesture configuration // probably should be removed
#define TPS43_REG_SINGLE_FINGER_GESTURES 0x06B7  /* 1 byte */
#define TPS43_REG_MULTI_FINGER_GESTURES  0x06B8  /* 1 byte */
#define TPS43_REG_TAP_TIME               0x06B9  /* 2 bytes, ms */
#define TPS43_REG_HOLD_TIME              0x06BD  /* 2 bytes, ms - press&hold triggers after TapTime+HoldTime */
#define TPS43_REG_SCROLL_INIT_DIST       0x06C8  /* 2 bytes, px - scroll initiation threshold (lower = more responsive) */


/* Maximum number of simultaneous touches (see 5.3). If the sensor sees more,
 * the TOO_MANY_FINGERS flag is set and XY data is zeroed out. Low by default
 * (~2), so for 3-finger gestures it needs to be raised to >=3. */
#define TPS43_REG_MAX_MULTITOUCH    0x066A  /* 1 byte */

/* Filter settings */
// Read-write
#define TPS43_REG_FILTER_SETTINGS   0x0632  /* 1 byte */


/* ============================================================ */
/* System Information 0 (0x000F) - Status Flags - 8 bits        */
/* ============================================================ */

#define TPS43_SHOW_RESET            BIT(0)      /* Check flag upon system reset */

/* ============================================================ */
/* System Information 1 (0x0010) - Status Flags - 8 bits        */
/* ============================================================ */

#define TPS43_SWITCH_STATE          BIT(5)      /* SW_IN pin state */
#define TPS43_SNAP_TOGGLE           BIT(4)      /* Snap channel toggle */
#define TPS43_RR_MISSED             BIT(3)      /* Report rate missed */
#define TPS43_TOO_MANY_FINGERS      BIT(2)      /* Maximum touch point count exceeded */
#define TPS43_PALM_DETECT           BIT(1)      /* Palm detected */
#define TPS43_TP_MOVEMENT           BIT(0)      /* Trackpad movement */


/* ============================================================ */
/* System Control 0 (0x0431) - 8 bits                           */
/* ============================================================ */

#define TPS43_ACK_RESET             BIT(7)      /* Acknowledge reset */
#define TPS43_AUTO_ATI              BIT(5)      /* Run ATI algorithm (re-tune gain) */

/* ============================================================ */
/* System Control 1 (0x0432) - 8 bits                           */
/* ============================================================ */

#define TPS43_RESET                 BIT(1)
#define TPS43_SUSPEND               BIT(0)      /* Sleep mode */


/* ============================================================ */
/* System Configuration 0 (0x058E) - 8 bits                     */
/* ============================================================ */

#define TPS43_SETUP_COMPLETE        BIT(6)      /* Setup complete flag */
#define TPS43_WDT_ENABLE            BIT(5)      /* Enable watchdog timer */


/* ============================================================ */
/* System Configuration 1 (0x058F) - 8 bits                     */
/* ============================================================ */

#define TPS43_EVENT_MODE            BIT(0)
#define TPS43_GESTURE_EVENT         BIT(1)
#define TPS43_TP_EVENT              BIT(2)
#define TPS43_TOUCH_EVENT           BIT(6)


/* ============================================================ */
/* XY Configuration 0 (0x0669) - 8 bits                         */
/* ============================================================ */

#define TPS43_FLIP_X                BIT(0)      /* Flip X axis */
#define TPS43_FLIP_Y                BIT(1)      /* Flip Y axis */
#define TPS43_SWITCH_XY_AXIS        BIT(2)      /* Swap X and Y axes */


/* ============================================================ */
/* Gesture Events 0 (0x000D) - 8 bits                           */
/* ============================================================ */

#define TPS43_SINGLE_TAP            BIT(0)      /* Single-finger tap detected */
#define TPS43_PRESS_AND_HOLD        BIT(1)      /* Press and hold detected */
#define TPS43_SWIPE_UP              BIT(2)      /* Single-finger swipe up */
#define TPS43_SWIPE_DOWN            BIT(3)      /* Single-finger swipe down */
#define TPS43_SWIPE_LEFT            BIT(4)      /* Single-finger swipe left */
#define TPS43_SWIPE_RIGHT           BIT(5)      /* Single-finger swipe right */

/* ============================================================ */
/* Gesture Events 1 (0x000E) - 8 bits                           */
/* ============================================================ */

#define TPS43_TWO_FINGER_TAP        BIT(0)      /* Two-finger tap */
#define TPS43_SCROLL                BIT(1)      /* Scroll up */
#define TPS43_ZOOM                  BIT(2)      /* Zoom gesture */

/* ============================================================ */
/* Filter Settings 0 (0x0632) - 8 bits                          */
/* ============================================================ */

#define TPS43_IIR_FILTER            BIT(0)      /* IIR filter */
#define TPS43_MAV_FILTER            BIT(1)      /* MAV filter */
#define TPS43_IIR_SELECT            BIT(2)      /* IIR select */
#define TPS43_ALP_COUNT_FILTER      BIT(3)      /* ALP count filter */


/* End of communication window - MUST be called after every read (writing to an invalid address 0xEEEE causes a NACK) */
#define TPS43_REG_END_COMM_WINDOW   0xEEEE

struct tps43_config {
    struct i2c_dt_spec i2c_bus;
    struct gpio_dt_spec rdy_gpio;
    struct gpio_dt_spec rst_gpio;

    bool single_tap;
    bool press_and_hold;
    /* Press-and-hold trigger delay = TapTime + hold_time_ms (datasheet 6.2).
     * Lower hold_time_ms = drag engages sooner. Written to reg 0x06BD when
     * press_and_hold is enabled. */
    int16_t hold_time_ms;
    bool two_finger_tap;
    bool scroll;
    bool swipes;          
    bool invert_x;
    bool invert_y;
    bool switch_xy;
    bool invert_scroll_x;
    bool invert_scroll_y;

    /* Tap-then-hold drag ("double-tap-drag"): after a single tap, a fresh touch
     * that lands within tap_drag_window_ms grabs (holds the left button) and
     * drags until all fingers lift. Faster/more reliable than the chip's native
     * press-and-hold, which requires holding a finger still for the hold timer. */
    bool tap_drag;
    int16_t tap_drag_window_ms;

    /* Three-finger middle-button drag: while >=3 fingers touch, hold the middle
     * mouse button and move the cursor (a quick 3-finger tap = middle click; a
     * 3-finger hold+move = middle-drag, e.g. rotate/pan in a 3D slicer). */
    bool three_finger_drag;
    /* Max simultaneous touches written to reg 0x066A. 0 = leave chip default.
     * Forced to >=3 when three_finger_drag is enabled. */
    uint8_t max_multitouches;

    int16_t sensitivity;
    int16_t scroll_sensitivity;
    /* Distance (px) the two fingers must travel before scroll starts (reg
     * 0x06C8). Lower = scroll engages on slighter movement. 0 = chip default. */
    int16_t scroll_init_distance;
    /* ATI target (reg 0x056D): touch-detection gain the chip auto-tunes to.
     * Higher = more sensitive, compensates for a glass/overlay. 0 = chip default.
     * Writing it re-runs AUTO_ATI so the new gain takes effect. */
    int16_t ati_target;

    bool enable_power_management;

    uint8_t filter_settings;
};

struct tps43_drv_data {
    const struct device *dev;
    struct k_sem lock;
    struct gpio_callback rdy_cb;
    struct k_work work;

    bool device_ready;
    bool initialized;
    bool scroll_active;
    bool drag_active;
    bool suspended;          

    /* Tap-then-hold drag state */
    int64_t last_tap_time;      /* uptime (ms) of the last single tap; 0 = none */
    uint8_t prev_num_fingers;   /* finger count from the previous work cycle */

    /* Three-finger middle-drag state: true while the middle button is held. */
    bool three_drag_active;

    /* Smooth-scroll accumulators: carry the fractional wheel remainder between
     * events (value is rel*scroll_sensitivity, /100 to whole lines) so low
     * sensitivities scroll evenly instead of in bursts. Reset when not scrolling. */
    int32_t scroll_accum_x;
    int32_t scroll_accum_y;
};

int tps43_set_sleep(const struct device *dev, bool sleep);

#ifdef __cplusplus
}
#endif
