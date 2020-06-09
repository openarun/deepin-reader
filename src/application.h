#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <DApplication>
#include "pdfControl/database/DBService.h"

class AppInfo;
//class AppConfig;

#if defined(dApp)
#undef dApp
#endif
#define dApp (static_cast<Application *>(QCoreApplication::instance()))

DWIDGET_USE_NAMESPACE

class Application : public DApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);
    void setSreenRect(const QRect &rect);
//    void adaptScreenView(int &w, int &h);

protected:
    void handleQuitAction() override;

public:
    DBService *m_pDBService = nullptr;
    AppInfo   *m_pAppInfo = nullptr;
//    AppConfig *m_pAppCfg = nullptr;

private:
    void initCfgPath();
    void initChildren();
    void initI18n();
};

#endif  // APPLICATION_H_
