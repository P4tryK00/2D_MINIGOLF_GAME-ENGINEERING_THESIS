#include "StateManager.h"

void StateManager::addState(StateRef newState, bool isReplacing) {
    m_isAdding = true;
    m_isReplacing = isReplacing;
    m_isClearing = false;
    m_newState = std::move(newState);
}

void StateManager::removeState() {
    m_isRemoving = true;
}

void StateManager::switchState(StateRef newState) {
    m_isAdding = true;
    m_isClearing = true;
    m_newState = std::move(newState);
}

void StateManager::processStateChanges() {
    // 1. Obsługa usuwania stanu
    if (m_isRemoving && !m_states.empty()) {
        m_states.pop(); // Niszczy górny stan (wywołuje destruktor)

        // Jeśli na stosie pozostały inne stany, wznów ten poniżej
        if (!m_states.empty()) {
            m_states.top()->resume();
        }
        m_isRemoving = false;
    }

    // 2. Obsługa dodawania stanu
    if (m_isAdding) {
        if (!m_states.empty()) {
            if (m_isClearing) {
                // Całkowite czyszczenie stosu (np. Game Over -> Menu)
                while (!m_states.empty()) {
                    m_states.pop();
                }
            }
            else if (m_isReplacing) {
                // Zastąpienie tylko górnego stanu (np. Intro -> Menu)
                m_states.pop();
            } else {
                // Zapauzowanie obecnego stanu przed nałożeniem nowego (np. Gra -> Pauza)
                m_states.top()->pause();
            }
        }

        // Dodanie nowego stanu na stos i jego inicjalizacja
        m_states.push(std::move(m_newState));
        if (!m_states.empty()) {
            m_states.top()->init();
        }
        m_isAdding = false;
        m_isClearing = false;
    }
}

StateRef& StateManager::getActiveState() {
    return m_states.top();
}