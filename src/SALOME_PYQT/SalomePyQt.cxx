using namespace std;
#include "SalomePyQt.hxx"

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"

#include "QAD_Config.h"
#include "QAD_Settings.h"

QWidget* SalomePyQt::getDesktop()
{
  return (QWidget*)(QAD_Application::getDesktop());
}

QWorkspace* SalomePyQt::getMainFrame()
{
  return (QWorkspace*)(QAD_Application::getDesktop()->getMainFrame());
}

QMenuBar* SalomePyQt::getMainMenuBar() 
{
  return (QMenuBar*)(QAD_Application::getDesktop()->getMainMenuBar());
}

int SalomePyQt::getStudyId()
{
  return QAD_Application::getDesktop()->getActiveStudy()->getStudyId();
}

SALOME_Selection* SalomePyQt::getSelection()
{
  return SALOME_Selection::Selection(QAD_Application::getDesktop()->getActiveStudy()->getSelection());
}

void SalomePyQt::putInfo( const QString& msg )
{
  QAD_Application::getDesktop()->putInfo(msg);
}

void SalomePyQt::putInfo( const QString& msg, int ms )
{
  QAD_Application::getDesktop()->putInfo(msg, ms);
}

void SalomePyQt::updateObjBrowser( int studyId, bool updateSelection)
{  
  QList<QAD_Study>& studies = QAD_Application::getDesktop()->getActiveApp()->getStudies();
  for ( QAD_Study* study = studies.first(); study; study = studies.next() )  {
    if ( study->getStudyId() == studyId ) {
      study->updateObjBrowser( updateSelection );
      break;
    }
  }
}

const QString& SalomePyQt::getActiveComponent()
{
  return QAD_Application::getDesktop()->getActiveComponent();
}

void SalomePyQt::addStringSetting(QString _name, QString _value, bool _autoValue)
{
  QAD_CONFIG->addSetting(_name, _value, _autoValue);
}

void SalomePyQt::addIntSetting(QString _name, int _value, bool _autoValue)
{
  QAD_CONFIG->addSetting(_name, _value, _autoValue);
}

void SalomePyQt::addDoubleSetting(QString _name, double _value, bool _autoValue)
{
  QAD_CONFIG->addSetting(_name, _value, _autoValue);
}

bool SalomePyQt::removeSettings(QString name)
{
  return QAD_CONFIG->removeSettings( name );
}

QString SalomePyQt::getSetting(QString name)
{
  return QAD_CONFIG->getSetting(name);
}
