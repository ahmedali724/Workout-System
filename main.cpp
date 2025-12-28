#include <iostream>
#include "tuiUtils.hpp"
#include "Database.hpp"
#include <conio.h>
#include <AuthService.hpp>
#include <User.hpp>
#include "ScreenData.hpp"
#include "AuthUI.hpp"

using namespace std;

using namespace tui;

/* ================= MAIN ================= */

int main()
{

    DB db("fitnessGo.db");
    AuthService authService(db);

    // User u{0, "youssef", "1234", "youssef@example.com", "Youssef Ahmed", 22, "Male", "Trainee"};
    // int rows = auth.registerUser(u);
    // cout<<rows;

    AuthUI authUI(authService);

    ScreenData<NoData> current{Screen::Welcome, NoData{}};

    while (current.nextScreen != Screen::Exit)
    {
        switch (current.nextScreen)
        {
        case Screen::Welcome:
            current = authUI.WelcomeScreen();
            break;
        case Screen::Login:
            current = authUI.LoginScreen();
            break;
        case Screen::Register:
            current = authUI.RegisterScreen();
            break;
        }
    }

    return 0;
}
