#include "movementcalculator.h"

movementCalculator::movementCalculator()
{

}

/* PUBLIC FUNCTIONS */
void        movementCalculator::setStartCoordinates(int xstart, int ystart, int zstart) {

    /* sets the start coordinates of the movement, which should be calculated
     *
     * the unit of the input variables is in millimeters. for easier calculation they are
     * transformed into the unit meter
     *
     * this array contains the values in the order
     *
     * [0]  x-coordinate
     * [1]  y-coordinate
     * [2]  z-coordinate
     */

    startCoordinates[0] = xstart * 0.001;
    startCoordinates[1] = ystart * 0.001;
    startCoordinates[2] = zstart * 0.001;

    Data.setStartpoint(xstart, ystart, zstart);
}
void        movementCalculator::setEndCoordinates(int xend, int yend, int zend) {

    /* sets the end coordinates of the movement, which should be calculated
     *
     * the unit of the input variables is in millimeters. for easier calculation they are
     * transformed into the unit meter
     *
     * this array contains the values in the order
     *
     * [0]  x-coordinate
     * [1]  y-coordinate
     * [2]  z-coordinate
     */

    endCoordinates[0] = xend * 0.001;
    endCoordinates[1] = yend * 0.001;
    endCoordinates[2] = zend * 0.001;

    Data.setEndpoint(xend, yend, zend);
}
void        movementCalculator::setMovementParameter(double vmax, double amax, double dmax) {

    /* sets the parameters of the movement, which should be calculated
     *
     * the given values are stored in the following variables:
     *
     *      maximum velocity:       vmax        -> maxVelocity
     *      maximum acceleration:   amax        -> maxAcceleration
     *      maximum deceleration:   dmax        -> maxDeceleration
     *
     */

    maxVelocity = vmax;
    maxAcceleration = amax;
    maxDeceleration = dmax;
}

void        movementCalculator::calculateMovement() {

    // give data to result class
    Data.setAccelerationValues(maxAcceleration, maxDeceleration);


    // calculation settings
    const int steps_per_segment = movementData::STEPS_PER_SEGMENT;



    // calculate the total length of the movement< out of the start and end coordinates
    calculateTotalMovementLength();


    // check if the maximum velocity is reached while this movement
    bool maxVelocityReached = checkIfMaxVelocityIsReached();
    Data.setMaximumVelocityReached(maxVelocityReached);


    // Handle case if movement is long enough to reach maximum velocity
    if(maxVelocityReached) {

        // maximum velocity reached is the same as maximum velocity given
        Data.setMaxVelocity(maxVelocity);


        // calculate the movement times
        ta = maxVelocity / maxAcceleration;
        td = (totalMovementLength / maxVelocity) + (0.5 * maxVelocity / maxAcceleration) - (0.5 * maxVelocity / maxDeceleration);
        te = (totalMovementLength / maxVelocity) + 0.5 * (maxVelocity / maxAcceleration + maxVelocity / maxDeceleration);
        Data.setTimeValues(ta, td, te);


        // get time vectors
        double t0_ta[steps_per_segment];
        double ta_td[steps_per_segment];
        double td_te[steps_per_segment];
        Data.getTimelineForAccelerationPart(t0_ta);
        Data.getTimelineForConstMovementPart(ta_td);
        Data.getTimelineForDecelerationPart(td_te);


        // calculate velocity graphs
        double velocity1[steps_per_segment];
        double velocity2[steps_per_segment];
        for(unsigned int i = 0; i < steps_per_segment; i++) {
            // velocity during acceleration
            velocity1[i] = maxAcceleration * t0_ta[i];

            // velocity during deceleration
            velocity2[i] = maxVelocity - maxDeceleration * (td_te[i] - td);
        }
        Data.setVelocityDuringAcceleration(velocity1);
        Data.setVelocityDuringDeceleration(velocity2);


        // calculate position graphs
        double offset = getPositionOffsetFromStartPosition();
        double position1[steps_per_segment];
        double position2[steps_per_segment];
        double position3[steps_per_segment];
        for(unsigned int i = 0; i < steps_per_segment; i++) {
            // position during acceleration
            position1[i] = 0.5 * maxAcceleration * t0_ta[i] * t0_ta[i] * 1000;

            // position during maxVelocity
            position2[i] = (0.5 * maxAcceleration * ta * ta + maxVelocity * (ta_td[i] - ta)) * 1000;

            // position during deceleration
            position3[i] = (totalMovementLength - (0.5 * maxDeceleration * (td_te[i] - te) * (td_te[i] - te))) * 1000;
        }
        Data.setPositionDuringAcceleration(position1);
        Data.setPositionDuringMaxVelocity(position2);
        Data.setPositionDuringDeceleration(position3);
        Data.setPostitionValues(position2[0], position3[0], totalMovementLength * 1000);

    }
    else {  // Handle case if maximum velocity isn't reached because of to short movement

        // calculate the real maximum reached velocity of this movement
        double      maxVelocityReal = sqrt((4*maxAcceleration*maxDeceleration*totalMovementLength) / (2*maxAcceleration + 2* maxDeceleration));
        Data.setMaxVelocity(maxVelocityReal);


        // calculate the movement times
        // in this case td = ta, because we're decelerating directly after accelerating
        ta = sqrt( totalMovementLength / (0.5 * (maxAcceleration + (maxAcceleration * maxAcceleration / maxDeceleration))));
        te = ta + ((maxAcceleration*ta) / maxDeceleration);
        td = ta;
        Data.setTimeValues(ta, td, te);



        // get time vectors
        double t0_ta[steps_per_segment];
        double ta_te[steps_per_segment];
        Data.getTimelineForAccelerationPart(t0_ta);
        Data.getTimelineForDecelerationPart(ta_te);




        // calculate the velocity graph
        double velocity1[steps_per_segment];
        double velocity2[steps_per_segment];

        for(unsigned int i = 0; i < steps_per_segment; i++) {
            // velocity during acceleration
            velocity1[i] = maxAcceleration * t0_ta[i];

            // velocity during deceleration
            velocity2[i] = maxVelocityReal - maxDeceleration * (ta_te[i] - ta);
        }

        Data.setVelocityDuringAcceleration(velocity1);
        Data.setVelocityDuringDeceleration(velocity2);




        // calculate position graphs
        double offset = getPositionOffsetFromStartPosition();
        double position1[steps_per_segment];
        double position2[steps_per_segment];

        for(unsigned int i = 0; i < steps_per_segment; i++) {
            // position during acceleration
            position1[i] = (offset + (0.5 * maxAcceleration * t0_ta[i] * t0_ta[i])) * 1000;

            // position during deceleration
            position2[i] = (offset + totalMovementLength - (0.5 * maxDeceleration * (ta_te[i] - te) * (ta_te[i] - te))) * 1000;
        }

        Data.setPositionDuringAcceleration(position1);
        Data.setPositionDuringDeceleration(position2);
        Data.setPostitionValues(position2[0], position2[0], totalMovementLength * 1000);
    }


    // prepare vectors for angle calculation
    QVector<double> xyzVector;
    xyzVector.append(endCoordinates[0] - startCoordinates[0]);
    xyzVector.append(endCoordinates[1] - startCoordinates[1]);
    xyzVector.append(endCoordinates[2] - startCoordinates[2]);

    QVector<double> xyVector;
    xyVector.append(endCoordinates[0] - startCoordinates[0]);
    xyVector.append(endCoordinates[1] - startCoordinates[1]);
    xyVector.append(0);

    QVector<double> xVector;
    xVector.append(endCoordinates[0] - startCoordinates[0]);
    xVector.append(0);
    xVector.append(0);


    // calculate angles
    double xyangle = acos(dotProductVector(xVector, xyVector) / (scalarProductVector(xVector) * scalarProductVector(xyVector)));
    double xyzangle = acos(dotProductVector(xyVector, xyzVector) / (scalarProductVector(xyVector) * scalarProductVector(xyzVector)));


    double xyFactor = cos(xyzangle);
    if(xyFactor < 0.00001)
        xyFactor = 0;

    double xFactor = xyFactor * cos(xyangle);
    if(xFactor < 0.00001)
        xFactor = 0;
    if(startCoordinates[0] > endCoordinates[0])
        xFactor = -xFactor;

    double yFactor = xyFactor * sin(xyangle);
    if(yFactor < 0.00001)
        yFactor = 0;
    if(startCoordinates[1] > endCoordinates[1])
        yFactor = -yFactor;

    double zFactor = sin(xyzangle);
    if(zFactor < 0.00001)
        zFactor = 0;
    if(startCoordinates[2] > endCoordinates[2])
        zFactor = -zFactor;


    Data.setAxeScalingFactors(xFactor, yFactor, zFactor);
}



/* PRIVATE FUNCTIONS */
void        movementCalculator::calculateTotalMovementLength() {

    /* Calculates the total length of the whole movement in m
     *
     *  totallength = sqrt(xlength² + ylength² + zlength²)
     *
     */

    double xlength = endCoordinates[0] - startCoordinates[0];
    double ylength = endCoordinates[1] - startCoordinates[1];
    double zlength = endCoordinates[2] - startCoordinates[2];

    totalMovementLength = sqrt(xlength * xlength + ylength * ylength + zlength * zlength);
}
bool        movementCalculator::checkIfMaxVelocityIsReached() {

    /* This function checks if the maximum velocity is reached during the movement
     *
     * This is done by calculating the lenght which is needed to accelerate to full speed
     * and decelerate back to 0 with the given parameters. If this length is smaller than
     * the total length of the movement, the full velocity is not reached.
     */

    double lengthToAccelerate = (maxVelocity * maxVelocity) / (2 * maxAcceleration);
    double lengthToDecelerate = (maxVelocity * maxVelocity) / (2 * maxDeceleration);

    return totalMovementLength > (lengthToAccelerate + lengthToDecelerate);
}
double      movementCalculator::getPositionOffsetFromStartPosition() {
    return sqrt(startCoordinates[0] * startCoordinates[0] + startCoordinates[1] * startCoordinates[1] + startCoordinates[2] * startCoordinates[2]);
}
double      movementCalculator::dotProductVector(QVector<double> vect1, QVector<double> vect2)
{
    return vect1[0] * vect2[0] + vect1[1] * vect2[1] + vect1[2] * vect2[2];
}
double      movementCalculator::scalarProductVector(QVector<double> vect)
{
    double scalar;

    scalar = vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[2];

    return sqrt(scalar);
}
