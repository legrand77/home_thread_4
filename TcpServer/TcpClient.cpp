#include "TcpClient.h"
#include <iostream>

int main()
{
    setlocale(LC_ALL, "Rus");
    TcpClent C;
    C.init_socket("127.0.0.1", 12345);
    C.handle();

    system("Pause");

    return EXIT_SUCCESS;
}
