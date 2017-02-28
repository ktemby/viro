/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import {
  requireNativeComponent,
  View,
  StyleSheet,
  findNodeHandle,
  Platform,
} from 'react-native';
import resolveAssetSource from "react-native/Libraries/Image/resolveAssetSource";
import React, { Component } from 'react';

var NativeModules = require('react-native').NativeModules;
var PropTypes = require('react/lib/ReactPropTypes');
var RCT_VIDEO_REF = 'virovideocomponent';

var ViroVideo = React.createClass({
  propTypes: {
    ...View.propTypes,
    position: PropTypes.arrayOf(PropTypes.number),
    rotation: PropTypes.arrayOf(PropTypes.number),
    scale: PropTypes.arrayOf(PropTypes.number),
    opacity: PropTypes.number,
    visible: PropTypes.bool,
    materials: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    transformBehaviors: PropTypes.oneOfType([
      PropTypes.arrayOf(PropTypes.string),
      PropTypes.string
    ]),
    width: PropTypes.number,
    height: PropTypes.number,
    paused: PropTypes.bool,
    loop: PropTypes.bool,
    muted: PropTypes.bool,
    volume: PropTypes.number,
    source: PropTypes.oneOfType([
        PropTypes.shape({
            uri: PropTypes.string,
        }),
        // Opaque type returned by require('./test_video.mp4')
        PropTypes.number
    ]).isRequired,

    onHover: React.PropTypes.func,
    onClick: React.PropTypes.func,
    onClickState: React.PropTypes.func,
    onTouch: React.PropTypes.func,
    onScroll: React.PropTypes.func,
    onSwipe: React.PropTypes.func,
    onDrag: React.PropTypes.func,

    /**
     * Callback that is called when the video is finished playing. This
     * function isn't called at the end of a video if looping is enabled.
     */
    onFinish: React.PropTypes.func,

    /**
      * Callback that is called when the current playback position has changed.
      * This is called in the form:
      *     onUpdateTime(currentPlaybackTimeInSeconds, totalPlayBackDurationInSeconds);
      */
    onUpdateTime: React.PropTypes.func,
  },

  _onFinish: function() {
    this.props.onFinish && this.props.onFinish();
  },

  _onUpdateTime: function(event: Event) {
    this.props.onUpdateTime && this.props.onUpdateTime(event.nativeEvent.currentTime, event.nativeEvent.totalTime);
  },

  _onHover: function(event: Event) {
    this.props.onHover && this.props.onHover(event.nativeEvent.source, event.nativeEvent.isHovering);
  },

  _onClick: function(event: Event) {
    this.props.onClick && this.props.onClick(event.nativeEvent.source);
  },

  _onClickState: function(event: Event) {
    this.props.onClickState && this.props.onClickState(event.nativeEvent.source, event.nativeEvent.clickState);
    let CLICKED = 3; // Value representation of Clicked ClickState within EventDelegateJni.
    if (event.nativeEvent.clickState == CLICKED){
        this._onClick(event)
    }
  },

  _onTouch: function(event: Event) {
    this.props.onTouch && this.props.onTouch(event.nativeEvent.source, event.nativeEvent.touchState, event.nativeEvent.touchPos);
  },

  _onScroll: function(event: Event) {
      this.props.onScroll && this.props.onScroll(event.nativeEvent.source, event.nativeEvent.scrollPos);
  },

  _onSwipe: function(event: Event) {
      this.props.onSwipe && this.props.onSwipe(event.nativeEvent.source, event.nativeEvent.swipeState);
  },

  _onDrag: function(event: Event) {
      this.props.onDrag
        && this.props.onDrag(event.nativeEvent.source, event.nativeEvent.dragToPos);
  },

  render: function() {
    if (this.props.src) {
      console.error('The <ViroVideo> component takes a `source` property rather than `src`.');
    }

    if (this.props.material) {
      console.error('The <ViroVideo> component takes a `materials` property rather than `material`.');
    }

    var source = resolveAssetSource(this.props.source);
    // Since materials and transformBehaviors can be either a string or an array, convert the string to a 1-element array.
    let materials = typeof this.props.materials === 'string' ? new Array(this.props.materials) : this.props.materials;
    let transformBehaviors = typeof this.props.transformBehaviors === 'string' ?
        new Array(this.props.transformBehaviors) : this.props.transformBehaviors;

    let nativeProps = Object.assign({}, this.props);
    nativeProps.ref = RCT_VIDEO_REF;
    nativeProps.style = [this.props.style];
    nativeProps.source = source;
    nativeProps.materials = materials;
    nativeProps.transformBehaviors = transformBehaviors;
    nativeProps.onFinishViro = this._onFinish;
    nativeProps.onUpdateTimeViro = this._onUpdateTime;
    nativeProps.onHoverViro = this._onHover;
    nativeProps.onClickViro = this._onClickState;
    nativeProps.onTouchViro = this._onTouch;
    nativeProps.onScrollViro = this._onScroll;
    nativeProps.onSwipeViro = this._onSwipe;
    nativeProps.onDragViro = this._onDrag;
    nativeProps.canHover = this.props.onHover != undefined;
    nativeProps.canClick = this.props.onClick != undefined || this.props.onClickState != undefined;
    nativeProps.canTouch = this.props.onTouch != undefined;
    nativeProps.canScroll = this.props.onScroll != undefined;
    nativeProps.canSwipe = this.props.onSwipe != undefined;
    nativeProps.canDrag = this.props.onDrag != undefined;
    return (
      <VRTVideoSurface {...nativeProps}/>
    );
  },

  getNodeHandle: function(): any {
    return findNodeHandle(this.refs[RCT_VIDEO_REF]);
  },

  seekToTime(timeInSeconds) {
    switch (Platform.OS) {
      case 'ios':
        NativeModules.VRTVideoSurfaceManager.seekToTime(this.getNodeHandle(), timeInSeconds);
        break;
      case 'android':
        NativeModules.UIManager.dispatchViewManagerCommand(
            this.getNodeHandle(),
            NativeModules.UIManager.VRTVideoSurface.Commands.seekToTime,
            [ timeInSeconds ]);
        break;
    }
  },
});

var VRTVideoSurface = requireNativeComponent(
    'VRTVideoSurface', ViroVideo, {
      nativeOnly: {
            onUpdateTimeViro: true,
            onFinishViro: true,
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
            onDragViro:true
            }
    }
);

module.exports = ViroVideo;
