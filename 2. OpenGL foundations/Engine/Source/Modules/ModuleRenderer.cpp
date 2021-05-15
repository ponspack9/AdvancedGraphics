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
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBufferSize);
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBlockAlignment);

	glGenBuffers(1, &bufferHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
	glBufferData(GL_UNIFORM_BUFFER, maxUniformBufferSize, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);



	return true;
}

bool ModuleRenderer::Update(float dt)
{
	M_Scene->camera->Move();

	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, "Shaded model");

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, App->displaySize.x, App->displaySize.y);


	// Draw meshes
	Program* texturedMeshProgram = M_Resources->programs[App->texturedGeometryProgramIdx];
	glUseProgram(texturedMeshProgram->handle);

	for (Model* model : M_Scene->models)
	{
		Mesh* mesh = model->mesh;

		if (mesh == nullptr) continue;

		// Uniforms
		glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
		u32 blockSize = sizeof(glm::mat4) * 2;
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, bufferHandle, 0, blockSize);

		u8* bufferData = (u8*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
		u32 bufferHead = 0;

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, model->position);
		transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

		

		memcpy(bufferData + bufferHead, glm::value_ptr(transform), sizeof(glm::mat4));
		bufferHead += sizeof(glm::mat4);


		memcpy(bufferData + bufferHead, glm::value_ptr(M_Scene->camera->GetViewProjectionMatrix()), sizeof(glm::mat4));
		bufferHead += sizeof(glm::mat4);

		

		for (u32 i = 0; i < mesh->submeshes.size(); ++i)
		{
			GLuint vao = FindVAO(mesh, i, texturedMeshProgram);

			glBindVertexArray(vao);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			u32 submeshMaterialIdx = model->materialIdx[i];
			Material* submeshMaterial = M_Resources->materials[submeshMaterialIdx];

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, submeshMaterial->albedoTexture->handle);
			glUniform1i(App->programUniformTexture, 0); // TODO App->texturedMeshProgram_uTexture

			Submesh& submesh = mesh->submeshes[i];
			glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, (void*)(u64)submesh.indexOffset);

			glBindVertexArray(0);
		}

		glUnmapBuffer(GL_UNIFORM_BUFFER);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	

	glUseProgram(0);

	glPopDebugGroup();

	return true;

	// Update Camera
	//M_Scene->camera->Move();

	////Get the viewprojection
	//camera->aspect = window_width / window_height;
	//Matrix44 viewprojection = camera->getViewProjectionMatrix();

	////enable the shader
	//shader->enable();
	//shader->setMatrix44("model", model_matrix); //upload info to the shader
	//shader->setMatrix44("viewprojection", viewprojection); //upload info to the shader

	//shader->setTexture("color_texture", texture, 0); //set texture in slot 0

	////render the data
	//mesh->render(GL_TRIANGLES);

	////disable shader
	//shader->disable();

	////swap between front buffer and back buffer
	//SDL_GL_SwapWindow(this->window);
}

#pragma endregion

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
	for (u32 i = 0; i < program->vertexInputLayout.attributes.size(); ++i)
	{
		bool attributeWasLinked = false;
		for (u32 j = 0; j < submesh.vertexBufferLayout.attributes.size(); ++j)
		{
			if (program->vertexInputLayout.attributes[i].location == submesh.vertexBufferLayout.attributes[j].location)
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