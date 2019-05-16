/**
 * @file UdpListenerTypes.h
 * @brief Utility types shared between dialog and app.
 */

#ifndef UDP_LISTENER_TYPES_H
#define UDP_LISTENER_TYPES_H

/*****************************************************************************
 * Types
 *****************************************************************************/

/** 
 * @brief An iterable enumeration of TDT units supported.
 *
 * This enum is intended to be lightweight and promises to be iterable in for-
 * loops, e.g.
 * @code
 *     for (int unit = 0; unit < TDT_UNIT_NUM_UNITS; ++unit) {
 *         eTdtUnit eUnit = (eTdtUnit)unit;
 *         // Do something with eUnit.
 *     }
 * @endcode
 *
 * Note also that it is suitable for array indices.
 */
enum eTdtUnit {
	TDT_UNIT_RZ = 0,      /**< The RZ. */
	TDT_UNIT_NUM_UNITS    /**< The number of units supported. */
};

/**
 * @brief An iterable enumeration of colors and their status significance.
 *
 * Actual color is implemented by the dialog. The meaning of the color is
 * encoded by the enum.
 */
enum eStatusColor {
	STATUS_COLOR_STOPPED = 0,  /**< The app is inactive. */
	STATUS_COLOR_LISTENING,    /**< The app is listening but hasn't received. */
	STATUS_COLOR_RECEIVING,    /**< The app is listening and has received data. */
	STATUS_COLOR_ERROR,        /**< There is an error.*/
	STATUS_COLOR_NUM_COLORS    /**< The number of status colors. */
};

#endif // UDP_LISTENER_TYPES_H
