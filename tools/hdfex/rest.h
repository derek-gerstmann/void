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

#ifndef VD_PROTOCOLS_REST_CLIENT_INCLUDED
#define VD_PROTOCOLS_REST_CLIENT_INCLUDED

// ============================================================================================== //

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

class Async
{
public:

    Async(boost::asio::io_service& io_service);

    void Get(
        const std::string& host,
        const std::string& port,
        const std::string& path,
        const std::string& accept);

    void Put(
        const std::string& host,
        const std::string& port,
        const std::string& path,
        const std::string& accept);

    void Post(
        const std::string& host,
        const std::string& port,
        const std::string& path,
        const std::string& accept);

    void Delete(
        const std::string& host,
        const std::string& port,
        const std::string& path,
        const std::string& accept);

protected:

    void OnResolve(
        const boost::system::error_code& err,
        tcp::resolver::iterator endpoint_iterator);

    void OnConnect(
        const boost::system::error_code& err);

    void OnWriteRequest(
        const boost::system::error_code& err);

    void OnReadStatusLine(
        const boost::system::error_code& err);

    void OnReadHeaders(
        const boost::system::error_code& err);

    void OnReadContent(
        const boost::system::error_code& err);

protected:

    tcp::resolver           m_Resolver;
    tcp::socket             m_Socket;
    boost::asio::streambuf  m_Request;
    boost::asio::streambuf  m_Response;
};

// ============================================================================================== //

}  // end namespace: Client
}  // end namespace: Rest

// ============================================================================================== //

#endif  // VD_PROTOCOLS_REST_CLIENT_INCLUDED
