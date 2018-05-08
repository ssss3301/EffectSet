#include "FlowLightButton.h"


static const float ZOOM_ACTION_DURATION = 0.05f;

FlowLightButton* FlowLightButton::create()
{
	FlowLightButton* button = new FlowLightButton();
	if (button && button->init())
	{
		button->autorelease();
		return button;
	}
	else
	{
		CC_SAFE_DELETE(button);
		return nullptr;
	}
}

FlowLightButton* FlowLightButton::create(const std::string& normalImg, const std::string& selectedImg
	, const std::string& disableImg, const std::string& flowLight)
{
	FlowLightButton* button = new FlowLightButton();
	if (button && button->init(normalImg, selectedImg, disableImg, flowLight))
	{
		button->autorelease();
		return button;
	}
	else
	{
		CC_SAFE_DELETE(button);
		return nullptr;
	}
}

FlowLightButton::FlowLightButton()
{
	m_normalRenderer = nullptr;
	m_selectedRenderer = nullptr;
	m_disableRenderer = nullptr;
	m_normalSize = cocos2d::Size::ZERO;
	m_selectedSize = cocos2d::Size::ZERO;
	m_disableSize = cocos2d::Size::ZERO;
	m_bPressedZoom = false;
	m_zoomScale = 0.05f;
}

FlowLightButton::~FlowLightButton()
{

}

bool FlowLightButton::init()
{
	if (!cocos2d::ui::Widget::init())
		return false;

	do 
	{
		m_normalRenderer = FlowLightSprite::create();
		if (m_normalRenderer == nullptr)
			break;

		m_selectedRenderer = FlowLightSprite::create();
		if (m_selectedRenderer == nullptr)
			break;
		m_selectedRenderer->setVisible(false);
		

		m_disableRenderer = FlowLightSprite::create();
		if (m_disableRenderer == nullptr)
			break;
		m_disableRenderer->setVisible(false);

		addChild(m_normalRenderer);
		addChild(m_selectedRenderer);
		addChild(m_disableRenderer);

		setTouchEnabled(true);

		return true;

	} while (0);

	return false;
}

bool FlowLightButton::init(const std::string& normalImg, const std::string& selectedImg, const std::string& disableImg
	, const std::string& flowLight)
{
	if (!init())
		return false;
	loadTextures(normalImg, selectedImg, disableImg, flowLight);
	return true;
}


void FlowLightButton::loadTextures(const std::string& normalImg, const std::string& selectedImg
	, const std::string& disableImg, const std::string& flowLight)
{
	if (m_normalRenderer)
	{
		m_normalRenderer->initWithFilename(normalImg, flowLight);
		m_normalSize = m_normalRenderer->getContentSize();
	}

	if (m_selectedRenderer)
	{
		m_selectedRenderer->initWithFilename(selectedImg, flowLight);
		m_selectedSize = m_selectedRenderer->getContentSize();
	}

	if (m_disableRenderer)
	{
		m_disableRenderer->initWithFilename(disableImg, flowLight);
		m_disableSize = m_disableRenderer->getContentSize();
	}

	updateContentSizeAndPosition();
}

void FlowLightButton::loadNormalTexture(const std::string& normalImg)
{
	cocos2d::Texture2D* tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(normalImg.c_str());
	if (tex)
	{
		m_normalRenderer->initWithTexture(tex);
		m_normalSize = tex->getContentSize();
		updateContentSizeAndPosition();
	}
}

void FlowLightButton::loadTexturePressed(const std::string& pressedImg)
{
	cocos2d::Texture2D* tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(pressedImg.c_str());
	if (tex)
	{
		m_selectedRenderer->initWithTexture(tex);
		m_selectedSize = tex->getContentSize();
		updateContentSizeAndPosition();
	}
}

void FlowLightButton::loadDisableTexture(const std::string& disableImg)
{
	cocos2d::Texture2D* tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(disableImg.c_str());
	if (tex)
	{
		m_disableRenderer->initWithTexture(tex);
		m_disableSize = tex->getContentSize();
		updateContentSizeAndPosition();
	}
}

void FlowLightButton::loadFlowLightTexture(const std::string& flowLightImg)
{
	cocos2d::Texture2D* tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(flowLightImg.c_str());
	if (tex)
	{
		if (m_normalRenderer)
			m_normalRenderer->setFlowLight(tex);
		if (m_selectedRenderer)
			m_selectedRenderer->setFlowLight(tex);
		if (m_disableRenderer)
			m_disableRenderer->setFlowLight(tex);
	}
}

void FlowLightButton::updateContentSizeAndPosition()
{
	do 
	{
		if (!m_normalSize.equals(cocos2d::Size::ZERO))
		{
			setContentSize(m_normalSize);
			break;
		}

		if (!m_selectedSize.equals(cocos2d::Size::ZERO))
		{
			setContentSize(m_selectedSize);
			break;
		}

		if (!m_disableSize.equals(cocos2d::Size::ZERO))
			setContentSize(m_disableSize);

	} while (0);

	cocos2d::Size contentSize = getContentSize();
	cocos2d::Vec2 center(contentSize.width *  0.5f, contentSize.height * 0.5f);
	if (m_normalRenderer)
		m_normalRenderer->setPosition(center);
	if (m_selectedRenderer)
		m_selectedRenderer->setPosition(center);
	if (m_disableRenderer)
		m_disableRenderer->setPosition(center);
}


void FlowLightButton::onPressStateChangedToNormal()
{
	float time = getFlowLightTime();
	if (m_normalRenderer)
	{
		m_normalRenderer->setVisible(true);
		m_normalRenderer->setUseGray(false);
		
		m_normalRenderer->stopAllActions();
		if (m_normalRenderer->getMainTexture() != nullptr)
		{
			cocos2d::ScaleTo* normalAction = cocos2d::ScaleTo::create(ZOOM_ACTION_DURATION, 1.0f);
			m_normalRenderer->runAction(normalAction);
		}
		m_normalRenderer->setFlowLightTime(time);
	}

	if (m_selectedRenderer)
	{
		m_selectedRenderer->setVisible(false);
		m_selectedRenderer->stopAllActions();
		if (m_selectedRenderer->getMainTexture() != nullptr)
		{
			cocos2d::ScaleTo* selectedAction = cocos2d::ScaleTo::create(ZOOM_ACTION_DURATION, 1.0f);
			m_selectedRenderer->runAction(selectedAction);
		}
	}

	if (m_disableRenderer)
		m_disableRenderer->setVisible(false);
}

void FlowLightButton::onPressStateChangedToPressed()
{
	float time = getFlowLightTime();
	if (m_normalRenderer)
	{
		m_normalRenderer->setVisible(false);
		m_normalRenderer->stopAllActions();

		if (m_normalRenderer->getMainTexture() != nullptr)
		{
			cocos2d::ScaleTo* normalAction = cocos2d::ScaleTo::create(ZOOM_ACTION_DURATION, 1.0f + m_zoomScale);
			m_normalRenderer->runAction(normalAction);
		}
	}

	if (m_selectedRenderer)
	{
		m_selectedRenderer->setVisible(true);
		m_selectedRenderer->stopAllActions();
		if (m_selectedRenderer->getMainTexture() != nullptr)
		{
			cocos2d::ScaleTo* selectedAction = cocos2d::ScaleTo::create(ZOOM_ACTION_DURATION, 1.0f + m_zoomScale);
			m_selectedRenderer->runAction(selectedAction);
		}
		m_selectedRenderer->setFlowLightTime(time);
	}

	if (m_disableRenderer)
		m_disableRenderer->setVisible(false);
}

void FlowLightButton::onPressStateChangedToDisabled()
{
	if (m_normalRenderer)
	{
		m_normalRenderer->setVisible(false);
		m_normalRenderer->stopAllActions();
		m_normalRenderer->setScale(1.0f);
	}

	if (m_selectedRenderer)
	{
		m_selectedRenderer->setVisible(false);
		m_selectedRenderer->stopAllActions();
		m_selectedRenderer->setScale(1.0f);
	}

	if (m_disableRenderer)
	{
		if (m_disableRenderer->getMainTexture() != nullptr)
		{
			m_disableRenderer->setVisible(true);
		}
		else
		{
			m_normalRenderer->setVisible(true);
			m_normalRenderer->setUseGray(true);
		}
	}
}

void FlowLightButton::setPressedZoomScaleEnable(bool enable)
{
	m_bPressedZoom = enable;
}

void FlowLightButton::setZoomScale(float scale)
{
	m_zoomScale = scale;
}

float FlowLightButton::getFlowLightTime()
{
	float time = 0.0f;
	if (m_normalRenderer && m_normalRenderer->isVisible())
		time = m_normalRenderer->getFlowLightTime();
	if (m_selectedRenderer && m_selectedRenderer->isVisible())
		time = m_selectedRenderer->getFlowLightTime();

	return time;
}