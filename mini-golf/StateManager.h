#ifndef MINI_GOLF_STATEMANAGER_H
#define MINI_GOLF_STATEMANAGER_H

#include <stack>
#include <memory>
#include "State.h"

/**
 * @typedef StateRef
 * @brief Alias dla unikalnego wskaźnika na obiekt klasy State.
 */
typedef std::unique_ptr<State> StateRef;

/**
 * @class StateManager
 * @brief Zarządza stosem stanów gry.
 *
 * Klasa odpowiedzialna za dodawanie, usuwanie i przełączanie między różnymi
 * stanami gry. Obsługuje przejścia między stanami w bezpiecznym momencie
 * (na początku pętli gry), aby uniknąć błędów pamięci podczas aktualizacji.
 */
class StateManager {
public:
    StateManager() : m_isRemoving(false), m_isAdding(false), m_isReplacing(false) {}
    ~StateManager() {}

    /**
     * @brief Planuje dodanie nowego stanu.
     * @param newState Unikalny wskaźnik do nowego stanu.
     * @param isReplacing Jeśli true, obecny górny stan zostanie usunięty przed dodaniem nowego.
     */
    void addState(StateRef newState, bool isReplacing = true);

    /**
     * @brief Planuje usunięcie obecnego górnego stanu.
     */
    void removeState();

    /**
     * @brief Przetwarza oczekujące zmiany stanów (dodawanie, usuwanie).
     * Powinna być wywoływana na początku głównej pętli gry.
     */
    void processStateChanges();

    /**
     * @brief Czyści cały stos i ustawia nowy stan (np. powrót do Menu Głównego).
     * @param newState Nowy stan bazowy.
     */
    void switchState(StateRef newState);

    /**
     * @brief Zwraca referencję do aktywnego (górnego) stanu.
     * @return Referencja do unikalnego wskaźnika obecnego stanu.
     */
    StateRef& getActiveState();

private:
    std::stack<StateRef> m_states; ///< Stos stanów gry.
    StateRef m_newState;           ///< Tymczasowe przechowanie nowego stanu do dodania.

    bool m_isRemoving;   ///< Flaga oznaczająca konieczność zdjęcia stanu ze stosu.
    bool m_isAdding;     ///< Flaga oznaczająca konieczność dodania stanu na stos.
    bool m_isReplacing;  ///< Flaga oznaczająca, czy dodanie ma zastąpić obecny stan.
    bool m_isClearing;   ///< Flaga oznaczająca, czy stos ma zostać całkowicie wyczyszczony.
};

#endif //MINI_GOLF_STATEMANAGER_H