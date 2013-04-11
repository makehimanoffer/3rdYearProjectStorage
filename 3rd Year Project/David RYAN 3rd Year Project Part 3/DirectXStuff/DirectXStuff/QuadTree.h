
 QuadTree.h

#ifndef _QuadTree_H_
#define _QuadTree_H_

//basically a tree and checking if 10,000 triangles

// GLOBALS //

const int MAX_TRIANGLES = 10000;

// MY CLASS INCLUDES //

#include "Terrain.h"
#include "Frustum.h"
#include "TerrainShader.h"



// Class name: QuadTree

class QuadTree
{
private:
	//require same as terrain class struct
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct NodeType
	{
		float positionX, positionZ, width;
		int triangleCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		NodeType* nodes[4];
	};

public:
	QuadTree();
	QuadTree(const QuadTree&);
	~QuadTree();

	bool Initialize(Terrain*, ID3D11Device*);
	void Shutdown();
	void Render(Frustum*, ID3D11DeviceContext*, TerrainShader*);

	int GetDrawCount();

private:
	void CalculateMeshDimensions(int, float&, float&, float&);
	void CreateTreeNode(NodeType*, float, float, float, ID3D11Device*);
	int CountTriangles(float, float, float);
	bool IsTriangleContained(int, float, float, float);
	void ReleaseNode(NodeType*);
	void RenderNode(NodeType*, Frustum*, ID3D11DeviceContext*, TerrainShader*);

private:
	int m_triangleCount, m_drawCount;

	//list of vertices from terrain class. will be stored in following array
	VertexType* m_vertexList;

	//parent node is root of quad tree

	NodeType* m_parentNode;
};

#endif