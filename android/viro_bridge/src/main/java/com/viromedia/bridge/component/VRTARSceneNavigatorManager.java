/**
 * Copyright © 2017 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.component;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.uimanager.ThemedReactContext;
import com.facebook.react.uimanager.annotations.ReactProp;

/**
 * ARSceneNavigatorManager for building a {@link VRTARSceneNavigator}
 * corresponding to the ViroARNavigator.js control.
 */
public class VRTARSceneNavigatorManager extends VRTViroViewGroupManager<VRTARSceneNavigator> {

    public VRTARSceneNavigatorManager(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTARSceneNavigator";
    }

    @Override
    protected VRTARSceneNavigator createViewInstance(ThemedReactContext reactContext) {
        return new VRTARSceneNavigator(reactContext);
    }

    @ReactProp(name = "currentSceneIndex")
    public void setCurrentSceneIndex(VRTARSceneNavigator view, int selectedIndex) {
        view.setCurrentSceneIndex(selectedIndex);
    }

    @ReactProp(name = "apiKey")
    public void setApiKey(VRTARSceneNavigator view, String apiKey) {
        view.setApiKey(apiKey);
    }
}
