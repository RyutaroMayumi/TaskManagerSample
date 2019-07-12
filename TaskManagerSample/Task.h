#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <exception>

using namespace std;


typedef struct _DATA {

	uint16_t id;		//!�^�X�NID
	float priority;		//!�D��x
	std::string name;	//!�^�X�N��
	uint16_t gid;		//!�O���[�vID

} DATA;


//!��O
//@brief �K�萔�ȏ�̃^�X�N�I�u�W�F�N�g�𐶐����悤�Ƃ����Ƃ��ɑ��o�����
class TooManyTaskObjectException : public exception {
public:
	const char* what() const noexcept {
		return "[exception] Too many object was created.";
	}
};


//!Task
//@brief �^�X�N�N���X�@���ۂɎg���Ƃ��͂��̃N���X���p�������N���X���g��
class Task {

public:
	//constructor and destructor
	//!�f�t�H���g�R���X�g���N�^
	Task() : m_id(Task::seqIdx++), m_priority(0.5), m_name(""), m_gid(0)
	{
		if (Task::counter < Task::LIMIT_TASK_COUNT) {
			Task::counter++;
		}
		else {
			Task::seqIdx--;	// �����ł��Ȃ��ꍇ�̓C���f�b�N�X����߂�
			throw TooManyTaskObjectException();
		}
	}

	//!�R���X�g���N�^
	//!�e�f�[�^��������
	Task(string name, uint16_t gid, float priority = 0.5) : m_id(Task::seqIdx++)
	{
		if (Task::counter < Task::LIMIT_TASK_COUNT) {
			Task::counter++;
			m_priority = priority;
			m_name = name;
			m_gid = gid;
		}
		else {
			Task::seqIdx--;	// �����ł��Ȃ��ꍇ�̓C���f�b�N�X����߂�
			throw TooManyTaskObjectException();
		}
	}

	//!�R�s�[�R���X�g���N�^
	//@param [in] task
	Task(const Task &task) : m_id(0)	// �C���f�b�N�X0�̓R�s�[�R���X�g���N�^�p�Ɏg�p����
	{
		*this = task;
	}

	//!�f�X�g���N�^
	virtual ~Task() {
		Task::counter--;
	}

	//!���s�֐�
	virtual void run() {}

public:
	//operator
	//! > �I�y���[�^��`
	//@param [in] task
	//@return bool
	inline bool operator>(const Task &task) const
	{
		printf(">\n");
		return (m_priority > task.m_priority);	// �Ȃ����I�y���[�^�[���@�\���Ȃ�
	}

	//! < �I�y���[�^��`
	//@param [in] task
	//@return bool
	inline bool operator<(const Task &task) const
	{
		printf("<\n");
		return (m_priority < task.m_priority);
	}

	//! =�I�y���[�^��`
	//@param [in] task
	//@return Task&
	inline Task& operator=(const Task &task)
	{
		*this = task;

		return (*this);
	}

	//! ==�I�y���[�^��`
	//@param [in] task
	//@return bool
	inline bool operator==(const Task &task) const
	{
		return (*this == task);
	}

	//! !=�I�y���[�^��`
	//@param [in] task
	//@return bool
	inline bool operator!=(const Task &task) const
	{
		return !(*this == task);
	}

	//! >=�I�y���[�^��`
	//@param [in] task
	//@return bool
	inline bool operator>=(const Task &task) const
	{
		return (m_priority >= task.m_priority);
	}

	//! <=�I�y���[�^��`
	//@param [in] task
	//@return bool
	inline bool operator<=(const Task &task) const
	{
		return (m_priority <= task.m_priority);
	}

public:
	//getter and setter*/
	//!�^�X�N�̃f�[�^���܂Ƃ߂Ď擾����
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

	//!�^�X�N��ID���擾����
	//@return uint16_t
	inline uint16_t GetID()
	{
		return m_id;
	}

	//!�D��x���擾����
	//!@return float
	inline float GetPriority()
	{
		return m_priority;
	}

	//!�^�X�N�����擾����
	//@return string
	inline string GetTaskName()
	{
		return m_name;
	}

	//!�O���[�vID���擾����
	//@return uint16_t
	inline uint16_t GetGroupID()
	{
		return m_gid;
	}

	//!�^�X�N�̐��������擾����
	//@return uint8_t
	inline static uint8_t GetTaskCount()
	{
		return counter;
	}

	//!�D��x��ݒ肷��
	//@param [in] float
	inline void SetPriority(float p)
	{
		m_priority = p;
	}

	//!�^�X�N����ݒ肷��
	//@param [in] string
	inline void SetTaskName(string name)
	{
		this->m_name.clear();
		this->m_name = name;
	}

	//!�O���[�vID��ݒ肷��
	//@param [in] uint16_t
	inline void SetGID(uint16_t gid)
	{
		m_gid = gid;
	}

	//public://enumerator
	//enum ComparePattern{PRIORITY, NAME, ID};

private:
	static const uint8_t LIMIT_TASK_COUNT;  //!�^�X�N���̏��
	static uint8_t counter;                 //!�������ꂽ�^�X�N��
	static uint16_t seqIdx;                 //!�������ꂽ�^�X�N�̘A�ԃC���f�b�N�X

	const uint16_t m_id;					//!ID

protected:
	//protected variable
	float m_priority;	//!�D��x�i0.1~1.0�j�傫���قǗD��x������
	string m_name;		//!�^�X�N��
	uint16_t m_gid;		//!�O���[�vID

};
// �萔�̒�`
const uint8_t Task::LIMIT_TASK_COUNT = 32;
// �ÓI�ϐ��̒�`
uint8_t Task::counter = 0;
uint16_t Task::seqIdx = 1;	// �C���f�b�N�X0�̓R�s�[�R���X�g���N�^�p�Ɏg�p����

