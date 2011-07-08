// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_DOWNLOAD_DOWNLOAD_TAB_HELPER_H_
#define CHROME_BROWSER_UI_DOWNLOAD_DOWNLOAD_TAB_HELPER_H_
#pragma once

#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "chrome/browser/download/save_package.h"
#include "content/browser/tab_contents/tab_contents_observer.h"

class DownloadItem;
class DownloadTabHelperDelegate;
class TabContentsWrapper;

// Per-tab download controller. Handles dealing with various per-tab download
// duties.
class DownloadTabHelper : public TabContentsObserver {
 public:
  explicit DownloadTabHelper(TabContentsWrapper* tab_contents);
  virtual ~DownloadTabHelper();

  DownloadTabHelperDelegate* delegate() const { return delegate_; }
  void set_delegate(DownloadTabHelperDelegate* d) { delegate_ = d; }

  // Prepare for saving the current web page to disk.
  void OnSavePage();

  // Prepare for saving the URL to disk.
  // URL may refer to the iframe on the page.
  void OnSaveURL(const GURL& url);

  // Save page with the main HTML file path, the directory for saving resources,
  // and the save type: HTML only or complete web page. Returns true if the
  // saving process has been initiated successfully.
  // This method is used in automated testing to bypass prompting the user for
  // file names. Instead, the names and paths are hard coded rather than
  // running them through file name sanitation and extension / mime checking.
  bool SavePage(const FilePath& main_file, const FilePath& dir_path,
                SavePackage::SavePackageType save_type);

  // Returns the SavePackage which manages the page saving job. May be NULL.
  SavePackage* save_package() const { return save_package_.get(); }

  // Notifies the delegate that a download is about to be started.
  // This notification is fired before a local temporary file has been created.
  bool CanDownload(int request_id);

  // Notifies the delegate that a download started.
  void OnStartDownload(DownloadItem* download);

 private:
  FRIEND_TEST_ALL_PREFIXES(SavePageBrowserTest, SaveFolder1);
  FRIEND_TEST_ALL_PREFIXES(SavePageBrowserTest, SaveFolder2);
  FRIEND_TEST_ALL_PREFIXES(SavePageBrowserTest, SaveFolder3);

  // Used in automated testing to bypass prompting the user for file names.
  // The difference between SavePageBasedOnDefaultPrefs() and SavePage()
  // is whether the default folder prefs are used. In case of SavePage(),
  // we need to give it the file path to which the file is saved.
  // On the other hand, in case of SavePageBasedOnDefaultPrefs(),
  // we need not to give the file path since the file path is determined
  // based on the default folder prefs. This method returns the title
  // of the current tab.
  string16 SavePageBasedOnDefaultPrefs();

  // TabContentsObserver overrides.
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void DidGetUserGesture() OVERRIDE;

  // SavePackage, lazily created.
  scoped_refptr<SavePackage> save_package_;

  // Owning TabContentsWrapper.
  TabContentsWrapper* tab_contents_wrapper_;

  // Delegate for notifying our owner (usually Browser) about stuff. Not owned
  // by us.
  DownloadTabHelperDelegate* delegate_;

  DISALLOW_COPY_AND_ASSIGN(DownloadTabHelper);
};

#endif  // CHROME_BROWSER_UI_DOWNLOAD_DOWNLOAD_TAB_HELPER_H_
