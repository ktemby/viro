/**
 * Copyright (c) 2015-present, Viro, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

import React, { Component } from 'react';
import {
  AppRegistry,
} from 'react-native';

var ViroExperienceSelector = require('./ViroExperienceSelector.js');

export default class ViroSample extends Component {
  render() {
    return (
      <ViroExperienceSelector />
    );
  }
}

AppRegistry.registerComponent('ViroSample', () => ViroSample);
