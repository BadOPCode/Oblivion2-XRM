#ifndef MENU_SYSTEM_HPP
#define MENU_SYSTEM_HPP

#include "state_base.hpp"
#include "menu_base.hpp"

#include "model/struct_compat.hpp"
#include "model/menu.hpp"

#include <boost/smart_ptr/shared_ptr.hpp>

#include <stdint.h>
#include <string>
#include <vector>
#include <functional>


class Config;
typedef boost::shared_ptr<Config> config_ptr;

class ConfigDao;
typedef boost::shared_ptr<ConfigDao> config_dao_ptr;

class ModBase;
typedef boost::shared_ptr<ModBase> module_ptr;

/**
 * @class MenuSystem
 * @author Michael Griffin
 * @date 10/11/2016
 * @file menu_system.hpp
 * @brief Main Menu System State Handles Core Loop
 */
class MenuSystem
    : public StateBase
    , public MenuBase
{
public:
    MenuSystem(session_data_ptr session_data);
    ~MenuSystem();

    virtual void update(const std::string &character_buffer, const bool &is_utf8);
    virtual bool onEnter();
    virtual bool onExit();

    virtual std::string getStateID() const
    {
        return m_menuID;
    }

    
    //int m_next_state;
    static const std::string m_menuID;

    // This matches the index for menu_functions.push_back
    enum
    {
        MENU_INPUT,
        MENU_EDITOR_INPUT,
        MODULE_PRELOGON_INPUT,
        MODULE_LOGON_INPUT,
        MODULE_INPUT
    };

    
    /**
     * @brief Process Command Keys passed from menu selection (Callback)
     * @param option
     */
    bool menuOptionsCallback(const MenuOption &option);

    /**
     * @brief Resets the Menu Input Method in the Function Array
     * @param index
     */
    void resetMenuInputIndex(int index);

    // Move this to it's own module eventually!
    void startupMenuEditor();
    void menuEditorInput(const std::string &character_buffer, const bool &);

    // Each system will have it's own module that is allocated and pushed to the
    // m_module container to easily push and pop from the stack.

    /**
     * @brief Clears All Modules
     */
    void clearAllModules();
    
    /**
     * @brief Exists and Shutsdown the current module
     */
    void shutdownModule();
    
    /**
     * @brief Exists and Shutsdown the current module
     */
    void startupModule(module_ptr module);

    /**
     * @brief Starts up Logon Module
     */
    void startupModulePreLogon();

    /**
     * @brief Starts up Logon Module
     */
    void startupModuleLogon();

    /**
     * @brief Starts up Signup Module
     */
    void startupModuleSignup();

    /**
     * @brief Handles Input for Login and PreLogin Sequences.
     * @param character_buffer
     * @param is_utf8
     */
    void handleLoginInputSystem(const std::string &character_buffer, const bool &is_utf8);

    /**
     * @brief Handles parsing input for PreLogon current module.
     */
    void modulePreLogonInput(const std::string &character_buffer, const bool &is_utf8);

    /**
     * @brief Handles parsing input for Logon current module.
     */
    void moduleLogonInput(const std::string &character_buffer, const bool &is_utf8);
    
    /**
     * @brief Handles parsing input for current module.
     */
    void moduleInput(const std::string &character_buffer, const bool &is_utf8);

};

#endif // MENU_SYSTEM_HPP
