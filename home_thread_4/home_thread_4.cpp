#include "UdpServer.h"


int main()
{
    setlocale(LC_ALL, "Rus");
    UdpServer S;
    S.recieve_and_answer();

    system("Pause");

    return EXIT_SUCCESS;
}

