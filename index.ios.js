/**
 * Copyright (c) 2015-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */
'use strict';

import React, { Component } from 'react';

import {
  AppRegistry,
  ViroSceneNavigator,
} from 'react-viro';


var scenes = {
    '360 Photo Tour': require('./js/360PhotoTour/MainScene'),
    'Hello World': require('./js/HelloWorld/HelloWorldScene'),
    'Human Body': require('./js/HumanBody/MainScene'),
}

var ViroCodeSamplesSceneNavigator = React.createClass({
  render: function() {
    // The 'viroAppProps={{...this.props}}' line below is used to pass
    // the initial properties from this base component to the ViroSceneNavigator
    // which will allow the scenes to access them.
    return (
      <ViroSceneNavigator
        initialScene={{
          scene: scenes['360 Photo Tour'],
        }}
        viroAppProps={{...this.props}}
        apiKey="1487A37C-0E71-49D8-97D7-F709BA3FC7B0"
      />
    );
  }
});

AppRegistry.registerComponent('ViroCodeSamples', () => ViroCodeSamplesSceneNavigator);
