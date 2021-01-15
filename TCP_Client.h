#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <fstream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\array.hpp>
#include <boost\thread.hpp>
#include "Structset.h"

using namespace std;

const char SERVER_IP[] = "127.0.0.1";
const unsigned short PORT_NUMBER = 31400;

class TCP_Client
{
public:
	TCP_Client(boost::asio::io_service &io_service) :
		m_Socket(io_service), m_io_service(io_service)
	{
	}

public:
	void Connect(boost::asio::ip::tcp::endpoint &endpoint)
	{
		m_Socket.async_connect(endpoint, boost::bind(&TCP_Client::handle_connect, this, 
										 boost::asio::placeholders::error));
	}
	
	void PostReceive();
	void PostChat();
	void StartFileupload(char filename[]);

private:
	boost::asio::ip::tcp::socket			m_Socket;
	boost::asio::io_service					&m_io_service;
	boost::shared_ptr<TCP_Client>			TCP;

	char status[100]						= S;

	char get_cmd[200]						= "\0";

	char command[4]							= "\0";
	char client_id[20]						= "\0";
	char client_message[2000]				= "\0";

	char recv_command[4]					= "\0";
	char recv_id[20]						= "\0";
	char recv_message[2000]					= "\0";

	boost::array<boost::asio::mutable_buffer, 3> buf =
	{
		boost::asio::buffer(command),
		boost::asio::buffer(client_id),
		boost::asio::buffer(client_message)
	};

	boost::array<boost::asio::mutable_buffer, 3> recv_buf =
	{
		boost::asio::buffer(recv_command),
		boost::asio::buffer(recv_id),
		boost::asio::buffer(recv_message) 
	};

private:
	int page;
	USER user;
	void PostLogin();
	bool CharBan();
	void Errset();
	void PostWrite();

	void handle_connect(const boost::system::error_code &error);
	void handle_write(const boost::system::error_code &, size_t);
	void handle_receive(const boost::system::error_code &error,  size_t bytes_transferred);

	void StartDownload();
	void handle_download(const boost::system::error_code &error, size_t bytes_transferred);
	void handle_readfile(const::boost::system::error_code & error, size_t bytes_transferred);

	void handle_uploadfile(const boost::system::error_code & error);

	void Print();
	void Help();

private:
	boost::asio::streambuf					requestBuf;
	char									f_read[40860]		= "\0";
	size_t									fileSize;
	ofstream								outputFile;

private:
	char									f_write[2000]		= "\0";
	boost::asio::streambuf					request;
	ifstream								sourceFile;

private:
	ofstream								chatLog;
	char									logFilename[30]		= "\0";
};