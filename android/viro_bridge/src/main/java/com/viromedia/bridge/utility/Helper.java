package com.viromedia.bridge.utility;

import android.content.Context;
import android.net.Uri;
import android.support.annotation.Nullable;

import com.facebook.react.bridge.ReadableArray;
import com.facebook.react.views.imagehelper.ResourceDrawableIdHelper;
import com.viro.core.Vector;

/**
 * Class containing few common helper methods.
 * Note - Feel free to split / rename this class as this class gets more methods
 */

public class Helper {

    private static final String RESOURCE_SCHEME = "res";

    public static @Nullable float[] toFloatArray(@Nullable ReadableArray value) {
        if (value == null){
            return null;
        }

        float[] result = new float[value.size()];
        for (int i = 0; i < value.size(); i++) {
            result[i] = (float) value.getDouble(i);
        }
        return result;
    }

    public static Vector toVector(ReadableArray value) {
        if (value == null) {
            return null;
        }

        if (value.size() != 3) {
            throw new IllegalArgumentException("Vectors require 3 values.");
        }

        return new Vector((float) value.getDouble(0), (float) value.getDouble(1), (float) value.getDouble(2));
    }

    public static Vector toRadiansVector(float[] degreesArray) {
        return new Vector(Math.toRadians(degreesArray[0]),
                Math.toRadians(degreesArray[1]), Math.toRadians(degreesArray[2]));
    }

    /**
     * This method takes a path and creates a Uri for it. If given a normal http path,
     * it'll leave it alone, but given a resource file name (that React gives us), it'll
     * return the corresponding android-resource:// uri (this is what React Native's
     * Image uses/does).
     */
    public static Uri parseUri(String path, Context context) {
        if (path == null) {
            return null;
        }

        Uri tempUri = Uri.parse(path);
        // if the scheme is null, then it's a local resource
        return tempUri.getScheme() == null ? computeLocalUri(path, context) : tempUri;
    }

    public static boolean isResourceUri(Uri uri) {
        return uri.getScheme().equals(RESOURCE_SCHEME);
    }

    private static Uri computeLocalUri(String path, Context context) {
        return ResourceDrawableIdHelper.getInstance().getResourceDrawableUri(context, path);
    }
}
