//
//  VRTSurface.h
//  ViroReact
//
//  Created by Andy Chu on 1/11/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTControl.h"

@interface VRTSurface : VRTControl

@property (nonatomic, assign) float width;
@property (nonatomic, assign) float height;

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@end
