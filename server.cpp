#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h> // close()
#include <fcntl.h>
#include <algorithm> // max_element()
#include <set>
#include <iostream>
//#include <cstring> // std::memcpy()
//#include <stdio.h>
//using namespace std;

#include "server.h"
#include "game.h"




void Server::initServer()
{
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
	    perror("socket");
	    exit(1);
	}

	fcntl(listener, F_SETFL, O_NONBLOCK);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = INADDR_ANY;
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
	    perror("bind");
	    exit(2);
	}

	listen(listener, 2);

	// std::set<int> clients;
	clients.clear();
}


void Server::mainLoop(Game &gm)
{
	while(1)
    {
        // Заполняем множество сокетов
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(listener, &readset);

        for(std::set<int>::iterator it = clients.begin(); it != clients.end(); it++)
            FD_SET(*it, &readset);

        // Задаём таймаут
        timeval timeout;
        timeout.tv_sec = 240;
        timeout.tv_usec = 0;

        // Ждём события в одном из сокетов
        int mx = std::max(listener, *max_element(clients.begin(), clients.end()));
        if(select(mx+1, &readset, NULL, NULL, &timeout) <= 0)
        {
            perror("select");
            exit(3);
        }
        
        // Определяем тип события и выполняем соответствующие действия
        if(FD_ISSET(listener, &readset))
        {
            std::cout << "Поступил новый запрос на соединение, используем accept" << std::endl;
            // Поступил новый запрос на соединение, используем accept
            int sock = accept(listener, NULL, NULL);
            if(sock < 0)
            {
                perror("accept");
                exit(3);
            }
            
            fcntl(sock, F_SETFL, O_NONBLOCK);

            clients.insert(sock);
        }

        for(std::set<int>::iterator it = clients.begin(); it != clients.end(); it++)
        {
            if(FD_ISSET(*it, &readset))
            {
                // Поступили данные от клиента, читаем их
                bytes_read = recv(*it, buf, 1024, 0);

                gm.recvData(buf);

                //std::cout << bytes_read << std::endl;

                if(bytes_read <= 0)
                {
                    // Соединение разорвано, удаляем сокет из множества
                    std::cout << "shutdown/close socet " << *it << std::endl;
                    shutdown(*it, SHUT_RDWR);
                    close(*it);
                    clients.erase(*it);
                    continue;
                }

                gm.sendData(buf);

                send(*it, buf, sizeof(sData) * 5, 0);
            }
        }
    }
}