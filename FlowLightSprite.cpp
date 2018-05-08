#include "FlowLightSprite.h"

const char* FlowLightSprite::progName = "flowlight_prog";
const char* FlowLightSprite::vert_shader = "flowlight_shader_vert.vert";
const char* FlowLightSprite::frag_shader = "flowlight_shader_frag.frag";
const char* FlowLightSprite::mvp_matrix_uniform_name = "CC_MVPMatrix";
const char* FlowLightSprite::main_texture_uniform_name = "mainTex";
const char* FlowLightSprite::secondary_texture_uniform_name = "secondaryTex";
const char* FlowLightSprite::use_flowlight_uniform_name = "useFlowLight";
const char* FlowLightSprite::use_gray_uniform_name = "useGray";

const char* FlowLightSprite::postion_location_name = "a_position";
const char* FlowLightSprite::main_texcoord_location_name = "a_texCoord";
const char* FlowLightSprite::secondary_texcoord_location_name = "a_secondaryTexCoord";

FlowLightSprite* FlowLightSprite::create()
{
	FlowLightSprite* pSprite = new FlowLightSprite();
	if (pSprite && pSprite->init())
	{
		pSprite->autorelease();
		return pSprite;
	}
	else
	{
		CC_SAFE_DELETE(pSprite);
		return nullptr;
	}
}

FlowLightSprite* FlowLightSprite::create(const std::string& filename, const std::string& flowlightFilename)
{
	FlowLightSprite* pSprite = new FlowLightSprite();
	if (pSprite && pSprite->initWithFilename(filename, flowlightFilename))
	{
		pSprite->autorelease();
		return pSprite;
	}
	else
	{
		CC_SAFE_DELETE(pSprite);
		return nullptr;
	}
}

FlowLightSprite* FlowLightSprite::create(cocos2d::Texture2D* texture, cocos2d::Texture2D* flowlightTexture)
{
	FlowLightSprite* pSprite = new FlowLightSprite();
	if (pSprite && pSprite->initWithTexture(texture, flowlightTexture))
	{
		pSprite->autorelease();
		return pSprite;
	}
	else
	{
		CC_SAFE_DELETE(pSprite);
		return nullptr;
	}
}

FlowLightSprite::FlowLightSprite()
{
	m_texture = nullptr;
	m_flowlightTexture = nullptr;
	m_mvpLocation = - 1;
	m_mainTexLocation = -1;
	m_secondaryTexLocation = -1;
	m_positionLocation = -1;
	m_texCoordLocation = -1;
	m_secondaryTexCoordLocation = -1;
	m_useGrayLocation = -1;
	m_vao = 0;
	memset(m_vbo, 0, sizeof(m_vbo));
	m_time = 0.0f;
	m_bUseGray = false;
}

FlowLightSprite::~FlowLightSprite()
{

}

bool FlowLightSprite::init()
{
	return initWithTexture(nullptr, nullptr);
}

bool FlowLightSprite::initWithFilename(const std::string& filename, const std::string& flowlightFilename)
{
	cocos2d::Texture2D* texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(filename);
	cocos2d::Texture2D* flowLightTexture = cocos2d::Director::getInstance()->getTextureCache()->addImage(flowlightFilename);
	return initWithTexture(texture, flowLightTexture);
}

bool FlowLightSprite::initWithTexture(cocos2d::Texture2D* texture, cocos2d::Texture2D* flowlightTexture)
{
	if (!Node::init())
		return false;

 	setAnchorPoint(cocos2d::Vec2(0.5, 0.5));

	m_texture = texture;
	m_flowlightTexture = flowlightTexture;

	cocos2d::Size contentsize = cocos2d::Size::ZERO;
	if (m_texture)
		contentsize = m_texture->getContentSize();
	setContentSize(contentsize);

	cocos2d::GLProgram* _glProgram = cocos2d::GLProgram::createWithFilenames(vert_shader, frag_shader);
	if (_glProgram == nullptr)
		return false;
	cocos2d::GLProgramCache::getInstance()->addGLProgram(_glProgram, progName);
	initUniformAndAttribLocation();
	setupVAOAndVBO();

	return true;
}

void FlowLightSprite::setTexture(const std::string& filename)
{
	cocos2d::Texture2D* tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(filename);
	if (tex)
	{
		m_texture = tex;
		setContentSize(m_texture->getContentSize());
	}
}

void FlowLightSprite::setTexture(cocos2d::Texture2D* texture)
{
	if (texture)
	{
		m_texture = texture;
		setContentSize(m_texture->getContentSize());
	}
}

void FlowLightSprite::setFlowLight(const std::string& filename)
{
	cocos2d::Texture2D* tex = cocos2d::Director::getInstance()->getTextureCache()->addImage(filename);
	if (tex)
	{
		m_flowlightTexture = tex;
	}
}

void FlowLightSprite::setFlowLight(cocos2d::Texture2D* texture)
{
	if (texture)
	{
		m_flowlightTexture = texture;
	}
}

void FlowLightSprite::initUniformAndAttribLocation()
{
	cocos2d::GLProgram* program = cocos2d::GLProgramCache::getInstance()->getGLProgram(progName);
	if (program == nullptr)
		return;

	GLuint prog = program->getProgram();
	m_mvpLocation = glGetUniformLocation(prog, mvp_matrix_uniform_name);
	m_mainTexLocation = glGetUniformLocation(prog, main_texture_uniform_name);
	m_secondaryTexLocation = glGetUniformLocation(prog, secondary_texture_uniform_name);
	m_useFlowLightLocation = glGetUniformLocation(prog, use_flowlight_uniform_name);
	m_useGrayLocation = glGetUniformLocation(prog, use_gray_uniform_name);

	m_positionLocation = glGetAttribLocation(prog, postion_location_name);
	m_texCoordLocation = glGetAttribLocation(prog, main_texcoord_location_name);
	m_secondaryTexCoordLocation = glGetAttribLocation(prog, secondary_texcoord_location_name);
}

void FlowLightSprite::setupVAOAndVBO()
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	
	glGenBuffers(2, m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glEnableVertexAttribArray(m_positionLocation);
	glVertexAttribPointer(m_positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F_T2F), (GLvoid*)offsetof(V3F_T2F_T2F, vertices));
	glEnableVertexAttribArray(m_texCoordLocation);
	glVertexAttribPointer(m_texCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F_T2F), (GLvoid*)offsetof(V3F_T2F_T2F, texCoords));
	glEnableVertexAttribArray(m_secondaryTexCoordLocation);
	glVertexAttribPointer(m_secondaryTexCoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(V3F_T2F_T2F), (GLvoid*)offsetof(V3F_T2F_T2F, secondaryTexCoords));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void FlowLightSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	if (m_texture == nullptr)
		return;

	m_transform = transform;
	m_renderCommand.init(getGlobalZOrder(), transform, flags);
	m_renderCommand.func = std::bind(&FlowLightSprite::drawFlowLightSprite, this);
	renderer->addCommand(&m_renderCommand);
}

void FlowLightSprite::drawFlowLightSprite()
{
	cocos2d::GLProgram* program = cocos2d::GLProgramCache::getInstance()->getGLProgram(progName);
	if (program == nullptr)
		return;

	program->use();

	if (m_texture != nullptr)
	{
		cocos2d::GL::bindTexture2D(m_texture->getName());
		glUniform1i(m_mainTexLocation, 0);
	}

	if (m_flowlightTexture != nullptr)
	{
		cocos2d::GL::bindTexture2DN(1, m_flowlightTexture->getName());
		glUniform1i(m_secondaryTexLocation, 1);
		glUniform1i(m_useFlowLightLocation, 1);
	}
	else
	{
		glUniform1i(m_useFlowLightLocation, 0);
	}

	glUniform1i(m_useGrayLocation, m_bUseGray);

	cocos2d::Mat4 projectionMatrix = cocos2d::Director::getInstance()->getMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
	cocos2d::Mat4 mvpMatrix = projectionMatrix * m_transform;
	glUniformMatrix4fv(m_mvpLocation, 1, GL_FALSE, mvpMatrix.m);

	float width = m_texture->getContentSize().width * getScaleX();
	float height = m_texture->getContentSize().height * getScaleY();

	float x1 = 0.0;
	float x2 = x1 + width;
	float y1 = 0.0;
	float y2 = y1 + height;

	V3F_T2F_T2F_QUAD quads;
	quads.bl.vertices.set(x1, y1, 0.0f);
	quads.br.vertices.set(x2, y1, 0.0f);
	quads.tl.vertices.set(x1, y2, 0.0f);
	quads.tr.vertices.set(x2, y2, 0.0f);

	float left = 0.0f;
	float right = 1.0f;
	float bottom = 1.0f;
	float top = 0.0f;
	quads.bl.texCoords.u = left;
	quads.bl.texCoords.v = bottom;
	quads.br.texCoords.u = right;
	quads.br.texCoords.v = bottom;
	quads.tl.texCoords.u = left;
	quads.tl.texCoords.v = top;
	quads.tr.texCoords.u = right;
	quads.tr.texCoords.v = top;

	left = 1.0 - m_time;
	right = 1.0 + left;
	quads.bl.secondaryTexCoords.u = left;
	quads.bl.secondaryTexCoords.v = bottom;
	quads.br.secondaryTexCoords.u = right;
	quads.br.secondaryTexCoords.v = bottom;
	quads.tl.secondaryTexCoords.u = left;
	quads.tl.secondaryTexCoords.v = top;
	quads.tr.secondaryTexCoords.u = right;
	quads.tr.secondaryTexCoords.v = top;

	GLushort verticesIndex[6] = { 0, 1, 2, 3, 2, 1 };

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quads), &quads, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndex), verticesIndex, GL_STATIC_DRAW);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (GLvoid*)0);
	glFlush();
	m_time += 1.0 / 120.0f;
	if (m_time > 2.0f)
		m_time -= 2.0f;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

cocos2d::Texture2D* FlowLightSprite::getMainTexture()
{
	return m_texture;
}

void FlowLightSprite::setUseGray(bool gray)
{
	m_bUseGray = gray;
}

bool FlowLightSprite::isUseGray() const
{
	return m_bUseGray;
}

void FlowLightSprite::setFlowLightTime(float time)
{
	m_time = time;
}

float FlowLightSprite::getFlowLightTime() const
{
	return m_time;
}