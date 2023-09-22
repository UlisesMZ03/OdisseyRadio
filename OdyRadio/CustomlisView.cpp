// customlistview.cpp
#include "CustomlisView.h"
#include <QWheelEvent>

CustomListView::CustomListView(QWidget *parent)
    : QListView(parent)
{
}

void CustomListView::wheelEvent(QWheelEvent *event)
{
    emit wheelScrolled(event->angleDelta().y());
    QListView::wheelEvent(event);
}
