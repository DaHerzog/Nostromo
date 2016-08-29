//
//  Model.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "Texture.h"
#include <vector>
#include <assert.h>
#include <math.h>
#include <map>
#include <float.h>


Vertex::Vertex() {}

Vertex::Vertex( const Vector& p, const Vector& n, float TexS, float TexT) {
    Position = p;
    Normal = n;
    TexcoordS = TexS;
    TexcoordT = TexT;
}

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox( const Vector& min, const Vector& max) : Min(min), Max(max) {}

Model::Model() : m_pVertices(NULL), m_pMaterials(NULL), m_MaterialCount(0), m_VertexCount(0) {}

Model::~Model() {
    if( m_pVertices)
        delete [] m_pVertices;
    if(m_pMaterials)
        delete [] m_pMaterials;
}

bool Model::load( const char* Filename, const char* VertexShader, const char* FragmentShader) {

	string compileErrors;

	/*
	char eingabe[256];
	string argument = "CompileShader";
	string delimLine = "-";
	string delimSpace = " ";
	string VS_string = "Shader//";
	string FS_string = "Shader//";
	bool hasVS = false;
	bool hasFS = false;

	string input = "CompileShader -v Vertexshader_Phong.glsl -f Pixelshader_Phong.glsl";

	if (argument.compare(input.substr(0,13)) == 0) {
		cout << "--CompileShader--" << endl;

		for (int i = 0; i < 2; i++) {
			int param = input.find(delimLine);

			if(param != -1) {
				if (input.at(param + 1) == 'f' && !hasFS) {
					cout << "FragmentShader detected" << endl;
					input.erase(input.begin(), input.end() - (input.size() - param -3));
					FS_string.append(input.substr(0, input.find(delimSpace)));
					input.erase(0, FS_string.size()-8);
					hasFS = true;

				}
				else {
					if (input.at(param + 1) == 'v' && !hasVS) {
						cout << "VertexShader detected" << endl;
						input.erase(input.begin(), input.end() - (input.size() - param - 3));
						VS_string.append(input.substr(0, input.find(delimSpace)));
						input.erase(0, VS_string.size() - 8);
						hasVS = true;
					}
				}
			}
		}

	}

	ShaderProgram shader = ShaderProgram();

	m_ShaderProgram = shader;
	m_ShaderProgram.load(hasVS, hasFS, VS_string.c_str(),FS_string.c_str());

	m_ShaderProgram.compile(&compileErrors);
	createModel(Filename);
    return true;
	*/

	m_ShaderProgram = ShaderProgram();
    
    //old
	//m_ShaderProgram.load(true,true,VertexShader,FragmentShader);
    
    m_ShaderProgram.load(VertexShader,FragmentShader);
	m_ShaderProgram.compile(&compileErrors);
	createModel(Filename);
	return true;

}

void Model::createModel(const char* Filename) {

	FILE *pfile;
	char c = NULL;
	string line;
	string delimSpace =" ";
	string delimSlash ="/";
	int faceCount;
	int faceCountGroup = 0;
	float faktor = 1;

	float maxX = 0;
	float maxY = 0;
	float maxZ = 0;
	float minX = 0;
	float minY = 0;
	float minZ = 0;

	struct Texcoord {
		float s, t;
	};

	struct Face {
		unsigned int pidx[3], tidx[3];
	};

	vector<Vector> MyPositionsFromFile = {};
	vector<Texcoord> MyTexcoordFromFile = {};
	vector<Face> MyFacesFromFile = {};
	vector<TextureGroup> MyTextureGroups = {};

	pfile = fopen(Filename, "r");

	//detect EOF
	while (c != -1) {
		if (c != '\n') {
			c = fgetc(pfile);
			line += c;
		} else {
			//Zeilenverarbeitung cases: -v- -vt- -f-
			switch (line.at(0)) {
			case 'v': 
				if (line.at(1) != 't') {
					line.erase(line.begin(), line.end() - (line.size() - 1));
					int k = line.find_first_not_of(delimSpace);
					line.erase(line.begin(), line.end() - (line.size() - k));

					string v_1s = line.substr(0, line.find(delimSpace));
					line.erase(line.begin(), line.end() - (line.size() - v_1s.size() - 1));
					string v_2s = line.substr(0, line.find(delimSpace));
					line.erase(line.begin(), line.end() - (line.size() - v_2s.size() - 1));
					string v_3s = line.substr(0, line.size() - 1);

					float v_1 = stof(v_1s);
					float v_2 = stof(v_2s);
					float v_3 = stof(v_3s);

					if (v_1 > maxX) {
						maxX = v_1;
					} 
					else if(v_1 < minX) {
						minX = v_1;
					}

					if (v_2 > maxY) {
						maxY = v_2;
					}
					else if (v_2 < minY) {
						minY = v_2;
					}
					if (v_3 > maxZ) {
						maxZ = v_3;
					}
					else if (v_3 < minZ) {
						minZ = v_3;
					}

					MyPositionsFromFile.push_back(Vector(v_1,v_2,v_3));
				}
				else {
					line.erase(line.begin(), line.end() - (line.size() - 3));

					string vt_1s = line.substr(0, line.find(delimSpace));
					line.erase(line.begin(), line.end() - (line.size() - vt_1s.size() - 1));
					string vt_2s = line.substr(0, line.find(delimSpace));

					float vt_1 = stof(vt_1s);
					float vt_2 = stof(vt_2s);

					MyTexcoordFromFile.push_back(Texcoord{vt_1, vt_2 });
				}
				break;
			case 'f': {

				line.erase(line.begin(), line.end() - (line.size() - 2));
				string f_1s = line.substr(0, line.find(delimSpace));
				line.erase(line.begin(), line.end() - (line.size() - f_1s.size() - 1));
				string f_2s = line.substr(0, line.find(delimSpace));
				line.erase(line.begin(), line.end() - (line.size() - f_2s.size() - 1));
				string f_3s = line.substr(0, line.find(delimSpace));
				line.erase(line.begin(), line.end() - (line.size() - f_3s.size() - 1));
				string f_4s = line.substr(0, line.size() - 1);

				unsigned int p_1 = stoi(f_1s.substr(0,f_1s.find(delimSlash)));
				unsigned int p_2 = stoi(f_2s.substr(0, f_2s.find(delimSlash)));
				unsigned int p_3 = stoi(f_3s.substr(0, f_3s.find(delimSlash)));

				unsigned int f_1 = stoi(f_1s.substr(f_1s.find(delimSlash)+1,f_1s.size()));
				unsigned int f_2 = stoi(f_2s.substr(f_2s.find(delimSlash)+1, f_2s.size()));
				unsigned int f_3 = stoi(f_3s.substr(f_3s.find(delimSlash)+1, f_3s.size()));

				Face triangle_1 = { p_1,p_2,p_3 ,f_1,f_2,f_3 };
				MyFacesFromFile.push_back(triangle_1);
				faceCountGroup++;

				if (f_4s.size() != 0) {
					unsigned int p_4 = stoi(f_4s.substr(0, f_4s.find(delimSlash)));
					unsigned int f_4 = stoi(f_4s.substr(f_4s.find(delimSlash) + 1, f_4s.size()));

					Face triangle_2 = { p_1,p_3,p_4 ,f_1,f_3,f_4 };
					MyFacesFromFile.push_back(triangle_2);
					faceCountGroup++;
				}

				break;
			}
			case 'u' : {

				if (!MyTextureGroups.empty()) {
					MyTextureGroups.back().size = faceCountGroup * 3;
				}
				//use size of last faceGroup
				//set faceCount=0 -> count size of new face Group

				line.erase(line.begin(), line.end() - (line.size() - 7));

				Material m;

				for (unsigned int i = 0; i < m_MaterialCount; i++) {
					m = m_pMaterials[i];
					if (m.getName() == line) {
						faceCountGroup = 0;

						TextureGroup tex = TextureGroup();
						tex.size = 0;
						tex.texid = i;
						MyTextureGroups.push_back(tex);
					}
				}

				break;
			}
			case 'm': {
				line.erase(line.begin(), line.end() - (line.size() - 7));
				line.erase(line.begin() + (line.size() - 1), line.end());

				string matFileS = Filename;
				matFileS.erase(matFileS.begin() + (matFileS.size() - line.size()),matFileS.end());
				matFileS += line;

				const char *matFile = matFileS.c_str();

				applyMaterial(matFile);
				break;
			}

			default:
				break;

			}


			//Zeile leeren
			line.erase(line.begin(), line.end());
			c = fgetc(pfile);
			line += c;
			//lineCount = 1;
		}
	}

	if (MyTextureGroups.empty()) {
		TextureGroup tex = TextureGroup();
		MyTextureGroups.push_back(tex);
	}

	MyTextureGroups.back().size = faceCountGroup * 3;


	faceCount = MyFacesFromFile.size();

	m_pVertices = new Vertex[faceCount * 3];
	m_VertexCount = faceCount * 3;
	for (int i = 0; i<faceCount; i++)
	{
		unsigned int PosIdx0 = MyFacesFromFile[i].pidx[0] - 1;
		unsigned int PosIdx1 = MyFacesFromFile[i].pidx[1] - 1;
		unsigned int PosIdx2 = MyFacesFromFile[i].pidx[2] - 1;

		unsigned int TexIdx0 = MyFacesFromFile[i].tidx[0] - 1;
		unsigned int TexIdx1 = MyFacesFromFile[i].tidx[1] - 1;
		unsigned int TexIdx2 = MyFacesFromFile[i].tidx[2] - 1;

		Vector a = m_pVertices[i * 3].Position = MyPositionsFromFile[PosIdx0] * faktor;
		Vector b = m_pVertices[i * 3 + 1].Position = MyPositionsFromFile[PosIdx1] * faktor;
		Vector c = m_pVertices[i * 3 + 2].Position = MyPositionsFromFile[PosIdx2] * faktor;

		m_pVertices[i * 3].TexcoordS = MyTexcoordFromFile[TexIdx0].s;
		m_pVertices[i * 3 + 1].TexcoordS = MyTexcoordFromFile[TexIdx1].s;
		m_pVertices[i * 3 + 2].TexcoordS = MyTexcoordFromFile[TexIdx2].s;

		m_pVertices[i * 3].TexcoordT = MyTexcoordFromFile[TexIdx0].t;
		m_pVertices[i * 3 + 1].TexcoordT = MyTexcoordFromFile[TexIdx1].t;
		m_pVertices[i * 3 + 2].TexcoordT = MyTexcoordFromFile[TexIdx2].t;

		Vector normal = (b - a).cross(c - a);
		normal.normalize();

		m_pVertices[i * 3].Normal =
			m_pVertices[i * 3 + 1].Normal =
			m_pVertices[i * 3 + 2].Normal = normal;

		
	}

	m_TextureGroupCount = MyTextureGroups.size();
	m_tGroups = new TextureGroup[m_TextureGroupCount];

	for (unsigned int i = 0; i < m_TextureGroupCount; i++) {
		m_tGroups[i].size = MyTextureGroups.at(i).size;
		m_tGroups[i].texid = MyTextureGroups.at(i).texid;
	}

	for (int i = 0; i < faceCount; i++) {

	}

	Vector min = Vector(minX, minY, minZ) * faktor;
	Vector max = Vector(maxX, maxY, maxZ) * faktor;
	m_Box = BoundingBox(min, max);

}

const BoundingBox& Model::boundingBox() const {

	float *boundPoints = new float[6];
	boundPoints[0] = m_Box.Max.X;
	boundPoints[1] = m_Box.Max.Y;
	boundPoints[2] = m_Box.Max.Z;
	boundPoints[3] = m_Box.Min.X;
	boundPoints[4] = m_Box.Min.Y;
	boundPoints[5] = m_Box.Min.Z;

	glBegin(GL_LINES);

	glVertex3f(boundPoints[0], boundPoints[1], boundPoints[2]);
	glVertex3f(boundPoints[3], boundPoints[1], boundPoints[2]);

	glVertex3f(boundPoints[0], boundPoints[1], boundPoints[2]);
	glVertex3f(boundPoints[0], boundPoints[4], boundPoints[2]);

	glVertex3f(boundPoints[0], boundPoints[1], boundPoints[2]);
	glVertex3f(boundPoints[0], boundPoints[1], boundPoints[5]);

	glVertex3f(boundPoints[3], boundPoints[4], boundPoints[2]);
	glVertex3f(boundPoints[0], boundPoints[4], boundPoints[2]);

	glVertex3f(boundPoints[3], boundPoints[4], boundPoints[2]);
	glVertex3f(boundPoints[3], boundPoints[1], boundPoints[2]);

	glVertex3f(boundPoints[3], boundPoints[4], boundPoints[2]);
	glVertex3f(boundPoints[3], boundPoints[4], boundPoints[5]);

	glVertex3f(boundPoints[3], boundPoints[1], boundPoints[5]);
	glVertex3f(boundPoints[3], boundPoints[1], boundPoints[2]);

	glVertex3f(boundPoints[3], boundPoints[1], boundPoints[5]);
	glVertex3f(boundPoints[3], boundPoints[4], boundPoints[5]);

	glVertex3f(boundPoints[3], boundPoints[1], boundPoints[5]);
	glVertex3f(boundPoints[0], boundPoints[1], boundPoints[5]);

	glVertex3f(boundPoints[0], boundPoints[4], boundPoints[5]);
	glVertex3f(boundPoints[3], boundPoints[4], boundPoints[5]);

	glVertex3f(boundPoints[0], boundPoints[4], boundPoints[5]);
	glVertex3f(boundPoints[0], boundPoints[1], boundPoints[5]);

	glVertex3f(boundPoints[0], boundPoints[4], boundPoints[5]);
	glVertex3f(boundPoints[0], boundPoints[4], boundPoints[2]);
	glEnd();

	return m_Box;
}

void Model::applyMaterial(const char* Matname) {
	FILE* mFile;
	string line;
	char c = NULL;
	int lineCount = 0;
	m_MaterialCount = 0;
	string delimSpace =" ";

	vector<Material> myMaterialsFromFile = {};
	vector<string> myMaterialsFromFileName = {};

	mFile = fopen(Matname, "r");


	while (c != -1) {
		if (c != '\n') {
			c = fgetc(mFile);
			line += c;
			lineCount++;
		} else {

			//neues Material
			if (line.at(0) == 'n') {
				Material mat = Material();
				myMaterialsFromFile.push_back(mat);
				string matName = line.substr(7,line.size());
				myMaterialsFromFile.at(m_MaterialCount).setName(matName);
				m_MaterialCount++;
			}
			else {
				if (line.size() > 2) {
					switch (line.at(1)) {
						case 'K': {

							string tmp = line;
							tmp.erase(tmp.begin(), tmp.end() - (tmp.size() - 4));
							string k_a = tmp.substr(0, tmp.find(delimSpace));
							tmp.erase(tmp.begin(), tmp.end() - (tmp.size() - k_a.size() - 1));
							string k_b = tmp.substr(0, tmp.find(delimSpace));
							tmp.erase(tmp.begin(), tmp.end() - (tmp.size() - k_b.size() - 1));
							string k_c = tmp.substr(0, tmp.size() - 1);

							float a = stof(k_a);
							float b = stof(k_b);
							float c = stof(k_c);

							switch (line.at(2)) {

								//ambient
								case 'a': {
									
									myMaterialsFromFile.at(m_MaterialCount - 1).setAmbientColor(Color(a,b,c));
									break;
								}
								//diffuse
								case 'd': {
									
									myMaterialsFromFile.at(m_MaterialCount - 1).setDiffuseColor(Color(a, b, c));
									break;
								}
								//specular
								case 's': {
									myMaterialsFromFile.at(m_MaterialCount - 1).setSpecularColor(Color(a, b, c));
									break;
								}
								default: {
									break;
								}
							}
							break;
						}
						case 'N': {
							if (line.at(2) == 's') {
								line.erase(line.size() -1, line.size());
								string n = line.substr(4, line.size());
								myMaterialsFromFile.at(m_MaterialCount - 1).setSpecularExponent(stof(n));
							}
							break;
						}
						case 'm': {
							if (line.at(6) == 'd') {
								line.erase(line.size() - 1, line.size());
								string n = line.substr(8,line.size());

								string name(Matname);

								size_t found = name.find_last_of("/\\");
								name.erase(found + 1, name.size());
								name.append(n);

								const char* bmpname = name.c_str();
								myMaterialsFromFile.at(m_MaterialCount - 1).setDiffuseTexture(bmpname);
							}
							break;
						}
						default: {
							break;
						}
					}
				}
			}

			line.erase(line.begin(), line.end());
			c = fgetc(mFile);
			line += c;
			lineCount = 1;
		}
	}
	

	m_pMaterials = new Material[m_MaterialCount];

	for (unsigned int i = 0; i < m_MaterialCount; i++) {
		Material tmp = myMaterialsFromFile.at(i);
		m_pMaterials[i] = tmp;
		m_pMaterials[i].setAmbientColor(tmp.getAmbientColor());
		m_pMaterials[i].setDiffuseColor(tmp.getDiffuseColor());
		m_pMaterials[i].setSpecularColor(tmp.getSpecularColor());
		m_pMaterials[i].setSpecularExponent(tmp.getSpecularExponent());
		m_pMaterials[i].setName(tmp.getName());

	}
	fclose(mFile);
}

void Model::drawTriangles() const
{
    // Aufgabe 1
	int vertexCounter_group = 0;
	int vertexCounter_all = 0;
	Material m;
	int i;
	ShaderProgram shader;
	shader = m_ShaderProgram;

	shader.activate();



	GLint loc_lightPos = shader.getParameterId("LightPos");
	GLint loc_lightColor = shader.getParameterId("LightColor");
	GLint loc_specExp = shader.getParameterId("SpecExp");
	GLint loc_diffColor = shader.getParameterId("DiffColor");
	GLint loc_specColor = shader.getParameterId("SpecColor");
	GLint loc_ambientColor = shader.getParameterId("AmbientColor");
	GLint diffuseTexture = shader.getParameterId("DiffuseTexture");

	shader.setParameter(loc_lightPos, (0.0f, 2.0f, 1.0f));
	shader.setParameter(loc_lightColor, (1.0f, 1.0f, 1.0f));


	for (unsigned int k = 0; k < m_TextureGroupCount;k++) {
		vertexCounter_group = m_tGroups[k].size;
		vertexCounter_group += vertexCounter_all;

		if (m_MaterialCount > 0) {
			m = m_pMaterials[m_tGroups[k].texid];

			if (m.getName() != "") {

				float diff[3] = { m.getDiffuseColor().R, m.getDiffuseColor().G, m.getDiffuseColor().B };
				float amb[3] = { m.getAmbientColor().R, m.getAmbientColor().G, m.getAmbientColor().B };
				float spec[3] = { m.getSpecularColor().R, m.getSpecularColor().G, m.getSpecularColor().B };

				shader.setParameter(loc_specExp, 1.0f);
				shader.setParameter(loc_diffColor, Vector(m.getDiffuseColor().R, m.getDiffuseColor().G, m.getDiffuseColor().B));
				shader.setParameter(loc_specColor, Vector(m.getSpecularColor().R, m.getSpecularColor().G, m.getSpecularColor().B));
				shader.setParameter(loc_ambientColor, Vector(m.getAmbientColor().R, m.getAmbientColor().G, m.getAmbientColor().B));

				Texture tex = m.getDiffuseTexture();
				tex.apply();
				shader.setParameter(diffuseTexture, 0);
			}

			//glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
			//glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
			//glMateriali(GL_FRONT, GL_SHININESS, 30);
			//glMaterialfv(GL_FRONT, GL_AMBIENT, amb);

		} else {
			shader.setParameter(loc_specExp, 1.0f);
			shader.setParameter(loc_diffColor, Vector(1.0, 1.0, 1.0));
			shader.setParameter(loc_specColor, Vector(1.0, 1.0, 1.0));
			shader.setParameter(loc_ambientColor, Vector(1.0, 1.0, 0.5));
		}


		glBegin(GL_TRIANGLES);
		while (vertexCounter_all < vertexCounter_group) {
			i = vertexCounter_all;

			glNormal3f(m_pVertices[i].Normal.X, m_pVertices[i].Normal.Y, m_pVertices[i].Normal.Z);
			glTexCoord2f(m_pVertices[i].TexcoordS, m_pVertices[i].TexcoordT);
			glVertex3f(m_pVertices[i].Position.X, m_pVertices[i].Position.Y, m_pVertices[i].Position.Z);

			glNormal3f(m_pVertices[i + 1].Normal.X, m_pVertices[i + 1].Normal.Y, m_pVertices[i + 1].Normal.Z);
			glTexCoord2f(m_pVertices[i + 1].TexcoordS, m_pVertices[i + 1].TexcoordT);
			glVertex3f(m_pVertices[i + 1].Position.X, m_pVertices[i + 1].Position.Y, m_pVertices[i + 1].Position.Z);

			glNormal3f(m_pVertices[i + 2].Normal.X, m_pVertices[i + 2].Normal.Y, m_pVertices[i + 2].Normal.Z);
			glTexCoord2f(m_pVertices[i + 2].TexcoordS, m_pVertices[i + 2].TexcoordT);
			glVertex3f(m_pVertices[i + 2].Position.X, m_pVertices[i + 2].Position.Y, m_pVertices[i + 2].Position.Z);

			vertexCounter_all += 3;

		}
		glEnd();

		//vertexCounter_all += vertexCounter_group;
	}
	shader.deactivate();
	
	/*for (unsigned int i = 0; i < m_VertexCount; i += 3) {

		glBegin(GL_TRIANGLES);
		glNormal3f(m_pVertices[i].Normal.X, m_pVertices[i].Normal.Y, m_pVertices[i].Normal.Z);
		glTexCoord2f(m_pVertices[i].TexcoordS, m_pVertices[i].TexcoordT);
		glVertex3f(m_pVertices[i].Position.X, m_pVertices[i].Position.Y, m_pVertices[i].Position.Z);

		glNormal3f(m_pVertices[i+1].Normal.X, m_pVertices[i+1].Normal.Y, m_pVertices[i+1].Normal.Z);
		glTexCoord2f(m_pVertices[i+1].TexcoordS, m_pVertices[i+1].TexcoordT);
		glVertex3f(m_pVertices[i + 1].Position.X, m_pVertices[i + 1].Position.Y, m_pVertices[i + 1].Position.Z);

		glNormal3f(m_pVertices[i+2].Normal.X, m_pVertices[i+2].Normal.Y, m_pVertices[i+2].Normal.Z);
		glTexCoord2f(m_pVertices[i+2].TexcoordS, m_pVertices[i+2].TexcoordT);
		glVertex3f(m_pVertices[i + 2].Position.X, m_pVertices[i + 2].Position.Y, m_pVertices[i + 2].Position.Z);
		glEnd();
	}*/
	
}



