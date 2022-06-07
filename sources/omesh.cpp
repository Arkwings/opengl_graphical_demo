/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "../headers/init.h"
#include "omesh.h"

#define GLCheckError() (glGetError() == GL_NO_ERROR)


#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define WVP_LOCATION 3
#define WORLD_LOCATION 7

    Mesh::Mesh()
    {
        m_VAO = 0;
        memset(m_Buffers,0,sizeof(m_Buffers));
    }


    Mesh::~Mesh()
    {
        Clear();
    }


    void Mesh::Clear()
    {
        for (unsigned int i = 0 ; i < m_Textures.size() ; i++) {
            if(m_Textures[i]){delete m_Textures[i];m_Textures[i]=NULL;}
        }

        if (m_Buffers[0] != 0) {
            glDeleteBuffers(sizeof(m_Buffers)/sizeof(m_Buffers[0]), m_Buffers);
        }
        
        if (m_VAO != 0) {
            glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
        }
    }


    bool Mesh::LoadMesh(const string& Filename)
    {
    // Release the previously loaded mesh (if it exists)
        Clear();
        
    // Create the VAO
        glGenVertexArrays(1, &m_VAO);   
        glBindVertexArray(m_VAO);
        
    // Create the buffers for the vertices attributes
        glGenBuffers(sizeof(m_Buffers)/sizeof(m_Buffers[0]), m_Buffers);

        bool Ret = false;
        Assimp::Importer Importer;

        const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
        
        if (pScene) {
            Ret = InitFromScene(pScene, Filename);
        }
        else {
            printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        }

    // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);	

        return Ret;
    }

    bool Mesh::InitFromScene(const aiScene* pScene, const string& Filename)
    {  
        m_Entries.resize(pScene->mNumMeshes);
        m_Textures.resize(pScene->mNumMaterials);

        vector<vec3> Positions;
        vector<vec3> Normals;
        vector<vec2> TexCoords;
        vector<unsigned int> Indices;

        unsigned int NumVertices = 0;
        unsigned int NumIndices = 0;
        
    // Count the number of vertices and indices
        for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
            m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;        
            m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
            m_Entries[i].BaseVertex = NumVertices;
            m_Entries[i].BaseIndex = NumIndices;
            
            NumVertices += pScene->mMeshes[i]->mNumVertices;
            NumIndices  += m_Entries[i].NumIndices;
        }
        
    // Reserve space in the vectors for the vertex attributes and indices
        Positions.reserve(NumVertices);
        Normals.reserve(NumVertices);
        TexCoords.reserve(NumVertices);
        Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
        for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
            const aiMesh* paiMesh = pScene->mMeshes[i];
            InitMesh(paiMesh, Positions, Normals, TexCoords, Indices);
        }

        if (!InitMaterials(pScene, Filename)) {
            return false;
        }

    // Generate and populate the buffers with vertex attributes and the indices
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(TEX_COORD_LOCATION);
        glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(NORMAL_LOCATION);
        glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
        
        for (unsigned int i = 0; i < 4 ; i++) {
            glEnableVertexAttribArray(WVP_LOCATION + i);
            glVertexAttribPointer(WVP_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
            glVertexAttribDivisor(WVP_LOCATION + i, 1);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);

        for (unsigned int i = 0; i < 4 ; i++) {
            glEnableVertexAttribArray(WORLD_LOCATION + i);
            glVertexAttribPointer(WORLD_LOCATION + i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
            glVertexAttribDivisor(WORLD_LOCATION + i, 1);
        }
        
        return GLCheckError();
    }

    void Mesh::InitMesh(const aiMesh* paiMesh,
        vector<vec3>& Positions,
        vector<vec3>& Normals,
        vector<vec2>& TexCoords,
        vector<unsigned int>& Indices)
    {    
        const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
        for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
            const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
            const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
            const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

            Positions.push_back(vec3(pPos->x, pPos->y, pPos->z));
            Normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
            TexCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
        }

    // Populate the index buffer
        for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
            const aiFace& Face = paiMesh->mFaces[i];
            assert(Face.mNumIndices == 3);
            Indices.push_back(Face.mIndices[0]);
            Indices.push_back(Face.mIndices[1]);
            Indices.push_back(Face.mIndices[2]);
        }
    }

    vector<Texture> Mesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            GLboolean skip = false;
            for(GLuint j = 0; j < textures_loaded.size(); j++)
            {
                if(textures_loaded[j].path == str)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if(!skip)
            {   // If texture hasn't been loaded already, load it
        Texture texture;
        texture.id = TextureFromFile(str.C_Str(), this->directory);
        texture.type = typeName;
        texture.path = str;
        textures.push_back(texture);
                this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }

    bool Mesh::InitMaterials(const aiScene* pScene, const string& Filename)
    {
     // 1. Diffuse maps
        vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. Specular maps
        vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        return true;
    }


    void Mesh::Render(unsigned int NumInstances, const mat4* WVPMats, const mat4* WorldMats)
    {        
        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WVP_MAT_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * NumInstances, WVPMats, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[WORLD_MAT_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * NumInstances, WorldMats, GL_DYNAMIC_DRAW);

        glBindVertexArray(m_VAO);
        
        for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
            const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

            assert(MaterialIndex < m_Textures.size());
            
            if (m_Textures[MaterialIndex]) {
                m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
            }

            glDrawElementsInstancedBaseVertex(GL_TRIANGLES, 
              m_Entries[i].NumIndices, 
              GL_UNSIGNED_INT, 
              (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex), 
              NumInstances,
              m_Entries[i].BaseVertex);
        }

    // Make sure the VAO is not changed from the outside    
        glBindVertexArray(0);
    }

