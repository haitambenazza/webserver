/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbassim <kbassim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:07:51 by kbassim           #+#    #+#             */
/*   Updated: 2025/07/19 01:46:18 by kbassim          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/webserver.hpp"

bool Client::SetClient()
{
    struct sockaddr_in addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("Socket");
        return false;
    }
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("Connect");
        return false;
    }
    send(fd, "hello world", 11 ,0);
    return (true);
}

Client::Client()
{
    if (!SetClient())
    {
        std::cerr << "connection to the server failed" << std::endl;
        return ;
    }
}

Client::Client( const Client& copy )
{
    (void)copy;
    if (!SetClient())
    {
        std::cerr << "connection to the server failed" << std::endl;
        return ;
    }
}

Client& Client::operator=( const Client& copy )
{
    (void)copy;
    if ( this != &copy )
    {
        close(fd);
        SetClient();
    }
    return (*this);
}

Client::~Client()
{
}

// int main()
// {
//     Client client;
// }