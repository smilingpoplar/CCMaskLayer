//
//  CCMaskLayer.cpp
//  CCMaskLayer
//
//  Created by smilingpoplar on 12-9-18.
//
//

#include "CCMaskLayer.h"

using namespace std;
USING_NS_CC;

// use vertical lines to split
void split(const CCRect &rect, const CCRect &hole, vector<CCRect> &output)
{
    if (!hole.intersectsRect(rect)) {
        output.push_back(rect);
        return;
    }
    float length;
    // left region
    if ((length = hole.getMinX()-rect.getMinX()) > 0) {
        output.push_back(CCRectMake(rect.getMinX(), rect.getMinY(), length, rect.size.height));
    }
    // right region
    if ((length = rect.getMaxX()-hole.getMaxX()) > 0) {
        output.push_back(CCRectMake(hole.getMaxX(), rect.getMinY(), length, rect.size.height));
    }
    // middle region
    float left = MAX(hole.getMinX(), rect.getMinX());
    float right = MIN(hole.getMaxX(), rect.getMaxX());
    // middle-below
    if ((length = hole.getMinY()-rect.getMinY()) > 0) {
        output.push_back(CCRectMake(left, rect.getMinY(), right-left, length));
    }
    // middle-above
    if ((length = rect.getMaxY()-hole.getMaxY()) > 0) {
        output.push_back(CCRectMake(left, hole.getMaxY(), right-left, length));
    }
}

// merge if two rects next to a vertical line
void merge(vector<CCRect> &output)
{
    sort(begin(output), end(output), [](const CCRect &a, const CCRect &b) -> bool {
        if (a.origin.y == b.origin.y) return a.origin.x < b.origin.x;
        return a.origin.y < b.origin.y;
    });
    int i = 0;
    while (i < output.size() - 1) {
        if (output[i].getMaxX() == output[i+1].getMinX() &&
            output[i].getMinY() == output[i+1].getMinY() &&
            output[i].getMaxY() == output[i+1].getMaxY()) {
            // merge
            output[i].size.width = output[i].size.width + output[i+1].size.width;
            output.erase(begin(output)+(i+1));
        } else {
            i++;
        }
    }
}

vector<CCRect> split(const CCRect &rect, const vector<CCRect> &holes)
{
    vector<CCRect> output = { rect };
    for (auto &hole : holes) {
        unsigned size = output.size();
        for (int i = 0; i < size; i++) {
            split(output[i], hole, output);
        }
        output.erase(begin(output), begin(output)+size);
    }
    merge(output);
    
    CCLOG("layer count: %d", output.size());
    for (auto &r : output) {
        CCLOG("(%.2f, %.2f, %.2f, %.2f)", r.origin.x, r.origin.y, r.size.width, r.size.height);
    }
    return output;
}

CCMaskLayer::CCMaskLayer()
: _colorLayerArray(NULL)
{
}

CCMaskLayer::~CCMaskLayer()
{
    CC_SAFE_RELEASE(_colorLayerArray);
}

bool CCMaskLayer::initWithColor(const ccColor4B &color)
{
    setColor(color);
    setColorLayerArray(CCArray::create());
    return true;
}

CCMaskLayer* CCMaskLayer::create(const cocos2d::ccColor4B &color)
{
    auto pRet = new CCMaskLayer();
    if (pRet && pRet->initWithColor(color)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void CCMaskLayer::addColorLayer(const cocos2d::CCRect &rect)
{
    auto colorLayer = CCLayerColor::create(getColor(), rect.size.width, rect.size.height);
    colorLayer->setPosition(rect.origin);
    addChild(colorLayer);
    _colorLayerArray->addObject(colorLayer);
}

void CCMaskLayer::scratchOff(const std::vector<cocos2d::CCRect> &holes)
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    CCRect screenRect = CCRectMake(0, 0, screenSize.width, screenSize.height);
    auto rectArray = split(screenRect, holes);
    for (auto &rect : rectArray) {
        addColorLayer(rect);
    }
}

void CCMaskLayer::begin()
{
    arrayMakeObjectsPerformSelectorWithObject(_colorLayerArray, removeFromParentAndCleanup, true, CCLayerColor *);
    _colorLayerArray->removeAllObjects();
    _holeArray.clear();
}

void CCMaskLayer::scratchOff(const cocos2d::CCRect &hole)
{
    _holeArray.push_back(hole);
}

void CCMaskLayer::end()
{
    scratchOff(_holeArray);
}