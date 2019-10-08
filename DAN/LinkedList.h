//===================================================================================================================================
//�yLinkedList.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/29
// [�X�V��]2019/10/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//�y�m�[�h�\���́z
//===================================================================================================================================
template <class T>
struct Node
{
	T value;
	Node<T>* prev;
	Node<T>* next;
	Node()			{ prev = NULL; next = NULL; }
	~Node()		{ prev = NULL; next = NULL; }
};

//===================================================================================================================================
//�y�o�����z���X�g�N���X�z
//===================================================================================================================================
template <class T>
class LinkedList:Base
{
protected:
	Node<T>* head;			//�擪�_�~�[�m�[�h
	Node<T>* current;		//���ڃ_�~�[�m�[�h
	Node<T>** nodeList;		//�S�m�[�h�̃|�C���^�z��
public:
	//Data
	int nodeNum;		//�S�m�[�h��

	//===================================================================================================================================
	//�ygetter�z
	//===================================================================================================================================
	Node<T>* getNode(int i) { return nodeList[i]; }		//�����_���A�N�Z�X�p���X�g�ɃA�N�Z�X
	T* getValue(int i) { return &nodeList[i]->value; }		//�m�[�h���̒l�ɃA�N�Z�X

	//===================================================================================================================================
	//�y�R���X�g���N�^�z
	//===================================================================================================================================
	LinkedList()
	{
		head = new Node<T>;
		head->next = head;			//�擪�_�~�[�m�[�h�̎��m�[�h�͎��g
		head->prev = head;			//�擪�_�~�[�m�[�h�̑O�m�[�h�͎��g
		current = head;					//�������ڃm�[�h�͐擪�_�~�[�m�[�h
		nodeNum = 0;
	}

	//===================================================================================================================================
	//�y�f�X�g���N�^�z
	//===================================================================================================================================
	~LinkedList()
	{

	}

	//===================================================================================================================================
	//�y�I�������z
	//===================================================================================================================================
	void terminate()
	{
		allClear();
		SAFE_DELETE(head);
	}

	//===================================================================================================================================
	//�y�m�[�h�̃Z�b�g�z
	//===================================================================================================================================
	void setNode(Node<T>* node, Node<T>* prev, Node<T>* next)
	{
		node->prev = prev;			//�O�m�[�h�ւ̃|�C���^
		node->next = next;			//���m�[�h�ւ̃|�C���^
	}

	//===================================================================================================================================
	//�y�󂩂ǂ����z
	//===================================================================================================================================
	bool isEnpty()
	{
		return head->next == head;
	}

	//===================================================================================================================================
	//�y�w��m�[�h�̌��ɑ}���z
	// Prev0		:	[node	]	[target]		[node	]					:�����O�̒��ڃm�[�h�ʒu
	// Next1	:	[node	]	[target]		[newNode]	[node	]	:���ڃm�[�h�̎��ɐV�K�ǉ�
	// Next2	:	[node	]	[node	]	[target]		[node	]	:�V�K�m�[�h�𒅖ڃm�[�h�֕ύX
	//===================================================================================================================================
	void insertAfter(Node<T>* target)
	{
		//���ڃm�[�h�̎��̈ʒu�ɐV�K�ǉ�
		Node<T>* newNode = new Node<T>;
		Node<T>* nextNode = target->next;					//�ύX�O�ɑΏۃm�[�h�̎��m�[�h�ۑ�����
		target->next = target->next->prev = newNode;	//�V�K�m�[�h�̑}��
		setNode(newNode, target, nextNode);					//�V�K�m�[�h�̃Z�b�g
		nodeNum++;
	}

	//===================================================================================================================================
	//�y�w��m�[�h���폜�z
	//Prev0	:	[node	]	[current]	[node	]	:�����O�̒��ڃm�[�h�ʒu
	//Next1	:	[current]	[deleteN]	[node	]	:���ڃm�[�h���폜�Ώۂɂ��A�폜�Ώۂ̑O�m�[�h�𒅖ڃm�[�h��
	//Next2	:	[current]	[node	]				:�폜�Ώۂ��폜
	//===================================================================================================================================
	bool remove(Node<T>* target)
	{
		if (target == head)return false;
		target->prev->next = target->next;			//�Ώۃm�[�h�̑O�m�[�h�̎��m�[�h�̓���ւ�
		target->next->prev = target->prev;			//�Ώۃm�[�h�̎��m�[�h�̑O�m�[�h�̓���ւ�
		delete target;
		nodeNum--;
		return true;
	}

	//===================================================================================================================================
	//�y�擪�m�[�h�̍폜�z
	//===================================================================================================================================
	void removeFront()
	{
		remove(head->next);
	}

	//===================================================================================================================================
	//�y�擪�Ƀm�[�h��}���z
	//===================================================================================================================================
	void insertFront()
	{
		insertAfter(head);
	}

	//===================================================================================================================================
	//�y�擪�Ƀm�[�h��}�����A�f�[�^���`�z
	//===================================================================================================================================
	void insertFront(T value)
	{
		insertAfter(head);
		head->next->value = value;
	}

	//===================================================================================================================================
	//�y�����_���A�N�Z�X�p�|�C���^�z��̍X�V�z
	//===================================================================================================================================
	void listUpdate()
	{
		SAFE_DELETE_ARRAY(nodeList);				//�|�C���^�z����폜
		if (nodeNum <= 0)return;						//�m�[�h����0�̏ꍇ�I��
		nodeList = new Node<T>*[nodeNum];	//�S�m�[�h�����̃|�C���^���������m��
		current = head->next;							//�擪�A�h���X�̎��̃|�C���^����J�n
		int iterator = 0;										//list�z�񃁃����A�N�Z�X�h�~�C�e���[�^
		
		//while�I�������F���݂̎w�������m�[�h���擪�m�[�h���C�e���[�^���S�m�[�h��
		while (current != head && iterator != nodeNum)
		{
			nodeList[iterator] = current;				//���݂̃m�[�h�|�C���^���|�C���^�z��֋L�^
			current = current->next;					//���̃m�[�h�|�C���^�ֈړ�
			iterator++;										//iterator������
		}
	}

	//===================================================================================================================================
	//�y�S�Ẵm�[�h�̍폜�z
	//===================================================================================================================================
	void allClear()
	{
		bool isRemaining = true;
		while (isRemaining)
		{
			isRemaining = remove(head->next);
		}
		SAFE_DELETE_ARRAY(nodeList);				//�|�C���^�z����폜
	}

};
