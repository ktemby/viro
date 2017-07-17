//
//  VRTSkyboxManager.m
//  React
//
//  Created by Raj Advani on 10/5/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <React/RCTImageSource.h>
#import "VRTSkyboxManager.h"
#import "VRTSkybox.h"
#import "VRTCubeMap.h"

@implementation VRTSkyboxManager

RCT_EXPORT_MODULE()

RCT_EXPORT_VIEW_PROPERTY(source, VRTCubeMap)
RCT_EXPORT_VIEW_PROPERTY(color, UIColor)
RCT_EXPORT_VIEW_PROPERTY(onViroSkyBoxLoadStart, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(onViroSkyBoxLoadEnd, RCTDirectEventBlock)
RCT_EXPORT_VIEW_PROPERTY(format, VROTextureInternalFormat)

- (VRTSkybox *)view
{
    return [[VRTSkybox alloc] initWithBridge:self.bridge];
}

@end
