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
#ifndef CATALOGTREEVIEW_H
#define CATALOGTREEVIEW_H

#include <DTreeView>

#include <QStandardItemModel>

#include "../ModelService/IObserver.h"
#include "docview/pagebase.h"

DWIDGET_USE_NAMESPACE

class CatalogTreeView : public DTreeView, public IObserver
{
    Q_OBJECT
    Q_DISABLE_COPY(CatalogTreeView)

public:
    explicit CatalogTreeView(DWidget *parent = nullptr);
    ~CatalogTreeView() override;

    // IObserver interface
public:
    int dealWithData(const int &, const QString &) override;
    void sendMsg(const int &, const QString &) override;
    void notifyMsg(const int &, const QString &) override;

    int qDealWithData(const int &, const QString &);
    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;

    // QAbstractItemView interface
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private:
    void initConnections();
    void parseCatalogData(const Section &, QStandardItem *);

    QList<QStandardItem *>   getItemList(const QString &, const int &, const qreal  &realleft, const qreal &realtop);

    void OnOpenFileOk(const QString &path);
    void OnFilePageChanged(const QString &);

private slots:
    void SlotClicked(const QModelIndex &);
    void SlotCollapsed(const QModelIndex &);
    void SlotExpanded(const QModelIndex &);

private:
    QString     m_strBindPath = "";
};

#endif // CATALOGTREEVIEW_H
