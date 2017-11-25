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

//某一个空间中的分割线是否与空间中的一个包围盒相交
bool BSPTree::isIntersect(float xmin, float xmax, float ymin, float ymax, \
	float data, bool isHori, std::vector<int>& id, int& num)
{
	bool flag = false;	//记录是否存在相交
	//分割线是水平的
	if (isHori)
	{
		//遍历所有包围盒
		for (int i = 0; i < boxNum; i++)
		{
			//如果包围盒完全处在空间中
			if (inBox(xmin, xmax, ymin, ymax, i))
			{
				num++;	//记录包围盒个数+1
				if (data > box[i].xmin && data < box[i].xmax)	//存在相交
				{
					id.push_back(i);	//记录包围盒id
					flag = true;	//存在相交为真
				}
			}
		}
	}
	else if (!isHori)	//分割线是竖直的
	{
		//遍历所有包围盒
		for (int i = 0; i < boxNum; i++)
		{
			//如果包围盒完全处在空间中
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

//包围盒id是否完全处在某个空间中
bool BSPTree::inBox(float x1, float x2, float y1, float y2, int id)
{
	return box[id].xmin >= x1 && box[id].xmax <= x2 && box[id].ymin >= y1 && box[id].ymax <= y2;
}

//寻找最优分割线
void BSPTree::split(float& data_x, float& data_y, float& dis_x, float& dis_y, float xmin, float xmax, float ymin, float ymax)
{
	float d = 10000;
	//先计算水平方向
	//遍历所有包围盒
	for (int i = 0; i < boxNum; i++)
	{
		//如果包围盒完全处在空间中
		if (inBox(xmin, xmax, ymin, ymax, i))
		{
			//计算包围盒左边界线到中心的距离
			d = box[i].xmin - ((xmax - xmin) / 2 + xmin);
			if (d < 0) d = -d;
			//如果有更小的距离，更新距离和分割线
			if (d < dis_x)
			{
				dis_x = d;
				data_x = box[i].xmin;
			}

			//计算包围盒右边界线到中心的距离
			d = box[i].xmax - ((xmax - xmin) / 2 + xmin);
			if (d < 0) d = -d;
			//如果有更小的距离，更新距离和分割线
			if (d < dis_x)
			{
				dis_x = d;
				data_x = box[i].xmax;
			}
		}
	}

	//再计算竖直方向
	//遍历所有包围盒
	for (int i = 0; i < boxNum; i++)
	{
		//如果包围盒完全处在空间中
		if (inBox(xmin, xmax, ymin, ymax, i))
		{
			//计算包围盒上边界线到中心的距离
			d = box[i].ymin - ((ymax - ymin) / 2 + ymin);
			if (d < 0)d = -d;
			//如果有更小的距离，更新距离和分割线
			if (d < dis_y)
			{
				dis_y = d;
				data_y = box[i].ymin;
			}
			//计算包围盒下边界线到中心的距离
			d = box[i].ymax - ((ymax - ymin) / 2 + ymin);
			if (d < 0) d = -d;
			//如果有更小的距离，更新距离和分割线
			if (d < dis_y)
			{
				dis_y = d;
				data_y = box[i].ymax;
			}
		}
	}
	//计算相对距离
	dis_x /= xmax - xmin;
	dis_y /= ymax - ymin;
}

//创建空叶节点
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

//创建空叶节点
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

//生成节点
BSPTree::BSPNode* BSPTree::genNode(bool isFull_1, bool isFull_2, int layer_count, float xmin, \
	float xmax, float ymin, float ymax, float data, bool isHori)
{
	BSPNode* node = new BSPNode();
	if (!root)
	{
		root = node;
	}
	//如果没有到达最大深度
	if (layer != layer_count)
	{
		//如果区域1是满的
		if (isFull_1)
		{
			//递归创建
			if (isHori)node->left = build(layer_count + 1,xmin,xmax,ymin,data);
			else node->left = build(layer_count+1,xmin,data,ymin,ymax);
		}
		else //如果区域1是空的
		{
			//直接创建空叶节点，不继续递归
			node->left = createEmptyNode();
		}
		
		//如果区域2是满的
		if (isFull_2)
		{
			//递归创建
			if (isHori)node->right = build(layer_count+1,xmin,xmax,data,ymax);
			else node->right = build(layer_count+1,data,xmax,ymin,ymax);
		}
		else //如果区域2是空的
		{
			//直接创建空叶节点，不继续递归
			node->right = createEmptyNode();
		}
	}
	else if (layer == layer_count)	//如果达到了最大深度
	{
		//如果区域1是满的
		if (isFull_1)
		{
			//直接创建满叶节点，不继续递归
			node->left = createSolidNode();
		}
		else //如果区域1是空的
		{
			//直接创建空叶节点，不继续递归
			node->left = createEmptyNode();
		}

		//如果区域2是满的
		if (isFull_2)
		{
			//直接创建满叶节点，不继续递归
			node->right = createSolidNode();
		}
		else //如果区域2是空的
		{
			//直接创建空叶节点，不继续递归
			node->right = createEmptyNode();
		}
	}
	//设置节点基本信息
	node->isLeaf = false;
	node->isHori = isHori;
	node->data = data;
	return node;
}

//包围盒id是否与某个空间有交集
bool BSPTree::isIntersect(float x1, float x2, float y1, float y2, int id)
{
	//两种情况：
	//1.水平，竖直方向都各有至少一条边界线落在区域内(不含恰好落在区域边界)
	//2.
	return ((box[id].xmin > x1 && box[id].xmin < x2 || box[id].xmax>x1 && box[id].xmax < x2 || box[id].xmin == x1 && box[id].xmax == x2)
		&& (box[id].ymin > y1&&box[id].ymin<y2 || box[id].ymax > y1&&box[id].ymax < y2) || box[id].ymin == y1 && box[id].ymax == y2);
}

//检测分割出的两个区域是否为满/空
void BSPTree::checkIsFull(bool& isFull_x_1, bool& isFull_x_2, bool& isFull_y_1, bool& isFull_y_2, \
	float xmin, float xmax, float ymin, float ymax, float data_x, float data_y)
{
	//遍历所有包围盒，如果有包围盒与该空间存在交集，那么这个空间就是满的。
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

//创建bsp树
BSPTree::BSPNode* BSPTree::build(int layer_count, float xmin, float xmax, float ymin, float ymax)
{
	//超过递归深度值直接返回null
	if (layer_count == layer + 1) return nullptr;
	BSPNode* node = nullptr;

	//初始化一些变量：距离，分割线，是否相交，子空间/满状态，相交包围盒的id,空间内包围盒的个数
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

	split(data_x,data_y,dis_x,dis_y,xmin,xmax,ymin,ymax);	//找到预备的最优分裂线

	//两者未赋值，说明没有可以选择的包围盒，也就是空间是空的，直接返回空叶节点。
	if (data_x == -1 && data_y == -1)
	{
		return createEmptyNode();
	}

	//判断最优分裂线与包围盒是否相交
	isIntersect_x = isIntersect(xmin,xmax,ymin,ymax,data_x,true,id_x,num_x);
	isIntersect_y = isIntersect(xmin,xmax,ymin,ymax,data_y,false,id_y,num_y);

	//判断分割的子空间为空/满
	checkIsFull(isFull_x_1, isFull_x_2, isFull_y_1, isFull_y_2, xmin, xmax, ymin, ymax, data_x, data_y);
	//空间中只有一个物体，直接返回满叶节点
	if (num_x == 1) return createSolidNode();

	//竖直分割线不相交，水平分割线相交，选择竖直分割线
	if (isIntersect_x && !isIntersect_y)
	{
		node = genNode(isFull_y_1,isFull_y_2,layer_count,xmin,xmax,ymin,ymax,data_y,true);
	}
	//竖直分割线不想交，水平分割线相交，选择竖直分割线
	else if (!isIntersect_x && isIntersect_y)
	{
		node = genNode(isFull_x_1,isFull_x_2,layer_count,xmin,xmax,ymin,ymax,data_x,false);
	}
	//都相交或都不想交，选择距离中心近的
	else
	{
		//竖直更近
		if (dis_x < dis_y)
		{
			//如果存在相交，分裂包围盒
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
		//水平更近
		else
		{
			//如果存在相交，分裂包围盒
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

//创建入口
void BSPTree::build()
{
	build(1,xmin,xmax,ymin,ymax);
}

//前序输出
void BSPTree::print()
{
	traverse(root);
}

//前序
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

//层序
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