//
//  VRTText.m
//  React
//
//  Created by Vik Advani on 1/12/16.
//  Copyright © 2017 Viro Media. All rights reserved.
//
#import <CoreText/CoreText.h>
#import <React/RCTConvert.h>
#import <React/RCTLog.h>
#import <React/RCTFont.h>
#import "VRTText.h"
#import "VRTUtils.h"
#import "VRTFlexView.h"


NSString *const VRTLabelReactTagAttributeName = @"ReactTagAttributeName";

@implementation VRTText {
  std::shared_ptr<VROText> _vroText;
  NSString *_text;
  NSString *_defaultFont;
  float _ratio;
  NSMutableAttributedString *_cachedMutableAttributedString;
  BOOL _boundsCalculated;
  CGRect _frame;
  BOOL _textNeedsUpdate;
}

- (instancetype)initWithBridge:(RCTBridge *)bridge {
  self = [super initWithBridge:bridge];
 
  if (self) {
    _boundsCalculated = false;
    _width = 1.0;
    _height = 1.0;
    _ratio = 1.0;
    _fontSize = 18;
    _defaultFont = [VRTText defaultFont];
    _maxLines = 0;
    _textClipMode = VROTextClipMode::ClipToBounds;
    _textLineBreakMode = VROLineBreakMode::WordWrap;
    _textAlign = VROTextHorizontalAlignment::Left;
    _textAlignVertical = VROTextVerticalAlignment::Top;
    _textNeedsUpdate = NO;
  }
  return self;
}

- (void)setText:(NSString *)text {
  _text = text;
  _textNeedsUpdate = YES;
}

- (void)setWidth:(float)width {
  _width = width;
  _textNeedsUpdate = YES;
}

- (void)setHeight:(float)height {
  _height = height;
  _textNeedsUpdate = YES;
}

- (void)setColor:(UIColor *)color {
  _color = color;
  _textNeedsUpdate = YES;
}

- (void)setFontFamily:(NSString *)fontFamily {
  _fontFamily = [fontFamily copy];
  _textNeedsUpdate = YES;
}

- (void)setFontSize:(CGFloat)fontSize {
  _fontSize = fontSize;
  _textNeedsUpdate = YES;
}

- (void)setMaxLines:(NSUInteger)maxLines {
  _maxLines = maxLines;
  _textNeedsUpdate = YES;
}

- (void)setTextAlign:(VROTextHorizontalAlignment)textAlign {
  _textAlign = textAlign;
  _textNeedsUpdate = YES;
}

- (void)setTextAlignVertical:(VROTextVerticalAlignment)textAlignVertical {
  _textAlignVertical = textAlignVertical;
  _textNeedsUpdate = YES;
}

- (void)setTextLineBreakMode:(VROLineBreakMode)textLineBreakMode {
  _textLineBreakMode = textLineBreakMode;
  _textNeedsUpdate = YES;
}

- (void)setTextClipMode:(VROTextClipMode)textClipMode {
  _textClipMode = textClipMode;
  _textNeedsUpdate = YES;
}

- (void)didSetProps:(NSArray<NSString *> *)changedProps {
  if (self.driver && _textNeedsUpdate) {
    [self updateLabel];
    _textNeedsUpdate = NO;
  }
}

- (void)updateLabel {
  NSString *fontFam = ([self.fontFamily length]) ? self.fontFamily: _defaultFont;

  std::string fontFamily = std::string([fontFam UTF8String]);
  int fontSize = (int)self.fontSize;

  std::shared_ptr<VROTypeface> typeFace = self.driver->newTypeface(fontFamily, fontSize);
  std::string text = std::string([_text UTF8String]);
  VROVector4f colorVector(1.0f, 1.0f, 1.0f, 1.0f);
  
  if(self.color != nil) {
    CGFloat red,green,blue, alpha;
    [self.color getRed:&red green:&green blue:&blue alpha:&alpha];
    colorVector.set(red, green, blue, alpha);
  }
  _vroText = VROText::createText(text, typeFace, colorVector, _width, _height,
                                 self.textAlign, self.textAlignVertical,
                                 self.textLineBreakMode, self.textClipMode, self.maxLines);
  
  [self node]->setGeometry(_vroText);
}

// Method invoked before being added to scene, meant to overridden by subclasses.
- (void)sceneWillAppear {
  [super sceneWillAppear];
  [self updateLabel];
}

// Static function to get default font.
+ (NSString *)defaultFont {
  static NSString *defaultFontFamily;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    defaultFontFamily = [UIFont systemFontOfSize:14].familyName;
  });
  return defaultFontFamily;
}

@end

@implementation RCTConvert (VRTText)
+ (VROTextHorizontalAlignment)VROTextHorizontalAlignment:(id)json {
  if (![json isKindOfClass:[NSString class]]) {
    RCTLogError(@"Error setting string. String required, received: %@", json);
    return VROTextHorizontalAlignment::Left;
  }

  NSString *value = (NSString *)json;
  if( [value caseInsensitiveCompare:@"left"] == NSOrderedSame ) {
    return VROTextHorizontalAlignment::Left;
  } else if([value caseInsensitiveCompare:@"center"] == NSOrderedSame) {
    return VROTextHorizontalAlignment::Center;
  } else if([value caseInsensitiveCompare:@"right"] == NSOrderedSame) {
    return VROTextHorizontalAlignment::Right;
  }

  return VROTextHorizontalAlignment::Left;
}

+ (VROTextVerticalAlignment)VROTextVerticalAlignment:(id)json {
  if (![json isKindOfClass:[NSString class]]) {
    RCTLogError(@"Error setting string. String required, received: %@", json);
    return VROTextVerticalAlignment::Top;
  }
  
  NSString *value = (NSString *)json;
  if([value caseInsensitiveCompare:@"top"] == NSOrderedSame) {
    return VROTextVerticalAlignment::Top;
  } else if([value caseInsensitiveCompare:@"center"] == NSOrderedSame) {
    return VROTextVerticalAlignment::Center;
  } else if([value caseInsensitiveCompare:@"bottom"] == NSOrderedSame) {
    return VROTextVerticalAlignment::Bottom;
  }
  
  return VROTextVerticalAlignment::Top;
}

+(VROLineBreakMode)VROLineBreakMode:(id)json {
  if (![json isKindOfClass:[NSString class]]) {
    RCTLogError(@"Error setting string. String required, received: %@", json);
    return VROLineBreakMode::None;
  }
  
  NSString *value = (NSString *)json;
  if([value caseInsensitiveCompare:@"wordwrap"] == NSOrderedSame) {
    return VROLineBreakMode::WordWrap;
  } else if([value caseInsensitiveCompare:@"charwrap"] == NSOrderedSame) {
    return VROLineBreakMode::CharWrap;
  } else if([value caseInsensitiveCompare:@"justify"] == NSOrderedSame) {
    return VROLineBreakMode::Justify;
  } else if([value caseInsensitiveCompare:@"none"] == NSOrderedSame) {
    return VROLineBreakMode::None;
  }
  return VROLineBreakMode::None;
}

+(VROTextClipMode)VROTextClipMode:(id)json {
  if (![json isKindOfClass:[NSString class]]) {
    RCTLogError(@"Error setting string. String required, received: %@", json);
    return VROTextClipMode::None;
  }
  
  NSString *value = (NSString *)json;
  
  if ([value caseInsensitiveCompare:@"none"] == NSOrderedSame) {
    return VROTextClipMode::None;
  }
  else if ([value caseInsensitiveCompare:@"cliptobounds"] == NSOrderedSame) {
    return VROTextClipMode::ClipToBounds;
  }
  return VROTextClipMode::None;
}

@end
