#pragma once

#include "RoleBase.h"

class MemberRole : public RoleBase {
public:
    MemberRole(GroupManager& manager, const std::string& roleName);
    void handleAction(int choice) override;

private:
    // member特有的操作函数声明
};