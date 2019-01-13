/*
 * ExpressionCalib.h
 *
 *  Created on: Jan. 13, 2019
 *      Author: blackaddr
 */
#include "BAPhysicalControls.h"

#ifndef EXPRESSIONCALIB_H_
#define EXPRESSIONCALIB_H_

void expressionCalib(int &min, int &max, bool &swap, BALibrary::BAPhysicalControls &controls, int switchHandle, int potHandle)
{
    if (Serial) { Serial.println("Put the expression pedal to HEEL down and push SW0"); }
    // Wait for button push
    while (!controls.isSwitchToggled(switchHandle)) {}

    // Store the calibration low value
    min = controls.getPotRawValue(potHandle);
    if (Serial) { Serial.println(String("MIN value is ") + min); }

    if (Serial) { Serial.println("Put the expression pedal to TOE down and push SW0"); }
    // Wait for button push
    while (!controls.isSwitchToggled(switchHandle)) {}

    // Store the calibration low value
    max = controls.getPotRawValue(potHandle);
    if (Serial) { Serial.println(String("MAX value is ") + max); }

    if (max < min) {
        // swap
        int tmp = max;
        max = min;
        min = tmp;
        swap = true;
    } else {
        swap = false;
    }

}



#endif /* EXPRESSIONCALIB_H_ */
