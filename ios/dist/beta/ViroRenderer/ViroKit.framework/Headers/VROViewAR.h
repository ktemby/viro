//
//  VROViewAR.h
//  ViroRenderer
//
//  Created by Raj Advani on 5/31/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import "VROView.h"
#import "VROARSession.h"
#import "VROARHitTestResult.h"

// TODO VIRO-1355 Remove after event delegate integration
typedef void (^VROViewARTapHandler)(VROARHitTestResult, std::shared_ptr<VROARSession>, std::shared_ptr<VROScene>);

class VROARSessionDelegate;
class VRORendererConfiguration;

@interface VROViewAR : GLKView <VROView, UIGestureRecognizerDelegate>

@property (readwrite, nonatomic) BOOL suspended;

- (instancetype)initWithFrame:(CGRect)frame
                       config:(VRORendererConfiguration)config
                      context:(EAGLContext *)context
               worldAlignment:(VROWorldAlignment)worldAlignment;

- (void)setARSessionDelegate:(std::shared_ptr<VROARSessionDelegate>)delegate;

/*
 Should be invoked before this object gets deallocated, to clean up GL
 resources on the rendering thread before the underlying EAGLContext used
 by this view is destroyed. This is required to prevent deadlocks in
 CVOpenGLTextureCache, which hangs on dealloc if the EAGLContext it's using
 is already gone.
 */
- (void)deleteGL;

/*
 Sets the paused state of the underlying CADisplayLink
 */
- (void)setPaused:(BOOL)paused;

/*
 Performs an AR hit test with the given ray assuming origin is the camera
 */
- (std::vector<VROARHitTestResult>)performARHitTest:(VROVector3f)ray;

/*
 Performs an AR hit test with the given 2D point on the screen
 */
- (std::vector<VROARHitTestResult>)performARHitTestWithPoint:(int)x y:(int)y;

/*
 Returns the ARSession
 */
- (std::shared_ptr<VROARSession>)getARSession;

// TODO VIRO-1355 Remove after event delegate integration
@property (readwrite, nonatomic) VROViewARTapHandler tapHandler;

@end
