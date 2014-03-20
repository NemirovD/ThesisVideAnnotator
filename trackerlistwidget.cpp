#include "trackerlistwidget.h"

TrackerListWidget::TrackerListWidget(QWidget *parent) :
    QListWidget(parent)
{
}

void TrackerListWidget::updateTrackerList(const QVector<ObjectTracker> otList)
{
    clear();
    for(int i = 0; i < otList.size(); ++i)
    {
        int index = otList[i].trackerIndex();
        QListWidgetItem* item = new QListWidgetItem(this);
        addItem(item);

        ObjectInfoListWidget *t = new ObjectInfoListWidget(_objectList[index].icon(),
                                                           _objectList[index].name(),
                                                           _objectList[index].URL(),
                                                           this);

        item->setSizeHint(t->minimumSizeHint());
        setItemWidget(item, t);
    }
}

void TrackerListWidget::updateObjectList(const QVector<ul::ObjectInfo> oiList)
{
    _objectList = oiList;
}
