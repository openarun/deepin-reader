#ifndef BOOKMARKITEMWIDGET_H
#define BOOKMARKITEMWIDGET_H

#include <DWidget>
#include <DLabel>
#include <QHBoxLayout>
#include <QFont>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

#include "header/CustomWidget.h"
#include "header/MsgHeader.h"

DWIDGET_USE_NAMESPACE

//操作类型
enum OPERATION_TYPE {
    ADDITEM = 0,        //增加item
    DLTITEM,            //删除item
};

class BookMarkItemWidget : public CustomWidget
{
    Q_OBJECT
public:
    BookMarkItemWidget(CustomWidget *parent = nullptr);

public:
    void setPicture(const QString &);
    void setPage(const QString &);
    // IObserver interface
    int update(const int &, const QString &) override;

private slots:
    void slotDltBookMark();
    void slotShowContextMenu(const QPoint &);

private:
    void initWidget();

private:
    DLabel *m_pPicture = nullptr;
    DLabel *m_pPage = nullptr;
    QHBoxLayout *m_pHLayout = nullptr;
};

#endif // BOOKMARKITEMWIDGET_H
