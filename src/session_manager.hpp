#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <memory>
#include <iostream>
#include <string>
#include <set>

class Session;
typedef std::shared_ptr<Session> session_ptr;

class SessionSSL;
typedef std::shared_ptr<SessionSSL> session_ssl_ptr;

/**
 * @class SessionManager
 * @author Michael Griffin
 * @date 15/08/2015
 * @file session_manager.hpp
 * @brief Main Channel of Communications between Sessions
 * Also Very Import, keeps handle on Active Sessions and keeps smart pointers alive!
 */
class SessionManager
{
public:
    explicit SessionManager()
    {
        std::cout << "SessionManager" << std::endl;
    }
    ~SessionManager();

    /**
     * @brief OverRides for Conencting TCP and SSL Sessions
     * @param Session
     */
    void join(session_ptr session);

    void leave(int node_number);
    void deliver(std::string msg);

    int connections();
    void shutdown();

private:

    std::set<session_ptr> m_sessions;
    std::set<session_ssl_ptr> m_sessions_ssl;
};

typedef std::shared_ptr<SessionManager> session_manager_ptr;
typedef std::weak_ptr<SessionManager>	session_manager_wptr;

#endif // CHAT_ROOM_HPP
