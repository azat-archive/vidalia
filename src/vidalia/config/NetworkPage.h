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
** \file NetworkPage.h
** \brief Network and firewall configuration options
*/

#ifndef _NETWORKPAGE_H
#define _NETWORKPAGE_H

#include "ui_NetworkPage.h"
#include "ConfigPage.h"
#include "Vidalia.h"

#include <QPoint>


class NetworkPage : public ConfigPage
{
  Q_OBJECT

public:
  /** Default Constructor */
  NetworkPage(QWidget *parent = 0);

  /** Saves the changes on this page */
  bool save(QString &errmsg);
  /** Loads the settings for this page */
  void load();

  /** Applies the network configuration settings to Tor. Returns true if the
   * settings were applied successfully. Otherwise, <b>errmsg</b> is set and
   * false is returned. */
  bool apply(QString &errmsg);
  /** Reverts the server configuration settings to their values at the last
   * time they were successfully applied to Tor. */
  void revert();
  /** Returns true if the user has changed their server settings since the
   * last time they were applied to Tor. */
  bool changedSinceLastApply();
  /** Called when the user changes the UI translation. */
  virtual void retranslateUi();

private slots:
  /** Adds a bridge to the bridge list box. */
  void addBridge();
  /** Removes one or more selected bridges from the bridge list box. */
  void removeBridge();
  /** Copies all selected bridges to the clipboard. */
  void copySelectedBridgesToClipboard();
  /** Called when the user right-clicks on a bridge and displays a context
   * menu. */
  void bridgeContextMenuRequested(const QPoint &pos);
  /** Called when the user changes which bridges they have selected. */
  void bridgeSelectionChanged();
  /** Called when a link in a label is clicked. <b>url</b> is the target of
   * the clicked link.*/
  void onLinkActivated(const QString &url);

  /** Disable proxy username and password fields when the user wants to use
   * a SOCKS 4 proxy. */
  void proxyTypeChanged(int selection);

private:
  /** Qt Designer generated object */
  Ui::NetworkPage ui;
};

#endif

