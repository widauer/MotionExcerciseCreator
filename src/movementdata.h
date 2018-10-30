#ifndef MOVEMENTDATA_H
#define MOVEMENTDATA_H

#define STEPSPERSEGMENT 1000;

#include <QVector>
#include <string>


class movementData
{
public:
    static const int  STEPS_PER_SEGMENT = 1000;


    movementData();

    // set the movement and data for constant graph data
    void        setStartpoint(int xstart, int ystart, int zstart);
    void        setEndpoint(int xend, int yend, int zend);
    void        setMaximumVelocityReached(bool isReached);
    void        setTimeValues(double _ta, double _td, double _te);
    void        setAccelerationValues(double maxAcceleration, double maxDeceleration);
    void        setMaxVelocity(double maxVelocityReached);
    void        setPostitionValues(double posAtTa, double posAtTd, double posAtTe);
    void        setAxeScalingFactors(double xFac, double yFac, double zFac);


    // set non constant graph data
    void        setPositionDuringAcceleration(double posDuringAcceleration[]);
    void        setPositionDuringMaxVelocity(double posDuringMaxVelocity[]);
    void        setPositionDuringDeceleration(double posDuringDeceleration[]);

    void        setVelocityDuringAcceleration(double velDuringAcceleration[]);
    void        setVelocityDuringDeceleration(double velDuringDeceleration[]);


    // get data fo calculation
    void        getTimelineForAccelerationPart(double* array);
    void        getTimelineForConstMovementPart(double* array);
    void        getTimelineForDecelerationPart(double* array);


    // concat graph data and return
    QVector<double>     getTimeAxis();

    QVector<double>     getSummaryVelocityGraph();
    QVector<double>     getSummaryAccelerationGraph();

    QVector<double>     getXAxisPositionGraph();
    QVector<double>     getXAxisVelocityGraph();
    QVector<double>     getXAxisAccelerationGraph();

    QVector<double>     getYAxisPositionGraph();
    QVector<double>     getYAxisVelocityGraph();
    QVector<double>     getYAxisAccelerationGraph();

    QVector<double>     getZAxisPositionGraph();
    QVector<double>     getZAxisVelocityGraph();
    QVector<double>     getZAxisAccelerationGraph();


    QVector<double>     getTimeValues();
    QVector<QString>    getTimeLabels();
    QVector<double>     getXAxisPositionValues();
    QVector<double>     getYAxisPositionValues();
    QVector<double>     getZAxisPositionValues();
    double              getSummaryMaxVelocity();
    double              getXAxisMaxVelocity();
    double              getYAxisMaxVelocity();
    double              getZAxisMaxVelocity();
    QVector<double>     getSummaryAccelerationValues();
    QVector<double>     getXAxisAccelerationValues();
    QVector<double>     getYAxisAccelerationValues();
    QVector<double>     getZAxisAccelerationValues();



private:
    // start and end point of movement
    QVector<double> startPoint;
    QVector<double> endPoint;

    // maximum velocity is reached?
    bool        maxVelocityIsReached;

    // time variables t0, ta, td, te
    double      t0, ta, td, te;

    // position values at t0, ta, td, te
    double      p0, pa, pd, pe;

    // maximum reached velocity
    double      vm;

    // acceleration and deceleration values
    double      am, dm;

    // scaling factors for the axes
    double      xFactor, yFactor, zFactor;



    // graphs without constant values
    double      positionDuringAcceleration[STEPS_PER_SEGMENT];
    double      positionDuringDeceleration[STEPS_PER_SEGMENT];
    double      positionDuringMaxVelocity[STEPS_PER_SEGMENT];

    double      velocityDuringAcceleration[STEPS_PER_SEGMENT];
    double      velocityDuringDeceleration[STEPS_PER_SEGMENT];


    QVector<double>     getPositionGraph();
    QVector<double>     getVelocityGraph();
    QVector<double>     getAccelerationGraph();
};

#endif // MOVEMENTDATA_H
