#pragma once
#include <vector>

class BSPTree
{
private:
	struct BSPNode
	{
		BSPNode* left;
		BSPNode* right;
		bool isLeaf;	//�Ƿ���Ҷ�ڵ�
		bool isSolid;	//�Ƿ�ʵ��
		bool isHori;	//�Ƿ�ˮƽ
		float data;		//�ָ���
	};//�ڵ����ݽṹ
	struct box_t
	{
		float xmin;
		float xmax;
		float ymin;
		float ymax;
		box_t();
		void set(float x1,float x2,float y1,float y2);
	};//��Χ�����ݽṹ
	BSPNode* root;	//���ڵ�
	std::vector<box_t> box;//��Χ������
	float xmin, xmax, ymin, ymax;		//���������������Χ��
	int boxNum;				//��Χ�еĸ���
	int layer;				//����������
	int layer_count;		//��¼��ǰ����
	BSPNode* createEmptyNode();	//���ɿ�Ҷ�ڵ�
	BSPNode* createSolidNode();	//���ɷǿ�Ҷ�ڵ�
	void split(float& data_x,float& data_y,float& dis_x,float& dis_y,float xmin,float xmax,\
						float ymin,float ymax);	//Ѱ�����ŷָ���
	BSPNode* genNode(bool isFull_1,bool isFull_2,int layer_count,float xmin,float xmax,\
						float ymin,float ymax,float data,bool isHori);		//�����µĽڵ�
	BSPNode* build(int layer_count,float xmin,float xmax,float ymin,float ymax);	//�����µĽڵ�
	bool isIntersect(float xmin,float xmax,float ymin,float ymax,float data,bool isHori,\
						std::vector<int>& id,int& num);	//ĳһ���ռ��еķָ����Ƿ���ռ��е�һ����Χ���ཻ
	void traverse(BSPNode* t);		//ǰ�����
	bool inBox(float x1,float x2,float y1,float y2,int id);	//��Χ��id�Ƿ���ȫ����ĳ���ռ���
	void checkIsFull(bool& isFull_x_1,bool& isFull_x_2,bool& isFull_y_1,bool& isFull_y_2,\
			float xmin,float xmax,float ymin,float ymax,float data_x,float data_y);//���ָ�������������Ƿ�Ϊ��/��
	bool isIntersect(float x1,float x2,float y1,float y2,int id);	//��Χ��id�Ƿ���ĳ���ռ��н���

public:
	BSPTree(float x1,float x2,float y1,float y2,int l);	//���캯����ǰ�ĸ�����Ϊ������Χ�У�lΪ���ݹ����
	void add(float x1,float x2,float y1,float y2);		//��Ӱ�Χ��
	void build();		//����bsp��
	void print();		//ǰ��������
	void levelOrder();	//��α������
};
