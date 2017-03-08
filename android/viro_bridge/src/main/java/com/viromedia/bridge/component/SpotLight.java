/**
 * Copyright © 2016 Viro Media. All rights reserved.
 */
package com.viromedia.bridge.component;

import android.content.Context;

import com.viro.renderer.jni.NodeJni;
import com.viro.renderer.jni.SpotLightJni;

public class SpotLight extends Light {
    private static final float[] DEFAULT_POSITION = {0, 0, 0};

    private float[] mDirection;
    private float[] mPosition = DEFAULT_POSITION;
    private float mAttenuationStartDistance;
    private float mAttenuationEndDistance;
    private float mInnerAngle;
    private float mOuterAngle;

    private SpotLightJni mNativeLight;

    public SpotLight(Context context) {
        super(context);
    }

    @Override
    public void addToNode(NodeJni nodeJni) {

        mNativeLight.addToNode(nodeJni);
    }

    @Override
    public void removeFromNode(NodeJni nodeJni) {

        mNativeLight.removeFromNode(nodeJni);
    }

    @Override
    public void onTearDown(){
        if (mNativeLight != null){
            mNativeLight.destroy();
            mNativeLight = null;
        }
        super.onTearDown();
    }

    public float[] getDirection() {
        return mDirection;
    }

    public void setDirection(float[] direction) {
        if (direction == null) {
            throw new IllegalArgumentException("Direction cannot be null for SpotLight");
        }
        this.mDirection = direction;
    }

    public float[] getPosition() {
        return mPosition;
    }

    public void setPosition(float[] position) {
        this.mPosition = position;
    }

    public float getAttenuationStartDistance() {
        return mAttenuationStartDistance;
    }

    public void setAttenuationStartDistance(float attenuationStartDistance) {
        this.mAttenuationStartDistance = attenuationStartDistance;
    }

    public float getAttenuationEndDistance() {
        return mAttenuationEndDistance;
    }

    public void setAttenuationEndDistance(float attenuationEndDistance) {
        this.mAttenuationEndDistance = attenuationEndDistance;
    }

    public float getInnerAngle() {
        return mInnerAngle;
    }

    public void setInnerAngle(float innerAngle) {
        this.mInnerAngle = mInnerAngle;
    }

    public float getOuterAngle() {
        return mOuterAngle;
    }

    public void setOuterAngle(float outerAngle) {
        this.mOuterAngle = outerAngle;
    }

    @Override
    public void onPropsSet() {
        super.onPropsSet();

        if (mNativeLight == null) {
            mNativeLight = new SpotLightJni(mColor, mAttenuationStartDistance, mAttenuationEndDistance,
                    mPosition, mDirection, mInnerAngle, mOuterAngle);
        } else {
            mNativeLight.setColor(mColor);
            mNativeLight.setAttenuationStartDistance(mAttenuationStartDistance);
            mNativeLight.setAttenuationEndDistance(mAttenuationEndDistance);
            mNativeLight.setPosition(mPosition);
            mNativeLight.setDirection(mDirection);
            mNativeLight.setInnerAngle(mInnerAngle);
            mNativeLight.setOuterAngle(mOuterAngle);
        }
    }
}
