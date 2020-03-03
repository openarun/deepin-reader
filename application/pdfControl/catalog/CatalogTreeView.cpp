/*
 * Copyright (C) 2019 ~ 2020 UOS Technology Co., Ltd.
 *
 * Author:     wangzhxiaun
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "CatalogTreeView.h"

#include <QHeaderView>
#include <DFontSizeManager>
#include <QDebug>

#include "application.h"

#include "docview/docummentproxy.h"
#include "utils/utils.h"

#include "widgets/main/MainTabWidgetEx.h"

CatalogTreeView::CatalogTreeView(DWidget *parent)
    : DTreeView(parent)
{
    setFrameShape(QFrame::NoFrame);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    this->header()->setHidden(true);
    this->viewport()->setAutoFillBackground(false);

    this->setContentsMargins(0, 0, 0, 0);

    initConnections();

    dApp->m_pModelService->addObserver(this);
}

CatalogTreeView::~CatalogTreeView()
{
    dApp->m_pModelService->removeObserver(this);
}

int CatalogTreeView::dealWithData(const QString &msgContent)
{
    return MSG_NO_OK;
}

int CatalogTreeView::dealWithData(const int &msgType, const QString &msgContent)
{
    if (msgType == MSG_OPERATION_OPEN_FILE_OK) {
        OnOpenFileOk(msgContent);
    } else if (msgType == MSG_FILE_PAGE_CHANGE) {    //  文档页变化, 需要跳转到对应项
        OnFilePageChanged(msgContent);
    }
    return MSG_NO_OK;
}

//void CatalogTreeView::sendMsg(const int &, const QString &)
//{

//}

void CatalogTreeView::notifyMsg(const int &, const QString &)
{
}

int CatalogTreeView::qDealWithData(const int &msgType, const QString &msgContent)
{
    return MSG_NO_OK;
}

void CatalogTreeView::initConnections()
{
    connect(this, SIGNAL(clicked(const QModelIndex &)), SLOT(SlotClicked(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), SLOT(SlotCollapsed(const QModelIndex &)));
    connect(this, SIGNAL(expanded(const QModelIndex &)), SLOT(SlotExpanded(const QModelIndex &)));
}

//  递归解析
void CatalogTreeView::parseCatalogData(const Section &ol, QStandardItem *parentItem)
{
    auto model = reinterpret_cast<QStandardItemModel *>(this->model());
    if (model) {
        foreach (auto s, ol.children) { //  2级显示
            if (s.link.page > 0) {
                auto itemList = getItemList(s.title, s.link.page, s.link.left, s.link.top);
                parentItem->appendRow(itemList);

                foreach (auto s1, s.children) { //  3级显示
                    if (s1.link.page > 0) {
                        auto itemList1 = getItemList(s1.title, s1.link.page, s1.link.left, s1.link.top);
                        itemList.at(0)->appendRow(itemList1);
                    }
                }
            }
        }
    }
}

//  获取 一行的 三列数据
QList<QStandardItem *> CatalogTreeView::getItemList(const QString &title, const int &page, const qreal  &realleft, const qreal &realtop)
{
    auto item = new QStandardItem(title);
    item->setData(page);
    item->setData(realleft, Qt::UserRole + 2);
    item->setData(realtop, Qt::UserRole + 3);

    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    auto item1 = new QStandardItem();
    item1->setData(page);
    item1->setData(realleft, Qt::UserRole + 2);
    item1->setData(realtop, Qt::UserRole + 3);

    auto item2 = new QStandardItem(QString::number(page));
    item2->setData(page);
    item2->setData(realleft, Qt::UserRole + 2);
    item2->setData(realtop, Qt::UserRole + 3);

    item2->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    return QList<QStandardItem *>() << item << item1 << item2;
}

//  文档打开成功, 加载对应目录
void CatalogTreeView::OnOpenFileOk(const QString &path)
{
    m_strBindPath = path;

    auto model = reinterpret_cast<QStandardItemModel *>(this->model());
    if (model) {
        model->clear();

        MainTabWidgetEx *pMtwe = MainTabWidgetEx::Instance();
        if (pMtwe) {
            DocummentProxy *_proxy =  pMtwe->getCurFileAndProxy(m_strBindPath);
            if (_proxy) {

                Outline ol = _proxy->outline();
                foreach (const Section &s, ol) {   //  1 级显示
                    if (s.link.page > 0) {
                        auto itemList = getItemList(s.title, s.link.page, s.link.left, s.link.top);
                        model->appendRow(itemList);

                        parseCatalogData(s, itemList.at(0));
                    }
                }

                int nCurPage = _proxy->currentPageNo();
                OnFilePageChanged(QString::number(nCurPage));
            }
        }
    }
}

//  点击 任一行, 实现 跳转页面
void CatalogTreeView::SlotClicked(const QModelIndex &index)
{
    MainTabWidgetEx *pMtwe = MainTabWidgetEx::Instance();
    if (pMtwe) {
        DocummentProxy *_proxy =  pMtwe->getCurFileAndProxy(m_strBindPath);
        if (_proxy) {
            int nPage = index.data(Qt::UserRole + 1).toInt();
            nPage--;

            double left = index.data(Qt::UserRole + 2).toDouble();
            double top = index.data(Qt::UserRole + 3).toDouble();

            _proxy->jumpToOutline(left, top, nPage);
        }
    }
}


//  文档页变化, 目录高亮随之变化
void CatalogTreeView::OnFilePageChanged(const QString &sPage)
{
    if (this->isVisible()) {
        this->clearSelection(); //  清除 之前的选中

        auto model = reinterpret_cast<QStandardItemModel *>(this->model());
        if (model) {

            int iPage = sPage.toInt();
            iPage++;

            auto itemList = model->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);
            foreach (auto item, itemList) {
                int itemPage = item->data().toInt();
                if (itemPage == iPage) {    //  找到了

                    auto curIndex = model->indexFromItem(item);
                    if (curIndex.isValid()) {

                        auto curSelIndex = curIndex;

                        auto parentIndex = curIndex.parent();   //  父 节点
                        if (parentIndex.isValid()) {    //  父节点存在
                            auto grandpaIndex = parentIndex.parent();       //  是否还存在 爷爷节点
                            if (grandpaIndex.isValid()) {    //  爷爷节点存在
                                bool isGrandpaExpand = this->isExpanded(grandpaIndex);
                                if (isGrandpaExpand) { //  爷爷节点 已 展开
                                    bool isExpand = this->isExpanded(parentIndex);
                                    if (!isExpand) {   //  父节点未展开, 则 父节点高亮
                                        curSelIndex = parentIndex;
                                    }
                                } else {    //  爷爷节点 未展开, 则 爷爷节点高亮
                                    curSelIndex = grandpaIndex;
                                }
                            } else {    //  没有 爷爷节点
                                bool isExpand = this->isExpanded(parentIndex);
                                if (!isExpand)     //  父节点未展开, 则 父节点高亮
                                    curSelIndex = parentIndex;
                            }
                        }

                        this->selectionModel()->setCurrentIndex(curSelIndex, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
                    }
                    break;
                }
            }
        }
    }
}

//  折叠 节点处理
void CatalogTreeView::SlotCollapsed(const QModelIndex &index)
{
    Q_UNUSED(index);
    MainTabWidgetEx *pMtwe = MainTabWidgetEx::Instance();
    if (pMtwe) {
        DocummentProxy *_proxy = pMtwe->getCurFileAndProxy(m_strBindPath);
        if (_proxy) {
            int nCurPage = _proxy->currentPageNo();

            OnFilePageChanged(QString::number(nCurPage));
        }
    }
}


//  展开 节点处理
void CatalogTreeView::SlotExpanded(const QModelIndex &index)
{
    if (index == this->selectionModel()->currentIndex()) {  //  展开的节点 是 高亮节点
        MainTabWidgetEx *pMtwe = MainTabWidgetEx::Instance();
        if (pMtwe) {
            DocummentProxy *_proxy =  pMtwe->getCurFileAndProxy(m_strBindPath);
            if (_proxy) {
                int nCurPage = _proxy->currentPageNo();
                OnFilePageChanged(QString::number(nCurPage));
            }
        }
    }
}

//  实现 上下左键 跳转
void CatalogTreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    MainTabWidgetEx *pMtwe = MainTabWidgetEx::Instance();
    if (pMtwe && this->isVisible()) {
        DocummentProxy *_proxy =  pMtwe->getCurFileAndProxy(m_strBindPath);
        if (_proxy) {
            int nPage = current.data(Qt::UserRole + 1).toInt();
            nPage--;

            double left = current.data(Qt::UserRole + 2).toDouble();
            double top = current.data(Qt::UserRole + 3).toDouble();

            _proxy->jumpToOutline(left, top, nPage);
        }
    }
}


//  窗口大小变化, 列的宽度随之变化
void CatalogTreeView::resizeEvent(QResizeEvent *event)
{
    setColumnWidth(0, this->width() - 120);
    setColumnWidth(1, 22);
    setColumnWidth(2, 60);

    DTreeView::resizeEvent(event);
}
