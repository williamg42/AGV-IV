/*
 * motorcommand
 *
 *  Created on: Dec 31, 2015
 *      Author: william
 */

#ifndef SRC_MOTORCOMMAND_H_
#define SRC_MOTORCOMMAND_H_


#include "math.h"


typedef struct {
int leftspeed;
int rightspeed;
} motorspeeds;


motorspeeds motorcommand(float v, float dangle);

#endif /* SRC_MOTORCOMMAND_H_ */
