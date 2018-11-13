/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

package com.facebook.react.devsupport;

import javax.annotation.Nullable;

import android.content.Context;
import android.graphics.PixelFormat;
import android.os.Build;
import android.view.WindowManager;
import android.widget.FrameLayout;

import com.facebook.react.bridge.ReactContext;

/**
 * Helper class for controlling overlay view with FPS and JS FPS info
 * that gets added directly to @{link WindowManager} instance.
 */
/* package */ class DebugOverlayController {

  private final WindowManager mWindowManager;
  private final ReactContext mReactContext;

  private @Nullable FrameLayout mFPSDebugViewContainer;

  /**
   * edited by woogie.kim
   * 2018.09.23
   */
  /**
   * OREO 용 dialog window type contants
   */
  private static final short WindowManager_LayoutParams_TYPE_APPLICATION_OVERLAY = 2038;

  /**
   * OREO 용 Build.VERSION_CODES.O
   */
  private static final short Build_VERSION_CODES_O = 26;

  public DebugOverlayController(ReactContext reactContext) {
    mReactContext = reactContext;
    mWindowManager = (WindowManager) reactContext.getSystemService(Context.WINDOW_SERVICE);
  }

  public void setFpsDebugViewVisible(boolean fpsDebugViewVisible) {
    if (fpsDebugViewVisible && mFPSDebugViewContainer == null) {
      mFPSDebugViewContainer = new FpsView(mReactContext);

      /**
       * edited by woogie.kim
       * 2018.09.23
       * 안드로이드 OREO 이상에서 dialog 창의 타입은 TYPE_APPLICATION_OVERLAY 이지만 현재 ReactAndroid 패키지에는 상수가 존재하지 않으므로 상수 패치함
       */
      short layoutFlag = 0;
      if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
        layoutFlag = WindowManager.LayoutParams.TYPE_TOAST;
      }
      if (Build.VERSION.SDK_INT >= Build_VERSION_CODES_O) {
        layoutFlag = WindowManager_LayoutParams_TYPE_APPLICATION_OVERLAY;
      }
      if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M)
      {
        layoutFlag = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;
      }

      WindowManager.LayoutParams params = new WindowManager.LayoutParams(
          WindowManager.LayoutParams.MATCH_PARENT,
          WindowManager.LayoutParams.MATCH_PARENT,
              layoutFlag,
          WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
              | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
          PixelFormat.TRANSLUCENT);
      mWindowManager.addView(mFPSDebugViewContainer, params);
    } else if (!fpsDebugViewVisible && mFPSDebugViewContainer != null) {
      mFPSDebugViewContainer.removeAllViews();
      mWindowManager.removeView(mFPSDebugViewContainer);
      mFPSDebugViewContainer = null;
    }
  }
}
