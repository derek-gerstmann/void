// ============================================================================================== //
//
// License:   The Lesser GNU Public License (LGPL) v3.0.
//
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA).
//        As well as the shoulders of many giants...
//
// This file is part of the Void framework.
//
// The Void framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Void framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Void framework.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== //

#include "rest.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

// ============================================================================================== //

namespace Rest   {
namespace Client {

// ============================================================================================== //

using boost::asio::ip::tcp;

// ============================================================================================== //

Async::Async(
    boost::asio::io_service& io_service
)
    : m_Resolver(io_service),
      m_Socket(io_service)
{
    // EMPTY!
}

void 
Async::Get(
    const std::string& host,
    const std::string& port,
    const std::string& path,
    const std::string& accept)
{
    std::ostream request_stream(&m_Request);

    request_stream << "GET http://" << host << ":" << port << "/" << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << ":" << port << "\r\n";
    request_stream << "Accept: " << accept << "\r\n";
    request_stream << "Connection: close\r\n\r\n";

    tcp::resolver::query query(host, port, boost::asio::ip::resolver_query_base::numeric_service);
    m_Resolver.async_resolve(query,
                             boost::bind(&Async::OnResolve, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::iterator));
}

void 
Async::Put(
    const std::string& host,
    const std::string& port,
    const std::string& path,
    const std::string& accept)
{
    std::ostream request_stream(&m_Request);
    
    request_stream << "PUT http://" << host << ":" << port << "/" << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << ":" << port << "\r\n";
    request_stream << "Accept: " << accept << "\r\n";
    request_stream << "Connection: close\r\n\r\n";

    tcp::resolver::query query(host, port, boost::asio::ip::resolver_query_base::numeric_service);
    m_Resolver.async_resolve(query,
                             boost::bind(&Async::OnResolve, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::iterator));
}

void 
Async::Post(
    const std::string& host,
    const std::string& port,
    const std::string& path,
    const std::string& accept)
{
    std::ostream request_stream(&m_Request);
    
    request_stream << "POST http://" << host << ":" << port << "/" << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << ":" << port << "\r\n";
    request_stream << "Accept: " << accept << "\r\n";
    request_stream << "Connection: close\r\n\r\n";

    tcp::resolver::query query(host, port, boost::asio::ip::resolver_query_base::numeric_service);
    m_Resolver.async_resolve(query,
                             boost::bind(&Async::OnResolve, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::iterator));
}

void 
Async::Delete(
    const std::string& host,
    const std::string& port,
    const std::string& path,
    const std::string& accept)
{
    std::ostream request_stream(&m_Request);
    
    request_stream << "GET http://" << host << ":" << port << "/" << path << " HTTP/1.0\r\n";
    request_stream << "Host: " << host << ":" << port << "\r\n";
    request_stream << "Accept: " << accept << "\r\n";
    request_stream << "Connection: close\r\n\r\n";

    tcp::resolver::query query(host, port, boost::asio::ip::resolver_query_base::numeric_service);
    m_Resolver.async_resolve(query,
                             boost::bind(&Async::OnResolve, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::iterator));

}

void 
Async::OnResolve(
    const boost::system::error_code& err,
    tcp::resolver::iterator endpoint_iterator)
{
    if (!err)
    {
        // resolved -- try to contact each endpoint until we get a connection...
        //
        boost::asio::async_connect(m_Socket, endpoint_iterator,
                                   boost::bind(&Async::OnConnect, this,
                                               boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void 
Async::OnConnect(
    const boost::system::error_code& err)
{
    if (!err)
    {
        // connection established -- send the request
        //
        boost::asio::async_write(m_Socket, m_Request,
                                 boost::bind(&Async::OnWriteRequest, this,
                                             boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void 
Async::OnWriteRequest(
    const boost::system::error_code& err)
{
    if (!err)
    {
        // write requested -- read the response status line
        //
        boost::asio::async_read_until(m_Socket, m_Response, "\r\n",
                                      boost::bind(&Async::OnReadStatusLine, this,
                                              boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void 
Async::OnReadStatusLine(
    const boost::system::error_code& err)
{
    if (!err)
    {
        std::istream response_stream(&m_Response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            std::cout << "Invalid response\n";
            return;
        }

        if (status_code != 200)
        {
            std::cout << status_message << "\n";
            std::cout << "Response returned with status code ";
            std::cout << status_code << "\n";
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::async_read_until(m_Socket, m_Response, "\r\n\r\n",
                                      boost::bind(&Async::OnReadHeaders, this,
                                              boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}

void 
Async::OnReadHeaders(
    const boost::system::error_code& err)
{
    if (!err)
    {
        std::istream response_stream(&m_Response);
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
            std::cout << header << "\n";
        std::cout << "\n";

        if (m_Response.size() > 0)
            std::cout << &m_Response;

        // continue reading until EOF
        //
        boost::asio::async_read(m_Socket, m_Response,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&Async::OnReadContent, this,
                                            boost::asio::placeholders::error));
    }
    else
    {
        std::cout << "Error: " << err << "\n";
    }
}

void 
Async::OnReadContent(
    const boost::system::error_code& err)
{
    if (!err)
    {
        std::cout << &m_Response;

        // continue reading until EOF
        //
        boost::asio::async_read(m_Socket, m_Response,
                                boost::asio::transfer_at_least(1),
                                boost::bind(&Async::OnReadContent, this,
                                            boost::asio::placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
        std::cout << "Error: " << err << "\n";
    }
}

// ============================================================================================== //

}  // end namespace: Client
}  // end namespace: Rest

// ============================================================================================== //
