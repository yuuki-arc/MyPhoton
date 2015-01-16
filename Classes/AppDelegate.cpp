#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    
    cocos2d::Size designSize = cocos2d::Size(320, 480); // ベースサイズ
    cocos2d::Size resourceSize;
    cocos2d::Size screenSize = glview->getFrameSize();   //画面サイズ取得
    
    std::vector<std::string> searchPaths;
    std::vector<std::string> resDirOrders;
    
    searchPaths.push_back("Resources");
    FileUtils::getInstance()->setSearchPaths(searchPaths);
    
    CCLOG("height: %f, width: %f", screenSize.width, screenSize.height);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (screenSize.height == 2208) {
        resourceSize = cocos2d::Size(640, 1240);
        resDirOrders.push_back("resources-iphonehd");
//        GameManager::getInstance()->screenMode = ScreenModeHd;
    } else if (screenSize.height > 480) {
        resourceSize = cocos2d::Size(640, 960);
        resDirOrders.push_back("resources-iphonehd");
//        GameManager::getInstance()->screenMode = ScreenModeHd;
    } else {
        resourceSize = cocos2d::Size(320, 480);
        resDirOrders.push_back("resources-iphone");
//        GameManager::getInstance()->screenMode = ScreenModeSd;
    }
#else
    if (screenSize.height > 480) {
        resourceSize = cocos2d::Size(640, 960);
        resDirOrders.push_back("resources-iphonehd");
//        GameManager::getInstance()->screenMode = ScreenModeHd;
    } else {
        resourceSize = cocos2d::Size(320, 480);
        resDirOrders.push_back("resources-iphone");
//        GameManager::getInstance()->screenMode = ScreenModeSd;
    }
#endif
    
    resDirOrders.push_back("font");
    FileUtils::getInstance()->setSearchPaths(resDirOrders);
    
    const float scaleW = resourceSize.width / designSize.width;
    const float scaleH = resourceSize.height / designSize.height;
    
    // 縦(Portrait)の場合
    const ResolutionPolicy policy = scaleW > scaleH ?
    ResolutionPolicy::FIXED_HEIGHT :
    ResolutionPolicy::FIXED_WIDTH;
    
    director->setContentScaleFactor(resourceSize.width / designSize.width);
    glview->setDesignResolutionSize(designSize.width, designSize.height, policy);
    

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
