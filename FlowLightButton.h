#ifndef __FlowLight_Button_H__
#define __FlowLight_Button_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include <string>
#include "FlowLightSprite.h"

class FlowLightButton : public cocos2d::ui::Widget
{
public:
	static FlowLightButton* create();
	static FlowLightButton* create(const std::string& normalImg = "", const std::string& selectedImg = ""
		, const std::string& disableImg="", const std::string& flowLight = "");

public:
	FlowLightButton();
	virtual ~FlowLightButton();
	virtual bool init();
	virtual bool init(const std::string& normalImg, const std::string& selectedImg, const std::string& disableImg
		, const std::string& flowLight);

public:
	void loadTextures(const std::string& normalImg = "", const std::string& selectedImg = ""
		, const std::string& disableImg = "", const std::string& flowLight= "");
	void loadNormalTexture(const std::string& normalImg);
	void loadTexturePressed(const std::string& pressedImg);
	void loadDisableTexture(const std::string& disableImg);
	void loadFlowLightTexture(const std::string& flowLightImg);

	virtual void onPressStateChangedToNormal();
	virtual void onPressStateChangedToPressed();
	virtual void onPressStateChangedToDisabled();

	void setPressedZoomScaleEnable(bool enable);
	void setZoomScale(float scale);

private:
	void updateContentSizeAndPosition();
	float getFlowLightTime();

private:
	FlowLightSprite*	m_normalRenderer;
	FlowLightSprite*	m_selectedRenderer;
	FlowLightSprite*	m_disableRenderer;

	cocos2d::Size		m_normalSize;
	cocos2d::Size		m_selectedSize;
	cocos2d::Size		m_disableSize;
	bool				m_bPressedZoom;
	float				m_zoomScale;
};

#endif