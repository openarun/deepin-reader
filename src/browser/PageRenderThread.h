/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhangsong<zhangsong@uniontech.com>
*
* Maintainer: zhangsong<zhangsong@uniontech.com>
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
#ifndef PAGERENDERTHREAD_H
#define PAGERENDERTHREAD_H

#include "Global.h"
#include "Model.h"

#include <QThread>
#include <QMutex>
#include <QStack>
#include <QImage>

class SheetBrowser;
class BrowserPage;
struct RenderPageTask {
    enum RenderPageTaskType {
        Image = 1,
        word = 2
    };
    int type = RenderPageTaskType::Image;
    SheetBrowser *view = nullptr;
    BrowserPage *item = nullptr;
    double scaleFactor = 1.0;
    Dr::Rotation rotation = Dr::RotateBy0;
    QRect renderRect;
};

/**
 * @brief The PageRenderThread class
 * 执行加载图片和文字等耗时操作的线程
 */
class PageRenderThread : public QThread
{
    Q_OBJECT
public:
    /**
     * @brief clearTask
     * 清除该项和该类型的任务
     * @param item 项指针
     * @param type 任务类型
     * @return 是否成功
     */
    static bool clearTask(BrowserPage *item, int type = RenderPageTask::Image);

    /**
     * @brief appendTask
     * 添加任务
     * @param task 任务
     */
    static void appendTask(RenderPageTask task);

    /**
     * @brief appendTasks
     * 批量添加任务
     * @param list 任务列表
     */
    static void appendTasks(QList<RenderPageTask> list);

    /**
     * @brief appendTasks
     * 根据任务参数添加任务
     * @param item 项指针
     * @param scaleFactor 缩放因子
     * @param rotation 旋转
     * @param renderRect 所占区域
     */
    static void appendTask(BrowserPage *item, double scaleFactor, Dr::Rotation rotation, QRect renderRect);

    /**
     * @brief destroyForever
     * 销毁线程且不会再被创建
     */
    static void destroyForever();

private:
    explicit PageRenderThread(QObject *parent = nullptr);

    ~PageRenderThread();

    void run();

signals:
    void sigImageTaskFinished(BrowserPage *item, QImage image, double scaleFactor, QRect rect);

    void sigWordTaskFinished(BrowserPage *item, QList<deepin_reader::Word> words);

private slots:
    void onImageTaskFinished(BrowserPage *item, QImage image, double scaleFactor, QRect rect);

    void onWordTaskFinished(BrowserPage *item, QList<deepin_reader::Word> words);

private:
    RenderPageTask m_curTask;
    QStack<RenderPageTask> m_tasks;
    QMutex m_mutex;
    bool m_quit = false;

    static bool quitForever;
    static QList<PageRenderThread *> instances;
    static PageRenderThread *instance(int itemIndex = -1);
};

#endif // PAGERENDERTHREAD_H
