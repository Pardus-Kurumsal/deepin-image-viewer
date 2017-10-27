#ifndef TIMELINE_DELEGATE_H
#define TIMELINE_DELEGATE_H

#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QStyledItemDelegate>

#include "timelineitem.h"
#include "controller/viewerthememanager.h"

class QMutex;
class TLThumbnailThread;
class TimelineDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit TimelineDelegate(QObject *parent = NULL);

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const Q_DECL_OVERRIDE;

signals:
    void thumbnailGenerated(const QString &path);

private:
    TimelineItem::ItemData itemData(const QModelIndex &index) const;
    QPixmap thumbnail(const TimelineItem::ItemData &data) const;
    void startThumbnailThread(const TimelineItem::ItemData &data) const;
    void onThemeChanged(ViewerThemeManager::AppTheme theme);

private:
    QColor m_borderColor;
    QColor m_dateColor;
    QColor m_seperatorColor;
    QString m_defaultThumbnail;
    mutable QMap<QString, TLThumbnailThread *> m_threads;
};

#endif // TIMELINE_DELEGATE_H
