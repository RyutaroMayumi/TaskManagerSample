// http://d.hatena.ne.jp/Crest/20090521/1242913566
// https://cpprefjp.github.io/reference/exception/exception.html
// http://ikautak.hatenablog.com/entry/2011/05/28/212100
// https://teratail.com/questions/79022

#include "Task.h"
#include "TaskMgr.h"
#include <iostream>


class SampleTask1 : public Task {

public:
	SampleTask1() { m_priority = 0.6f; m_name = "SampleTask1"; m_gid = 1; }
	SampleTask1(const SampleTask1 &task) : Task(task) {}
	~SampleTask1() { std::cout << "Class SampleTask1's destructor was called." << std::endl; }

	void run(void)
	{
		std::cout << "[run] タスク<" << this->GetTaskName() << ":" << this->GetID() << ":" << this->GetGroupID() << ">が起動しました" << std::endl;
	}

};

class SampleTask2 : public Task {

public:
	SampleTask2() { m_priority = 0.5f; m_name = "SampleTask2"; m_gid = 2; }
	SampleTask2(const SampleTask2 &task) : Task(task) {}
	~SampleTask2() { std::cout << "Class SampleTask2's destructor was called." << std::endl; }

	void run(void)
	{
		std::cout << "[run] タスク<" << this->GetTaskName() << ":" << this->GetID() << ":" << this->GetGroupID() << ">が起動しました" << std::endl;
	}

};

class SampleTask3 : public Task {

public:
	SampleTask3() { m_priority = 0.9f; m_name = "SampleTask3"; m_gid = 3; }
	SampleTask3(const SampleTask3 &task) : Task(task) {}
	~SampleTask3() { std::cout << "Class SampleTask3's destructor was called." << std::endl; }

	void run(void)
	{
		std::cout << "[run] タスク<" << this->GetTaskName() << ":" << this->GetID() << ":" << this->GetGroupID() << ">が起動しました" << std::endl;
	}

};

int main(int argc, char **argv)
{
	TaskMgr *mgr = new TaskMgr();

	std::cout << "----- テスト開始 -----" << std::endl;
	std::cout << "登録タスク数：" << mgr->GetTaskCount() << std::endl;
	std::cout << std::endl;

	std::cout << "----- タスクの生成とマネージャへの登録テスト -----" << std::endl;
	SampleTask1 *task1 = new SampleTask1();
	mgr->Insert(task1);
	SampleTask2 *task2 = new SampleTask2();
	mgr->Insert(task2);
	SampleTask3 *task3 = new SampleTask3();
	mgr->Insert(task3);
	SampleTask2 *task4 = new SampleTask2();
	mgr->Insert(task4);
	SampleTask1 *task5 = new SampleTask1();
	mgr->Insert(task5);
	std::cout << "登録タスク数：" << mgr->GetTaskCount() << std::endl;
	for (int i = 0; i < mgr->GetTaskCount(); i++) {
		Task *task = mgr->GetTask(mgr->GetTaskPos(i));
		if (task != NULL) {
			std::cout << "タスク名: " << task->GetTaskName() << ", "
				<< "タスクID: " << task->GetID() << ", "
				<< "グループID: " << task->GetGroupID() << std::endl;
		}
	}
	Task *task = mgr->GetTask(mgr->GetTaskPos(5));
	std::cout << std::endl;

	std::cout << "----- 生成するタスク数の制限機能のテスト -----" << std::endl;
	SampleTask3 *tasks[32];
	for (int i = 0; i < 32; i++)
	{
		try {
			tasks[i] = new SampleTask3();
		}
		catch (exception& e) {
			std::cout << e.what() << std::endl;
			break;
		}
	}
	std::cout << "生成タスク総数：" << to_string(Task::GetTaskCount()) << std::endl;
	std::cout << std::endl;

	std::cout << "----- 優先度順（降順）での実行テスト -----" << std::endl;
	mgr->Sort(DESCENDING);
	mgr->ExecuteAll();
	std::cout << std::endl;

	std::cout << "----- 優先度順（昇順）での実行テスト -----" << std::endl;
	mgr->Sort(ASCENDING);
	mgr->ExecuteAll();
	std::cout << std::endl;

	std::cout << "----- グループID指定での実行テスト -----" << std::endl;
	mgr->ExecuteByGID(1);
	std::cout << std::endl;

	std::cout << "----- 登録タスクの解除テスト -----" << std::endl;
	mgr->Delete("SampleTask2");
	mgr->DeleteByGID(2);
	std::cout << "登録タスク数：" << mgr->GetTaskCount() << std::endl;
	mgr->ExecuteAll();
	std::cout << std::endl;

	std::cout << "----- 登録タスクを解除してタスクオブジェクトを破棄するテスト -----" << std::endl;
	mgr->DeleteByID(task1->GetID());
	delete task1;
	std::cout << "登録タスク数：" << mgr->GetTaskCount() << std::endl;
	std::cout << "生成タスク総数：" << to_string(Task::GetTaskCount()) << std::endl;
	mgr->ExecuteAll();
	std::cout << std::endl;

	std::cout << "----- 破棄したタスクオブジェクトと同じグループのタスクオブジェクトを生成するテスト -----" << std::endl;
	SampleTask1 *task1_2 = new SampleTask1();
	std::cout << "生成タスク総数：" << to_string(Task::GetTaskCount()) << std::endl;
	mgr->Insert(task1_2);
	std::cout << "登録タスク数：" << mgr->GetTaskCount() << std::endl;
	mgr->ExecuteAll();
	std::cout << std::endl;

	delete task1;
	delete task2;
	delete task3;
	delete task4;
	delete task5;
	for (int i = 0; i < 32; i++) {
		delete tasks[i];
	}
	delete task1_2;
	delete mgr;

	return 0;
}

