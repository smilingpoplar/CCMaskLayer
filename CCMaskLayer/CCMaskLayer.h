//
//  CCMaskLayer.h
//  CCMaskLayer
//
//  Created by smilingpoplar on 12-9-18.
//
//

#ifndef __CCMaskLayer__CCMaskLayer__
#define __CCMaskLayer__CCMaskLayer__

#include "cocos2d.h"
#include <vector>

class CCMaskLayer : public cocos2d::CCNode, public cocos2d::CCRGBAProtocol {
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, _colorLayerArray, ColorLayerArray);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, _spriteArray, SpriteArray);
private:
    std::vector<cocos2d::CCRect> _holeArray;
    
    cocos2d::ccColor4B _layerColor;
    GLubyte _opacity;
    cocos2d::ccColor3B _color;
    
    cocos2d::ccColor3B getModifiedColor();
    GLubyte getModifiedOpacity();
    void addColorLayer(const cocos2d::CCRect &rect);
    void clearColorLayerArray();
    cocos2d::CCSprite* copySprite(cocos2d::CCSprite *sprite);
    void addSprite(cocos2d::CCSprite *sprite);
    void clearSpriteArray();
    void scratchOff(const std::vector<cocos2d::CCRect> &holes);
    bool initWithColor(const cocos2d::ccColor4B &color);
    CCMaskLayer();
    ~CCMaskLayer();
public:
    static CCMaskLayer* create(const cocos2d::ccColor4B &color);
    void begin();
    void scratchOff(const cocos2d::CCRect &hole);
    // XXX: sprite from spriteFrameCache loses coordinate precision when loading from plist,
    // use sprite from file here
    void scratchOff(cocos2d::CCSprite *sprite);
    void end();
    // CCRGBAProtocol
    virtual GLubyte getOpacity(void) { return _opacity; }
    virtual void setOpacity(GLubyte opacity);
    virtual const cocos2d::ccColor3B& getColor(void) { return _color; }
    virtual void setColor(const cocos2d::ccColor3B& color);
    virtual void setOpacityModifyRGB(bool bValue) { CC_UNUSED_PARAM(bValue); }
    virtual bool isOpacityModifyRGB(void) { return false; }
};

#endif
