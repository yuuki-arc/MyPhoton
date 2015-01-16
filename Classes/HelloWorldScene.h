#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include "NetworkLogic.h"
#include "XTLayer.h"
#include <array>

USING_NS_CC;

class HelloWorld : public XTLayer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    // オブジェクト
    Sprite* ball;
    
    // XTLayer拡張
//    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *event);
    virtual void xtTouchesBegan(cocos2d::Point position);
    virtual void xtTouchesMoved(cocos2d::Point position);
    virtual void xtTouchesEnded(cocos2d::Point position);

    virtual void xtTapGesture(cocos2d::Point position);
    virtual void xtDoubleTapGesture(cocos2d::Point position);
    virtual void xtLongTapGesture(cocos2d::Point position);
    virtual void xtSwipeGesture(XTTouchDirection direction, float distance, float speed);
    
private:
    virtual void update(float delta);
    
    void addParticle(int playerNr, float x, float y);
    
    NetworkLogic* networkLogic;
};

#endif // __HELLOWORLD_SCENE_H__
