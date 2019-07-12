#pragma once

#include "Task.h"
#include <functional>
#include <list>
//#include <exception>

#define DebugPrint( message )	fprintf( stderr, message )

enum SortingType {

	DESCENDING,
	ASCENDING,

};

using namespace std;

//!�^�X�N�}�l�[�W���N���X
//@brief �S�Ẵ^�X�N�͂��̃N���X�ŊǗ������.�܂��A�^�X�N�ŗL�̎��s�֐������̃N���X�o�R�ŗ��p�����
class TaskMgr {

public:
	//construct & destructor
	//!�f�t�H���g�R���X�g���N�^
	TaskMgr() : m_cnt(0)
	{
		if (InitTaskList() == false)
		{
			DebugPrint("�������Ɏ��s���܂���");
			exit(-1);
		}
	}

	//!�R�s�[�R���X�g���N�^�i���邩�H�j
	TaskMgr(const TaskMgr& taskmgr)
	{
		*this = taskmgr;
	}

	//!�f�X�g���N�^
	~TaskMgr()
	{
		ReleaseTaskList();
	}

public:
	//operator
	inline bool operator==(const TaskMgr &t)
	{
		return (*this == t);
	}

public:
	//public functions
	//!�^�X�N���^�X�N���X�g�ɑ}������
	//@param [in] Task*
	void Insert(Task* task)
	{
		this->m_tasklist.push_back(task);
		this->m_cnt = static_cast<uint8_t>(m_tasklist.size());

		//�D��x��ۂ�
		Sort();
	}

	//!�^�X�N���X�g������������
	//@return bool(true�Ȃ琬��)
	bool InitTaskList()
	{
		bool res = true;

		//clear all task
		ReleaseTaskList();
		m_cnt = static_cast<uint8_t>(m_tasklist.size());

		if (m_tasklist.size())
		{
#if DEBUG
			DebugPrintfLn("tasklist initialize is false...");
#endif
			res = false;
		};

#if DEBUG
		DebugPrintfLn("tasklist initialize is success...");
#endif
		return res;
	}

	//!�^�X�N���X�g�ɓo�^����Ă���^�X�N�̓��Aname�Ɉ�v������̂��폜����
	//@param string
	//@return bool (�폜�ɐ��������true)
	bool Delete(string name)
	{
		bool res = false;

		//for (list<Task*>::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		//{
		//	Task * task = *it;

		//	if (task->GetTaskName() == name)
		//	{
		//		res = true;

		//		it = m_tasklist.erase(it);
		//		m_cnt = static_cast<uint8_t>(m_tasklist.size());

		//		if (it == m_tasklist.end())
		//		{
		//			break;
		//		}
		//	}
		//}
		list<Task*>::iterator it = m_tasklist.begin();
		while (it != m_tasklist.end())
		{
			Task * task = *it;

			if (task->GetTaskName() == name)
			{
				res = true;

				it = m_tasklist.erase(it);
				m_cnt = static_cast<uint8_t>(m_tasklist.size());

				continue;
			}

			it++;
		}

		Sort();

		return res;
	}

	//!�^�X�N���X�g�ɓo�^����Ă���^�X�N�̓��AID�Ɉ�v������̂��폜����
	//@param uint16_t
	//@return bool (�폜�ɐ��������true)
	bool DeleteByID(uint16_t id)
	{
		bool res = false;

		//for (list< Task* >::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		//{
		//	Task * task = *it;

		//	if (task->GetID() == id)
		//	{
		//		res = true;

		//		it = m_tasklist.erase(it);
		//		m_cnt = static_cast<uint8_t>(m_tasklist.size());

		//		if (it == m_tasklist.end())
		//		{
		//			break;
		//		}
		//	}
		//}
		list<Task*>::iterator it = m_tasklist.begin();
		while (it != m_tasklist.end())
		{
			Task * task = *it;

			if (task->GetID() == id)
			{
				res = true;

				it = m_tasklist.erase(it);
				m_cnt = static_cast<uint8_t>(m_tasklist.size());

				continue;
			}

			it++;
		}

		Sort();

		return res;
	}

	//!�^�X�N���X�g�ɓo�^����Ă���^�X�N�̓��A�O���[�vID�Ɉ�v������̂��폜����
	//@param uint16_t
	//@return bool (�폜�ɐ��������true)
	bool DeleteByGID(uint16_t gid)
	{
		bool res = false;

		list<Task*>::iterator it = m_tasklist.begin();
		while (it != m_tasklist.end())
		{
			Task * task = *it;

			if (task->GetGroupID() == gid)
			{
				res = true;

				it = m_tasklist.erase(it);
				m_cnt = static_cast<uint8_t>(m_tasklist.size());

				continue;
			}

			it++;
		}

		Sort();

		return res;
	}

	//!�^�X�N���X�g���������
	//@return bool(true�ł���ΐ���)
	bool ReleaseTaskList()
	{
		bool res = true;

		m_tasklist.clear();

		m_cnt = static_cast<uint8_t>(m_tasklist.size());

		if (!m_tasklist.empty() && m_cnt != 0)
		{
#if DEBUG
			DebugPrintfLn("TaskList Release is false... ");
#endif
			res = false;
		}
#if DEBUG
		DebugPrintfLn("TaskList Release is success... ");
#endif
		return res;
	}

	//!�^�X�N���X�g���̃^�X�N�̑S�Ă̎��s�֐������s����
	void ExecuteAll()
	{
		for (list<Task*>::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		{
			Execute(it);
		}
	}

	//!�^�X�N���X�g���̓����ID�Ɉ�v����^�X�N�����s����
	//@param [in] enum ID
	void ExecuteByID(uint16_t ID)
	{
		for (list< Task* >::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		{
			Task* task = *it;

			if (ID == task->GetID())
			{
				Execute(it);
			}
		}
	}

	//!�^�X�N���X�g���̓���̃O���[�vID�Ɉ�v����^�X�N�����s����
	//@param [in] enum GID
	void ExecuteByGID(uint16_t GID)
	{
		for (list< Task* >::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		{
			Task* task = *it;

			if (GID == task->GetGroupID())
			{
				Execute(it);
			}
		}
	}

	//!�^�X�N�̎��s�֐������s����
	//@param list<Task*>::iterator
	inline void Execute(list < Task* >::iterator it)
	{
		Task* task = *it;

		task->run();
	}

private:
	//�\�[�g�p�ÓI�����o�֐��i�I�y���[�^���@�\���Ȃ����߁j
	inline static bool SortDescending(Task *task1, Task *task2)
	{
		return task1->GetPriority() > task2->GetPriority();
	}

	inline static bool SortAscending(Task *task1, Task *task2)
	{
		return task1->GetPriority() < task2->GetPriority();
	}

public:
	//!tasklist��D��x���Ƀ\�[�g����
	//@param enum SortingType
	void Sort(enum SortingType type = DESCENDING)
	{
		if (type == DESCENDING)
		{
			//m_tasklist.sort(less<Task*>());	// �I�y���[�^���@�\���Ȃ�
			m_tasklist.sort(SortDescending);
		}
		else if (type == ASCENDING)
		{
			//m_tasklist.sort(greater<Task*>());	// �I�y���[�^���@�\���Ȃ�
			m_tasklist.sort(SortAscending);
		}
	}

public:
	//accessor
	//!�Ǘ�����Ă���^�X�N�̂����A�O���[�vID�Ɉ�v������̂�Ԃ�
	//@param enum GID
	//@return list<Task*>
	inline list<Task*> GetTaskByGID(const uint16_t GID)
	{
		list<Task*> res;	//!�^�X�N�̃��X�g

		for (list< Task *>::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		{
			Task* task = *it;
			if (GID == task->GetGroupID())
			{
				res.push_back(task);
			}
		}

		return res;
	}

	//!�Ǘ�����Ă���^�X�N�̓��Apos�Ԗڂ̃^�X�N���擾����
	//@param list<Task*>::iterator pos
	//@return Task*
	inline Task* GetTask(const list<Task*>::iterator pos)
	{
		//Task* task = *pos;
		Task *task = nullptr;
		if (pos != m_tasklist.end()) {
			task = *pos;
		}

		return task;
	}

	//!�w�肵���^�X�NID�̃C�e���[�^���擾����
	//@return list<Task*>::iterator
	inline list<Task*>::iterator GetTaskPos(const uint16_t ID)
	{
		for (list< Task *>::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		{
			Task* task = *it;
			if (ID == task->GetID())
			{
				return it;
			}
		}
		return m_tasklist.end();
	}

	//!�Ǘ�����Ă���^�X�N�̂����Aname�Ɉ�v������̂�Ԃ�
	//@param [in] string
	//@return Task*
	inline Task* GetTaskByName(string name)
	{
		Task* res = NULL;

		for (list< Task *>::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		{
			Task* task = *it;

			if (name == task->GetTaskName())
			{
				res = task;
			}

		}

		return res;
	}

	//!�Ǘ�����Ă���^�X�N�̐����擾����
	//@return int
	inline uint16_t GetTaskCount()
	{
		return m_cnt;
	}

private:
	//private variable
	uint8_t m_cnt;			//!�ۗL���Ă���^�X�N�̐�
	list<Task*> m_tasklist;	//!�^�X�N�̃��X�g
};

