/*
**  This file is part of Vidalia, and is subject to the license terms in the
**  LICENSE file, found in the top level directory of this distribution. If you
**  did not receive the LICENSE file with this file, you may obtain it from the
**  Vidalia source package distributed by the Vidalia Project at
**  http://www.torproject.org/projects/vidalia.html. No part of Vidalia,
**  including this file, may be copied, modified, propagated, or distributed
**  except according to the terms described in the LICENSE file.
*/

/*
** \file MainWindow.h
** \brief Main window. Creates tray menu and child windows
*/

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "config.h"
#include "ui_MainWindow.h"

#include "VidaliaWindow.h"
#include "HelperProcess.h"
#include "AboutDialog.h"
#include "MessageLog.h"
#include "BandwidthGraph.h"
#include "ConfigDialog.h"
#include "HelpBrowser.h"
#include "NetViewer.h"

#include "TorControl.h"

#if defined(USE_AUTOUPDATE)
#include "UpdateProcess.h"
#include "UpdateProgressDialog.h"
#endif
#if defined(USE_MINIUPNPC)
#include "UPNPControl.h"
#endif

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>

class MainWindow : public VidaliaWindow
{
  Q_OBJECT

public:
  /** Default constructor */
  MainWindow();
  /** Destructor. */
  ~MainWindow();

public slots:
  /** Shows or hides this window. **/
  virtual void setVisible(bool visible);

protected:
  /** Called when the user changes the UI translation. */
  virtual void retranslateUi();

private slots:
  /** Respond to a double-click on the tray icon by opening the Control Panel
   * window. */
  void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
  /** Displays the help browser and displays the most recently viewed help
   * topic. */
  void showHelpDialog();
  /** Called when a child window requests the given help <b>topic</b>. */
  void showHelpDialog(const QString &topic);
  /** Called when the user selects "Start" from the menu. */
  void start();
  /** Called when the user changes a setting that needs Tor restarting */
  void restart();
  /** Called when the Tor process fails to start. */
  void startFailed(QString errmsg);
  /** Called when the Tor process has successfully started. */
  void started();
  /** Called when the user selects "Stop" form the menu. */
  bool stop();
  /** Called when the Tor process has exited, either expectedly or not. */
  void stopped(int errorCode, QProcess::ExitStatus exitStatus);
  /** Called when the control socket has connected to Tor. */
  void connected();
  /** Called when the control connection fails. */
  void connectFailed(QString errmsg);
  /** Called when Vidalia wants to disconnect from a Tor it did not start. */
  void disconnect();
  /** Called when the control socket has been disconnected. */
  void disconnected();
  /** Called when Vidalia has successfully authenticated to Tor. */
  void authenticated();
  /** Called when Vidalia fails to authenticate to Tor. The failure reason is
   * specified in <b>errmsg</b>. */
  void authenticationFailed(QString errmsg);
  /** Re-enables the 'New Identity' button after a delay from the previous time
   * 'New Identity' was used. */
  void enableNewIdentity();
  /** Called when the user selects the "New Identity" action from the menu. */
  void newIdentity();
  /** Called when the user exits Vidalia. */
  void close();
  /** Called when the application has started and the main event loop is
   * running. */
  void running();
  /** Terminate the Tor process if it is being run under Vidalia, disconnect
   * all TorControl signals, and exit Vidalia. */
  void aboutToQuit();
  /** Creates and displays Vidalia's About dialog. */
  void showAboutDialog();
  /** Creates and displays the Configuration dialog with the current page set
   * to <b>page</b>. */
  void showConfigDialog(ConfigDialog::Page page = ConfigDialog::General);
  /** Displays the Configuration dialog, set to the Server page. */
  void showServerConfigDialog();
  /** Called when the "show on startup" checkbox is toggled. */
  void toggleShowOnStartup(bool checked);
  /** Called when the web browser or IM client have stopped */
  void onSubprocessFinished(int exitCode, QProcess::ExitStatus exitStatus);
  /** Called periodically to check if the browser is running. If it is not,
   * exit Vidalia cleanly */
  void onCheckForBrowser();
  /** Called web the web browser failed to start */
  void onBrowserFailed(QString errmsg);
  /** Called web the IM client failed to start */
  void onIMFailed(QString errmsg);
  /** Called when the proxy server fails to start */
  void onProxyFailed(QString errmsg);

  /** Called when Tor has successfully established a circuit. */
  void circuitEstablished();
  /** Called when Tor thinks the user has tried to connect to a port that
   * typically is used for unencrypted applications. Warns the user and allows
   * them to ignore future warnings on <b>port</b>. */
  void warnDangerousPort(quint16 port, bool rejected);
  /** Called when Tor's bootstrapping status changes. <b>bse</b> represents
   * Tor's current estimate of its bootstrapping progress. */
  void bootstrapStatusChanged(const BootstrapStatus &bs);
  /** Called when Tor thinks its version is old or unrecommended, and displays
   * a message notifying the user. */
  void dangerousTorVersion(tc::TorVersionStatus reason,
                           const QString &version,
                           const QStringList &recommended);

  /** Called when torControl emits logMessage() */
  void log(tc::Severity type, const QString &message);
  /** Toggles the color for the Message Log button */
  void warnButton();

#if defined(USE_AUTOUPDATE)
  /** Called when the user clicks the 'Check Now' button in the General
   * settings page. */
  void checkForUpdatesWithUi();
  /** Called when the update interval timer expires, notifying Vidalia that
   * we should check for updates again. */
  void checkForUpdates(bool showProgress = false);
  /** Called when the check for software updates fails. */
  void checkForUpdatesFailed(const QString &errmsg);
  /** Called when there is an update available for installation. */
  void updatesAvailable(UpdateProcess::BundleInfo bi,
                        const PackageList &packageList);
  /** Stops Tor (if necessary), installs any available for <b>bi</b>, and
   * restarts Tor (if necessary). */
  void installUpdates(UpdateProcess::BundleInfo bi);
  /** Called when all <b>numUpdates</b> software updates have been installed
   * successfully. */
  void updatesInstalled(int numUpdates);
  /** Called when an update fails to install. <b>errmsg</b> contains details
   * about the failure. */
  void installUpdatesFailed(const QString &errmsg);
#endif

#if defined(USE_MINIUPNPC)
  /** Called when a UPnP error occurs. */
  void upnpError(UPNPControl::UPNPError error);
#endif

private:
  enum TorStatus {
    Unset,      /**< Tor's status has not yet been set. */
    Stopping,   /**< Tor is in the process of shutting down. */
    Stopped,    /**< Tor is not running. */
    Starting,   /**< Tor is in the process of starting. */
    Started,    /**< Tor is currently running. */
    Authenticating, /**< Vidalia is authenticating to Tor. */
    Authenticated,  /**< Vidalia has authenticated to Tor. */
    CircuitEstablished /**< Tor has built a circuit. */
  };
  /** Create the actions on the tray menu or menubar */
  void createActions();
  /** Creates a tray icon with a context menu and adds it to the system
   * notification area. On Mac, we also set up an application menubar. */
  void createTrayIcon();
  /** Create the tray popup menu and it's submenus */
  QMenu* createTrayMenu();
  /** Creates a default menubar on Mac */
  void createMenuBar();
  /** Sets the current tray or dock icon image to <b>iconFile</b>. */
  void setTrayIcon(const QString &iconFile);
  /** Updates the UI to reflect Tor's current <b>status</b>. Returns the
   * previously set TorStatus value. */
  TorStatus updateTorStatus(TorStatus status);
  /** Start a web browser when given the directory containing the executable and profile */
  void launchBrowserFromDirectory();
  /** Starts the web browser, if appropriately configured */
  void startSubprocesses();
  /** Starts the proxy server, if appropriately configured */
  void startProxy();
  /** Converts a TorStatus enum value to a string for debug logging purposes. */
  QString toString(TorStatus status);
  /** Authenticates Vidalia to Tor's control port. */
  bool authenticate();
  /** Searches for and attempts to load the control authentication cookie.
   * This assumes the cookie is named 'control_auth_cookie'. If
   * <b>cookiePath</b> is empty, this method will search some default locations
   * depending on the current platform. <b>cookiePath</b> can point to either
   * a cookie file or a directory containing the cookie file. */
  QByteArray loadControlCookie(QString cookiePath = QString());
  /** Checks the status of the current version of Tor to see if it's old,
   * unrecommended, or obsolete. */
  void checkTorVersion();
  /** Alerts the user that their current Tor version is either obsolete or
   * no longer recommended. If Vidalia was built with auto-update support,
   * they will be given the option to check for available updates. */
  void displayTorVersionWarning();
  /** Sets the visibility of the startup status description and progress bar
   * to <b>visible</b>. */
  void setStartupProgressVisible(bool visible);
  /** Sets the progress bar completion value to <b>progressValue</b> and sets
   * the status text to <b>description</b>. */
  void setStartupProgress(int percentComplete, const QString &description);
  /** Adds the needed variables to the environment that the browser will
   * need */
  QStringList updateBrowserEnv();

  /** The current status of Tor. */
  TorStatus _status;
  /** Used to determine if the Tor process exiting was intentional or not */
  bool _isIntentionalExit;
  /** Tracks whether we started a delayed server shutdown. */
  bool _delayedShutdownStarted;
  /** Set to true if Vidalia started its own Tor process. */
  bool _isVidaliaRunningTor;
  /** A MessageLog object which handles logging Tor messages */
  MessageLog* _messageLog;
  /** A BandwidthGraph object which handles monitoring Tor bandwidth usage */
  BandwidthGraph* _bandwidthGraph;
  /** A NetViewer object which displays the Tor network graphically */
  NetViewer* _netViewer;
  /** A ConfigDialog object which lets the user configure Tor and Vidalia */
  ConfigDialog* _configDialog;
  /** A TorControl object that handles communication with Tor */
  TorControl* _torControl;
  /** A HelperProcess object that manages the web browser */
  HelperProcess* _browserProcess;
  /** A HelperProcess object that manages the IM client */
  HelperProcess* _imProcess;
  /** A HelperProcess object that manages the proxy server */
  HelperProcess* _proxyProcess;
  /** Remembers the control password between when we start Tor with a hash of
   * the password and when we need to provide the password itself. */
  QString _controlPassword;
  /** Set to true if we should use the control password saved in TorSettings
   * when authenticating to Tor. */
  bool _useSavedPassword;
  /** The Vidalia icon that sits in the tray. */
  QSystemTrayIcon _trayIcon;

#if defined(USE_AUTOUPDATE)
  /** Timer used to remind us to check for software updates. */
  QTimer _updateTimer;
  /** The auto-update process used to check for and download updates. */
  UpdateProcess _updateProcess;
  /** Dialog instance that is be used to show the progress of the auto-update
   * executable. */
  UpdateProgressDialog _updateProgressDialog;
  /** Set to true if Vidalia should restart Tor after a software upgrade. */
  bool _restartTorAfterUpgrade;
#endif
  /** The menubar (Mac OS X only). */
  QMenuBar *_menuBar;

  /** Defines the actions for the tray menu */
  QAction* _actionShowControlPanel;
  QAction* _actionStartStopTor;
  QAction* _actionShowConfig;
  QAction* _actionShowAbout;
  QAction* _actionExit;
  QAction* _actionShowBandwidth;
  QAction* _actionShowMessageLog;
  QAction* _actionShowHelp;
  QAction* _actionShowNetworkMap;
  QAction* _actionNewIdentity;

  quint16 _autoControlPort;

  /** Timer and bool used to flash buttons with a different color */
  QTimer *_warnTimer;
  bool _flashToggle;

  Ui::MainWindow ui; /**< Qt Designer generated object. */
};

#endif


