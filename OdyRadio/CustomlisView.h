// customlistview.h
#ifndef CUSTOMLISVIEW_H
#define CUSTOMLISVIEW_H

#include <QListView>

class CustomListView : public QListView
{
    Q_OBJECT

public:
    CustomListView(QWidget *parent = nullptr);

signals:
    void wheelScrolled(int delta);

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // CUSTOMLISVIEW_H
