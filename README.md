## CCMaskLayer: a color layer with scratch-off effect

implement scratch-off effect using CCLayerColors,
[MIT Licence](http://opensource.org/licenses/mit-license.php), for cocos2dx-2.x

Intentionally avoid using CCRenderTexture, because it's soooo buggy on android. The GLSurfaceView onPause() trashes opengl context whenever lock screen or click home button. The [sdk-15 solution](http://stackoverflow.com/a/11167948/1263403) almost works when android:targetSdkVersion in AndroidManifest.xml <= 12, but it fails on my device when click home and relaunch, twice.

### Usage:
    CCMaskLayer *maskLayer = CCMaskLayer::create(ccc4(255, 0, 0, 200));
    addChild(maskLayer);
    maskLayer->begin(); // begin

    maskLayer->scratchOff(CCRectMake(100, 100, 200, 200)); // 1

    CCSprite* circle = CCSprite::create("mask-circle.png");
    circle->setPosition(ccp(400, 300));
    circle->setScale(1.2);
    maskLayer->scratchOff(circle); // 2

    maskLayer->end(); // end
    