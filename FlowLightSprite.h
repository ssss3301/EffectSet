#ifndef __FlowLight_H__
#define __FlowLight_H__

#include <string>
#include "cocos2d.h"

struct V3F_T2F_T2F
{
	cocos2d::Vec3	vertices;
	cocos2d::Tex2F	texCoords;
	cocos2d::Tex2F	secondaryTexCoords;
};

struct V3F_T2F_T2F_QUAD
{
	/// bottom left
	V3F_T2F_T2F    bl;
	/// bottom right
	V3F_T2F_T2F    br;
	/// top left
	V3F_T2F_T2F    tl;
	/// top right
	V3F_T2F_T2F    tr;
};

class FlowLightSprite : public cocos2d::Node
{
	static const char* progName;
	static const char* vert_shader;
	static const char* frag_shader;
	static const char* mvp_matrix_uniform_name;
	static const char* main_texture_uniform_name;
	static const char* secondary_texture_uniform_name;
	static const char* use_flowlight_uniform_name;

	static const char* postion_location_name;
	static const char* main_texcoord_location_name;
	static const char* secondary_texcoord_location_name;
	static const char* use_gray_uniform_name;

public:
	static FlowLightSprite* create();
	static FlowLightSprite* create(const std::string& filename, const std::string& flowlightFilename = "");
	static FlowLightSprite* create(cocos2d::Texture2D* texture, cocos2d::Texture2D* flowlightTexture = nullptr);

public:
	FlowLightSprite();
	virtual ~FlowLightSprite();
	bool init();
	bool initWithFilename(const std::string& filename, const std::string& flowlightFilename = "");
	bool initWithTexture(cocos2d::Texture2D* texture, cocos2d::Texture2D* flowlightTexture = nullptr);
	void setTexture(const std::string& filename);
	void setTexture(cocos2d::Texture2D* texture);
	void setFlowLight(const std::string& filename);
	void setFlowLight(cocos2d::Texture2D* texture);
	cocos2d::Texture2D* getMainTexture();
	void setUseGray(bool gray);
	bool isUseGray() const;
	void setFlowLightTime(float time);
	float getFlowLightTime() const;

	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);

private:
	void initUniformAndAttribLocation();
	void setupVAOAndVBO();
	void drawFlowLightSprite();

private:
	cocos2d::Texture2D*		m_texture;
	cocos2d::Texture2D*		m_flowlightTexture;
	GLint					m_mvpLocation;
	GLint					m_mainTexLocation;
	GLint					m_secondaryTexLocation;
	GLint					m_positionLocation;
	GLint					m_texCoordLocation;
	GLint					m_secondaryTexCoordLocation;
	GLint					m_useFlowLightLocation;
	GLint					m_useGrayLocation;
	GLuint					m_vao;
	GLuint					m_vbo[2];
	float					m_time;
	cocos2d::CustomCommand	m_renderCommand;
	cocos2d::Mat4			m_transform;
	bool					m_bUseGray;

};

#endif //__FlowLight_H__