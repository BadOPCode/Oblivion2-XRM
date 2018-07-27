#ifndef MOD_USER_EDITOR_HPP
#define MOD_USER_EDITOR_HPP

#include "mod_base.hpp"

#include <string>
#include <memory>
#include <functional>
#include <vector>

class SessionData;
typedef std::shared_ptr<SessionData> session_data_ptr;

class Directory;
typedef std::shared_ptr<Directory> directory_ptr;

/**
 * @class ModUserEditor
 * @author Michael Griffin
 * @date 26/06/2018
 * @file mod_user_editor.hpp
 * @brief User Editor
 */
class ModUserEditor
    : public ModBase
{
public:
    ModUserEditor(session_data_ptr session_data, config_ptr config, ansi_process_ptr ansi_process)
        : ModBase(session_data, config, ansi_process)
        , m_session_io(session_data)
        , m_filename("mod_user_editor.yaml")
        , m_text_prompts_dao(new TextPromptsDao(GLOBAL_DATA_PATH, m_filename))
        , m_mod_setup_index(MOD_DISPLAY_USER_LIST)
        , m_mod_function_index(MOD_USER_INPUT)
        , m_mod_user_state_index(USER_CHANGE)
        , m_is_text_prompt_exist(false)
        , m_page(0)
        , m_rows_per_page(0)
        , m_current_user_id(0)
        , m_current_field(0)
        , m_wildcard_filter("")
    {
        std::cout << "ModUserEditor" << std::endl;

        // Push function pointers to the stack.
        
        m_setup_functions.push_back(std::bind(&ModUserEditor::setupUserList, this));
        m_setup_functions.push_back(std::bind(&ModUserEditor::setupUserEditFields, this));

        m_mod_functions.push_back(std::bind(&ModUserEditor::userListInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorPausedInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorUserInput, this, std::placeholders::_1));
        
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorFieldInput, this, std::placeholders::_1));
        m_mod_functions.push_back(std::bind(&ModUserEditor::userEditorFieldHandler, this, std::placeholders::_1));
            
        // Check of the Text Prompts exist.
        m_is_text_prompt_exist = m_text_prompts_dao->fileExists();
        if (!m_is_text_prompt_exist)
        {
            createTextPrompts();
        }

        // Loads all Text Prompts for current module
        m_text_prompts_dao->readPrompts();
    }

    virtual ~ModUserEditor() override
    {
        std::cout << "~ModUserEditor" << std::endl;
        std::vector<std::function< void()> >().swap(m_setup_functions);
        std::vector<std::function< void(const std::string &)> >().swap(m_mod_functions);
    }

    virtual bool update(const std::string &character_buffer, const bool &) override;
    virtual bool onEnter() override;
    virtual bool onExit() override;

    // Setup Methods
    enum 
    {
        MOD_DISPLAY_USER_LIST   = 0,
        MOD_DISPLAY_USER_FIELDS = 1
    };

    // Input Module Index
    enum
    {
        MOD_USER_INPUT          = 0, // User List Input Parser
        MOD_PAUSE               = 1, // Pauses on display of menus/options
        MOD_USER_NAME           = 2, // User Name Input Handler        
        MOD_USER_FIELD_INPUT    = 3, // Selecting Individual User Fields.
        MOD_USER_FIELD          = 4  // Updates Current Field
    };

    // Input Menu State Index
    // Used for both Menus and Options.
    enum
    {
        USER_CHANGE    = 0,
        USER_DELETE    = 1,
        USER_COPY      = 2,
        USER_FILTER    = 3
    };
    
    // Box drawing characters
    enum
    {
        BORDER_TOP_LEFT  = (char)214,  // ╓
        BORDER_BOT_LEFT  = (char)211,  // ╙
        BORDER_ROW       = (char)196,  // ─
        BORDER_TOP_RIGHT = (char)183,  // ╖
        BORDER_BOT_RIGHT = (char)189,  // ╜
        BORDER_MID_TOP   = (char)210,  // ╥
        BORDER_MID_BOT   = (char)208,  // ╨
        BORDER_MID       = (char)186   // ║
    };
    
    // Create Prompt Constants, these are the keys for key/value lookup
    const std::string PROMPT_HEADER = "header";
    const std::string PROMPT_INPUT_TEXT = "input_text";
    const std::string PROMPT_INVALID = "invalid_input";
    const std::string PROMPT_PAUSE = "pause_prompt";

    const std::string PROMPT_USER_CHANGE = "user_change";
    const std::string PROMPT_USER_DELETE = "user_delete";    
    const std::string PROMPT_USER_COPY = "user_copy";
    const std::string PROMPT_USER_FILTER = "user_filter";

    const std::string PROMPT_INVALID_USER_NOT_EXISTS = "invalid_user_doesnt_exist";
    const std::string PROMPT_USER_EDIT_HEADER = "user_editor_field_header";
    const std::string PROMPT_USER_FIELD_INPUT_TEXT = "user_editor_field_input";

    /**
     * @brief Create Default Text Prompts for module
     */
    void createTextPrompts();
    
    /**
     * @brief Sets an indivdual input module index.
     * @param mod_function_index
     */
    void changeInputModule(int mod_function_index);
    
    /**
     * @brief Sets an indivdual setup method module index.
     * @param mod_function_index
     */
    void changeSetupModule(int mod_function_index);
    
    /**
     * @brief Sets an indivdual Menu Input State Add/Change/Delete
     * @param mod_menu_state_index
     */
    void changeMenuInputState(int mod_menu_state_index);
    
    /**
     * @brief Redisplay's the current module prompt.
     * @return
     */
    void redisplayModulePrompt();

    /**
     * @brief Toggle the Option View.
     * @return
     */
    void toggleNextOptionView();

    /**
     * @brief Pull and Display Prompts
     * @param prompt
     */
    void displayPrompt(const std::string &prompt);
    
    /**
     * @brief Pull and Display Prompts with MCI Code
     * @param prompt
     * @param mci_field
     */
    void displayPromptMCI(const std::string &prompt, const std::string &mci_field);

    /**
     * @brief Pull and Display Prompts with following newline
     * @param prompt
     */
    void displayPromptAndNewLine(const std::string &prompt);

    /**
     * @brief Setup for the User Editor 
     * @return
     */
    void setupUserList();

    /**
     * @brief Setup for the User Field Editor
     * @return
     */
    void setupUserEditFields();

    /**
     * Handle Input Commands.
     */

    /**
     * @brief Handles input command for User Editor Prompt
     * @return
     */
    void userListInput(const std::string &input);
    
    /**
     * @brief Handles User Number Input, Parses Strings and checks Valid User Id
     * @param input
     */
    void userEditorUserInput(const std::string &input);
    
    /**
     * @brief handle each user seperate state and what to do next on input.
     * @param does_user_exist
     * @param user_id
     */
    void handleUserInputState(bool does_user_exist, long user_id);

    /**
     * @brief Copy an Existing User Record
     * @param user_id
     */
    void copyExistingUser(long user_id);

    /**
     * @brief Delete an Existing User Record
     * @param user_id
     */
    void deleteExistingUser(long user_id);

    /**
     * @brief Check if the user exists in the current listing by String Id
     * @param user_id
     */
    bool checkUserExistsById(long user_id);

    /**
     * @brief Check if the user exists in the current listing by String Id
     * @param user_id
     */
    bool loadUserById(long user_id);

    /**
     * @brief Handles Input (Waiting for Any Key Press)
     * @param input
     */
    void userEditorPausedInput(const std::string &input);
    
    /**
     * Display Methods
     */
     
    /**
     * @brief Displays the current page of users
     * @param input_state
     */
    void displayCurrentPage(const std::string &input_state);
    
    /**
     * @brief User Editor, Read and Modify User Preferences
     * @return
     */
    std::string displayUserList();
    
    /**
     * @brief Handles User Field Editor Command Selection
     * @param input
     */
    void userEditorFieldInput(const std::string &input);
    
    /**
     * @brief Handles Field Updates for User Data
     * @param input
     */
    void userEditorFieldHandler(const std::string &input);
    
    /**
     * @brief User Editor, for Dispalying Menu Fields to Edit
     * @return
     */
    std::string displayUserEditScreen();
    
    /**
     * @brief User Editor, for Dispalying User Extended Fields to Edit
     * @return
     */
    std::string displayUserEditExtendedScreen();
    
    /**
     * @brief Displays the current page of user items
     * @param input_state
     */
    void displayCurrentEditPage(const std::string &input_state);
    
private:

    // Function Input Vector.
    std::vector<std::function< void()> >                    m_setup_functions;
    std::vector<std::function< void(const std::string &)> > m_mod_functions;
    std::vector<std::string>                                m_user_display_list;
    std::vector<user_ptr>                                   m_loaded_user;

    SessionIO              m_session_io;
    std::string            m_filename;
    text_prompts_dao_ptr   m_text_prompts_dao;

    unsigned int           m_mod_setup_index;
    unsigned int           m_mod_function_index;
    unsigned int           m_mod_user_state_index;
    
    bool                   m_is_text_prompt_exist;
    unsigned int           m_page;
    unsigned int           m_rows_per_page;
    
    unsigned int           m_current_user_id;
    unsigned int           m_current_field;
    std::string            m_wildcard_filter;
    
    CommonIO               m_common_io;
    directory_ptr          m_directory;
    
};


#endif // MOD_USER_EDITOR_HPP
