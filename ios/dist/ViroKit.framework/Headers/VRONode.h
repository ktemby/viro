//
//  VRONode.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/15/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VRONode_h
#define VRONode_h

#include <memory>
#include <stack>
#include <vector>
#include <string>
#include <algorithm>
#include "VROMatrix4f.h"
#include "VROQuaternion.h"
#include "VRORenderContext.h"
#include "VRODriver.h"
#include "VRORenderParameters.h"
#include "VROAnimatable.h"
#include "VROBoundingBox.h"
#include "VROSortKey.h"
#include "VROLog.h"
#include "VROEventDelegate.h"
#include "VROSound.h"
#include "VROThreadRestricted.h"

class VROGeometry;
class VROLight;
class VROAction;
class VRONodeCamera;
class VROHitTestResult;
class VROConstraint;

extern bool kDebugSortOrder;

class VRONode : public VROAnimatable, public VROThreadRestricted {
    
public:
    
    static void resetDebugSortIndex();
    
    /*
     Default constructor.
     */
    VRONode();
    
    /*
     Copy constructor. This copies the node but *not* the underlying
     geometries or lights. Instead, these are shared by reference with the
     copied node. Additionally, this constructor will not copy child nodes.
     
     To copy child nodes recursively, invoke the clone() function.
     */
    VRONode(const VRONode &node);
    virtual ~VRONode();
    
    /*
     Copy constructor that recursively copies all child nodes. This copies
     the node but *not* the underlying geometries or lights. Instead, these
     are shared by reference with the copied node.
     */
    std::shared_ptr<VRONode> clone();

    void render(int elementIndex,
                std::shared_ptr<VROMaterial> &material,
                const VRORenderContext &context,
                std::shared_ptr<VRODriver> &driver);
    
    void updateSortKeys(uint32_t depth,
                        VRORenderParameters &params,
                        const VRORenderContext &context,
                        std::shared_ptr<VRODriver> &driver);
    void getSortKeys(std::vector<VROSortKey> *outKeys);
    
    std::vector<std::shared_ptr<VROLight>> &getComputedLights() {
        return _computedLights;
    }
    
    void setGeometry(std::shared_ptr<VROGeometry> geometry) {
        passert_thread();
        _geometry = geometry;
    }
    std::shared_ptr<VROGeometry> getGeometry() const {
        return _geometry;
    }
    
    /*
     Camera.
     */
    void setCamera(std::shared_ptr<VRONodeCamera> camera) {
        passert_thread();
        _camera = camera;
    }
    const std::shared_ptr<VRONodeCamera> &getCamera() const {
        return _camera;
    }
    
    /*
     Transforms.
     */
    void computeTransform(const VRORenderContext &context, VROMatrix4f parentTransforms);
    VROVector3f getTransformedPosition() const;
    
    VROVector3f getPosition() const {
        return _position;
    }
    VROVector3f getScale() const {
        return _scale;
    }
    VROQuaternion getRotation() const {
        return _rotation;
    }
    VROVector3f getRotationEuler() const {
        return _euler;
    }
    
    /*
     Set the rotation, position, or scale. Animatable.
     */
    void setRotation(VROQuaternion rotation);
    void setPosition(VROVector3f position);
    void setScale(VROVector3f scale);
    
    /*
     Set the rotation as a vector of Euler angles. Using this method
     will update the Euler angles stored internally in a predictable
     way. Setting rotation by quaternion updates Euler angles in an 
     unpredictable way (i.e. the quaternion axis may change).
     */
    void setRotationEuler(VROVector3f euler);
    
    /*
     These piecewise setters are used in order to change one axis
     only, without altering the remaining axes. Useful when animating
     across multiple axes across separate calls. Animatable.
     */
    void setPositionX(float x);
    void setPositionY(float y);
    void setPositionZ(float z);
    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);
    void setRotationEulerX(float radians);
    void setRotationEulerY(float radians);
    void setRotationEulerZ(float radians);
    
    float getOpacity() const {
        return _opacity;
    }
    void setOpacity(float opacity);

    void setHighAccuracyGaze(bool enabled);

    bool getHighAccuracyGaze() const {
        return _highAccuracyGaze;
    }

    bool isHidden() const {
        return _hidden;
    }
    void setHidden(bool hidden);
    
    int getRenderingOrder() const {
        return _renderingOrder;
    }
    void setRenderingOrder(int renderingOrder) {
        _renderingOrder = renderingOrder;
    }
    
    bool isHierarchicalRendering() const {
        return _hierarchicalRendering;
    }
    void setHierarchicalRendering(bool hierarchicalRendering) {
        _hierarchicalRendering = hierarchicalRendering;
    }
    
    /*
     Lights.
     */
    void addLight(std::shared_ptr<VROLight> light) {
        passert_thread();
        _lights.push_back(light);
    }
    void removeLight(std::shared_ptr<VROLight> light) {
        passert_thread();
        _lights.erase(
                      std::remove_if(_lights.begin(), _lights.end(),
                                     [light](std::shared_ptr<VROLight> candidate) {
                                         return candidate == light;
                                     }), _lights.end());
    }
    void removeAllLights() {
        passert_thread();
        _lights.clear();
    }
    std::vector<std::shared_ptr<VROLight>> &getLights() {
        return _lights;
    }

    /*
     Sounds.
     */
    void addSound(std::shared_ptr<VROSound> sound) {
        passert_thread();
        if (sound->getType() == VROSoundType::Spatial) {
            _sounds.push_back(sound);
        }
    }
    void removeSound(std::shared_ptr<VROSound> sound) {
        passert_thread();
        _sounds.erase(
                std::remove_if(_sounds.begin(), _sounds.end(),
                               [sound](std::shared_ptr<VROSound> candidate) {
                                   return candidate == sound;
                               }), _sounds.end());
    }

    /*
     Child management.
     */
    void addChildNode(std::shared_ptr<VRONode> node) {
        passert_thread();
        passert (node);
        
        _subnodes.push_back(node);
        node->_supernode = std::static_pointer_cast<VRONode>(shared_from_this());
    }
    void removeFromParentNode() {
        passert_thread();
        
        std::shared_ptr<VRONode> supernode = _supernode.lock();
        if (supernode) {
            std::vector<std::shared_ptr<VRONode>> &parentSubnodes = supernode->_subnodes;
            parentSubnodes.erase(
                                 std::remove_if(parentSubnodes.begin(), parentSubnodes.end(),
                                                [this](std::shared_ptr<VRONode> node) {
                                                    return node.get() == this;
                                                }), parentSubnodes.end());
            _supernode.reset();
        }
    }
    
    /*
     Return a copy of the subnode list.
     */
    std::vector<std::shared_ptr<VRONode>> getSubnodes() const {
        return _subnodes;
    }
    
    /*
     Return the parent node. Null if this node is root or does not have a parent.
     */
    std::shared_ptr<VRONode> getParentNode() const {
        return _supernode.lock();
    }
    
    /*
     Action management.
     */
    void runAction(std::shared_ptr<VROAction> action);
    void removeAction(std::shared_ptr<VROAction> action);
    void removeAllActions();
    
    /*
     Hit testing.
     */
    VROBoundingBox getBoundingBox(const VRORenderContext &context);
    std::vector<VROHitTestResult> hitTest(const VROCamera &camera, VROVector3f origin, VROVector3f ray,
                                          bool boundsOnly = false);
    
    void setSelectable(bool selectable) {
        _selectable = selectable;
    }

    void setEventDelegate(std::shared_ptr<VROEventDelegate> delegate) {
        passert_thread();
        
        auto autoWeakDelegate = delegate;
        _eventDelegateWeak = autoWeakDelegate;
    }

    std::shared_ptr<VROEventDelegate> getEventDelegate(){
        if (_eventDelegateWeak.expired()){
            return nullptr;
        }
        return _eventDelegateWeak.lock();
    }

    bool isSelectable() const {
        return _selectable;
    }
    
    /*
     Constraints.
     */
    void addConstraint(std::shared_ptr<VROConstraint> constraint);
    void removeConstraint(std::shared_ptr<VROConstraint> constraint);
    void removeAllConstraints();

protected:
    
    /*
     The node's parent and children.
     */
    std::vector<std::shared_ptr<VRONode>> _subnodes;
    std::weak_ptr<VRONode> _supernode;
    
private:
    
    std::shared_ptr<VROGeometry> _geometry;
    std::vector<std::shared_ptr<VROLight>> _lights;
    std::vector<std::shared_ptr<VROSound>> _sounds;
    std::shared_ptr<VRONodeCamera> _camera;
    
    VROVector3f _scale;
    VROVector3f _position;
    
    /*
     Rotation is stored as a quaternion, but we also maintain euler angles
     for use in animation (since we cannot additively rotate by reading euler
     angles from a quaternion and writing them again).
     */
    VROQuaternion _rotation;
    VROVector3f _euler;
    
    /*
     User-defined rendering order for this node.
     */
    int _renderingOrder;
    
    /*
     Parameters computed by descending down the tree. These are updated whenever
     any parent or this node itself is updated.
     */
    VROMatrix4f _computedTransform;
    VROMatrix4f _computedInverseTransposeTransform;
    float _computedOpacity;
    std::vector<std::shared_ptr<VROLight>> _computedLights;
    VROVector3f _computedPosition;
    
    /*
     True if this node is hidden. Hidden nodes are not rendered, and do not 
     respond to tap events. Hiding a node within an animation results in a 
     fade-out animation. The _opacityFromHiddenFlag is the opacity as derived
     from _hidden: 0.0 if _hidden is true, 1.0 if _hidden is false, or somewhere
     in-between during animation.
     */
    bool _hidden;
    float _opacityFromHiddenFlag;
    
    /*
     The opacity of the node (0.0 is transparent, 1.0 is opaque). When opacity
     drops below a threshold value, the node is hidden. This opacity is set by
     the user.
     */
    float _opacity;
    
    /*
     True if this node is selectable by hit testing. Defaults to true.
     */
    bool _selectable;

    /*
     Delegate through which events are notified from the VROEventManager.
     */
    std::weak_ptr<VROEventDelegate> _eventDelegateWeak;

    /*
     True if we want to perform more accurate hit testing against this node's geometry
     rather than it's bounding box.
     */
    bool _highAccuracyGaze;

    /*
     Active actions on this node.
     */
    std::vector<std::shared_ptr<VROAction>> _actions;
    
    /*
     Constraints on the node, which can modify the node's transformation matrix.
     */
    std::vector<std::shared_ptr<VROConstraint>> _constraints;
    
    /*
     True indicates that this node's descendants (children, grand-children, and so on)
     should be rendered by order of their scene graph depth. Useful when rendering
     2D layouts like flexbox views. Defaults to false.
     */
     bool _hierarchicalRendering;
    
    /*
     Action processing: execute all current actions and remove those that are
     expired.
     */
    void processActions();
    
    /*
     Hit test helper functions.
     */
    void hitTest(const VROCamera &camera, VROVector3f origin, VROVector3f ray, VROMatrix4f parentTransform,
                 bool boundsOnly, std::vector<VROHitTestResult> &results);
    bool hitTestGeometry(VROVector3f origin, VROVector3f ray, VROMatrix4f transform);

};

#endif /* VRONode_h */
