//
// Created by patry on 13.01.2026.
//

#ifndef MINI_GOLF_STATEMANAGER_H
#define MINI_GOLF_STATEMANAGER_H




#include <stack>
#include <memory>
#include "State.h"

typedef std::unique_ptr<State> StateRef;

class StateManager {
public:
    StateManager() : m_isRemoving(false), m_isAdding(false), m_isReplacing(false) {}
    ~StateManager() {}

    void addState(StateRef newState, bool isReplacing = true);
    void removeState();
    void processStateChanges();
    void switchState(StateRef newState);
    StateRef& getActiveState();

private:
    std::stack<StateRef> m_states;
    StateRef m_newState;
    bool m_isRemoving;
    bool m_isAdding;
    bool m_isReplacing;
    bool m_isClearing;
};



#endif //MINI_GOLF_STATEMANAGER_H