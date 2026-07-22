#include "StateMachine.h"
#include <typeinfo>

SingularStateMachine::~SingularStateMachine()
{
	delete m_State;
}

void SingularStateMachine::Update(float deltaTime, GameObject* gameObject)
{
	m_State->Update(deltaTime, gameObject);
}

void SingularStateMachine::CheckForNextState()
{
	State* newState = m_State->CheckForNextState();

	if (newState != nullptr)
	{
		delete m_State;
		m_State = newState;
	}
}



PushDownStateMachine::~PushDownStateMachine()
{
	m_States.empty();
}

void PushDownStateMachine::Update(float deltaTime, GameObject* gameObject)
{
	m_States.top()->Update(deltaTime, gameObject);
}

void PushDownStateMachine::CheckForNextState()
{
	State* newState = m_States.top()->CheckForNextState();

	if (newState == nullptr)
	{
		return;
	}

	State* previousState = m_States.top();
	m_States.pop();

	//check if new state is same as state before previous state
	//if true, leave popped
	if (typeid(newState) == typeid(m_States.top()) ||
		typeid(newState) == typeid(UnknownState))
	{
		m_States.top() = newState;
	}
	else
	{
		//if stack too big, clear it
		if (m_States.size() > 50)
		{
			m_States.empty();
		}

		m_States.push(previousState);
		m_States.push(newState);
	}
}
