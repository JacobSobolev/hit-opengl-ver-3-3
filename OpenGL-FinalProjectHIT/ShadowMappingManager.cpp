#include "ShadowMappingManager.h"

ShadowMappingManager::ShadowMappingManager()
{
	createShaders();
	createDepthMap();
}


ShadowMappingManager::~ShadowMappingManager()
{
}

void ShadowMappingManager::createShaders()
{
	_simpleDepthShader = new Shader("shader_depth.vs", "shader_depth.fs");
	_debugDepthQuad = new Shader("shader_depth_debug.vs", "shader_depth_debug.fs");
}

void ShadowMappingManager::createDepthMap()
{
	// configure depth map FBO
	// -----------------------
	
	glGenFramebuffers(1, &DepthMapFBO);
	// create depth texture
	glGenTextures(1, &DepthMap);
	glBindTexture(GL_TEXTURE_2D, DepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMappingManager::configureShaders()
{
	_debugDepthQuad->use();
	_debugDepthQuad->setInt("depthMap", 0);
}

void ShadowMappingManager::RenderSceneToTextureBegin(glm::mat4 lightSpaceMatrix)
{
	
	// render scene from light's point of view
	_simpleDepthShader->use();
	_simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);

}

void ShadowMappingManager::RenderSceneToTextureEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMappingManager::RenderShadowMappDebug(float near_plane, float far_plane)
{
	_debugDepthQuad->use();
	_debugDepthQuad->setFloat("near_plane", near_plane);
	_debugDepthQuad->setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DepthMap);
}
