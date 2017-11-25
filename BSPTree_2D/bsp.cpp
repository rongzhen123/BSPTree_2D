#include "bsp.h"
#include <algorithm>
#include <queue>

BSPTree::box_t::box_t() {}

void BSPTree::box_t::set(float x1, float x2, float y1, float y2)
{
	xmin = x1;
	xmax = x2;
	ymin = y1;
	ymax = y2;
}

BSPTree::BSPTree(float x1, float x2, float y1, float y2, int l)
{
	xmin = x1;
	xmax = x2;
	ymin = y1;
	ymax = y2;
	layer = l;
	boxNum = 0;
	layer_count = 0;
	root = nullptr;
}

void BSPTree::add(float x1, float x2, float y1, float y2)
{
	box_t b;
	boxNum++;
	b.set(x1,x2,y1,y2);
	box.push_back(b);
}

//ĳһ���ռ��еķָ����Ƿ���ռ��е�һ����Χ���ཻ
bool BSPTree::isIntersect(float xmin, float xmax, float ymin, float ymax, \
	float data, bool isHori, std::vector<int>& id, int& num)
{
	bool flag = false;	//��¼�Ƿ�����ཻ
	//�ָ�����ˮƽ��
	if (isHori)
	{
		//�������а�Χ��
		for (int i = 0; i < boxNum; i++)
		{
			//�����Χ����ȫ���ڿռ���
			if (inBox(xmin, xmax, ymin, ymax, i))
			{
				num++;	//��¼��Χ�и���+1
				if (data > box[i].xmin && data < box[i].xmax)	//�����ཻ
				{
					id.push_back(i);	//��¼��Χ��id
					flag = true;	//�����ཻΪ��
				}
			}
		}
	}
	else if (!isHori)	//�ָ�������ֱ��
	{
		//�������а�Χ��
		for (int i = 0; i < boxNum; i++)
		{
			//�����Χ����ȫ���ڿռ���
			if (inBox(xmin, xmax, ymin, ymax, i))
			{
				num++;
				if (data > box[i].ymin && data < box[i].ymax)
				{
					id.push_back(i);
					flag = true;
				}
			}
		}
	}
	return flag;
}

//��Χ��id�Ƿ���ȫ����ĳ���ռ���
bool BSPTree::inBox(float x1, float x2, float y1, float y2, int id)
{
	return box[id].xmin >= x1 && box[id].xmax <= x2 && box[id].ymin >= y1 && box[id].ymax <= y2;
}

//Ѱ�����ŷָ���
void BSPTree::split(float& data_x, float& data_y, float& dis_x, float& dis_y, float xmin, float xmax, float ymin, float ymax)
{
	float d = 10000;
	//�ȼ���ˮƽ����
	//�������а�Χ��
	for (int i = 0; i < boxNum; i++)
	{
		//�����Χ����ȫ���ڿռ���
		if (inBox(xmin, xmax, ymin, ymax, i))
		{
			//�����Χ����߽��ߵ����ĵľ���
			d = box[i].xmin - ((xmax - xmin) / 2 + xmin);
			if (d < 0) d = -d;
			//����и�С�ľ��룬���¾���ͷָ���
			if (d < dis_x)
			{
				dis_x = d;
				data_x = box[i].xmin;
			}

			//�����Χ���ұ߽��ߵ����ĵľ���
			d = box[i].xmax - ((xmax - xmin) / 2 + xmin);
			if (d < 0) d = -d;
			//����и�С�ľ��룬���¾���ͷָ���
			if (d < dis_x)
			{
				dis_x = d;
				data_x = box[i].xmax;
			}
		}
	}

	//�ټ�����ֱ����
	//�������а�Χ��
	for (int i = 0; i < boxNum; i++)
	{
		//�����Χ����ȫ���ڿռ���
		if (inBox(xmin, xmax, ymin, ymax, i))
		{
			//�����Χ���ϱ߽��ߵ����ĵľ���
			d = box[i].ymin - ((ymax - ymin) / 2 + ymin);
			if (d < 0)d = -d;
			//����и�С�ľ��룬���¾���ͷָ���
			if (d < dis_y)
			{
				dis_y = d;
				data_y = box[i].ymin;
			}
			//�����Χ���±߽��ߵ����ĵľ���
			d = box[i].ymax - ((ymax - ymin) / 2 + ymin);
			if (d < 0) d = -d;
			//����и�С�ľ��룬���¾���ͷָ���
			if (d < dis_y)
			{
				dis_y = d;
				data_y = box[i].ymax;
			}
		}
	}
	//������Ծ���
	dis_x /= xmax - xmin;
	dis_y /= ymax - ymin;
}

//������Ҷ�ڵ�
BSPTree::BSPNode* BSPTree::createEmptyNode()
{
	BSPNode* node = new BSPNode();
	node->left = nullptr;
	node->right = nullptr;
	node->isLeaf = true;
	node->isSolid = false;
	node->data = 0.0f;
	return node;
}

//������Ҷ�ڵ�
BSPTree::BSPNode* BSPTree::createSolidNode()
{
	BSPNode* node = new BSPNode();
	node->left = nullptr;
	node->right = nullptr;
	node->isLeaf = true;
	node->isSolid = true;
	node->data = 0.0f;
	return node;
}

//���ɽڵ�
BSPTree::BSPNode* BSPTree::genNode(bool isFull_1, bool isFull_2, int layer_count, float xmin, \
	float xmax, float ymin, float ymax, float data, bool isHori)
{
	BSPNode* node = new BSPNode();
	if (!root)
	{
		root = node;
	}
	//���û�е���������
	if (layer != layer_count)
	{
		//�������1������
		if (isFull_1)
		{
			//�ݹ鴴��
			if (isHori)node->left = build(layer_count + 1,xmin,xmax,ymin,data);
			else node->left = build(layer_count+1,xmin,data,ymin,ymax);
		}
		else //�������1�ǿյ�
		{
			//ֱ�Ӵ�����Ҷ�ڵ㣬�������ݹ�
			node->left = createEmptyNode();
		}
		
		//�������2������
		if (isFull_2)
		{
			//�ݹ鴴��
			if (isHori)node->right = build(layer_count+1,xmin,xmax,data,ymax);
			else node->right = build(layer_count+1,data,xmax,ymin,ymax);
		}
		else //�������2�ǿյ�
		{
			//ֱ�Ӵ�����Ҷ�ڵ㣬�������ݹ�
			node->right = createEmptyNode();
		}
	}
	else if (layer == layer_count)	//����ﵽ��������
	{
		//�������1������
		if (isFull_1)
		{
			//ֱ�Ӵ�����Ҷ�ڵ㣬�������ݹ�
			node->left = createSolidNode();
		}
		else //�������1�ǿյ�
		{
			//ֱ�Ӵ�����Ҷ�ڵ㣬�������ݹ�
			node->left = createEmptyNode();
		}

		//�������2������
		if (isFull_2)
		{
			//ֱ�Ӵ�����Ҷ�ڵ㣬�������ݹ�
			node->right = createSolidNode();
		}
		else //�������2�ǿյ�
		{
			//ֱ�Ӵ�����Ҷ�ڵ㣬�������ݹ�
			node->right = createEmptyNode();
		}
	}
	//���ýڵ������Ϣ
	node->isLeaf = false;
	node->isHori = isHori;
	node->data = data;
	return node;
}

//��Χ��id�Ƿ���ĳ���ռ��н���
bool BSPTree::isIntersect(float x1, float x2, float y1, float y2, int id)
{
	//���������
	//1.ˮƽ����ֱ���򶼸�������һ���߽�������������(����ǡ����������߽�)
	//2.
	return ((box[id].xmin > x1 && box[id].xmin < x2 || box[id].xmax>x1 && box[id].xmax < x2 || box[id].xmin == x1 && box[id].xmax == x2)
		&& (box[id].ymin > y1&&box[id].ymin<y2 || box[id].ymax > y1&&box[id].ymax < y2) || box[id].ymin == y1 && box[id].ymax == y2);
}

//���ָ�������������Ƿ�Ϊ��/��
void BSPTree::checkIsFull(bool& isFull_x_1, bool& isFull_x_2, bool& isFull_y_1, bool& isFull_y_2, \
	float xmin, float xmax, float ymin, float ymax, float data_x, float data_y)
{
	//�������а�Χ�У�����а�Χ����ÿռ���ڽ�������ô����ռ�������ġ�
	for (int i = 0; i < boxNum; i++)
	{
		if (!isFull_x_1 && isIntersect(xmin, data_x, ymin, ymax, i))
		{
			isFull_x_1 = true;
		}
		if (!isFull_x_2 && isIntersect(data_x, xmax, ymin, ymax, i))
		{
			isFull_x_2 = true;
		}
		if (!isFull_y_1 && isIntersect(xmin, xmax, ymin, data_y, i))
		{
			isFull_y_1 = true;
		}
		if (!isFull_y_2 && isIntersect(xmin, xmax, data_y, ymax, i))
		{
			isFull_y_2 = true;
		}
	}
	return;
}

//����bsp��
BSPTree::BSPNode* BSPTree::build(int layer_count, float xmin, float xmax, float ymin, float ymax)
{
	//�����ݹ����ֱֵ�ӷ���null
	if (layer_count == layer + 1) return nullptr;
	BSPNode* node = nullptr;

	//��ʼ��һЩ���������룬�ָ��ߣ��Ƿ��ཻ���ӿռ�/��״̬���ཻ��Χ�е�id,�ռ��ڰ�Χ�еĸ���
	float dis_x = 10000;
	float dis_y = 10000;
	float data_x = -1;
	float data_y = -1;
	bool isIntersect_x;
	bool isIntersect_y;
	bool isFull_x_1 = false;
	bool isFull_x_2 = false;
	bool isFull_y_1 = false;
	bool isFull_y_2 = false;
	std::vector<int> id_x;
	std::vector<int> id_y;
	int num_x = 0;
	int num_y = 0;

	split(data_x,data_y,dis_x,dis_y,xmin,xmax,ymin,ymax);	//�ҵ�Ԥ�������ŷ�����

	//����δ��ֵ��˵��û�п���ѡ��İ�Χ�У�Ҳ���ǿռ��ǿյģ�ֱ�ӷ��ؿ�Ҷ�ڵ㡣
	if (data_x == -1 && data_y == -1)
	{
		return createEmptyNode();
	}

	//�ж����ŷ��������Χ���Ƿ��ཻ
	isIntersect_x = isIntersect(xmin,xmax,ymin,ymax,data_x,true,id_x,num_x);
	isIntersect_y = isIntersect(xmin,xmax,ymin,ymax,data_y,false,id_y,num_y);

	//�жϷָ���ӿռ�Ϊ��/��
	checkIsFull(isFull_x_1, isFull_x_2, isFull_y_1, isFull_y_2, xmin, xmax, ymin, ymax, data_x, data_y);
	//�ռ���ֻ��һ�����壬ֱ�ӷ�����Ҷ�ڵ�
	if (num_x == 1) return createSolidNode();

	//��ֱ�ָ��߲��ཻ��ˮƽ�ָ����ཻ��ѡ����ֱ�ָ���
	if (isIntersect_x && !isIntersect_y)
	{
		node = genNode(isFull_y_1,isFull_y_2,layer_count,xmin,xmax,ymin,ymax,data_y,true);
	}
	//��ֱ�ָ��߲��뽻��ˮƽ�ָ����ཻ��ѡ����ֱ�ָ���
	else if (!isIntersect_x && isIntersect_y)
	{
		node = genNode(isFull_x_1,isFull_x_2,layer_count,xmin,xmax,ymin,ymax,data_x,false);
	}
	//���ཻ�򶼲��뽻��ѡ��������Ľ���
	else
	{
		//��ֱ����
		if (dis_x < dis_y)
		{
			//��������ཻ�����Ѱ�Χ��
			if (isIntersect_x)
			{
				for (int i = 0; i < id_x.size(); i++)
				{
					float x1 = box[id_x[i]].xmin;
					float x2 = box[id_x[i]].xmax;
					float y1 = box[id_x[i]].ymin;
					float y2 = box[id_x[i]].ymax;
					boxNum++;
					box[id_x[i]].set(x1,data_x,y1,y2);
					box_t b;
					b.set(data_x,x2,y1,y2);
					box.push_back(b);
				}
				id_x.clear();
			}
			node = genNode(isFull_x_1,isFull_x_2,layer_count,xmin,xmax,ymin,ymax,data_x,false);
		}
		//ˮƽ����
		else
		{
			//��������ཻ�����Ѱ�Χ��
			if (isIntersect_y)
			{
				for (int i = 0; i < id_y.size(); i++)
				{
					float x1 = box[id_y[i]].xmin;
					float x2 = box[id_y[i]].xmax;
					float y1 = box[id_y[i]].ymin;
					float y2 = box[id_y[i]].ymax;
					boxNum++;
					box[id_y[i]].set(x1,x2,y1,data_y);
					box_t b;
					b.set(x1,x2,data_y,y2);
					box.push_back(b);
				}
				id_y.clear();
			}
			node = genNode(isFull_y_1,isFull_y_2,layer_count,xmin,xmax,ymin,ymax,data_y,true);
		}
	}
	return node;
}

//�������
void BSPTree::build()
{
	build(1,xmin,xmax,ymin,ymax);
}

//ǰ�����
void BSPTree::print()
{
	traverse(root);
}

//ǰ��
void BSPTree::traverse(BSPNode* t)
{
	if (!t) return;
	if (t->data != 0) printf("%f ",t->data);
	else printf("leaf:%d",t->isSolid);
	if (t->isHori) printf("h\n");
	else printf("v\n");

	traverse(t->left);
	traverse(t->right);
}

//����
void BSPTree::levelOrder()
{
	std::queue<BSPNode*> q;
	q.push(root);
	while (!q.empty())
	{
		BSPNode* t = q.front();
		if (t->data != 0)printf("%f ",t->data);
		else printf("leaf:%d",t->isSolid);
		if (t->isHori)printf("h\n");
		else printf("v\n");
		q.pop();
		if (t->left != nullptr) q.push(t->left);
		if (t->right != nullptr) q.push(t->right);
	}
	return;
}