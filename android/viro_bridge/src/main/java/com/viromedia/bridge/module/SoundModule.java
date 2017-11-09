/*
 * Copyright © 2017 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.module;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.bridge.ReadableMap;
import com.facebook.react.bridge.ReadableMapKeySetIterator;
import com.viro.core.SoundData;

import java.util.HashMap;

public class SoundModule extends ReactContextBaseJavaModule {

    private final HashMap<String, SoundData> mSoundDataMap = new HashMap<>();

    public SoundModule(ReactApplicationContext context) {
        super(context);
    }

    @Override
    public String getName() {
        return "VRTSoundModule";
    }

    /**
     * This function takes a map of keys to sounds and creates SoundData objects
     * to prefetch the data before storing them in a local map. We currently only
     * support external urls (web-based).
     *
     * @param soundMap a map of String keys to String url's.
     */
    @ReactMethod
    public void preloadSounds(ReadableMap soundMap) {
        ReadableMapKeySetIterator iter = soundMap.keySetIterator();
        while(iter.hasNextKey()) {
            String key = iter.nextKey();
            SoundData nativeSoundData = new SoundData(soundMap.getString(key), false);
            mSoundDataMap.put(key, nativeSoundData);
        }
    }

    @ReactMethod
    public void unloadSounds(ReadableArray soundArray) {
        for (int i = 0; i < soundArray.size(); i++) {
            String keyToRemove = soundArray.getString(i);
            SoundData dataToRemove = mSoundDataMap.get(keyToRemove);
            if (dataToRemove != null) {
                dataToRemove.destroy();
                mSoundDataMap.remove(keyToRemove);
            }
        }
    }

    public SoundData getSoundData(String name) {
        return mSoundDataMap.get(name);
    }
}
