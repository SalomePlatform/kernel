%Module SalomePyQt

%Import qtmod.sip

class SALOME_Selection : QObject
{
%HeaderCode
#include <SALOME_Selection.h>
%End

public:
  SALOME_Selection(const QString &);
  void Clear();
  void ClearIObjects();

signals:
  void currentSelectionChanged();
};

class SalomePyQt
{
%HeaderCode
#include <SalomePyQt.hxx>
%End

public:
  static QWidget*    getDesktop();
  static QWorkspace* getMainFrame();
  static QMenuBar*   getMainMenuBar();
  static SALOME_Selection* getSelection();
  static int getStudyId();
  static void putInfo( const QString& );
  static void putInfo( const QString&, int );

  static const QString& getActiveComponent();

  static void updateObjBrowser( int, bool );


  static bool removeSettings(QString);
  static QString getSetting(QString);
  static void addStringSetting(QString, QString, bool);
  static void addIntSetting(QString, int, bool);
  static void addDoubleSetting(QString, double, bool);

  static QString getFileName(QWidget*, const QString&, const QStringList&, const QString&, bool);
  static QString getExistingDirectory(QWidget*, const QString&, const QString&);
  static void helpContext(const QString&, const QString&);
};
