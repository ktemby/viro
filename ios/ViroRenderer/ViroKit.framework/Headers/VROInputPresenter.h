//
//  VROInputPresenter.h
//  ViroRenderer
//
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROInputPresenter_H
#define VROInputPresenter_H

#include <memory>
#include "VROEventDelegate.h"
#include "VRORenderContext.h"
#include "VROReticle.h"
#include "VRONode.h"
#include "VROMath.h"
#include "VROInputType.h"

static const float kReticleSizeMultiple = 3;
static const bool kDebugSceneBackgroundDistance = false;

/**
 * VROInputPresenter contains all UI view implementations to be displayed for a given
 * VROInputController.
 */
class VROInputPresenter : public VROEventDelegate {
public:
    
    VROInputPresenter() {
        _reticle = nullptr;
        _rootNode = std::make_shared<VRONode>();
    }

    ~VROInputPresenter() {}

    std::shared_ptr<VRONode> getRootNode(){
        return _rootNode;
    }

    void setEventDelegate(std::shared_ptr<VROEventDelegate> delegate){
        _eventDelegateWeak = delegate;
    }

    virtual void onHover(int source, bool isHovering) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnHover)){
            delegate->onHover(source, isHovering);
        }
    }

    virtual void onClick(int source, ClickState clickState) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnClick)){
            delegate->onClick(source, clickState);
        }
    }

    virtual void onTouch(int source, TouchState state, float x, float y){
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnTouch)){
            delegate->onTouch(source, state, x, y);
        }
    }

    virtual void onMove(int source, VROVector3f rotation, VROVector3f position) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnMove)){
            delegate->onMove(source, rotation, position);
        }
    }

    virtual void onControllerStatus(int source, ControllerStatus status) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnControllerStatus)){
            delegate->onControllerStatus(source, status);
        }
    }

    virtual void onSwipe(int source, SwipeState swipeState) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnSwipe)){
            delegate->onSwipe(source, swipeState);
        }
    }

    virtual void onScroll(int source, float x, float y) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnScroll)){
            delegate->onScroll(source, x, y);
        }
    }

    virtual void onGazeHit(int source, const VROHitTestResult &hit) {
        //No-op
    }

    virtual void onDrag(int source, VROVector3f newPosition) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnDrag)){
            delegate->onDrag(source, newPosition);
        }
    }

    virtual void onFuse(int source, float timeToFuseRatio) {
        std::shared_ptr<VROEventDelegate> delegate = getDelegate();
        if (delegate != nullptr && delegate->isEventEnabled(VROEventDelegate::EventAction::OnFuse)){
            delegate->onFuse(source, timeToFuseRatio);
        }

        // TimeToFuseRatio is (time that has passed since fuse began) / (total time to fuse).
        // When the timeToFuseRatio reaches 1, it is an indication that the node has been "onFused".
        if (timeToFuseRatio == kOnFuseReset){
            _reticle->stopFuseAnimation();
        } else {
            _reticle->animateFuse(1 - timeToFuseRatio);
        }
    }

    std::shared_ptr<VROReticle> getReticle() {
            return _reticle;
    }

    void setReticle(std::shared_ptr<VROReticle> reticle){
        _reticle = reticle;
        _reticleInitialPositionSet = false;
    }

protected:
    std::shared_ptr<VRONode> _rootNode;

    void onReticleGazeHit(const VROHitTestResult &hit) {
        if (_reticle == nullptr){
            return;
        }

        float depth = -hit.getDistance();

        if (_reticle->getPointerMode()) {
            _reticle->setPosition(hit.getLocation());
            
            float worldPerScreen = hit.getCamera().getWorldPerScreen(depth);
            float radius = fabs(worldPerScreen) * kReticleSizeMultiple;
            _reticle->setRadius(radius);
        }
        else {
            // Lock the Reticle's position to the center of the screen
            // for non-pointer mode (usually Cardboard). This works because
            // the reticle uses the HUDView matrix as its transform, which
            // cancels out the camera's view matrix
            
            // Only use the background depth if this is our first time
            // positioning the reticle. Otherwise we maintain the current
            // reticle depth, to avoid reticle 'popping' that occurs when
            // the user moves from an actual focused object to the background.
            // The background has no 'actual' depth so this is ok.
            if (!_reticleInitialPositionSet || !hit.isBackgroundHit() || kDebugSceneBackgroundDistance) {
                _reticle->setPosition(VROVector3f(0, 0, depth));
                _reticleInitialPositionSet = true;
                
                float worldPerScreen = hit.getCamera().getWorldPerScreen(depth);
                float radius = fabs(worldPerScreen) * kReticleSizeMultiple;
                _reticle->setRadius(radius);
            }
        }
    }

private:

    std::weak_ptr<VROEventDelegate> _eventDelegateWeak;

    std::shared_ptr<VROReticle> _reticle;
    bool _reticleInitialPositionSet;

    /*
     Event delegate for triggering calls back to Controller_JNI.
     */
    std::shared_ptr<VROEventDelegate> getDelegate(){
        if (_eventDelegateWeak.expired()){
            return nullptr;
        }
        return _eventDelegateWeak.lock();
    }
    
};
#endif
