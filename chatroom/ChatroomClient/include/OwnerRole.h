#pragma once

#include "RoleBase.h"

class OwnerRole : public RoleBase {
public:
    OwnerRole(GroupManager& manager, const std::string& roleName);
    void handleAction(int choice) override;

private:
    // owner特有的操作函数声明
};
