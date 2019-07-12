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

//!タスクマネージャクラス
//@brief 全てのタスクはこのクラスで管理される.また、タスク固有の実行関数もこのクラス経由で利用される
class TaskMgr {

public:
	//construct & destructor
	//!デフォルトコンストラクタ
	TaskMgr() : m_cnt(0)
	{
		if (InitTaskList() == false)
		{
			DebugPrint("初期化に失敗しました");
			exit(-1);
		}
	}

	//!コピーコンストラクタ（いるか？）
	TaskMgr(const TaskMgr& taskmgr)
	{
		*this = taskmgr;
	}

	//!デストラクタ
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
	//!タスクをタスクリストに挿入する
	//@param [in] Task*
	void Insert(Task* task)
	{
		this->m_tasklist.push_back(task);
		this->m_cnt = static_cast<uint8_t>(m_tasklist.size());

		//優先度を保つ
		Sort();
	}

	//!タスクリストを初期化する
	//@return bool(trueなら成功)
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

	//!タスクリストに登録されているタスクの内、nameに一致するものを削除する
	//@param string
	//@return bool (削除に成功すればtrue)
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

	//!タスクリストに登録されているタスクの内、IDに一致するものを削除する
	//@param uint16_t
	//@return bool (削除に成功すればtrue)
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

	//!タスクリストに登録されているタスクの内、グループIDに一致するものを削除する
	//@param uint16_t
	//@return bool (削除に成功すればtrue)
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

	//!タスクリストを解放する
	//@return bool(trueであれば成功)
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

	//!タスクリスト内のタスクの全ての実行関数を実行する
	void ExecuteAll()
	{
		for (list<Task*>::iterator it = m_tasklist.begin(); it != m_tasklist.end(); it++)
		{
			Execute(it);
		}
	}

	//!タスクリスト内の特定のIDに一致するタスクを実行する
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

	//!タスクリスト内の特定のグループIDに一致するタスクを実行する
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

	//!タスクの実行関数を実行する
	//@param list<Task*>::iterator
	inline void Execute(list < Task* >::iterator it)
	{
		Task* task = *it;

		task->run();
	}

private:
	//ソート用静的メンバ関数（オペレータが機能しないため）
	inline static bool SortDescending(Task *task1, Task *task2)
	{
		return task1->GetPriority() > task2->GetPriority();
	}

	inline static bool SortAscending(Task *task1, Task *task2)
	{
		return task1->GetPriority() < task2->GetPriority();
	}

public:
	//!tasklistを優先度順にソートする
	//@param enum SortingType
	void Sort(enum SortingType type = DESCENDING)
	{
		if (type == DESCENDING)
		{
			//m_tasklist.sort(less<Task*>());	// オペレータが機能しない
			m_tasklist.sort(SortDescending);
		}
		else if (type == ASCENDING)
		{
			//m_tasklist.sort(greater<Task*>());	// オペレータが機能しない
			m_tasklist.sort(SortAscending);
		}
	}

public:
	//accessor
	//!管理されているタスクのうち、グループIDに一致するものを返す
	//@param enum GID
	//@return list<Task*>
	inline list<Task*> GetTaskByGID(const uint16_t GID)
	{
		list<Task*> res;	//!タスクのリスト

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

	//!管理されているタスクの内、pos番目のタスクを取得する
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

	//!指定したタスクIDのイテレータを取得する
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

	//!管理されているタスクのうち、nameに一致するものを返す
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

	//!管理されているタスクの数を取得する
	//@return int
	inline uint16_t GetTaskCount()
	{
		return m_cnt;
	}

private:
	//private variable
	uint8_t m_cnt;			//!保有しているタスクの数
	list<Task*> m_tasklist;	//!タスクのリスト
};

