#pragma once

#include "GroupManager.h"

class RoleBase {
public:
    RoleBase(GroupManager& manager, const std::string& roleName);
    virtual void handleAction(int choice) = 0;

protected:
    GroupManager& m_manager;
    std::string m_roleName;
};
