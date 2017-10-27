#ifndef TIMELINEITEM_H
#define TIMELINEITEM_H

#include <QList>
#include <QVariant>
#include <QPixmap>

class QMutex;
class TimelineItem
{
public:
    struct ItemData {
        bool isTitle;
        QString path;
        QString timeline;
        QByteArray thumbArray; // SKILL: For storage, use QByteArray can save a lot of memory
    };

    explicit TimelineItem(TimelineItem *parentItem = 0);
    ~TimelineItem();

    void appendData(const ItemData &data);
    void updateData(const ItemData &data);
    void removeData(const QString &path);


    void insertChild(int index, TimelineItem *child);
    void appendChild(TimelineItem *child);
    void removeChild(TimelineItem *child);

    TimelineItem *child(int row);
    int childCount() const;
    int columnCount() const;
    ItemData data(int column) const;
    int row() const;
    TimelineItem *parentItem();

private:
    QMutex *m_mutex;
    QList<TimelineItem*> m_childItems;
    QMap<QString, ItemData> m_datas;
    QStringList m_paths;
    TimelineItem *m_parentItem;
};

#endif // TIMELINEITEM_H
