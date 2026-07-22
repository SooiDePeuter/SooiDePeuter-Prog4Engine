class GameObject;

#include <stack>

class State
{

	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

public:

	State()
	{
		OnEnter();
	}
	virtual ~State()
	{
		OnExit();
	}

	virtual State* CheckForNextState() = 0;

	//for actions that take longer than one frame. for single-frame actions, use OnEnter/OnExit
	virtual void Update(float deltaTime, GameObject* gameObject) = 0;
};

//fallback state for PushDownStateMachine, resembles the previous state
class UnknownState final : public State
{
};

//no FSM, the engine should be unaware of the game it is runnung
// ->child states should be defined inside the game

class SingularStateMachine final
{
	State* m_State{nullptr};

	SingularStateMachine() = default;
	~SingularStateMachine();

public:
	
	void Update(float deltaTime, GameObject* gameObject);
	void CheckForNextState();
};

class PushDownStateMachine final
{
	std::stack<State*> m_States;

	PushDownStateMachine() = default;
	~PushDownStateMachine();

public:

	void Update(float deltaTime, GameObject* gameObject);
	void CheckForNextState();
};