#ifndef MOVEMENTCALCULATOR_H
#define MOVEMENTCALCULATOR_H

#include <QObject>
#include <QVector>
#include <math.h>
#include <tgmath.h>
#include <movementdata.h>

using namespace std;

class movementCalculator
{
public:
    // result data
    movementData    Data;


    movementCalculator();

    void    setStartCoordinates(int xstart, int ystart, int zstart);
    void    setEndCoordinates(int xend, int yend, int zend);
    void    setMovementParameter(double vmax, double amax, double dmax);

    void    calculateMovement();

private:


    // movement inputs
    double      startCoordinates[3];
    double      endCoordinates[3];
    double      maxVelocity;
    double      maxAcceleration;
    double      maxDeceleration;


    // movement outputs
    double      totalMovementLength;
    double      ta;
    double      td;
    double      te;


    // calculation functions
    void        calculateTotalMovementLength();
    bool        checkIfMaxVelocityIsReached();
    double      getPositionOffsetFromStartPosition();
    double      dotProductVector(QVector<double> vect1, QVector<double> vect2);
    double      scalarProductVector(QVector<double> vect);

};

#endif // MOVEMENTCALCULATOR_H
