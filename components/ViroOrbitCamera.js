/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * @providesModule ViroOrbitCamera
 * @flow
 */
'use strict';

import { requireNativeComponent, View, StyleSheet } from 'react-native';
import React, { Component } from 'react';
import PropTypes from 'prop-types';

var ViroOrbitCamera = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    focalPoint: PropTypes.arrayOf(PropTypes.number),
    active: PropTypes.bool.isRequired,
  },

  componentDidMount() {
    this.context.cameraDidMount(this);
  },

  componentWillUnmount() {
    this.context.cameraWillUnmount(this);
  },

  componentWillReceiveProps(nextProps) {
    this.context.cameraWillReceiveProps(this, nextProps);
  },

  setNativeProps: function(nativeProps) {
    this._component.setNativeProps(nativeProps);
  },

  render: function() {
    return (
      <VRTOrbitCamera
        ref={ component => {this._component = component; }}
        {...this.props}
      />
    );
  }
});

ViroOrbitCamera.contextTypes = {
  cameraDidMount: PropTypes.func,
  cameraWillUnmount: PropTypes.func,
  cameraWillReceiveProps: PropTypes.func,
};

var VRTOrbitCamera = requireNativeComponent(
  'VRTOrbitCamera',
  ViroOrbitCamera, {
    nativeOnly: {
                scale:[1,1,1],
                materials:[],
                visible: true,
                canHover: true,
                canClick: true,
                canTouch: true,
                canScroll: true,
                canSwipe: true,
                canDrag: true,
                onHoverViro:true,
                onClickViro:true,
                onTouchViro:true,
                onScrollViro:true,
                onSwipeViro:true,
                onDragViro:true,
                transformBehaviors:true,
                canFuse: true,
                onFuseViro:true,
                timeToFuse: true,
                viroTag: true,
                scalePivot: true,
                rotationPivot: true,
                canCollide:true,
                onCollisionViro:true,
                onNativeTransformDelegateViro:true,
                hasTransformDelegate:true,
                physicsBody:true
      }
});

module.exports = ViroOrbitCamera;
