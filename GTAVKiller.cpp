#include <Windows.h>
#include <iostream>

constexpr auto kKillId = 0x03;

inline bool IsProcessRunAsAdmin()
{
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;

    auto rc = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup);

    if (rc) {
        CheckTokenMembership(NULL, AdministratorsGroup, &rc);
        FreeSid(AdministratorsGroup);
    }

    return rc == TRUE;
}

int main()
{
    if (!IsProcessRunAsAdmin()) {
        std::cout << "请以管理员身份启动本程序" << std::endl;
        system("pause");
        return -1;
    }

    if (!RegisterHotKey(nullptr, kKillId, MOD_ALT | MOD_NOREPEAT, 0x4B)) {
        std::cout << "注册热键失败" << std::endl;
        system("pause");
        return -1;
    }

    std::cout << "热键注册成功，按Alt+K可结束GTA进程" << std::endl;

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0) {
        if (msg.message == WM_HOTKEY && msg.wParam == kKillId) {
            std::cout << "正在强制结束GTAV进程" << std::endl;
            auto rc = system("taskkill /f /t /im GTA5.exe");
            std::cout << rc << std::endl;
        }
    }

    return 0;
}
