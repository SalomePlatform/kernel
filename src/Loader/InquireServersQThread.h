//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : InquireServersQThread.h
//  Author : Vasily RUSYAEV
//  Module : SALOME
//  $Header$

#include <qapplication.h>
#include <qthread.h> 
#include <qvbox.h> 
#include <qprogressbar.h> 
#include <qlabel.h> 

/**********************************************************
**  Class:   InquireEvent
**  Descr:   Contains QCustomEvents for posting to InquireServersQThread
**  Level:   Private
***********************************************************/

class InquireEvent : public QCustomEvent
{
public:

  enum myCustomEvents{ ProgressEvent = QEvent::User + 10, ProgressEventLabel, ProgressEventError };
  
  InquireEvent( QEvent::Type type , void* data = 0 )
        : QCustomEvent( type, data ) {}
  ~InquireEvent() 
      {
        type() == (QEvent::Type)ProgressEvent ?
	  delete ( int* )data() : delete ( QString* )data();
      }
};

class InquireServersGUI;

class InquireServersQThread : public QThread
{
public:
  InquireServersQThread( InquireServersGUI* r );

  //the main loop of this thread
  virtual void run() ;
  //stop to ask servers
  void stop() 
    {
      IsChecking = false;
      myExitStatus = 1;
    }
  //return exit status: 0 - OK, >0 - BAD (some servers doesn't exists or user click cancel button) 
  int getExitStatus() { return myExitStatus;}
  //return count of inquired servers
  int getInquiredServers() { return myServersCount; }

private:

//functions:

  bool AskServer(int iteration, QString ** message);
  bool pingServer(int iteration, QString& errMessage);

//variables:

  InquireServersGUI* receiver;
  int _argc ;
  char ** _argv;
  //this variable is true if we are checking servers
  bool IsChecking;
  //count of inquired servers
  int myServersCount;
  //how many times we should repeat attempt to get response from all needed for launching SALOME servers
  int myRepeat;
  //define delay time between two attempts in microseconds
  int myDelay;
  //this strings' array contains messages for each server (e.g. "Loading: SALOMEDS_Server") 
  QString myMessages[8];
  //exit status: 0 - OK, >0 - BAD (some servers doesn't exists or user click cancel button) 
  int myExitStatus;

} ;

class InquireServersGUI : public QVBox
{
    Q_OBJECT

public:
  InquireServersGUI() ;
  ~InquireServersGUI();

  //returns arguments of QApplication
  //they are needed for CORBA servers initialization
  void getArgs(  int& _argc, char *** _argv);
  //return exit status: 0 - OK, >0 - BAD (some servers doesn't exists or user click cancel button) 
  int getExitStatus();
  //launch IAPP
  bool withGUI() { return myGUI; }
  void withGUI(bool gui) { myGUI = gui; }

protected:
  virtual void customEvent( QCustomEvent* ); 
  virtual void closeEvent ( QCloseEvent * );

private:
  InquireServersQThread* myThread;
  QProgressBar* myPrgBar;
  //this string contains description of currently asked server
  QLabel* myLabel;
  bool myGUI;

private slots:

    void ClickOnCancel();
} ;
