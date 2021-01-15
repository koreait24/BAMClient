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
		cout << "\n\n�α��� �����Դϴ�." << endl;
		do {
			cout << "ID�� �Է����ּ���" << endl;
			cin.getline(client_id, sizeof(client_id));			
		} while (!CharBan());

		strcpy_s(user.user_id, 20, client_id);
		strcat_s(client_message, client_id);

		do {
			cout << "\n\nPW�� �Է����ּ���" << endl;
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
		cout << "\n\nȸ������ �����Դϴ�." << endl;
		do {
			cout << "����ϴ� ID�� �Է����ּ���" << endl;
			cin.getline(client_id, sizeof(client_id));
		} while (!CharBan());

		strcpy_s(user.user_id, 20, client_id);
		strcpy_s(client_message, 20, client_id);

		do {
			std::cout << "\n\n����ϴ� PW�� �Է����ּ���" << endl;
			cin.getline(client_id, sizeof(client_id));
			
		} while (!CharBan());

		strcpy_s(user.user_pw, 20, client_id);
		strcat(client_message, " ");
		strcat(client_message, client_id);

		PostWrite();
	}

	else
	{
		cout << "�޴��� �߸� �����ϼ̽��ϴ�." << endl;
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
				cout << "���α׷��� �����մϴ�.." << endl;
				chatLog.close();
				return;
			}

			else if (!strcmp(client_message, "/u"))
			{
				strcpy_s(command, 4, "/u");
				PostWrite();
				cout << "���� ���ε� �����Դϴ�. ���ε��� ���� ���� �Է����ּ���." << endl;
				cin.getline(user.file_name, sizeof(user.file_name));
				strcpy_s(client_message, 2000, "uploads\\");
				strcat(client_message, user.file_name);
				StartFileupload(client_message);
			}
			 
			else if (!strcmp(client_message, "/d"))
			{
				cout << "���� �ٿ�ε� �����Դϴ�. �ٿ�ε� ���� ���� ���� �Է����ּ���." << endl;
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
		cout << "\n\n�̹� �����ϴ� �����Դϴ�!\n\n" << endl;
		PostLogin();
		PostReceive();
	}

	if (!strcmp(recv_command, "_1"))
	{
		cout << "\n\n�������� �ʴ� �����Դϴ�!\n\n" << endl;
		PostLogin();
		PostReceive();
	}

	if (!strcmp(recv_command, "__1"))
	{
		cout << "\n\n�̹� �������� �����Դϴ�!\n\n" << endl;
		PostLogin();
		PostReceive();
	}

	if (!strcmp(recv_command, "2_"))
	{
		system("cls");

		cout << "\n\n������ �����߽��ϴ�!\n\n" << endl;

		strcpy_s(command, 4, "3");
		strcpy_s(status, 20, S_1_3);

		cout << "\n\n===============ä�ü����� �����մϴ�=============\n\n" << endl;

		strcpy_s(client_id, 20, "Server");
		strcpy_s(client_message, 20, user.user_id);
		strcat_s(client_message, "���� ä�ü����� �����մϴ�.");
		strcpy_s(logFilename, sizeof(logFilename), "Logs\\");
		strcat(logFilename, user.user_id);
		strcat(logFilename, "_Logs.txt");
		chatLog.open(logFilename);
		PostWrite();
	}

	if (!strcmp(recv_command, "1_"))
	{      
		system("cls");

		cout << "\n\n�α��ο� �����Ͽ����ϴ�!\n\n" << endl;

		strcpy_s(command, 4, "3");
		strcpy_s(status, 20, S_1_3);

		cout << "\n\n===============ä�ü����� �����մϴ�=============\n\n" << endl;

		strcpy_s(client_id, 20, "Server");
		strcpy_s(client_message, 20, user.user_id);
		strcat_s(client_message, "���� ä�ü����� �����մϴ�.");
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
			cout << "������ ������ ���������ϴ�." << endl;
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
	cout << "=====================ä�����α׷�=================" << endl;
	cout << "1.\t\t�α���\t\t" << endl;
	cout << "2.\t\t��������\t\t" << endl;
	cout << "==================================================" << endl;
}

void TCP_Client::Help()
{
	cout << "=====================���� ����Ʈ================" << endl;
	cout << "1. \t/? ����" << endl;
	cout << "2. \t/q ä�� ����" << endl;
	cout << "4. \t/c ä��â �ʱ�ȭ"<< endl;
	cout << "5. \t/v ���� �������� ���� ��ȸ" << endl;
	cout << "6. \t/u ���� ���ε� �ϱ�" << endl;
	cout << "7. \t/d ���� �ٿ�ε� �ϱ�" << endl;
	cout << "8. \t/f ���� ����Ʈ ����" << endl;
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