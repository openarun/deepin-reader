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
#ifndef FILEVIEWWIDGET_H
#define FILEVIEWWIDGET_H

#include "CustomControl/CustomWidget.h"

class TextOperationMenu;
class DefaultOperationMenu;
class DocummentProxy;
class FileViewWidgetPrivate;

/**
 * @brief The FileViewWidget class
 * @brief   文档显示区域
 */
class FileViewWidget : public CustomWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(FileViewWidget)

public:
    explicit FileViewWidget(CustomWidget *parent = nullptr);
    ~FileViewWidget() override;

signals:
    void sigFileOpenOK();

    // IObserver interface
public:
    int dealWithData(const int &, const QString &) override;
    int qDealWithShortKey(const QString &) override;
    void SetFindOperation(const int &, const QString &);


    void setFileChange(bool bchanged);
    bool getFileChange();
    // CustomWidget interface
protected:
    void initWidget() override;

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;

private slots:
    void slotCustomContextMenuRequested(const QPoint &);

    void slotBookMarkStateChange(int, bool);
    void slotDocFilePageChanged(int);
    void SlotDocFileOpenResult(bool);
    void slotDealWithMenu(int type, const QString &strcontents);

private:
    void initConnections();

    void onSetHandShape(const QString &);
    void onSetWidgetAdapt();

    void onFileAddNote(const QString &);

    void onFileAddAnnotation();
    void onFileAddNote();
    void __SetCursor(const QCursor &cursor);

    void OnSetViewChange(const QString &);
    void OnSetViewScale(const QString &);
    void OnSetViewRotate(const QString &);
    void OnSetViewHit(const QString &);

    void setScaleRotateViewModeAndShow();
    bool OpenFilePath(const QString &);

    void OnShortCutKey_Ctrl_l();
    void OnShortCutKey_Ctrl_i();
    void OnShortCutKey_Ctrl_c();

private:
    inline DocummentProxy *getDocumentProxy() {return m_pProxy;}

private:
    TextOperationMenu       *m_operatemenu;
    DefaultOperationMenu    *m_pDefaultMenu;
    DocummentProxy          *m_pProxy = nullptr;
    QString                 m_strPath = "";

    int                     m_nAdapteState = NO_ADAPTE_State;       //  当前自适应状态
    QPoint                  m_pStartPoint;
    QPoint                  m_pEndSelectPoint;

    int                     m_nDoubleShow = false;
    int                     m_rotateType = 0;            // 旋转类型(后台所需旋转类型)
    int                     m_nScale = 0;

    bool                    m_bFirstShow = true;        //  是否是第一次显示,  用于判断 resizeEvent

    friend class FVMMouseEvent;
    friend class DocShowShellWidget;

    FileViewWidgetPrivate *d_ptr;
    Q_DECLARE_PRIVATE_D(qGetPtrHelper(d_ptr), FileViewWidget)
};


#endif // FILEVIEWWIDGET_H
