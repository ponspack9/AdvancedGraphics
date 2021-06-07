#include "pch.h"
#include "ModuleRenderer.h"
#include "ModuleResources.h"
#include "ModuleScene.h"

#pragma region Module overrides

ModuleRenderer* M_Renderer = nullptr;

ModuleRenderer::ModuleRenderer()
{
	this->name = "ModuleRenderer";
	LOG_DEBUG("Created module [{0}]", name);
}

ModuleRenderer::~ModuleRenderer()
{
	LOG_DEBUG("Deleted module [{0}]", name);
}

bool ModuleRenderer::Init()
{
	// Init GBuffer
	gbuffer.Init();

	return true;
}

bool ModuleRenderer::Update(float dt)
{
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Shaded model");

	// Update Camera
	M_Scene->camera->Move(dt);

	// --- Render
	glBindFramebuffer(GL_FRAMEBUFFER, gbuffer.FBO); // Render on GBuffer's FBO
	GLenum drawBuffers[] = {
		GL_COLOR_ATTACHMENT0, // Light
		GL_COLOR_ATTACHMENT1, // Albedo
		GL_COLOR_ATTACHMENT2, // Normals
		GL_COLOR_ATTACHMENT3, // Position
		GL_COLOR_ATTACHMENT4, // Depth
	};
	glDrawBuffers(ARRAY_COUNT(drawBuffers), drawBuffers);

	// --- Geometry Pass
	GeometryPass(M_Resources->programs[App->reliefMappingProgramIdx]);

	// --- SSAO
	if (applySSAO)
		SSAOPass(M_Resources->programs[App->SSAOProgramIdx]);

	// --- Light Pass
	LightPass(M_Resources->programs[App->dirLightProgramIdx], M_Resources->programs[App->pointLightProgramIdx]);

	// --- Render to Screen
	RenderType();

	glPopDebugGroup();
	return true;
}

#pragma endregion

void ModuleRenderer::GeometryPass(Program* program)
{
	// Clear Screen & Set Viewport
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, App->displaySize.x, App->displaySize.y);

	// Set Default Flags
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glUseProgram(program->handle); // Enable Shader

	// Render
	for (Model* model : M_Scene->models)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, model->position);
		transform = glm::rotate(transform, glm::radians(00.0f), glm::vec3(0.0, 0.0, 1.0));
		transform = glm::scale(transform, model->scale);

		// Draw Mesh
		Mesh* mesh = model->mesh;
		if (mesh == nullptr) continue;
		for (u32 i = 0; i < mesh->submeshes.size(); ++i)
		{
			// VAO
			GLuint vao = FindVAO(mesh, i, program);
			glBindVertexArray(vao);

			// Material
			u32 submeshMaterialIdx = model->materialIdx[i];
			Material* submeshMaterial = M_Resources->materials[submeshMaterialIdx];

			// Pass Uniforms
			glUniformMatrix4fv(glGetUniformLocation(program->handle, "uModel"), 1, GL_FALSE, (GLfloat*)&transform);
			glUniformMatrix4fv(glGetUniformLocation(program->handle, "uViewProjection"), 1, GL_FALSE, (GLfloat*)&M_Scene->camera->GetViewProjectionMatrix());
			glUniform3f(glGetUniformLocation(program->handle, "uCameraPos"), M_Scene->camera->pos.x, M_Scene->camera->pos.y, M_Scene->camera->pos.z);

			glUniform1f(glGetUniformLocation(program->handle, "bumpiness"), model->bumpiness);
			glUniform1i(glGetUniformLocation(program->handle, "hasNormalMap"), model->hasNormalMap);
			glUniform1i(glGetUniformLocation(program->handle, "hasBumpTexture"), (int)model->hasBumpTexture);

			// Pass the Textures in Order
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, submeshMaterial->albedoTexture->handle);
			glUniform1i(glGetUniformLocation(program->handle, "uTexture"), 0);
			if (model->hasNormalMap)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, submeshMaterial->normalsTexture->handle);
				glUniform1i(glGetUniformLocation(program->handle, "uNormalMap"), 1);
			}
			if (model->hasBumpTexture)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, submeshMaterial->bumpTexture->handle);
				glUniform1i(glGetUniformLocation(program->handle, "uBumpTexture"), 2);
			}

			// Draw
			Submesh& submesh = mesh->submeshes[i];
			glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, (void*)(u64)submesh.indexOffset);

			glBindVertexArray(0);
		}
	}

	glUseProgram(0); // Disable Shader
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRenderer::LightPass(Program* dirLight_program, Program* pointLight_program)
{
	// --- DIRECTIONAL LIGHT ---
	glUseProgram(dirLight_program->handle); // Enable Directional Light Shader
	glDepthMask(false);

	// Pass the Textures in Order
	glUniform1i(glGetUniformLocation(dirLight_program->handle, "oAlbedo"), 0);
	glUniform1i(glGetUniformLocation(dirLight_program->handle, "oNormal"), 1);
	glUniform1i(glGetUniformLocation(dirLight_program->handle, "oPosition"), 2);
	glUniform1i(glGetUniformLocation(dirLight_program->handle, "oDepth"), 3);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[1]);
	glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[2]);
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[3]);
	glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[4]);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glUniform3f(glGetUniformLocation(dirLight_program->handle, "uCameraPos"), M_Scene->camera->pos.x, M_Scene->camera->pos.y, M_Scene->camera->pos.z);
	glUniformMatrix4fv(glGetUniformLocation(dirLight_program->handle, "uViewProjection"), 1, GL_FALSE, (GLfloat*)&M_Scene->camera->GetViewProjectionMatrix());

	glUniform3f(glGetUniformLocation(dirLight_program->handle, "uLightDirection"), M_Scene->dirLight->direction.x, M_Scene->dirLight->direction.y, M_Scene->dirLight->direction.z);
	glUniform3f(glGetUniformLocation(dirLight_program->handle, "uLightColor"), M_Scene->dirLight->color.x, M_Scene->dirLight->color.y, M_Scene->dirLight->color.z);

	// Draw
	glBindVertexArray(M_Resources->quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glDepthMask(true);
	glUseProgram(0); // Disable Shader


	// --- POINT LIGHTS ---
	// Set Flags
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glFrontFace(GL_CW); // render only the inner faces of the light sphere

	// Enable Point Light Shader
	glUseProgram(pointLight_program->handle);

	// Pass the Textures in Order
	glUniform1i(glGetUniformLocation(pointLight_program->handle, "oAlbedo"), 0);
	glUniform1i(glGetUniformLocation(pointLight_program->handle, "oNormal"), 1);
	glUniform1i(glGetUniformLocation(pointLight_program->handle, "oPosition"), 2);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[1]);
	glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[2]);
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[3]);

	glUniform3f(glGetUniformLocation(pointLight_program->handle, "uCameraPos"), M_Scene->camera->pos.x, M_Scene->camera->pos.y, M_Scene->camera->pos.z);
	glUniformMatrix4fv(glGetUniformLocation(pointLight_program->handle, "uViewProjection"), 1, GL_FALSE, (GLfloat*)&M_Scene->camera->GetViewProjectionMatrix());

	for (PointLight* light : M_Scene->pointLights)
	{
		if (!light->active) continue;

		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, light->position);
		transform = glm::rotate(transform, glm::radians(00.0f), glm::vec3(0.0, 0.0, 1.0));
		transform = glm::scale(transform, glm::vec3(light->radius));
		glUniformMatrix4fv(glGetUniformLocation(pointLight_program->handle, "uModel"), 1, GL_FALSE, (GLfloat*)&transform);

		glUniform3f(glGetUniformLocation(pointLight_program->handle, "uLightPosition"), light->position.x, light->position.y, light->position.z);
		glUniform3f(glGetUniformLocation(pointLight_program->handle, "uLightColor"), light->color.x, light->color.y, light->color.z);
		glUniform1f(glGetUniformLocation(pointLight_program->handle, "uLightK"), 1.0f);
		glUniform1f(glGetUniformLocation(pointLight_program->handle, "uLightL"), 0.35f);
		glUniform1f(glGetUniformLocation(pointLight_program->handle, "uLightQ"), 0.44f);

		// draw sphere
		glBindVertexArray(M_Resources->sphereVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, M_Resources->sphereIdxVBO);
		glDrawElements(GL_TRIANGLES, M_Resources->sphereIdxCount, GL_UNSIGNED_INT, NULL);
	}

	glUseProgram(0); // Disable Shader
}

void ModuleRenderer::SSAOPass(Program* program)
{
	glUseProgram(program->handle); // Enable Directional Light Shader
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	// Pass the Textures in Order
	glUniform1i(glGetUniformLocation(program->handle, "oAlbedo"), 0);
	glUniform1i(glGetUniformLocation(program->handle, "oNormal"), 1);
	glUniform1i(glGetUniformLocation(program->handle, "oPosition"), 2);
	glUniform1i(glGetUniformLocation(program->handle, "oDepth"), 3);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[1]);
	glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[2]);
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[3]);
	glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, gbuffer.textures[4]);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glUniform3f(glGetUniformLocation(program->handle, "uCameraPos"), M_Scene->camera->pos.x, M_Scene->camera->pos.y, M_Scene->camera->pos.z);
	glUniformMatrix4fv(glGetUniformLocation(program->handle, "uViewProjection"), 1, GL_FALSE, (GLfloat*)&M_Scene->camera->GetViewProjectionMatrix());

	glUniform3f(glGetUniformLocation(program->handle, "uLightDirection"), M_Scene->dirLight->direction.x, M_Scene->dirLight->direction.y, M_Scene->dirLight->direction.z);
	glUniform3f(glGetUniformLocation(program->handle, "uLightColor"), M_Scene->dirLight->color.x, M_Scene->dirLight->color.y, M_Scene->dirLight->color.z);

	glUniform1f(glGetUniformLocation(program->handle, "intensity"), SSAOIntensity);
	glUniform1i(glGetUniformLocation(program->handle, "showOnlySSAO"), showOnlySSAO);
	glUniform1i(glGetUniformLocation(program->handle, "time"), (float)ImGui::GetTime());

	// Draw
	glBindVertexArray(M_Resources->quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glDepthMask(true);
	glUseProgram(0); // Disable Shader
}

//--------------------------------------------------------------------
GLuint ModuleRenderer::FindVAO(Mesh* mesh, u32 submeshIndex, const Program* program)
{
	Submesh& submesh = mesh->submeshes[submeshIndex];

	// Try finding a vao for this submesh/program
	for (u32 i = 0; i < (u32)submesh.vaos.size(); ++i)
	{
		if (submesh.vaos[i].programHandle == program->handle)
			return submesh.vaos[i].handle;
	}

	//Create a new vao for this submesh/program
	GLuint vaoHandle = CreateNewVao(mesh, submesh, program);

	//Store it in the list of vaos for this submesh
	VAO vao = { vaoHandle, program->handle };
	submesh.vaos.push_back(vao);

	return vaoHandle;
}

GLuint ModuleRenderer::CreateNewVao(Mesh* mesh, Submesh& submesh, const Program* program)
{
	GLuint vaoHandle = 0;
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBufferHandle);

	// We have to link all vertex inputs attributes to attributes in the vertex buffer
	for (u32 i = 0; i < program->vertexShaderLayout.attributes.size(); ++i)
	{
		bool attributeWasLinked = false;
		for (u32 j = 0; j < submesh.vertexBufferLayout.attributes.size(); ++j)
		{
			if (program->vertexShaderLayout.attributes[i].location == submesh.vertexBufferLayout.attributes[j].location)
			{
				const u32 index = submesh.vertexBufferLayout.attributes[j].location;
				const u32 nComp = submesh.vertexBufferLayout.attributes[j].componentCount;
				const u32 offset = submesh.vertexBufferLayout.attributes[j].offset + submesh.vertexOffset;
				const u32 stride = submesh.vertexBufferLayout.stride;

				glVertexAttribPointer(index, nComp, GL_FLOAT, GL_FALSE, stride, (void*)(u64)offset);
				glEnableVertexAttribArray(index);

				attributeWasLinked = true;
				break;
			}
		}
		assert(attributeWasLinked); // The submesh should provide an attribute for each vertex input
	}
	glBindVertexArray(0);

	return vaoHandle;
}

void ModuleRenderer::RenderType()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer.FBO);

	glReadBuffer(GL_COLOR_ATTACHMENT0 + renderType);
	glBlitFramebuffer(0, 0, App->displaySize.x, App->displaySize.y, 0, 0, App->displaySize.x, App->displaySize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}