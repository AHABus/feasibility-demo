/*!
 * @file        rtx.h
 * @brief       RTTY-based link layer functions.
 * @details     The RTTY radio link is controlled using digital logic levels.
 *              The pin used to control the radio transmitter should be linked
 *              to a voltage divider bridge to obtain the desired voltage offset
 *              between logical 1 and 0, in turn producing the right frequency
 *              offset.
 * @author      Cesar Parent
 * @copyright   2016 Cesar Parent
 */
// TODO: provide a way to select TxD pin.
// TODO: provide a way to choose number of stop bits.
#pragma once
#include <habcore/types.h>

/*!
 * @brief       Starts the RTTY transmission state machine.
 * @param       bps         RTTY link speed in Bauds.
 */
void rtx_start(uint16_t bps);

/*!
 * @brief       Stops the RTTY transmission state machine.
 */
void rtx_stop(void);

/*!
 * @brief       Returns whether the RTTY link is ready to accept another byte.
 * @return      Whether the RTTY link is ready to accept another byte.
 */
bool rtx_ready();

/*!
 * @brief       Transmit a byte of data through the RTTY link.
 * @details     The byte is added to the link's transmission buffer, and the
 *              function returns immediatly. The byte is sent once every byte
 *              before it have been transmitted.
 *              
 *              If the RTTY transmission buffer is full, rtx_write() will block
 *              until a byte is consumed and a new one can be added to the
 *              buffer.
 * @param       data        The byte to transmit.
 * @return      The number of bytes written (1, or 0 on failure).
 */
int8_t rtx_write(byte_t data);

/*!
 * @brief       Transmit a series of bytes of data through the RTTY link.
 * @details     If the RTTY transmission buffer is full, rtx_write_bytes()
 *              will block until the entire series of bytes can be written.
 * @param       data        A pointer to an array of bytes to send.
 * @param       length      The number of bytes to send.
 * @return      The number of bytes successfully transmitted.
 */
index_t rtx_write_bytes(const byte_t* data, index_t length);
