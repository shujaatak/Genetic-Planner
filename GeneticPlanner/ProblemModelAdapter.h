#ifndef PROBLEMMODELADAPTER_H
#define PROBLEMMODELADAPTER_H

#include <QObject>
#include <QWeakPointer>
#include <QPointer>
#include <QHash>

#include "PlanningProblem.h"
#include "MapGraphicsScene.h"
#include "CircleObject.h"
class TaskAreaObject;

class ProblemModelAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ProblemModelAdapter(MapGraphicsScene * scene, QObject *parent = 0);

private:
    MapGraphicsScene * _scene;
    QWeakPointer<PlanningProblem> _problem;
    QPointer<CircleObject> _startMarker;
    QPointer<CircleObject> _endMarker;

    QHash<TaskAreaObject *, QWeakPointer<TaskArea> > _objectToArea;


    
signals:
    
public slots:
    void setProblem(QWeakPointer<PlanningProblem>);

private slots:
    void handleStartPositionChanged(const Position& pos);
    void handleStartPositionRemoved();

    void handleEndPositionChanged(const Position& pos);
    void handleEndPositionRemoved();

    void handleStartMarkerMoved();
    void handleStartMarkerDestroyed();

    void handleEndMarkerMoved();
    void handleEndMarkerDestroyed();

    void handleAreaAdded(QSharedPointer<TaskArea> area);
    void handleAreaObjectDestroyed();
    
};

#endif // PROBLEMMODELADAPTER_H
