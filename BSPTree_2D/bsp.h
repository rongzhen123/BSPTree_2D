#pragma once
#include <vector>

class BSPTree
{
private:
	struct BSPNode
	{
		BSPNode* left;
		BSPNode* right;
		bool isLeaf;	//是否是叶节点
		bool isSolid;	//是否实心
		bool isHori;	//是否水平
		float data;		//分割线
	};//节点数据结构
	struct box_t
	{
		float xmin;
		float xmax;
		float ymin;
		float ymax;
		box_t();
		void set(float x1,float x2,float y1,float y2);
	};//包围盒数据结构
	BSPNode* root;	//根节点
	std::vector<box_t> box;//包围盒容器
	float xmin, xmax, ymin, ymax;		//整个场景的轴向包围盒
	int boxNum;				//包围盒的个数
	int layer;				//树的最大深度
	int layer_count;		//记录当前层数
	BSPNode* createEmptyNode();	//生成空叶节点
	BSPNode* createSolidNode();	//生成非空叶节点
	void split(float& data_x,float& data_y,float& dis_x,float& dis_y,float xmin,float xmax,\
						float ymin,float ymax);	//寻找最优分割线
	BSPNode* genNode(bool isFull_1,bool isFull_2,int layer_count,float xmin,float xmax,\
						float ymin,float ymax,float data,bool isHori);		//生成新的节点
	BSPNode* build(int layer_count,float xmin,float xmax,float ymin,float ymax);	//创建新的节点
	bool isIntersect(float xmin,float xmax,float ymin,float ymax,float data,bool isHori,\
						std::vector<int>& id,int& num);	//某一个空间中的分割线是否与空间中的一个包围盒相交
	void traverse(BSPNode* t);		//前序遍历
	bool inBox(float x1,float x2,float y1,float y2,int id);	//包围盒id是否完全处在某个空间中
	void checkIsFull(bool& isFull_x_1,bool& isFull_x_2,bool& isFull_y_1,bool& isFull_y_2,\
			float xmin,float xmax,float ymin,float ymax,float data_x,float data_y);//检测分割出的两个区域是否为满/空
	bool isIntersect(float x1,float x2,float y1,float y2,int id);	//包围盒id是否与某个空间有交集

public:
	BSPTree(float x1,float x2,float y1,float y2,int l);	//构造函数，前四个参数为场景包围盒，l为最大递归深度
	void add(float x1,float x2,float y1,float y2);		//添加包围盒
	void build();		//创建bsp树
	void print();		//前序遍历输出
	void levelOrder();	//层次遍历输出
};
