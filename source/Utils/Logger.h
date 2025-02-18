/*
 * Logger.h
 *
 *  Created on: Feb 13, 2025
 *      Author: BULM
 */

#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace MaroVST
{

class Logger
{
public:
	static void logToFile(const std::string& message)
	{
		return;
		std::ofstream logFile("C:\\Users\\Public\\VST3_Log.txt", std::ios::app);
		if (logFile.is_open())
		{
			auto now = std::chrono::system_clock::now();
			auto time = std::chrono::system_clock::to_time_t(now);

		    std::stringstream ss;
		    logFile << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] " << message << std::endl;

			logFile.close();
		}
	}
};

} // namespace MaroVST
