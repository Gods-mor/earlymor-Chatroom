#pragma once

#include "RoleBase.h"

class AdminRole : public RoleBase {
public:
    AdminRole(GroupManager& manager, const std::string& roleName);
    void handleAction(int choice) override;

private:
    // admin特有的操作函数声明
};