#pragma once
#include "framework.h"
#include "../Draw/ILine.h"

struct BSphere : public ILine
{
	BSphere()
	{
		Center = D3DXVECTOR3(0, 0, 0);
		Radius = 0.0f;
	}

	BSphere(D3DXVECTOR3& center, float radius)
		: Center(center), Radius(radius)
	{
		
	}

	~BSphere()
	{
		
	}

	bool Intersect(Ray* ray, OUT float& result)
	{
		result = 0.0f;

		D3DXVECTOR3 tempVec;
		tempVec = Center - Center;

		float x = tempVec.x - ray->Position.x;
		float y = tempVec.y - ray->Position.y;
		float z = tempVec.z - ray->Position.z;

		float sqr = x * x + y * y + z * z;
		float radius = Radius * Radius;

		if (sqr <= radius)
			return true;

		float direction = x * ray->Direction.x + y * ray->Direction.y + z * ray->Direction.z;
		if (direction < 0.0f)
			return false;

		float temp = sqr - direction * direction;
		if (temp > radius)
			return false;

		
		result = direction - sqrtf(radius - temp);
		return true;
	}

	void GetLine(D3DXMATRIX& world, vector<D3DXVECTOR3>& lines)
	{
		BSphere sphere = Transform(world);


		float phiStep = Math::PI / StackCount;
		float thetaStep = 2.0f * Math::PI / SliceCount;

		UINT vertexCount = 2 + (SliceCount + 1) * (StackCount - 1);

		vector<D3DXVECTOR3> vertices;
		vertices.assign(vertexCount, D3DXVECTOR3());


		UINT index = 0;
		for (UINT i = 0; i <= StackCount; i++)
		{
			//���� ���� ������ ó��
			if (i == 0 || i == StackCount)
			{
				vertices[index] = sphere.Center + D3DXVECTOR3(0, i == 0 ? sphere.Radius : -sphere.Radius, 0);

				index++;
			}
			else
			{
				float phi = i * phiStep;

				for (UINT k = 0; k <= SliceCount; k++)
				{
					float theta = k * thetaStep;

					float x = sphere.Radius * sinf(phi) * cos(theta);
					float y = sphere.Radius * cosf(phi);
					float z = sphere.Radius * sinf(phi) * sinf(theta);

					vertices[index] = sphere.Center + D3DXVECTOR3(x, y, z);
					index++;
				}
			}//if(i)
		}//for(i)


		UINT lineCount = (2 + (StackCount - 2) + (StackCount - 1)) * SliceCount;
		lines.assign(lineCount * 2, D3DXVECTOR3());

		index = 0;


		//����
		for (UINT i = 1; i <= SliceCount; i++)
		{
			lines[index++] = vertices[0];
			lines[index++] = vertices[i];
		}

		//����
		UINT baseIndex = 1; //�� ������ ����
		UINT ringVertexCount = SliceCount + 1;

		//StackCount - 2�� ���� -> ���� 4�� �Ἥ �� �� �Ʒ� ������ �����ϰ� -1����� ��.
		for (UINT i = 0; i < StackCount - 1; i++)
		{
			for (UINT k = 0; k < SliceCount; k++)
			{
				//������
				if (i < StackCount - 2)
				{
					lines[index++] = vertices[baseIndex + i * ringVertexCount + k];
					lines[index++] = vertices[baseIndex + (i + 1) * ringVertexCount + k];
				}

				//������
				lines[index++] = vertices[baseIndex + i * ringVertexCount + k];
				lines[index++] = vertices[baseIndex + i * ringVertexCount + (k + 1)];
			}//for(k)
		}//for(i)


		 //�Ʒ���
		UINT southPoleIndex = vertexCount - 1;
		baseIndex = southPoleIndex - ringVertexCount;

		for (UINT i = 0; i < SliceCount; i++)
		{
			lines[index++] = vertices[southPoleIndex];
			lines[index++] = vertices[baseIndex + i];
		}
	}

	BSphere Transform(D3DXMATRIX& world)
	{
		BSphere sphere;

		
		D3DXVec3TransformNormal(&sphere.Center, &this->Center, &world);
		
		float x = world._11;
		float y = world._22;
		float z = world._33;

		float temp = max(x, max(y, z));
		sphere.Radius = temp * this->Radius;

		return sphere;
	}
	
	D3DXVECTOR3 Center;
	float Radius;

	UINT StackCount = 20;
	UINT SliceCount = 20;
};