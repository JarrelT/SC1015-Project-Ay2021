#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>

using namespace std;

#define _VERSION_ "Subsidiary program v1.03 202204020003"
#define _BIRTH___ "202203231855"

typedef struct {
	string weather[4];
	string weatherS[4];
	double p_value;
	int accS[4];
	int accT;
} row_t;

char* isInt(const char* b_);

time_t strTime(char* b_);

string getCol(const char* buf, int idx);

void initRows(row_t* r, size_t n);

int main(int argc, char* argv[])
{
	/* compile this source and run the program with country (1 at a time) as argument */
	/* eg. ./[prog] 'New York' */
	/* 
		g++ clean.cpp -std=c++11
		./a.out Miami -4 && \
		./a.out Denver -6 && \
		./a.out Minneapolis -5 && \
		./a.out 'New York' -4 && \
		./a.out Phoenix -7 

		./a.out Miami -4 && ./a.out Denver -6 && ./a.out Minneapolis -5 && ./a.out 'New York' -4 && ./a.out Phoenix -7
	*/
	const size_t IDX = 2200;

	ifstream weather("WeatherEvents_Jan2016-Dec2021.csv"); // File name change accordingly
	ifstream acc("US_Accidents_Dec21_updated.csv");
	ofstream of(string((argc >= 2) ? argv[1] : "Miami") + ".csv");

	const time_t ST = 1451577600; /* 20160101000000 */
	const time_t ET = 1640880000; /* 20211231000000 */
	const time_t RT = 3600 * 24; /* Time resolution is a day */
	row_t* rows = new row_t[IDX]; /* 6 years ~ 2190 days */

	string buf;
	int colCount = 0;

	/*
		total 2016 01 30 to 2021 12 31, 6 years ~ 2190 days
		date,t1,t2,t3,t4,ts1,ts2,ts3,ts4,accS1,accS2,accS3,accS4,p_value,accT,accTF
		UTC TIME IS USED FOR WEATHER
		WHILE LOCAL TIME IS USED FOR ACCIDENTS
	*/
	time_t CTZ = 3600 * ((argc >= 3) ? atoi(argv[2]) : -4), row_time = ST; /* GMT-4 for Miami [UPDATE AS REQUIRED] */
	const string city = (argc >= 2) ? argv[1] : "Miami"; /* [UPDATE AS REQUIRED] */

	if (!weather || !acc || !of) {
		cerr << "File not found or permission denied for I/O operation\n";
		return 1;
	}
	initRows(rows, IDX);

	getline(weather, buf);
	cout << "Processing: " << getCol(buf.c_str(), 10) << " time_zone : " << CTZ << "\n";
	while (getline(weather, buf)) {
		if (getCol(buf.c_str(), 10) == city) {
			string type = getCol(buf.c_str(), 1); /* 'Snow', 'Fog', 'Cold', 'Storm', 'Rain', 'Precipitation', 'Hail' */
			string severity = getCol(buf.c_str(), 2); /* 'Light', 'Severe', 'Moderate', 'Heavy', 'UNK', 'Other' */
			string startTime = getCol(buf.c_str(), 3);
			string endTime = getCol(buf.c_str(), 4);
			time_t st = strTime((char*)startTime.c_str()); /* Given UTC TIME */
			time_t et = strTime((char*)endTime.c_str()); /* Given UTC TIME */
			size_t row_idx = (size_t)((double)(st - ST) / RT);
			size_t col_idx_s = ((st - ST) % RT) / 21600;
			size_t col_idx_e = ((et - ST) % RT) / 21600;
			double p_value = stof(getCol(buf.c_str(), 5)); /* CHANGE INDEX ACCORDINGLY */
			rows[row_idx].weather[col_idx_s] = type;
			rows[row_idx].weather[col_idx_e] = type;
			rows[row_idx].weatherS[col_idx_s] = severity;
			rows[row_idx].weatherS[col_idx_e] = severity;
			rows[row_idx].weatherS[col_idx_e] = severity;
			rows[row_idx].p_value += p_value;
			colCount++;
			// if (colCount <= 10) /* just to ensure it is working */
			// 	cout << row_idx << ":" << col_idx_s << "\t" << severity << "\t" << startTime << "\t" << p_value << endl;
		}
	}
	cout << "Found " << colCount << " entries for " << city << " (weather)" << endl;

	getline(acc, buf);
	colCount = 0;
	while (getline(acc, buf)) {
		if (getCol(buf.c_str(), 13) == city) {
			string severity = getCol(buf.c_str(), 1); /* 4, 3, 2, 1 */
			string startTime = getCol(buf.c_str(), 2); /* Given local time */
			time_t st = strTime((char*)startTime.c_str()) + CTZ; /* UTC */
			size_t row_idx = (size_t)((double)(st - ST) / RT);
			size_t col_idx_s = ((st - ST) % RT) / 21600;
			rows[row_idx].accS[col_idx_s] = atoi(severity.c_str());
			rows[row_idx].accT++;
			colCount++;
			// if (colCount <= 10) /* just to ensure it is working */
			// 	cout << row_idx << ":" << col_idx_s << "\t" << severity << "\t" << startTime << endl;
		}
	}
	cout << "Found " << colCount << " entries for " << city << " (accidients)" << endl;
	weather.close();
	acc.close();

	of << "Date,W0000_0600,W0601_1200,W1201_1800,W1801_2359,WS1,WS2,WS3,WS4,";
	of << "AS0000_0600,AS0601_1200,AS1201_1800,AS1801_2359,P_Value(in),A_Total,Accident\n";
	for (size_t i = 0; i < IDX || row_time <= ET; i++) {
		struct tm* tm_ = localtime(&row_time);
		of << setfill('0') << setw(4) << tm_->tm_year + 1900 << "-";
		of << setfill('0') << setw(2) << tm_->tm_mon + 1 << "-";
		of << setfill('0') << setw(2) << tm_->tm_mday << ",";
		for (size_t j = 0; j < 4; j++)
			of << rows[i].weather[j] << ",";
		for (size_t j = 0; j < 4; j++)
			of << rows[i].weatherS[j] << ",";
		for (size_t j = 0; j < 4; j++)
			of << rows[i].accS[j] << ",";
		of << rows[i].p_value << ",";
		of << rows[i].accT << "," << ((rows[i].accT) ? "TRUE\n" : "FALSE\n");
		row_time += RT;
	}

	return 0;
}
void initRows(row_t* r, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		r[i].accT = 0;
		r[i].weather[0] = "Clear";
		r[i].weather[1] = "Clear";
		r[i].weather[2] = "Clear";
		r[i].weather[3] = "Clear";
		r[i].weatherS[0] = "NA";
		r[i].weatherS[1] = "NA";
		r[i].weatherS[2] = "NA";
		r[i].weatherS[3] = "NA";
		r[i].p_value = 0.0;
		r[i].accS[0] = 0;
		r[i].accS[1] = 0;
		r[i].accS[2] = 0;
		r[i].accS[3] = 0;
	}
}

string getCol(const char* buf, int idx)
{
	int s = 0;
	char *cur = (char*)buf, *prev = (char*)buf;
	while ((cur = strchr(cur, ','))) {
		if (s == idx)
			return string(prev, cur - prev);
		s++;
		prev = ++cur;
	}
	// last cell will not be ignored
	return "";
}

char* isInt(const char* b_)
{
	for (size_t i = 0; b_[i] != '\0'; i++)
		if (!(b_[i] >= '0' && b_[i] <= '9'))
			return nullptr;
	return (b_[0] == '\0') ? nullptr : (char*)b_;
}

time_t strTime(char* b_)
{
	/*
		Basically this converts char* to time_t
		YYYY-MM-DD hh:mm:ss hh:mm
	*/

	struct tm indv = { 0 };
	string buf(b_), date_, time_;

	indv.tm_isdst = -1;

	date_ = buf.substr(0, buf.find(' '));
	time_ = buf.substr(buf.find(' ') + 1);

	indv.tm_year = stoi(date_.substr(0, date_.find('-'))) - 1900;
	indv.tm_mon = stoi(date_.substr(date_.find('-') + 1, 2)) - 1;
	indv.tm_mday = stoi(date_.substr(date_.find_last_of('-') + 1, 2));
	indv.tm_hour = stoi(time_.substr(0, time_.find(':')));
	indv.tm_min = stoi(time_.substr(time_.find(':') + 1, 2));
	indv.tm_sec = stoi(time_.substr(time_.find_last_of(':') + 1, 2));

	return mktime(&indv);
}
