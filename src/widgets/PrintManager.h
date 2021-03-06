/*
 * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     wangzhixuan<wangzhixuan@uniontech.com>
 *
 * Maintainer: wangzhixuan<wangzhixuan@uniontech.com>
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
#ifndef PRINTMANAGER_H
#define PRINTMANAGER_H

#include <DWidget>

#include <QObject>

#include "dprintpreviewdialog.h"

DWIDGET_USE_NAMESPACE

class DocSheet;
class QPrinter;
class PrintManager : public QObject
{
    Q_OBJECT
public:
    explicit PrintManager(DocSheet *sheet, QObject *parent = nullptr);

    /**
     * @brief showPrintDialog
     * 显示打印窗口
     * @param widget
     */
    void showPrintDialog(DWidget *widget);

    /**
     * @brief setPrintPath
     * 设置打印文件
     * @param strPrintPath
     */
    void setPrintPath(const QString &strPrintPath);

private slots:
    /**
     * @brief slotPrintPreview
     * 打印预览
     * @param printer
     */
    void slotPrintPreview(DPrinter *printer);

private:
    QString     m_strPrintName = "";
    QString     m_strPrintPath = "";
    DocSheet   *m_sheet;
};

#endif // PRINTMANAGER_H
