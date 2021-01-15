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
#include "TCP_Client.h"

using namespace std;

void TCP_Client::PostLogin()
{
	if (m_Socket.is_open() == false)
	{
		return;
	}

	Print();

	cin.getline(get_cmd, sizeof(get_cmd));

	if (!strcmp(get_cmd, "1"))
	{
		strcpy_s(command, 4, "1");
		cout << "\n\n로그인 세션입니다." << endl;
		do {
			cout << "ID를 입력해주세요" << endl;
			cin.getline(client_id, sizeof(client_id));			
		} while (!CharBan());

		strcpy_s(user.user_id, 20, client_id);
		strcat_s(client_message, client_id);

		do {
			cout << "\n\nPW를 입력해주세요" << endl;
			cin.getline(client_id, sizeof(client_id));
		} while (!CharBan());

		strcpy_s(user.user_pw, 20, client_id);
		strcat_s(client_message, " ");
		strcat_s(client_message, client_id);

		PostWrite();
	}

	else if (!strcmp(get_cmd, "2"))
	{
		strcpy_s(command, 4, "2");
		cout << "\n\n회원가입 세션입니다." << endl;
		do {
			cout << "희망하는 ID를 입력해주세요" << endl;
			cin.getline(client_id, sizeof(client_id));
		} while (!CharBan());

		strcpy_s(user.user_id, 20, client_id);
		strcpy_s(client_message, 20, client_id);

		do {
			std::cout << "\n\n희망하는 PW를 입력해주세요" << endl;
			cin.getline(client_id, sizeof(client_id));
			
		} while (!CharBan());

		strcpy_s(user.user_pw, 20, client_id);
		strcat(client_message, " ");
		strcat(client_message, client_id);

		PostWrite();
	}

	else
	{
		cout << "메뉴를 잘못 선택하셨습니다." << endl;
		PostLogin();
	}

	strcpy_s(command, 4, "\0");
	strcpy_s(client_id, 20, "\0");
	strcpy_s(client_message, 128, "\0");
}

bool TCP_Client::CharBan()
{
	if (strcmp(client_id, "\0"))
	{
		for (page = 0; page < sizeof(client_id); page++)
		{
			if (client_id[page] == ' ' || client_id[page] == '\\' ||
				client_id[page] == '/' || client_id[page] == ':' || 
				client_id[page] == '*' || client_id[page] == '?' || 
				client_id[page] == '\"' || client_id[page] == '<' || 
				client_id[page] == '>' || client_id[page] == '|' )
			{
				cout << E_1 << endl;
				return false;
			}
		}
	}
	else
	{
		cout << E_1 << endl;
		return false;
	}
	return true;
}

void TCP_Client::PostChat()
{
	while (1)
	{
		if (!strcmp(status, S_1_3))
		{
			do {
				cin.getline(client_message, sizeof(client_message));
			} while (!strcmp(client_message, "\0"));

			if (!strcmp(client_message, "/?"))
			{
				Help();
			}

			else if (!strcmp(client_message, "/c"))
			{
				system("cls");
			}

			else if (!strcmp(client_message, "/q"))
			{
				m_Socket.close();
				cout << "프로그램을 종료합니다.." << endl;
				chatLog.close();
				return;
			}

			else if (!strcmp(client_message, "/u"))
			{
				strcpy_s(command, 4, "/u");
				PostWrite();
				cout << "파일 업로드 세션입니다. 업로드할 파일 명을 입력해주세요." << endl;
				cin.getline(user.file_name, sizeof(user.file_name));
				strcpy_s(client_message, 2000, "uploads\\");
				strcat(client_message, user.file_name);
				StartFileupload(client_message);
			}
			 
			else if (!strcmp(client_message, "/d"))
			{
				cout << "파일 다운로드 세션입니다. 다운로드 받을 파일 명을 입력해주세요." << endl;
				cin.getline(user.file_name, sizeof(user.file_name));
				strcpy_s(command, 4, "/d");
				strcpy_s(client_message, 100, user.file_name);
				PostWrite();
			}

			else
			{
				strcpy_s(client_id, 20, user.user_id);
				strcpy_s(command, 4, "3");
				PostWrite();
			}
		}
		buf.empty();
		recv_buf.empty();
	}
}

void TCP_Client::Errset()
{
	if (!strcmp(recv_command, "_2"))
	{
		cout << "\n\n이미 존재하는 계정입니다!\n\n" << endl;
		PostLogin();
		PostReceive();
	}

	if (!strcmp(recv_command, "_1"))
	{
		cout << "\n\n존재하지 않는 계정입니다!\n\n" << endl;
		PostLogin();
		PostReceive();
	}

	if (!strcmp(recv_command, "__1"))
	{
		cout << "\n\n이미 접속중인 계정입니다!\n\n" << endl;
		PostLogin();
		PostReceive();
	}

	if (!strcmp(recv_command, "2_"))
	{
		system("cls");

		cout << "\n\n계정을 생성했습니다!\n\n" << endl;

		strcpy_s(command, 4, "3");
		strcpy_s(status, 20, S_1_3);

		cout << "\n\n===============채팅서버에 접속합니다=============\n\n" << endl;

		strcpy_s(client_id, 20, "Server");
		strcpy_s(client_message, 20, user.user_id);
		strcat_s(client_message, "님이 채팅서버에 참여합니다.");
		strcpy_s(logFilename, sizeof(logFilename), "Logs\\");
		strcat(logFilename, user.user_id);
		strcat(logFilename, "_Logs.txt");
		chatLog.open(logFilename);
		PostWrite();
	}

	if (!strcmp(recv_command, "1_"))
	{      
		system("cls");

		cout << "\n\n로그인에 성공하였습니다!\n\n" << endl;

		strcpy_s(command, 4, "3");
		strcpy_s(status, 20, S_1_3);

		cout << "\n\n===============채팅서버에 접속합니다=============\n\n" << endl;

		strcpy_s(client_id, 20, "Server");
		strcpy_s(client_message, 20, user.user_id);
		strcat_s(client_message, "님이 채팅서버에 참여합니다.");
		strcpy_s(logFilename, sizeof(logFilename), "Logs\\");
		strcat(logFilename, user.user_id);
		strcat(logFilename, "_Logs.txt");
		chatLog.open(logFilename);
		PostWrite();
	}
}

void TCP_Client::PostReceive()
{
	m_Socket.async_read_some(recv_buf,
		boost::bind(&TCP_Client::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	recv_buf.empty();
}

void TCP_Client::PostWrite()
{
	buf = { boost::asio::buffer(command), boost::asio::buffer(client_id), boost::asio::buffer(client_message) };
	boost::asio::async_write(m_Socket, buf,
		boost::bind(&TCP_Client::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	buf.empty();
}

void TCP_Client::handle_connect(const boost::system::error_code &error)
{
	if (error)
	{
		cout << "connect failed : " << error.message() << endl;
	}
	else
	{
		cout << "Connected" << endl;
		boost::thread thread(boost::bind(&TCP_Client::PostLogin, this));
		thread.join();
	}
}

void TCP_Client::handle_write(const boost::system::error_code &, size_t)
{
	if (!strcmp(status, S_1_3))
	{
		chatLog << client_id << " : " << client_message << endl;
	}
}

void TCP_Client::handle_receive(const boost::system::error_code &error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
		{
			cout << "서버와 연결이 끊어졌습니다." << endl;
		}
		else
		{
			cout << "error No : " << error.value() << "error Message : " << error.message() << endl;
		}
		return;
	}

	if (!strcmp(recv_command, "_1") || !strcmp(recv_command, "__1") || !strcmp(recv_command, "1_") ||
		!strcmp(recv_command, "_2") || !strcmp(recv_command, "2_"))
	{
		Errset();
	}
	
	if (!strcmp(recv_command, "/d"))
	{
		StartDownload();
	}

	if (!strcmp(status, S_1_3) && strcmp((char*)recv_buf[0].data(), "/d"))
	{
		cout  << recv_command << " : " << recv_id << " : " << recv_message << endl;
		chatLog << recv_id << " : " << recv_message << endl;
		PostReceive();
	}

	buf.empty();
	recv_buf.empty();
}

void TCP_Client::Print()
{
	cout << "==================================================" << endl;
	cout << "=====================채팅프로그램=================" << endl;
	cout << "1.\t\t로그인\t\t" << endl;
	cout << "2.\t\t계정생성\t\t" << endl;
	cout << "==================================================" << endl;
}

void TCP_Client::Help()
{
	cout << "=====================도움말 리스트================" << endl;
	cout << "1. \t/? 도움말" << endl;
	cout << "2. \t/q 채팅 종료" << endl;
	cout << "4. \t/c 채팅창 초기화"<< endl;
	cout << "5. \t/v 현재 접속중인 유저 조회" << endl;
	cout << "6. \t/u 파일 업로드 하기" << endl;
	cout << "7. \t/d 파일 다운로드 하기" << endl;
	cout << "8. \t/f 파일 리스트 보기" << endl;
	cout << "==================================================\n" << endl;
}

void TCP_Client::StartDownload()
{
	cout << __FUNCTION__ << endl;
	m_Socket.async_read_some(recv_buf,
		boost::bind(&TCP_Client::handle_download, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void TCP_Client::handle_download(const boost::system::error_code &error, size_t bytes_transferred)
{
	if (error)
	{
		cout << "error No : " << error.value() << " error Message: " << error.message() << endl;
		return;
	}

	if (!strcmp(recv_command, "e"))
	{
		cout << "download failed!" << endl;
		PostReceive();
		return;
	}

	cout << __FUNCTION__ << "(" << bytes_transferred << ")"
		<< ", in_avail = " << recv_id
		<< ", size = " << recv_id
		<< ", file_dir = " << recv_message
		<< "\n" << endl;

	istream requestStream(&requestBuf);
	string filePath = recv_message;
	requestStream >> filePath;
	requestStream >> recv_id;

	requestStream.read(f_read, 2);

	std::cout << filePath << " size is " <<
		recv_id << ", tellg = " <<
		requestStream.tellg() << endl;

	filePath = recv_message;

	outputFile.open(filePath.c_str(), ios_base::binary);

	if (!outputFile)
	{
		cout << "file open failed : " << filePath << endl;
		PostReceive();
		return;
	}

	do 
	{
		requestStream.read(f_read, (streamsize)strlen(recv_message));
		cout << __FUNCTION__ << " write  : " << strlen(recv_message) << " Bytes.\n";
		outputFile.write(f_read, requestStream.gcount());
	} while (requestStream.gcount() > 0);

	m_Socket.async_read_some(recv_buf,
		boost::bind(&TCP_Client::handle_readfile, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void TCP_Client::handle_readfile(const::boost::system::error_code& error, size_t bytes_transferred)
{
	if (bytes_transferred > 0)
	{
		outputFile.write(recv_message, (streamsize)atoi(recv_id));
		cout << __FUNCTION__ << " recv : " << recv_id << " Bytes : " << endl;
		if (outputFile.tellp() >= (streamsize)atoi(recv_id))
		{
			strcpy_s(f_read, sizeof(f_read),  "\0");
			outputFile.close();
			PostReceive();
			return;
		}
	}

	if (error)
	{
		cout << error.message() << endl;
	}

	m_Socket.async_read_some(recv_buf,
		boost::bind(&TCP_Client::handle_readfile, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void TCP_Client::handle_uploadfile(const boost::system::error_code& error)
{
	if (!error)
	{
		if (sourceFile.eof() == false)
		{
			sourceFile.read(f_write, (streamsize)boost::asio::buffer(f_write).size());
			if (sourceFile.gcount() <= 0)
			{
				cout << "read file error" << endl;
				return;
			}

			cout << "send : " << sourceFile.gcount() 
				<< "bytes, total = " << sourceFile.tellg() << " bytes\n";

			char _buf[20] = "\0";

			strcpy_s(client_id, 20, itoa(sourceFile.gcount(), _buf, 10));
			 
			buf.empty();

			buf = { boost::asio::buffer(command), boost::asio::buffer(client_id), boost::asio::buffer(f_write) };

			boost::asio::async_write(m_Socket, buf,
				boost::bind(&TCP_Client::handle_write, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
			handle_uploadfile(error);

			if (error)
			{
				std::cout << "send error : " << error.message() << endl;
				sourceFile.close();
				return;
			}
		}
		else
		{
			std::cout << "send success : " << error.message() << endl;
			strcpy_s(f_write, sizeof(f_write), "\0");
			sourceFile.close();
			return;
		}
	}
}

void TCP_Client::StartFileupload(char filename[])
{
	sourceFile.open(filename, ios_base::binary | ios_base::ate);

	if (!sourceFile)
	{
		cout << "failed to open : " << filename << endl;
		strcpy_s(command, "e");
		PostWrite();
		return;
	}

	size_t fileSize = sourceFile.tellg();
	sourceFile.seekg(0);
	char _buf[20] = "\0";
	ostream requestStream(&request);

	strcpy_s(command, 4, "/u");
	strcpy_s(client_id, 20, itoa(fileSize, _buf, 10));
	strcpy_s(client_message, 2000, "downloads\\");
	strcat(client_message, user.file_name);

	buf.empty();
	buf = { boost::asio::buffer(command), boost::asio::buffer(client_id), boost::asio::buffer(client_message) };

	boost::asio::async_write(m_Socket, buf,
		boost::bind(&TCP_Client::handle_uploadfile, this,
			boost::asio::placeholders::error));
}