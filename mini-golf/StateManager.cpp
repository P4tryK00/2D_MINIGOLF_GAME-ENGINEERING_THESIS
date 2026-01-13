//
// Created by patry on 13.01.2026.
//

#include "StateManager.h"

void StateManager::addState(StateRef newState, bool isReplacing) {
    m_isAdding = true;
    m_isReplacing = isReplacing;
    m_newState = std::move(newState);
}

void StateManager::removeState() {
    m_isRemoving = true;
}

void StateManager::processStateChanges() {
    if (m_isRemoving && !m_states.empty()) {
        m_states.pop();
        if (!m_states.empty()) m_states.top()->resume();
        m_isRemoving = false;
    }

    if (m_isAdding) {
        if (!m_states.empty()) {
            if (m_isReplacing) m_states.pop();
            else m_states.top()->pause();
        }
        m_states.push(std::move(m_newState));
        m_states.top()->init();
        m_isAdding = false;
    }
}

StateRef& StateManager::getActiveState() {
    return m_states.top();
}