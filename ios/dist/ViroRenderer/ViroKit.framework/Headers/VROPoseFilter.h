//
//  VROPoseFilter.h
//  ViroKit
//
//  Created by Raj Advani on 2/26/19.
//  Copyright © 2019 Viro Media. All rights reserved.
//

#ifndef VROPoseFilter_h
#define VROPoseFilter_h

#include <map>
#include "VROBodyTracker.h"

/*
 Superclass for enabling spatial and temporal filtering of body pose data as
 received from an ML model. All data that passes the spatial filter is included
 in a 'tracking window' of recorded data, onto which temporal filtering is then
 applied.
 
 Filters can be stacked upon one another as well.
 */
class VROPoseFilter {
public:
    
    /*
     Create a new pose filter that will operate on all received joints
     that are *above* the given confidence level. For temporal filtering,
     the filter will maintain a tracking window of all joint positions
     received over the past trackingPeriodMs.
     */
    VROPoseFilter(float trackingPeriodMs, float confidenceThreshold) :
        _trackingPeriodMs(trackingPeriodMs),
        _confidenceThreshold(confidenceThreshold) {
            _combinedFrame = newPoseFrame();
        }
    virtual ~VROPoseFilter() {}
    
    /*
     Filter a new set of joints. Returns the filtered set of joints.
     */
    VROPoseFrame filterJoints(const VROPoseFrame &frame);
    
protected:
    
    /*
     Apply a spatial filter to the given new frame of joints. All previous frames
     are provided in pastFrames and combinedFrames. Combined frames contains the
     same data as pastFrames, but with the pose structures flattened out.
     
     When spatial filtering, neither pastFrames nor combinedFrame contain the newFrame.
     
     Return a new frame with all spatially incoherent results thrown out. The returned
     joints will be added to the tracking window for use in temporal filtering. Note
     that joints that are thrown out will *not* be accessed by the temporal filter.
     
     The default behavior is to not do any spatial filtering (the provided newFrame
     is returned).
     */
    virtual VROPoseFrame spatialFilter(const std::vector<VROPoseFrame> &pastFrames, const VROPoseFrame &combinedFrame,
                                       const VROPoseFrame &newFrame) {
        return newFrame;
    }
    
    /*
     Apply a temporal filter to the given new frame of joints. All previous frames,
     *including the new frame*, are provided in frames and combinedFrames.
     Combined frames contains the same data as frames, but with the pose structures
     flattened out.
     
     Return the filtered joints.
     */
    virtual VROPoseFrame temporalFilter(const std::vector<VROPoseFrame> &frames, const VROPoseFrame &combinedFrame,
                                        const VROPoseFrame &newFrame) = 0;
    
private:
    
    float _trackingPeriodMs;
    float _confidenceThreshold;
    std::vector<VROPoseFrame> _frames;
    VROPoseFrame _combinedFrame;
    
};

#endif /* VROPoseFilter_h */
