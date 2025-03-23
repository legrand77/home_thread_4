#include "TcpServer.h"
#include <iostream>

int main()
{
    setlocale(LC_ALL, "Rus");
    TcpServer S;
    S.init_socket(12345);
    S.run();

    system("Pause");

    return EXIT_SUCCESS;
}
