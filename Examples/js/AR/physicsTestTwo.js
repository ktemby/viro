/**
 * Copyright (c) 2017-present, Viro Media, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import {
  ViroScene,
  ViroARScene,
  ViroARPlane,
  ViroBox,
  ViroMaterials,
  ViroNode,
  ViroOrbitCamera,
  ViroImage,
  ViroVideo,
  ViroSkyBox,
  Viro360Video,
  ViroText,
  ViroSurface
} from 'react-viro';

import TimerMixin from 'react-timer-mixin';

var position = [0,0,0];
var rotation = [0,0,0];
var width = 0;
var height = 0;

var testARScene = React.createClass({
  mixins: [TimerMixin],
  getInitialState: function() {
    return {
      text : "not tapped",
      boxes: [],
      count : 0,
      width : .5,
    }
  },
  render: function() {
    return (
        <ViroARScene ref="arscene" reticleEnabled={false} physicsWorld={{gravity:[0, -10,0], drawBounds : true}}>
          {this._getARPlanes()}
          {this._getBoxes()}
        </ViroARScene>
    );
  },
  /*
            <ViroVideo
            height={.2} width={.2} position={[0,.15,-.5]}
            onClick={()=>{this._onTap(); this._addOneBox()}}
            source={{"uri":"https://s3-us-west-2.amazonaws.com/viro/Climber1Top.mp4"}}
            transformConstraints={"billboard"}
          />
        
          <ViroText style={styles.welcome} position={[0, .5, -1]} text={this.state.text} />

   */
  _getARPlanes() {
    let prefix = "plane"
    let planes = []
    for (var i = 0; i < 15; i++) {
      var color;
      if (i%3 == 0) {
        color = "blue"
      } else if (i%3 == 1) {
        color = "red"
      } else {
        color = "green"
      }
      planes.push((
        <ViroARPlane key={prefix + i} onClick={this._addOneBox} onComponentUpdated={this._onComponentUpdated}>
          <ViroBox scale={[this.state.width, .1, .5]} position={[0,0,0]} rotation={[0,0,0]}
            physicsBody={{
                      type:'static', restitution:0.8
                    }}
            materials={color}
            onClick={()=>{this.setState({width : this.state.width + .001});  this._addOneBox();}}/>
        </ViroARPlane>
      ));
    }
    return planes;
  },
  //
  _onComponentUpdated(map) {
    console.log("------")
    console.log(map.position);
    console.log(map.rotation);
    console.log(map.width);
    console.log(map.width);
    console.log("Diffs: ");
    console.log([(map.position[0] - position[0]), (map.position[1] - position[1]), (map.position[2] - position[2])] )
    console.log([(map.rotation[0] - rotation[0]), (map.rotation[1] - rotation[1]), (map.rotation[2] - rotation[2])] )
    console.log(map.width - width);
    console.log(map.height - height);
    position = map.position;
    rotation = map.rotation;
    width = map.width;
    height = map.height;
  },
  _getBoxes() {
    let prefix = "box"
    let boxes = [];
    for (var i = 0; i < this.state.boxes.length; i++) {
      boxes.push((<ViroBox key={prefix + i} position={this.state.boxes[i]} width={.1} height={.1} length={.1}
              onDrag={()=>{}}
              physicsBody={{
                type:'dynamic',
                mass:1
              }} />));
    }
    return boxes;
  },
  _addOneBox() {
    let position = this.refs["arscene"].getCameraPositionAsync().then((position) => {
        this.state.boxes.push([position[0], 3, position[2] - .5])
        this.setState({
          boxes : this.state.boxes
        })
    });
  },
  _onTap() {
    console.log("tapped video!!!!")
    this.setState({
      text : "tapped!"
    })
    this.setTimeout( () => {
      this.setState({
        text : "not tapped"
      });
    }, 1000);
  },
  _onLoadStart(component) {
    return () => {
      console.log("scene1 " + component + " load start");
    }
  },
  _onLoadEnd(component) {
    return () => {
      console.log("scene1 " + component + " load end");
    }
  },
  _onFinish(component) {
    return () => {
      console.log("scene1 " + component + " finished");
    }
  }
});

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#FFFFFF',
  },
  welcome: {
    fontSize: 13,
    textAlign: 'center',
    color: '#ffffff',
    margin: 2,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});

ViroMaterials.createMaterials({
  blue: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#0000ff88"
  },
  green: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#00ff0088"
  },
  red: {
    shininess: 2.0,
    lightingModel: "Constant",
    diffuseColor: "#ff000088"
  },
  wework_title: {
    shininess: 1.0,
    lightingModel: "Constant",
    diffuseTexture: {"uri": "https://s3-us-west-2.amazonaws.com/viro/guadalupe_360.jpg"},
    diffuseTexture: require("../res/new_menu_screen.jpg"),
    fresnelExponent: .5,
  },
  box_texture: {
    diffuseTexture: require("../res/sun_2302.jpg"),
  }
});

module.exports = testARScene;
