#include <iostream>
#include "tuiUtils.hpp"
#include "Database.hpp"
#include <conio.h>
#include <AuthService.hpp>
#include <User.hpp>

using namespace std;

using namespace tui;

/* ================= MAIN ================= */

int main()
{

    DB db("fitnessGo.db");
    AuthService auth(db);

    User u{0, "youssef", "1234", "youssef@example.com", "Youssef Ahmed", 22, "Male", "Trainee"};
    int rows = auth.registerUser(u);
    cout<<rows;
    return 0;
}
