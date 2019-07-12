#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <exception>

using namespace std;


typedef struct _DATA {

	uint16_t id;		//!タスクID
	float priority;		//!優先度
	std::string name;	//!タスク名
	uint16_t gid;		//!グループID

} DATA;


//!例外
//@brief 規定数以上のタスクオブジェクトを生成しようとしたときに送出される
class TooManyTaskObjectException : public exception {
public:
	const char* what() const noexcept {
		return "[exception] Too many object was created.";
	}
};


//!Task
//@brief タスククラス　実際に使うときはこのクラスを継承したクラスを使う
class Task {

public:
	//constructor and destructor
	//!デフォルトコンストラクタ
	Task() : m_id(Task::seqIdx++), m_priority(0.5), m_name(""), m_gid(0)
	{
		if (Task::counter < Task::LIMIT_TASK_COUNT) {
			Task::counter++;
		}
		else {
			Task::seqIdx--;	// 生成できない場合はインデックスを一つ戻す
			throw TooManyTaskObjectException();
		}
	}

	//!コンストラクタ
	//!各データを初期化
	Task(string name, uint16_t gid, float priority = 0.5) : m_id(Task::seqIdx++)
	{
		if (Task::counter < Task::LIMIT_TASK_COUNT) {
			Task::counter++;
			m_priority = priority;
			m_name = name;
			m_gid = gid;
		}
		else {
			Task::seqIdx--;	// 生成できない場合はインデックスを一つ戻す
			throw TooManyTaskObjectException();
		}
	}

	//!コピーコンストラクタ
	//@param [in] task
	Task(const Task &task) : m_id(0)	// インデックス0はコピーコンストラクタ用に使用する
	{
		*this = task;
	}

	//!デストラクタ
	virtual ~Task() {
		Task::counter--;
	}

	//!実行関数
	virtual void run() {}

public:
	//operator
	//! > オペレータ定義
	//@param [in] task
	//@return bool
	inline bool operator>(const Task &task) const
	{
		printf(">\n");
		return (m_priority > task.m_priority);	// なぜかオペレーターが機能しない
	}

	//! < オペレータ定義
	//@param [in] task
	//@return bool
	inline bool operator<(const Task &task) const
	{
		printf("<\n");
		return (m_priority < task.m_priority);
	}

	//! =オペレータ定義
	//@param [in] task
	//@return Task&
	inline Task& operator=(const Task &task)
	{
		*this = task;

		return (*this);
	}

	//! ==オペレータ定義
	//@param [in] task
	//@return bool
	inline bool operator==(const Task &task) const
	{
		return (*this == task);
	}

	//! !=オペレータ定義
	//@param [in] task
	//@return bool
	inline bool operator!=(const Task &task) const
	{
		return !(*this == task);
	}

	//! >=オペレータ定義
	//@param [in] task
	//@return bool
	inline bool operator>=(const Task &task) const
	{
		return (m_priority >= task.m_priority);
	}

	//! <=オペレータ定義
	//@param [in] task
	//@return bool
	inline bool operator<=(const Task &task) const
	{
		return (m_priority <= task.m_priority);
	}

public:
	//getter and setter*/
	//!タスクのデータをまとめて取得する
	//@param [out] data
	inline DATA GetDATA()
	{
		DATA data;
		data.id = m_id;
		data.priority = m_priority;
		data.name = m_name;
		data.gid = m_gid;

		return data;
	}

	//!タスクのIDを取得する
	//@return uint16_t
	inline uint16_t GetID()
	{
		return m_id;
	}

	//!優先度を取得する
	//!@return float
	inline float GetPriority()
	{
		return m_priority;
	}

	//!タスク名を取得する
	//@return string
	inline string GetTaskName()
	{
		return m_name;
	}

	//!グループIDを取得する
	//@return uint16_t
	inline uint16_t GetGroupID()
	{
		return m_gid;
	}

	//!タスクの生成数を取得する
	//@return uint8_t
	inline static uint8_t GetTaskCount()
	{
		return counter;
	}

	//!優先度を設定する
	//@param [in] float
	inline void SetPriority(float p)
	{
		m_priority = p;
	}

	//!タスク名を設定する
	//@param [in] string
	inline void SetTaskName(string name)
	{
		this->m_name.clear();
		this->m_name = name;
	}

	//!グループIDを設定する
	//@param [in] uint16_t
	inline void SetGID(uint16_t gid)
	{
		m_gid = gid;
	}

	//public://enumerator
	//enum ComparePattern{PRIORITY, NAME, ID};

private:
	static const uint8_t LIMIT_TASK_COUNT;  //!タスク数の上限
	static uint8_t counter;                 //!生成されたタスク数
	static uint16_t seqIdx;                 //!生成されたタスクの連番インデックス

	const uint16_t m_id;					//!ID

protected:
	//protected variable
	float m_priority;	//!優先度（0.1~1.0）大きいほど優先度が高い
	string m_name;		//!タスク名
	uint16_t m_gid;		//!グループID

};
// 定数の定義
const uint8_t Task::LIMIT_TASK_COUNT = 32;
// 静的変数の定義
uint8_t Task::counter = 0;
uint16_t Task::seqIdx = 1;	// インデックス0はコピーコンストラクタ用に使用する

