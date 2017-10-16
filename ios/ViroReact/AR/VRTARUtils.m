//
//  VRTARUtils.m
//  ViroReact
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import "VRTARUtils.h"

@implementation VRTARUtils

+ (NSDictionary *)createDictionaryFromAnchor:(std::shared_ptr<VROARAnchor>) anchor {
    
    NSMutableDictionary *dict = [[NSMutableDictionary alloc] init];
    
    [dict setObject:[NSString stringWithUTF8String:anchor->getId().c_str()] forKey:@"anchorId"];
    
    VROMatrix4f transform =  anchor->getTransform();
    VROVector3f position = transform.extractTranslation();
    VROVector3f scale = transform.extractScale();
    VROVector3f rotation = transform.extractRotation(scale).toEuler();
    
    [dict setObject:@[@(position.x), @(position.y), @(position.z)] forKey:@"position"];
    [dict setObject:@[@(scale.x), @(scale.y), @(scale.z)] forKey:@"scale"];
    [dict setObject:@[@(rotation.x), @(rotation.y), @(rotation.z)] forKey:@"rotation"];

    // default type is "anchor", override below.
    [dict setObject:@"anchor" forKey:@"type"];
    
    std::shared_ptr<VROARPlaneAnchor> planeAnchor = std::dynamic_pointer_cast<VROARPlaneAnchor>(anchor);
    if (planeAnchor) {
        [dict setObject:@"plane" forKey:@"type"];
        [dict setObject:@[@(planeAnchor->getCenter().x), @(planeAnchor->getCenter().y), @(planeAnchor->getCenter().z)] forKey:@"center"];
        [dict setObject:@(planeAnchor->getExtent().x) forKey:@"width"];
        [dict setObject:@(planeAnchor->getExtent().z) forKey:@"height"];
        [dict setObject:@"horizontal" forKey:@"alignment"];
    }
    
    return dict;
}


@end
