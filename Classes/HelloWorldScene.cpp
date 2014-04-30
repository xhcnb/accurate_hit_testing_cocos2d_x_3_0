#include "HelloWorldScene.h"
#include "MyBodyParser.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();

    //打开debug draw
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Physics Body Loader Demo", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    status_label = LabelTTF::create("请点击屏幕!", "Arial", 36);
    status_label->setPosition(Point(origin.x + visibleSize.width/2, 1.2*status_label->getContentSize().height));
    this->addChild(status_label);

    // add "2dx.png"
    sp_2dx = Sprite::create("2dx.png");

    // position the sprite on the center of the screen
    sp_2dx->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //加载body定义文件
    MyBodyParser::getInstance()->parseJsonFile("bodies.json");

    //为我们的2dx.png绑定body, 第二个参数是我们在body editor中设置的名字
    auto _body = MyBodyParser::getInstance()->bodyFormJson(sp_2dx, "2dx");
    if (_body != nullptr) {
        _body->setDynamic(false); //设置静止,不然body会受力或其他body的影响
        sp_2dx->setPhysicsBody(_body);
    }

    // add the sprite as a child to this layer
    this->addChild(sp_2dx, 0);

    //添加事件监听
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    //转换到layer内的坐标
    auto location = this->convertTouchToNodeSpace(touch);
    //得到当前点下方的物理shapes
    auto arr = dynamic_cast<Scene*>(this->getParent())->getPhysicsWorld()->getShapes(location);

    //遍历当前点击到的所有shapes, 看看有没有我们的2dx!
    PhysicsBody* body = nullptr;
    for (auto& obj : arr)
    {
        //是我们的2dx.png Sprite
        if ( obj->getBody()->getNode() == sp_2dx)
        {
            body = obj->getBody();
            break;
        }
    }

    //找到了
    if (body != nullptr)
    {
        status_label->setColor(Color3B::GREEN);
        status_label->setString("Ohoo, 你点到我了!");
    }
    else
    {
        status_label->setColor(Color3B::RED);
        status_label->setString("Haha, 没点到!");
    }

    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    status_label->setColor(Color3B::WHITE);
    status_label->setString("请点击屏幕!");
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
