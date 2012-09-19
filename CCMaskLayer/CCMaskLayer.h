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

class CCMaskLayer : public cocos2d::CCNode {
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::ccColor4B, _color, Color);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, _colorLayerArray, ColorLayerArray)
private:
    std::vector<cocos2d::CCRect> _holeArray;
    void addColorLayer(const cocos2d::CCRect &rect);
    void scratchOff(const std::vector<cocos2d::CCRect> &holes);
    bool initWithColor(const cocos2d::ccColor4B &color);
    CCMaskLayer();
    ~CCMaskLayer();
public:
    static CCMaskLayer* create(const cocos2d::ccColor4B &color);
    void begin();
    void scratchOff(const cocos2d::CCRect &hole);
    void end();
};

#endif
