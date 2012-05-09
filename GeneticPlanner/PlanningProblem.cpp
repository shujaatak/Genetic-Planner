#include "PlanningProblem.h"

#include "FlyThroughTask.h"
#include "EndingTask.h"

PlanningProblem::PlanningProblem()
{
    _isStartingDefined = false;
    _isEndingDefined = false;
    _endingTask = 0;
}

PlanningProblem::PlanningProblem(const UAVParameters &uavParams, const SensorDefinition &sensorParams)
{
    this->setUAVSettings(uavParams);
    this->setSensorSettings(sensorParams);

    _isStartingDefined = false;
    _isEndingDefined = false;
    _endingTask = 0;
}

bool PlanningProblem::isReady() const
{
    return (this->isStartingDefined());
}

qreal PlanningProblem::fitness(const Individual &individual)
{
    if (!this->isReady())
        return 0.0;

    qreal toRet = 0.0;

    //Get the geo points of the path and run them all through our tasks!
    QList<QPointF> geoPositions = individual.generateGeoPoints(this->startingPos());

    foreach(PathTask * task, _tasks)
        toRet += task->performance(geoPositions);

    //Do the "ending task" if applicable
    if (_endingTask)
    {
        toRet += _endingTask->performance(geoPositions);
    }

    //Punish long paths
    {
        qreal shortReward = 10 * toRet / geoPositions.length();
        toRet += shortReward;
    }

    return toRet;
}

UAVParameters PlanningProblem::uavSettings() const
{
    return _uavSettings;
}

SensorDefinition PlanningProblem::sensorSettings() const
{
    return _sensorSettings;
}

bool PlanningProblem::isStartingDefined() const
{
    return _isStartingDefined;
}

QPointF PlanningProblem::startingPos() const
{
    return _startingPos;
}

qreal PlanningProblem::startingAlt() const
{
    return _startingAlt;
}

bool PlanningProblem::isEndingDefined() const
{
    return _isEndingDefined;
}

QPointF PlanningProblem::endingPos() const
{
    return _endingPos;
}

qreal PlanningProblem::endingAlt() const
{
    return _endingAlt;
}

void PlanningProblem::setUAVSettings(const UAVParameters &uavParams)
{
    _uavSettings = uavParams;
}

void PlanningProblem::setSensorSettings(const SensorDefinition &sensorParams)
{
    _sensorSettings = sensorParams;
}

void PlanningProblem::setStartingPos(QPointF startingPos, qreal startingAlt)
{
    _startingPos = startingPos;
    _startingAlt = startingAlt;
    _isStartingDefined = true;
}

void PlanningProblem::setEndingPos(QPointF endingPos, qreal endingAlt)
{
    _endingPos = endingPos;
    _endingAlt = endingAlt;
    _isEndingDefined = true;

    if (_endingTask)
        delete _endingTask;
    _endingTask = new EndingTask(endingPos,15.0);
}
