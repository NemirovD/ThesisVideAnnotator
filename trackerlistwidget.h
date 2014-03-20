#ifndef TRACKERLISTWIDGET_H
#define TRACKERLISTWIDGET_H

#include <QListWidget>
#include "utils.h"
#include "objecttracker.h"
#include "objectinfolistwidget.h"

class TrackerListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit TrackerListWidget(QWidget *parent = 0);

signals:

public slots:
    void updateTrackerList(const QVector<ObjectTracker>);
    void updateObjectList(const QVector<ul::ObjectInfo>);
private:
    QVector<ul::ObjectInfo> _objectList;
};

#endif // TRACKERLISTWIDGET_H
