#include "android_debug.h"

#include <android/sensor.h>
#include <android/log.h>
#include <errno.h>


void AndroidDebug::print(const string &msg)
{
	int size = msg.size();
	int prio = ANDROID_LOG_INFO;
	string msgSubStr = msg;
	if(size >= 8 &&  msg.substr(0, 8) == "SUCCESS:") {
		//prio = ANDROID_LOG_DEBUG;
		msgSubStr = msg.substr(8);
	}else if(size >= 6 && msg.substr(0, 6) == "ERROR:") {
		prio = ANDROID_LOG_ERROR;
		msgSubStr = msg.substr(6);
	}else if(size >= 5 && msg.substr(0, 5) == "WARN:") {
		prio = ANDROID_LOG_WARN;
		msgSubStr = msg.substr(5);
	}
	__android_log_print(prio, "X2DActivity", msgSubStr.c_str());
}