#include "movementdata.h"

movementData::movementData()
{

}

void movementData::setStartpoint(int xstart, int ystart, int zstart)
{
    startPoint.clear();
    startPoint.append(xstart);
    startPoint.append(ystart);
    startPoint.append(zstart);
}
void movementData::setEndpoint(int xend, int yend, int zend)
{
    endPoint.clear();
    endPoint.append(xend);
    endPoint.append(yend);
    endPoint.append(zend);
}
void movementData::setMaximumVelocityReached(bool isReached)
{
    maxVelocityIsReached = isReached;
}
void movementData::setTimeValues(double _ta, double _td, double _te)
{
    this->t0 = 0;
    this->ta = _ta;
    this->td = _td;
    this->te = _te;
}
void movementData::setAccelerationValues(double maxAcceleration, double maxDeceleration)
{
    this->am = maxAcceleration;
    this->dm = maxDeceleration;
}
void movementData::setMaxVelocity(double maxVelocityReached)
{
    this->vm = maxVelocityReached;
}
void movementData::setPostitionValues(double posAtTa, double posAtTd, double posAtTe)
{
    this->p0 = 0;
    this->pa = posAtTa;
    this->pd = posAtTd;
    this->pe = posAtTe;
}
void movementData::setAxeScalingFactors(double xFac, double yFac, double zFac)
{
    this->xFactor = xFac;
    this->yFactor = yFac;
    this->zFactor = zFac;
}



void movementData::setPositionDuringAcceleration(double posDuringAcceleration[])
{
    for(unsigned int i = 0; i < STEPS_PER_SEGMENT; i++)
        positionDuringAcceleration[i] = posDuringAcceleration[i];
}
void movementData::setPositionDuringMaxVelocity(double posDuringMaxVelocity[])
{
    for(unsigned int i = 0; i < STEPS_PER_SEGMENT; i++)
        positionDuringMaxVelocity[i] = posDuringMaxVelocity[i];
}
void movementData::setPositionDuringDeceleration(double posDuringDeceleration[])
{
    for(unsigned int i = 0; i < STEPS_PER_SEGMENT; i++)
        positionDuringDeceleration[i] = posDuringDeceleration[i];
}

void movementData::setVelocityDuringAcceleration(double *velDuringAcceleration)
{
    for(unsigned int i = 0; i < STEPS_PER_SEGMENT; i++)
        velocityDuringAcceleration[i] = velDuringAcceleration[i];
}
void movementData::setVelocityDuringDeceleration(double *velDuringDeceleration)
{
    for(unsigned int i = 0; i < STEPS_PER_SEGMENT; i++)
        velocityDuringDeceleration[i] = velDuringDeceleration[i];
}

void movementData::getTimelineForAccelerationPart(double* array)
{
    double timestep = ta / STEPS_PER_SEGMENT;

    for(unsigned int i = 0; i < STEPS_PER_SEGMENT; i++)
        array[i] = timestep * i;
}
void movementData::getTimelineForConstMovementPart(double *array)
{
    double timestep = (td - ta) / STEPS_PER_SEGMENT;

    for(unsigned int i = 0; i < STEPS_PER_SEGMENT; i++)
        array[i] = timestep * i + ta;

}
void movementData::getTimelineForDecelerationPart(double *array)
{
    double tbegin;
    if(maxVelocityIsReached)
        tbegin = td;
    else
        tbegin = ta;

    double timestep = (te - tbegin) / STEPS_PER_SEGMENT;

    for(unsigned int i = 0; i < STEPS_PER_SEGMENT; i++)
        array[i] = timestep * i + tbegin;
}



QVector<double> movementData::getTimeAxis()
{
    QVector<double> timeVector;


    timeVector.append(0);


    // calculate t0 -> ta
    double timestep = ta / STEPS_PER_SEGMENT;
    for(int i = 0; i < STEPS_PER_SEGMENT; i++)
        timeVector.append(timestep * i);


    // calculate ta -> td
    if(maxVelocityIsReached) {
        timestep = (td-ta) / STEPS_PER_SEGMENT;
        for(int i = 0; i < STEPS_PER_SEGMENT; i++)
            timeVector.append(ta + (timestep * i));
    }


    // calculate td -> te
    timestep = (te-td) / STEPS_PER_SEGMENT;
    for(int i = 0; i < STEPS_PER_SEGMENT; i++)
        timeVector.append(td + (timestep * i));


    timeVector.append(te);

    timeVector.append(te);


    return timeVector;
}


QVector<double> movementData::getSummaryVelocityGraph()
{
    return getVelocityGraph();
}
QVector<double> movementData::getSummaryAccelerationGraph()
{
    return getAccelerationGraph();
}

QVector<double> movementData::getXAxisPositionGraph()
{
    QVector<double> data = getPositionGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = startPoint[0] + (data[i] * xFactor);

    return data;
}
QVector<double> movementData::getXAxisVelocityGraph()
{
    QVector<double> data = getVelocityGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = data[i] * xFactor;

    return data;
}
QVector<double> movementData::getXAxisAccelerationGraph()
{
    QVector<double> data = getAccelerationGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = data[i] * xFactor;

    return data;
}

QVector<double> movementData::getYAxisPositionGraph()
{
    QVector<double> data = getPositionGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = startPoint[1] + (data[i] * yFactor);

    return data;
}
QVector<double> movementData::getYAxisVelocityGraph()
{
    QVector<double> data = getVelocityGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = data[i] * yFactor;

    return data;
}
QVector<double> movementData::getYAxisAccelerationGraph()
{
    QVector<double> data = getAccelerationGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = data[i] * yFactor;

    return data;
}

QVector<double> movementData::getZAxisPositionGraph()
{
    QVector<double> data = getPositionGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = startPoint[1] + (data[i] * zFactor);

    return data;
}
QVector<double> movementData::getZAxisVelocityGraph()
{
    QVector<double> data = getVelocityGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = data[i] * zFactor;

    return data;
}
QVector<double> movementData::getZAxisAccelerationGraph()
{
    QVector<double> data = getAccelerationGraph();

    for(int i = 0; i < data.length(); i++)
        data[i] = data[i] * zFactor;

    return data;
}




QVector<double> movementData::getTimeValues()
{
    QVector<double> timeValues;
    timeValues.append(0);
    timeValues.append(ta);
    if(maxVelocityIsReached)
        timeValues.append(td);
    timeValues.append(te);

    return timeValues;
}
QVector<QString> movementData::getTimeLabels()
{
    QVector<QString> timeLabels;
    timeLabels.append(QString(""));
    timeLabels.append(QString("ta = "));
    if(maxVelocityIsReached)
        timeLabels.append(QString("td = "));
    timeLabels.append(QString("te = "));
    return timeLabels;
}

QVector<double> movementData::getXAxisPositionValues()
{
    QVector<double> positionValues;
    positionValues.clear();
    positionValues.append(startPoint[0]);
    positionValues.append(startPoint[0] + (pa * xFactor));
    positionValues.append(startPoint[0] + (pd * xFactor));
    positionValues.append(startPoint[0] + (pe * xFactor));
    return positionValues;
}
QVector<double> movementData::getYAxisPositionValues()
{
    QVector<double> positionValues;
    positionValues.clear();
    positionValues.append(startPoint[1]);
    positionValues.append(startPoint[1] + (pa * yFactor));
    positionValues.append(startPoint[1] + (pd * yFactor));
    positionValues.append(startPoint[1] + (pe * yFactor));
    return positionValues;
}
QVector<double> movementData::getZAxisPositionValues()
{
    QVector<double> positionValues;
    positionValues.clear();
    positionValues.append(startPoint[2]);
    positionValues.append(startPoint[2] + (pa * zFactor));
    positionValues.append(startPoint[2] + (pd * zFactor));
    positionValues.append(startPoint[2] + (pe * zFactor));
    return positionValues;
}
double movementData::getSummaryMaxVelocity()
{
    return vm;
}
double movementData::getXAxisMaxVelocity()
{
    return vm * xFactor;
}
double movementData::getYAxisMaxVelocity()
{
    return vm * yFactor;
}
double movementData::getZAxisMaxVelocity()
{
    return vm * zFactor;
}
QVector<double> movementData::getSummaryAccelerationValues()
{
    QVector<double> accelerationValues;
    accelerationValues.clear();
    accelerationValues.append(am);
    accelerationValues.append(-dm);

    return accelerationValues;
}
QVector<double> movementData::getXAxisAccelerationValues()
{
    QVector<double> accelerationValues;
    accelerationValues.clear();
    accelerationValues.append(am * xFactor);
    accelerationValues.append(-dm * xFactor);
    return accelerationValues;
}
QVector<double> movementData::getYAxisAccelerationValues()
{
    QVector<double> accelerationValues;
    accelerationValues.clear();
    accelerationValues.append(am * yFactor);
    accelerationValues.append(-dm * yFactor);
    return accelerationValues;
}
QVector<double> movementData::getZAxisAccelerationValues()
{
    QVector<double> accelerationValues;
    accelerationValues.clear();
    accelerationValues.append(am * zFactor);
    accelerationValues.append(-dm * zFactor);
    return accelerationValues;
}




// get graphs without factoring
QVector<double> movementData::getPositionGraph()
{
    QVector<double> positionVector;

    positionVector.append(positionDuringAcceleration[0]);


    // position during t0 -> ta
    for(int i = 0; i < STEPS_PER_SEGMENT; i++)
        positionVector.append(positionDuringAcceleration[i]);


    // position during ta -> td
    if(maxVelocityIsReached)
        for(int i = 0; i < STEPS_PER_SEGMENT; i++)
            positionVector.append(positionDuringMaxVelocity[i]);


    // position during td -> te
    for(int i = 0; i < STEPS_PER_SEGMENT; i++)
        positionVector.append(positionDuringDeceleration[i]);


    positionVector.append(pe);
    positionVector.append(pe);

    return positionVector;
}
QVector<double> movementData::getVelocityGraph()
{
    QVector<double> velocityVector;

    velocityVector.append(0);


    // values t0 -> ta
    for(int i = 0; i < STEPS_PER_SEGMENT; i++)
        velocityVector.append(velocityDuringAcceleration[i]);


    // velocity during ta -> td
    if(maxVelocityIsReached)
        for(int i = 0; i < STEPS_PER_SEGMENT; i++)
            velocityVector.append(vm);


    // velocity during td -> te
    for(int i = 0; i < STEPS_PER_SEGMENT; i++)
        velocityVector.append(velocityDuringDeceleration[i]);


    velocityVector.append(0);
    velocityVector.append(0);


    return velocityVector;
}
QVector<double> movementData::getAccelerationGraph()
{
    QVector<double> accelerationVector;


    accelerationVector.append(0);


    // calculate values t0 -> ta
    QVector<double> temp;
    temp.fill(am, STEPS_PER_SEGMENT);
    accelerationVector.append(temp);


    // calculate values ta -> td
    if(maxVelocityIsReached) {
        temp.fill(0, STEPS_PER_SEGMENT);
        accelerationVector.append(temp);
    }


    // calculate values td -> te
    temp.fill(-dm, STEPS_PER_SEGMENT);
    accelerationVector.append(temp);



    accelerationVector.append(-dm);
    accelerationVector.append(0);

    return accelerationVector;
}



