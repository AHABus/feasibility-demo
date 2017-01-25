/*!
 * @file        habcore.h
 * @brief       Main library include for HABcore
 * @author      Cesar Parent
 * @copyright   2016 Cesar Parent
 */
#pragma once
#include <habcore/types.h>
#include <habcore/utils.h>

/*!
 * @brief       Setup function called once when the controller starts.
 * @details     setup() is not implemented by the library: the implementation
 *              must be provided by the programme, and should contain any
 *              initialization code required for the programme to run.
 */
void setup(void);

/*!
 * @brief       Main programme loop.
 * @details     loop() is not implemented by the library: the implementation
 *              must be provided by the programme. The main loop is called
 *              forever by the main entry point.
 */
void loop(void);

